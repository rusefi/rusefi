#pragma once
#include <cstdint>
#include <cstring>
using systime_t = uint32_t;
enum can_baudrate_e { B500KBPS, B1MBPS };
struct CANConfig { can_baudrate_e rate; };
struct CANRxFrame { uint32_t SID=0, EID=0; uint8_t IDE=0, RTR=0, DLC=0; uint8_t data8[8] = {}; };
using CANTxFrame = CANRxFrame;
extern int CAND1;
#define STM32_CAN_USE_CAN1 1
#ifndef TEST_FDCAN
#define STM32_CAN_MAX_FILTERS 28
#endif
#define BOOT_COM_CAN_CHANNEL_INDEX 0
#define BOOT_COM_CAN_RX_MSG_ID 0x80010667U
#define BOOT_COM_CAN_TX_MSG_ID 0x800107E1U
#define GPIOB 0
#define EFI_CAN_TX_AF 9
#define EFI_CAN_RX_AF 9
#define PAL_MODE_ALTERNATE(x) (x)
#define CAN_ISX(f) ((f).IDE)
#define CAN_ISRTR(f) ((f).RTR)
#define CAN_EID(f) ((f).EID)
#define CAN_SID(f) ((f).SID)
#define CAN_ANY_MAILBOX 0
#define TIME_IMMEDIATE 0
#define MSG_OK 0
#define TIME_MS2I(x) (x)
#define TIME_I2MS(x) (x)
void palSetPadMode(int, int, int);
void canSTM32SetFilters(int*, int, int, void*);
void canStart(int*, const CANConfig*);
int canReceiveTimeout(int*, int, CANRxFrame*, int);
int canTransmitTimeout(int*, int, const CANTxFrame*, int);
systime_t chVTGetSystemTime();
void chThdSleepMilliseconds(int);
void NVIC_SystemReset();
