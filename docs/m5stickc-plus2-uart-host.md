# M5StickC Plus2 UART-host build (work in progress)

This branch aims to support an M5StickC Plus2 (ESP32-PICO-V3-02) as a UART-connected ESP32 Marauder module for Flipper Zero-compatible hosts.

## Current status

- [x] Optional 115200 baud UART0 remap to the Grove/HY2.0-4P pins
- [x] Preserve GPS on Grove GPIO32/33, move interim host UART0 to HAT GPIO25/26
- [x] Add compile-time gates for display, buttons, battery telemetry and temperature features (compile validation pending)
- [ ] Compile and test the target firmware
- [ ] Validate UART command/response behavior with the Flipper WiFi Marauder app
- [ ] Validate external 5V power and boot behavior on real hardware

## Initial configuration

In `esp32_marauder/configs.h`, enable both:

```cpp
#define MARAUDER_M5STICKCP2
#define MARAUDER_M5STICKCP2_UART_HOST
```

UART wiring:

| M5StickC Plus2 | Host |
| --- | --- |
| HAT GPIO25 (RX) | UART TX (3.3 V logic) |
| HAT GPIO26 (TX) | UART RX (3.3 V logic) |
| GND | GND |

Power separately for the first test. Do not feed the host's 5 V into the M5StickC Plus2 Grove 5 V output. Use a verified 5 V input path when integrating power.

**Important:** The interim host UART remap uses `Serial` (UART0) on GPIO25/26, which also changes runtime USB serial logging behavior. The intended final architecture uses a dedicated UART2 host interface while preserving UART0 for USB debugging. This requires routing all CLI input and protocol output, not just initializing UART2. This is not yet a headless or tested build. The UART-host build disables screen, buttons and battery telemetry at compile time. GPS remains enabled on Grove GPIO32/33 (UART1). This has not yet been compiled or hardware-tested.

## Next steps

Separate display/UI dependencies from the M5StickC Plus2 board profile, compile the target, and test UART protocol compatibility with the Flipper WiFi Marauder app.

## UART2 integration blocker

`CommandLine.cpp` reads from `Serial` and prints protocol output through `Serial` extensively. Other subsystems also emit data through `Serial`. `GpsInterface.cpp` declares a global `HardwareSerial Serial2(GPS_SERIAL_INDEX)`, so the identifier `Serial2` is already occupied by the GPS implementation (configured for UART1 on M5StickC Plus2). A dedicated host UART2 must use a distinct object name and requires a reviewed routing abstraction for both input and all protocol output. Simply calling `Serial2.begin` would conflict with the GPS implementation.
