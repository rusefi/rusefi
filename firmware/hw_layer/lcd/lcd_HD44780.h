/**
 * @file lcd_HD44780.h
 *
 * @date 13.12.2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef LCD_2X16_H_
#define LCD_2X16_H_

extern void lcd_HD44780_init(void);
extern void lcd_HD44780_set_position(uint8_t row, uint8_t column);
extern void lcd_HD44780_print_char(char data);
extern void lcd_HD44780_print_string(char *string);

void lcdShowFatalMessage(char *message);

#endif /* LCD_2X16_H_ */
