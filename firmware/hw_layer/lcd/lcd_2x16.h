/*
 * @file lcd_2x16.h
 * @date 13.12.2013
 */

#ifndef LCD_2X16_H_
#define LCD_2X16_H_

extern void lcd_HD44780_init(void);
extern void lcd_HD44780_set_position(uint8_t row, uint8_t column);
extern void lcd_HD44780_print_char(char data);
extern void lcd_HD44780_print_string(char *string);
extern const uint8_t lcd_2x16_decode[];

void lcdShowFatalMessage(char *message);

#endif /* LCD_2X16_H_ */
