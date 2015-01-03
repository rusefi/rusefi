/**
 * @file lcd_controller.h
 *
 * @date Aug 14, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef LCD_CONTROLLER_H_
#define LCD_CONTROLLER_H_

#include "engine.h"

#define MAX_LCD_WIDTH 20

typedef enum {
	LL_VERSION,
	LL_CONFIG,
	LL_RPM,
	LL_CLT_TEMPERATURE,
	LL_IAT_TEMPERATURE,
	LL_ALGORITHM,
} lcd_line_e;

#define TOTAL_OPTIONS 6

void updateHD44780lcd(Engine *engine);

#endif /* LCD_CONTROLLER_H_ */
