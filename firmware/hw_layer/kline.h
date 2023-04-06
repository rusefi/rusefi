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

void initKLine();

/* Stop/Start for config update */
void startKLine();
void stopKLine();

typedef size_t (*ByteSource)(uint8_t *, int);
size_t readWhileGives(ByteSource source, uint8_t *buffer, size_t bufferSize);
