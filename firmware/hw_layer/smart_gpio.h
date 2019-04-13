/*
 * @file smart_gpio.h
 *
 * @date Apr 13, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#ifndef HW_LAYER_SMART_GPIO_H_
#define HW_LAYER_SMART_GPIO_H_

/* TLE6240 pins go right after on chips */
#define TLE6240_PIN(n)		((brain_pin_e)((int)BRAIN_PIN_LAST_ONCHIP + 1 + (n)))
/* MC33972 pins go right after TLE6240 */
#define MC33972_PIN(n)		((brain_pin_e)((int)BRAIN_PIN_LAST_ONCHIP + 1 + 16 + (n)))

void initSmartGpio(void);

#if (defined(STM32F405xx) || defined(STM32F407xx) || defined (STM32F469xx))
 #define	STM_F4_FAMILY
#elif (defined(STM32F765xx) || defined(STM32F767xx) || defined(STM32F746xx))
 #define	STM_F7_FAMILY
#else
 unexpected platform
#endif


#if (BOARD_TLE6240_COUNT > 0)

#if defined(STM_F4_FAMILY)
 #define SPI_CR1_16BIT_MODE SPI_CR1_DFF
 #define SPI_CR2_16BIT_MODE 0

 // TODO
 #define SPI_CR1_24BIT_MODE 0
 #define SPI_CR2_24BIT_MODE 0
#elif defined(STM_F7_FAMILY)
 #define SPI_CR1_16BIT_MODE 0
 #define SPI_CR2_16BIT_MODE SPI_CR2_DS_3 | SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0

 #define SPI_CR1_24BIT_MODE 0
 /* 3 x 8-bit transfer */
 #define SPI_CR2_24BIT_MODE SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0
#else
 unexpected platform
#endif

#endif /* (BOARD_TLE6240_COUNT > 0) */

#endif /* HW_LAYER_SMART_GPIO_H_ */
