/**
 * @file lcd_controller.h
 *
 * @date Aug 14, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef LCD_CONTROLLER_H_
#define LCD_CONTROLLER_H_

#include "engine.h"

typedef enum {
	LL_VERSION,
	LL_CONFIG,
	LL_RPM,
	LL_TEMPERATURE

} lcd_line_e;

void updateHD44780lcd(Engine *engine);

#endif /* LCD_CONTROLLER_H_ */
