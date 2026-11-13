# M5StickC Plus2 UART-host build (work in progress)

This branch aims to support an M5StickC Plus2 (ESP32-PICO-V3-02) as a UART-connected ESP32 Marauder module for Flipper Zero-compatible hosts.

## Current status

- [x] Optional 115200 baud UART0 remap to the Grove/HY2.0-4P pins
- [x] Disable the default GPS feature when UART-host mode is selected (GPIO32/33 conflict)
- [ ] Remove display and button dependencies for a truly headless build
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
| Grove GPIO33 (RX) | UART TX (3.3 V logic) |
| Grove GPIO32 (TX) | UART RX (3.3 V logic) |
| Grove GND | GND |

Power separately for the first test. Do not feed the host's 5 V into the M5StickC Plus2 Grove 5 V output. Use a verified 5 V input path when integrating power.

**Important:** The initial UART remap uses `Serial` (UART0), which also changes runtime serial logging behavior. This is not yet a headless or tested build. Existing screen initialization and battery code remain in place.

## Next steps

Separate display/UI dependencies from the M5StickC Plus2 board profile, compile the target, and test UART protocol compatibility with the Flipper WiFi Marauder app.
