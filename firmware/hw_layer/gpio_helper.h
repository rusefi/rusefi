/*
 * @file gpio_helper.h
 * @brief   General I/O helper
 *
 * @date Aug 25, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef GPIO_HELPER_H_
#define GPIO_HELPER_H_

#define INITIAL_PIN_STATE -1


/**
 * @brief   Single output pin reference and state
 */
typedef struct {
	GPIO_TypeDef *port;
	int pin;
	/**
	 * we track current pin status so that we do not touch the actual hardware if we want to write new pin bit
	 * which is same as current pin value. This maybe helps in case of status leds, but maybe it's a total over-engineering
	 */
	int currentLogicValue;
} OutputPin;

void initOutputPin(const char *msg, OutputPin *outputPin, GPIO_TypeDef *port, uint32_t pinNumber);
void initOutputPinExt(const char *msg, OutputPin *outputPin, GPIO_TypeDef *port, uint32_t pinNumber, iomode_t mode);
int getLogicPinValue(OutputPin * outputPin);
void setPinValue(OutputPin * outputPin, int electricalValue, int logicValue);

#endif /* GPIO_HELPER_H_ */
