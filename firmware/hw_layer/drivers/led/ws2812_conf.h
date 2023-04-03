#define WS2812_LED_N    22 // Number of LEDs
#define WS2812_PORT     GPIOE
#define WS2812_PIN      9
#define WS2812_TIM_N    1  // timer, 1-11
#define WS2812_TIM_CH   0 // timer channel, 0-3
#define WS2812_DMA_STREAM STM32_DMA_STREAM_ID(2, 5) //DMA2 Stream 5 // DMA stream for TIMx_UP (look up in reference manual under DMA Channel selection)
#define WS2812_DMA_CHANNEL 6                  // DMA channel for TIMx_UP

#define WS2812_PWM_FREQUENCY (STM32_SYSCLK / 2) /**< Clock frequency of PWM, must be valid with respect to system clock! */
#define WS2812_PWM_PERIOD (WS2812_PWM_FREQUENCY / 800000)

#define WS2812_DUTYCYCLE_0 (WS2812_PWM_FREQUENCY / (1000000000 / 220))
#define WS2812_DUTYCYCLE_1 (WS2812_PWM_FREQUENCY / (1000000000 / 580))

#define CONCAT_SYMBOLS(s1, s2) s1##s2
#define CONCAT_EXPANDED_SYMBOLS(s1, s2) CONCAT_SYMBOLS(s1, s2)

#define WS2812_PWM_DRIVER CONCAT_EXPANDED_SYMBOLS(PWMD, WS2812_TIM_N)

#define WS2812_BUFLEN ((WS2812_LED_N + 4) * 24)