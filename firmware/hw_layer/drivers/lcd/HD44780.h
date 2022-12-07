/**
 * @file HD44780.h
 *
 * @date 13.12.2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void startHD44780_pins();
void stopHD44780_pins();
void lcd_HD44780_init();
void lcd_HD44780_set_position(uint8_t row, uint8_t column);
void lcd_HD44780_print_char(char data);
void lcd_HD44780_print_string(const char *string);
int getCurrentHD44780row(void);
int getCurrentHD44780column(void);

void lcdShowPanicMessage(char *message);

#ifdef __cplusplus
}
#endif /* __cplusplus */

