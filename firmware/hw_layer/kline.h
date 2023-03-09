#pragma once


#ifndef KLINE_SERIAL_DEVICE
#define KLINE_SERIAL_DEVICE SD2
#endif
#ifndef KLINE_SERIAL_DEVICE_RX
#define KLINE_SERIAL_DEVICE_RX H144_UART2_RX
#endif
#ifndef KLINE_SERIAL_DEVICE_TX
#define KLINE_SERIAL_DEVICE_TX H144_UART2_TX
#endif

// The standard transmission rate
#define KLINE_BAUD_RATE 10400

#define KLINE_READ_TIMEOUT TIME_MS2I(1)

void initKLine();

/* Stop/Start for config update */
void startKLine();
void stopKLine();
