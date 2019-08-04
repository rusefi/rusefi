/**
 * @file	stm32_common_mpu_util.h
 * @brief	Low level common STM32 header
 *
 * @date Aug 3, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

// burnout or 'Burn Out'
typedef enum {
	BOR_Level_None = OB_BOR_OFF, // 0x0C=12  Supply voltage ranges from 1.62 to 2.10 V
	BOR_Level_1 = OB_BOR_LEVEL1, // 0x08     Supply voltage ranges from 2.10 to 2.40 V
	BOR_Level_2 = OB_BOR_LEVEL2, // 0x04     Supply voltage ranges from 2.40 to 2.70 V
	BOR_Level_3 = OB_BOR_LEVEL3  // 0x00     Supply voltage ranges from 2.70 to 3.60 V
} BOR_Level_t;

typedef enum {
	BOR_Result_Ok = 0x00,
	BOR_Result_Error
} BOR_Result_t;

BOR_Level_t BOR_Get(void);
BOR_Result_t BOR_Set(BOR_Level_t BORValue);

void baseMCUInit(void);
void turnOnSpi(spi_device_e device);
void jump_to_bootloader();

#if HAL_USE_CAN
bool isValidCanTxPin(brain_pin_e pin);
bool isValidCanRxPin(brain_pin_e pin);
CANDriver * detectCanDevice(brain_pin_e pinRx, brain_pin_e pinTx);
#endif /* HAL_USE_CAN */
