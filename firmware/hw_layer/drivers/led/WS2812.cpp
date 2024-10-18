/**
 * @file WS2812.cpp
 * @brief WS2812 RGB LED driver
 *
 * @date 25.03.2023
 * @author Benas Brazdziunas
 */

#include "pch.h"

#if EFI_WS2812

#include "WS2812.h"
#include "ws2812_conf.h"


static WS2812_RGB_t WS2812_LED_BUF[WS2812_LED_N];
static uint32_t WS2812_TIM_BUF[WS2812_BUFLEN];
static uint8_t WS2812_BRIGHTNESS = 20;

void initWS2812()
{
	palSetPadMode(WS2812_PORT, WS2812_PIN, PAL_MODE_ALTERNATE(1));

	static const PWMConfig ws2812_pwm_config = {
		.frequency = WS2812_PWM_FREQUENCY,
		.period = WS2812_PWM_PERIOD,
		.callback = NULL,
		.channels = {
			[0] = {.mode = WS2812_TIM_CH == 0 ? PWM_OUTPUT_ACTIVE_HIGH : PWM_OUTPUT_DISABLED, .callback = NULL}, // Turn on the channel we care about
			[1] = {.mode = WS2812_TIM_CH == 1 ? PWM_OUTPUT_ACTIVE_HIGH : PWM_OUTPUT_DISABLED, .callback = NULL}, // Turn on the channel we care about
			[2] = {.mode = WS2812_TIM_CH == 2 ? PWM_OUTPUT_ACTIVE_HIGH : PWM_OUTPUT_DISABLED, .callback = NULL}, // Turn on the channel we care about
			[3] = {.mode = WS2812_TIM_CH == 3 ? PWM_OUTPUT_ACTIVE_HIGH : PWM_OUTPUT_DISABLED, .callback = NULL}, // Turn on the channel we care about
		},
		.cr2 = 0,
		.bdtr = 0,
		.dier = TIM_DIER_UDE, // DMA on update event for next period
	};

	const stm32_dma_stream_t *dma = dmaStreamAlloc(WS2812_DMA_STREAM, 10, NULL, NULL);
	dmaStreamSetPeripheral(dma, &(WS2812_PWM_DRIVER.tim->CCR[WS2812_TIM_CH]));
	dmaStreamSetMemory0(dma, WS2812_TIM_BUF);
	dmaStreamSetTransactionSize(dma, WS2812_BUFLEN);
	// M2P: Memory 2 Periph; PL: Priority Level
	dmaStreamSetMode(dma,
					 STM32_DMA_CR_CHSEL(WS2812_DMA_CHANNEL) | 
					 STM32_DMA_CR_DIR_M2P | 
					 STM32_DMA_CR_PSIZE_WORD | 
					 STM32_DMA_CR_MSIZE_WORD |
					 STM32_DMA_CR_MINC | 
					 STM32_DMA_CR_CIRC | 
					 STM32_DMA_CR_PL(3)
					);

	dmaStreamEnable(dma);
	pwmStart(&PWMD1, &ws2812_pwm_config);
	pwmEnableChannel(&PWMD1, WS2812_TIM_CH, 0); //(WS2812_PWM_PERIOD/2)

	//TEST FUNCTIONALITY
	clearWS2812All();
	setWS2812Brightness(5);
	setWS2812All((WS2812_RGB_t){255, 0, 0});
	setWS2812One(19,(WS2812_RGB_t){0, 0, 255});
	setWS2812One(9,(WS2812_RGB_t){0, 255, 0});
	clearWS2812One(18);
}

/**
 * Set one LEDs to 0 (off)
 */
void clearWS2812One(uint32_t num)
{
	if (num < WS2812_LED_N)
	{
		WS2812_LED_BUF[num] = (WS2812_RGB_t){0, 0, 0};
	}
	calcBuf();
}

/**
 * Set all LEDs to 0 (off)
 */
void clearWS2812All()
{
	for (uint16_t num = 0; num < WS2812_LED_N; num++)
	{
		WS2812_LED_BUF[num] = (WS2812_RGB_t){0, 0, 0};
	}
	calcBuf();
}

/**
 * Set one LED (R, G, B values).
 */
void setWS2812One(uint32_t num, WS2812_RGB_t rgb_col)
{
	if (num < WS2812_LED_N)
	{
		WS2812_LED_BUF[num] = rgb_col;
	}
	calcBuf();
}

/**
 * Set all LEDs (R, G, B values).
 */
void setWS2812All(WS2812_RGB_t rgb_col)
{
	for (uint16_t num = 0; num < WS2812_LED_N; num++)
	{
		WS2812_LED_BUF[num] = rgb_col;
	}
	calcBuf();
}

/**
 * Set all LEDs Brightness.
 */
void setWS2812Brightness(uint8_t num)
{
	num = num >= 100 ? 100 : num;
	num = num <= 0 ? 0 : num;
	WS2812_BRIGHTNESS = num;
	calcBuf();
}

/**
 * Calculate Timer DMA buffer
 */
void calcBuf()
{
	uint32_t pos = 0;
	// set timings for all LEDs
	for (uint32_t num = 0; num < WS2812_LED_N; num++)
	{
		WS2812_RGB_t led = WS2812_LED_BUF[num];
		float brightness = WS2812_BRIGHTNESS / 100.0;

		led.red = (uint8_t)(led.red * brightness);
		led.green = (uint8_t)(led.green * brightness);
		led.blue = (uint8_t)(led.blue * brightness);

		// Col:Green , Bit:7..0
		WS2812_TIM_BUF[pos++] = ((led.green & 0x80) != 0) ? WS2812_DUTYCYCLE_1 : WS2812_DUTYCYCLE_0;
		WS2812_TIM_BUF[pos++] = ((led.green & 0x40) != 0) ? WS2812_DUTYCYCLE_1 : WS2812_DUTYCYCLE_0;
		WS2812_TIM_BUF[pos++] = ((led.green & 0x20) != 0) ? WS2812_DUTYCYCLE_1 : WS2812_DUTYCYCLE_0;
		WS2812_TIM_BUF[pos++] = ((led.green & 0x10) != 0) ? WS2812_DUTYCYCLE_1 : WS2812_DUTYCYCLE_0;
		WS2812_TIM_BUF[pos++] = ((led.green & 0x08) != 0) ? WS2812_DUTYCYCLE_1 : WS2812_DUTYCYCLE_0;
		WS2812_TIM_BUF[pos++] = ((led.green & 0x04) != 0) ? WS2812_DUTYCYCLE_1 : WS2812_DUTYCYCLE_0;
		WS2812_TIM_BUF[pos++] = ((led.green & 0x02) != 0) ? WS2812_DUTYCYCLE_1 : WS2812_DUTYCYCLE_0;
		WS2812_TIM_BUF[pos++] = ((led.green & 0x01) != 0) ? WS2812_DUTYCYCLE_1 : WS2812_DUTYCYCLE_0;

		// Col:Red , Bit:7..0
		WS2812_TIM_BUF[pos++] = ((led.red & 0x80) != 0) ? WS2812_DUTYCYCLE_1 : WS2812_DUTYCYCLE_0;
		WS2812_TIM_BUF[pos++] = ((led.red & 0x40) != 0) ? WS2812_DUTYCYCLE_1 : WS2812_DUTYCYCLE_0;
		WS2812_TIM_BUF[pos++] = ((led.red & 0x20) != 0) ? WS2812_DUTYCYCLE_1 : WS2812_DUTYCYCLE_0;
		WS2812_TIM_BUF[pos++] = ((led.red & 0x10) != 0) ? WS2812_DUTYCYCLE_1 : WS2812_DUTYCYCLE_0;
		WS2812_TIM_BUF[pos++] = ((led.red & 0x08) != 0) ? WS2812_DUTYCYCLE_1 : WS2812_DUTYCYCLE_0;
		WS2812_TIM_BUF[pos++] = ((led.red & 0x04) != 0) ? WS2812_DUTYCYCLE_1 : WS2812_DUTYCYCLE_0;
		WS2812_TIM_BUF[pos++] = ((led.red & 0x02) != 0) ? WS2812_DUTYCYCLE_1 : WS2812_DUTYCYCLE_0;
		WS2812_TIM_BUF[pos++] = ((led.red & 0x01) != 0) ? WS2812_DUTYCYCLE_1 : WS2812_DUTYCYCLE_0;

		// Col:Blue , Bit:7..0
		WS2812_TIM_BUF[pos++] = ((led.blue & 0x80) != 0) ? WS2812_DUTYCYCLE_1 : WS2812_DUTYCYCLE_0;
		WS2812_TIM_BUF[pos++] = ((led.blue & 0x40) != 0) ? WS2812_DUTYCYCLE_1 : WS2812_DUTYCYCLE_0;
		WS2812_TIM_BUF[pos++] = ((led.blue & 0x20) != 0) ? WS2812_DUTYCYCLE_1 : WS2812_DUTYCYCLE_0;
		WS2812_TIM_BUF[pos++] = ((led.blue & 0x10) != 0) ? WS2812_DUTYCYCLE_1 : WS2812_DUTYCYCLE_0;
		WS2812_TIM_BUF[pos++] = ((led.blue & 0x08) != 0) ? WS2812_DUTYCYCLE_1 : WS2812_DUTYCYCLE_0;
		WS2812_TIM_BUF[pos++] = ((led.blue & 0x04) != 0) ? WS2812_DUTYCYCLE_1 : WS2812_DUTYCYCLE_0;
		WS2812_TIM_BUF[pos++] = ((led.blue & 0x02) != 0) ? WS2812_DUTYCYCLE_1 : WS2812_DUTYCYCLE_0;
		WS2812_TIM_BUF[pos++] = ((led.blue & 0x01) != 0) ? WS2812_DUTYCYCLE_1 : WS2812_DUTYCYCLE_0;
	}
}

#endif /* EFI_WS2812 */
