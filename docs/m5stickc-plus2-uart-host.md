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

## Dedicated UART2 CLI (in progress)

- `Serial` / UART0 is preserved for USB logging and programming.
- `MarauderHostSerial` is `HardwareSerial(2)`, RX GPIO25 and TX GPIO26, 115200 8N1.
- `CommandLine.cpp` routes its own command input and output to `MarauderHostSerial` under the opt-in flag.
- GPS retains UART1 on Grove GPIO32/33.
- **Not yet protocol-complete:** outputs from other modules still use `Serial` and will not reach the UART2 host. Review scan/wardrive output and CLI-dependent subsystems before claiming Flipper compatibility.
- **Not compiled or hardware tested.**

## Protocol-output routing milestone

`MarauderHostOutput.h` provides a conditional per-translation-unit `Serial` alias for the opt-in M5StickC Plus2 UART-host build. The alias is included **after** dependencies in `CommandLine.cpp`, `WiFiScan.cpp`, and `Buffer.cpp`, so their CLI, scan, and buffered output target `MarauderHostSerial` (UART2) while ordinary firmware initialization retains the USB `Serial` (UART0). GPS continues using its existing UART1 object.

This is an incremental routing strategy, not a completed audit: other source files may still print data or binary streams to USB UART0, and cross-module code paths need verification. Compile, pin-level validation, host-app interoperability, and USB log separation have **not** been tested. Do not flash or merge as a validated release.
