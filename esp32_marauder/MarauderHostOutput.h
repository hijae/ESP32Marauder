#pragma once

#include "configs.h"

// Include this header after other includes in protocol-producing translation
// units. It redirects subsequent Serial calls to the dedicated UART2 host.
// The GPS receiver remains on UART1, and the USB console remains UART0.
// Do not use in firmware initialization or hardware driver code.
#if defined(MARAUDER_M5STICKCP2) && defined(MARAUDER_M5STICKCP2_UART_HOST)
  #include <HardwareSerial.h>
  extern HardwareSerial MarauderHostSerial;
  #define Serial MarauderHostSerial
#endif
