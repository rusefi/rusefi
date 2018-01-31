/**
 * @file lcd_HD44780.cpp
 * @brief HD44780 character display driver
 *
 * see http://en.wikipedia.org/wiki/Hitachi_HD44780_LCD_controller
 * @date 13.12.2013
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "main.h"

#if EFI_HD44780_LCD

#include "lcd_HD44780.h"
#include "pin_repository.h"
#include "string.h"

#include "engine_configuration.h"

extern engine_configuration_s *engineConfiguration;
extern board_configuration_s *boardConfiguration;

static Logging *logger;

enum {
	LCD_HD44780_DISPLAY_CLEAR = 0x01,
	LCD_HD44780_SHIFT_CURSOR_RIGHT = 0x06,
	LCD_HD44780_DISPLAY_ON = 0x0C,
	LCD_HD44780_4_BIT_BUS = 0x20,
	LCD_HD44780_RESET = 0x30,
	LCD_HD44780_DDRAM_ADDR = 0x80,

//	LCD_2X16_8_BIT_BUS = 0x30,
//	LCD_2X16_LINE_ONE = 0x20,
//	LCD_2X16_LINES_TWO = 0x28,
//	LCD_2X16_FONT_5X8 = 0x20,
//	LCD_2X16_FONT_5X10 = 0x24,
//	LCD_2X16_DISPLAY_HOME = 0x02,
//	LCD_2X16_DISPLAY_RIGHT = 0x1C,
//	LCD_2X16_DISPLAY_LEFT = 0x18,
//	LCD_2X16_DISPLAY_SHIFT = 0x05,
//	LCD_2X16_CURSOR_ON = 0x0A,
//	LCD_2X16_CURSOR_BLINK = 0x09,
//	LCD_2X16_CURSOR_RIGHT = 0x14,
//	LCD_2X16_CURSOR_LEFT = 0x10,
//	LCD_2X16_SHIFT_LEFT = 0x04,
//	LCD_2X16_CGRAM_ADDR = 0x40,
//	LCD_2X16_BUSY_FLAG = 0x80,
//	LCD_2X16_COMMAND = 0x01,
//	LCD_2X16_DATA = 0x00,
} lcd_HD44780_command;

// http://web.alfredstate.edu/weimandn/lcd/lcd_addressing/lcd_addressing_index.html
static const int lineStart[] = { 0, 0x40, 0x14, 0x54 };

static int BUSY_WAIT_DELAY = FALSE;
static int currentRow = 0;
static int currentColumn = 0;

static void lcdSleep(int period) {
	if (BUSY_WAIT_DELAY) {
		// this mode is useful for displaying messages to report OS fatal issues

		int ticks = 168000000 / 1000000 * period;
		int a = 0;
		for (int i = 0; i < ticks; i++)
			a += i;
		// the purpose of this code is to fool the compiler so that the loop is not optimized away
		efiAssertVoid(a != 0, "true");

	} else {
		chThdSleepMicroseconds(period);
	}
}

//static char txbuf[1];
#define LCD_PORT_EXP_ADDR 0x20

// todo: use this method wider!
static void writePad(const char *msg, brain_pin_e pin, int bit) {
	palWritePad(getHwPort(msg, pin), getHwPin(msg, pin), bit);
}

//-----------------------------------------------------------------------------
static void lcd_HD44780_write(uint8_t data) {
	if (engineConfiguration->displayMode == DM_HD44780) {
		writePad("lcd", boardConfiguration->HD44780_db7,
				data & 0x80 ? 1 : 0);
		writePad("lcd", boardConfiguration->HD44780_db6,
				data & 0x40 ? 1 : 0);
				writePad("lcd", boardConfiguration->HD44780_db5,
				data & 0x20 ? 1 : 0);
				writePad("lcd", boardConfiguration->HD44780_db4,
				data & 0x10 ? 1 : 0);

		writePad("lcd", boardConfiguration->HD44780_e, 1); // En high
		lcdSleep(10); // enable pulse must be >450ns
		writePad("lcd", boardConfiguration->HD44780_e, 0); // En low
		lcdSleep(40); // commands need > 37us to settle
	} else {

		//	LCD D4_pin -> P4
		//	LCD D5_pin -> P5
		//	LCD D6_pin -> P6
		//	LCD D7_pin -> P7
		//	LCD Pin RS -> P0
		//	LCD Pin RW -> P1
		//	LCD Pin E  -> P2

		// todo: finish all this stuff
				i2cAcquireBus(&I2CD1);
//
//				txbuf[0] = 4;
//				i2cMasterTransmit(&I2CD1, LCD_PORT_EXP_ADDR, txbuf, 1, NULL, 0);
//				lcdSleep(10); // enable pulse must be >450ns
//
//				txbuf[0] = 0;
//				i2cMasterTransmit(&I2CD1, LCD_PORT_EXP_ADDR, txbuf, 1, NULL, 0);
//
				i2cReleaseBus(&I2CD1);

	}
}

//-----------------------------------------------------------------------------
void lcd_HD44780_write_command(uint8_t data) {
	palClearPad(getHwPort("lcd", boardConfiguration->HD44780_rs), getHwPin("lcd", boardConfiguration->HD44780_rs));

	lcd_HD44780_write(data);
	lcd_HD44780_write(data << 4);
}

//-----------------------------------------------------------------------------
void lcd_HD44780_write_data(uint8_t data) {
	palSetPad(getHwPort("lcd", boardConfiguration->HD44780_rs), getHwPin("lcd", boardConfiguration->HD44780_rs));

	lcd_HD44780_write(data);
	lcd_HD44780_write(data << 4);
	currentColumn++;

	palClearPad(getHwPort("lcd", boardConfiguration->HD44780_rs), getHwPin("lcd", boardConfiguration->HD44780_rs));
}

//-----------------------------------------------------------------------------
void lcd_HD44780_set_position(uint8_t row, uint8_t column) {
	efiAssertVoid(row <= engineConfiguration->HD44780height, "invalid row");
	currentRow = row;
	currentColumn = column;
	lcd_HD44780_write_command(LCD_HD44780_DDRAM_ADDR + lineStart[row] + column);
}

int getCurrentHD44780row(void) {
	return currentRow;
}

int getCurrentHD44780column(void) {
	return currentColumn;
}

void lcd_HD44780_print_char(char data) {
	if (data == '\n') {
		lcd_HD44780_set_position(++currentRow, 0);
	} else {
		lcd_HD44780_write_data(data);
	}
}

void lcd_HD44780_print_string(const char* string) {
	while (*string != 0x00)
		lcd_HD44780_print_char(*string++);
}
//getHwPin(boardConfiguration->HD44780_db7)
static void lcdInfo(void) {
	scheduleMsg(logger, "HD44780 RS=%s", hwPortname(boardConfiguration->HD44780_rs));
	scheduleMsg(logger, "HD44780 E=%s", hwPortname(boardConfiguration->HD44780_e));
	scheduleMsg(logger, "HD44780 D4=%s", hwPortname(boardConfiguration->HD44780_db4));
	scheduleMsg(logger, "HD44780 D5=%s", hwPortname(boardConfiguration->HD44780_db5));
	scheduleMsg(logger, "HD44780 D6=%s", hwPortname(boardConfiguration->HD44780_db6));
	scheduleMsg(logger, "HD44780 D7=%s", hwPortname(boardConfiguration->HD44780_db7));
}

void lcd_HD44780_init(Logging *sharedLogger) {
	logger = sharedLogger;

	addConsoleAction("lcdinfo", lcdInfo);

	if (engineConfiguration->displayMode > DM_HD44780_OVER_PCF8574) {
		warning(CUSTOM_ERR_DISPLAY_MODE, "Unexpected displayMode %d", engineConfiguration->displayMode);
		// I2C pins need initialization, code needs more work & testing
		return;
	}

	printMsg(logger, "lcd_HD44780_init %d", engineConfiguration->displayMode);

	if (engineConfiguration->displayMode == DM_HD44780) {
		// initialize hardware lines
		efiSetPadMode("lcd RS", boardConfiguration->HD44780_rs, PAL_MODE_OUTPUT_PUSHPULL);
		efiSetPadMode("lcd E", boardConfiguration->HD44780_e, PAL_MODE_OUTPUT_PUSHPULL);
		efiSetPadMode("lcd DB4", boardConfiguration->HD44780_db4, PAL_MODE_OUTPUT_PUSHPULL);
		efiSetPadMode("lcd DB5", boardConfiguration->HD44780_db5, PAL_MODE_OUTPUT_PUSHPULL);
		efiSetPadMode("lcd DB6", boardConfiguration->HD44780_db6, PAL_MODE_OUTPUT_PUSHPULL);
		efiSetPadMode("lcd DB7", boardConfiguration->HD44780_db7, PAL_MODE_OUTPUT_PUSHPULL);
		// and zero values
		palWritePad(getHwPort("lcd", boardConfiguration->HD44780_rs), getHwPin("lcd", boardConfiguration->HD44780_rs), 0);
		palWritePad(getHwPort("lcd", boardConfiguration->HD44780_e), getHwPin("lcd", boardConfiguration->HD44780_e), 0);
		palWritePad(getHwPort("lcd", boardConfiguration->HD44780_db4), getHwPin("lcd", boardConfiguration->HD44780_db4), 0);
		palWritePad(getHwPort("lcd", boardConfiguration->HD44780_db5), getHwPin("lcd", boardConfiguration->HD44780_db5), 0);
		palWritePad(getHwPort("lcd", boardConfiguration->HD44780_db6), getHwPin("lcd", boardConfiguration->HD44780_db6), 0);
		palWritePad(getHwPort("lcd", boardConfiguration->HD44780_db7), getHwPin("lcd", boardConfiguration->HD44780_db7), 0);
	}

	chThdSleepMilliseconds(20); // LCD needs some time to wake up
	lcd_HD44780_write(LCD_HD44780_RESET); // reset 1x
	chThdSleepMilliseconds(1);
	lcd_HD44780_write(LCD_HD44780_RESET); // reset 2x
	lcd_HD44780_write(LCD_HD44780_RESET); // reset 3x

	lcd_HD44780_write(LCD_HD44780_4_BIT_BUS);	// 4 bit, 2 line
	chThdSleepMicroseconds(40);

	lcd_HD44780_write(LCD_HD44780_4_BIT_BUS);	// 4 bit, 2 line
	lcd_HD44780_write(0x80);
	chThdSleepMicroseconds(40);

	lcd_HD44780_write_command(0x08);	// display and cursor control
	chThdSleepMicroseconds(40);

	lcd_HD44780_write_command(LCD_HD44780_DISPLAY_CLEAR);
	chThdSleepMilliseconds(2);

	lcd_HD44780_write_command(LCD_HD44780_SHIFT_CURSOR_RIGHT);
	chThdSleepMilliseconds(2);

	lcd_HD44780_write_command(LCD_HD44780_DISPLAY_ON);

	lcd_HD44780_set_position(0, 0);
	printMsg(logger, "lcd_HD44780_init() done");
}

void lcdShowPanicMessage(char *message) {
	BUSY_WAIT_DELAY = TRUE;
	lcd_HD44780_set_position(0, 0);
	lcd_HD44780_print_string("PANIC\n");
	lcd_HD44780_print_string(message);
}

#endif /* EFI_HD44780_LCD */
