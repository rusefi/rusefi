/**
 * @file efifeatures.h
 *
 * @brief In this header we can configure which modules are used for EGT2CAN firmware.
 *
 * @date Dec 23, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef EFIFEATURES_H_
#define EFIFEATURES_H_

#define DL_OUTPUT_BUFFER 128

#define EFI_USB_SERIAL TRUE

#define hal_lld_get_counter_value()         DWT_CYCCNT

#define EFI_CONSOLE_TX_PORT GPIOC
#define EFI_CONSOLE_TX_PIN 10
#define EFI_CONSOLE_RX_PORT GPIOC
#define EFI_CONSOLE_RX_PIN 11
#define EFI_CONSOLE_AF 7


#define LED_ERROR_PORT GPIOD
#define LED_ERROR_PIN 3

#define LED_COMMUNICATION_PORT GPIOD
#define LED_COMMUNICATION_PIN 4

#define SERIAL_SPEED 115200
/**
 * This macros is used to hide pieces of the code from unit tests, so it only makes sense in folders exposed to the tests project.
 * This macros is NOT about taking out logging in general.
 */
#define EFI_PROD_CODE FALSE

#define CCM_OPTIONAL

#define EFI_EGT TRUE

#define EFI_UNIT_TEST FALSE

#define EFI_MAX_31855 TRUE

#define CONSOLE_MAX_ACTIONS 32

#ifndef CONFIG_RESET_SWITCH_PORT
#define CONFIG_RESET_SWITCH_PORT GPIOD
#endif

#define CONFIG_RESET_SWITCH_PIN 6

/**
 * This is the size of the MemoryStream used by chvprintf
 */
#define INTERMEDIATE_LOGGING_BUFFER_SIZE 256


#endif /* EFIFEATURES_H_ */
