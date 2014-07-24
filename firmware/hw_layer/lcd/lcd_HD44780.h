/**
 * @file lcd_HD44780.h
 *
 * @date 13.12.2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef LCD_HD44780_H_
#define LCD_HD44780_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void lcd_HD44780_init(void);
void lcd_HD44780_set_position(uint8_t row, uint8_t column);
void lcd_HD44780_print_char(char data);
void lcd_HD44780_print_string(const char *string);

void lcdShowFatalMessage(char *message);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LCD_HD44780_H_ */
