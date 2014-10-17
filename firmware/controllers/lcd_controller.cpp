/**
 * @file lcd_controller.cpp
 *
 * @date Aug 14, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "lcd_controller.h"
#include "lcd_HD44780.h"
#include "efilib.h"
#include "rpm_calculator.h"
#include "allsensors.h"
#include "engine.h"
#include "rtc_helper.h"
#include "io_pins.h"
#include "efiGpio.h"

extern Engine engine;
extern engine_configuration_s *engineConfiguration;

#define LCD_WIDTH 20
// this value should be even
#define NUMBER_OF_DIFFERENT_LINES 4

char * appendStr(char *ptr, const char *suffix) {
	for (uint32_t i = 0; i < efiStrlen(suffix); i++) {
		*ptr++ = suffix[i];
	}
	return ptr;
}

static char * prepareVBattMapLine(char *buffer) {
	char *ptr = buffer;
	*ptr++ = 'V';
	ptr = ftoa(ptr, getVBatt(), 10.0f);

	ptr = appendStr(ptr, " M");
	ptr = ftoa(ptr, getRawMap(), 10.0f);
	return ptr;
}

static char * prepareCltIatTpsLine(char *buffer) {
	engine_configuration2_s *engineConfiguration2 = engine.engineConfiguration2;
	char *ptr = buffer;
	*ptr++ = 'C';
	ptr = ftoa(ptr, getCoolantTemperature(engineConfiguration2), 10.0f);

	ptr = appendStr(ptr, " C");
	ptr = ftoa(ptr, getIntakeAirTemperature(engineConfiguration2), 10.0f);

	ptr = appendStr(ptr, " TP");
	ptr = itoa10(ptr, (int) getTPS(engine.engineConfiguration));
	return ptr;
}

static const char* algorithmStr[] = { "MAF", "TPS", "MAP", "SD" };
static const char* ignitionModeStr[] = { "1C", "IND", "WS" };
static const char* injectionModeStr[] = { "Sim", "Seq", "Bch" };
static const char* idleModeStr[] = { "I:A", "I:M" };

static const char *getPinShortName(io_pin_e pin) {
	switch (pin) {
	case ALTERNATOR_SWITCH:
		return "AL";
	case FUEL_PUMP_RELAY:
		return "FP";
	case FAN_RELAY:
		return "FN";
	case O2_HEATER:
		return "O2H";
	default:
		firmwareError("No short name for %d", (int) pin);
		return "";
	}
}

char * appendPinStatus(char *buffer, io_pin_e pin) {
	char *ptr = appendStr(buffer, getPinShortName(pin));
	int state = getOutputPinValue(pin);
	// todo: should we handle INITIAL_PIN_STATE?
	if (state) {
		return appendStr(ptr, ":Y ");
	} else {
		return appendStr(ptr, ":n ");
	}
}

static char * prepareInfoLine(char *buffer) {
	char *ptr = buffer;

	ptr = appendStr(ptr, algorithmStr[engineConfiguration->algorithm]);

	ptr = appendStr(ptr, " ");
	ptr = appendStr(ptr, ignitionModeStr[engineConfiguration->ignitionMode]);

	ptr = appendStr(ptr, " ");
	ptr = appendStr(ptr, injectionModeStr[engineConfiguration->injectionMode]);

	ptr = appendStr(ptr, " ");
	ptr = appendStr(ptr, idleModeStr[engineConfiguration->idleMode]);

	ptr = appendStr(ptr, " ");
	return ptr;
}

static char * prepareStatusLine(char *buffer) {
	char *ptr = buffer;

	ptr = appendPinStatus(ptr, FUEL_PUMP_RELAY);
	ptr = appendPinStatus(ptr, FAN_RELAY);
	ptr = appendPinStatus(ptr, O2_HEATER);
	return ptr;
}

static char buffer[LCD_WIDTH + 4];
static char dateBuffer[30];

static void prepareCurrentSecondLine(int index) {
	memset(buffer, ' ', LCD_WIDTH);
	char *ptr;
	switch (index) {
	case 0:
		ptr = prepareCltIatTpsLine(buffer);
		break;
	case 1:
		ptr = prepareInfoLine(buffer);
		break;
	case 2:
		ptr = prepareVBattMapLine(buffer);
		break;
	case 3:
		ptr = prepareStatusLine(buffer);
		break;
	default:
		firmwareError("unexpected case");
		return;
	}
	*ptr = ' ';
}

void updateHD44780lcd(void) {

	lcd_HD44780_set_position(0, 9);
	/**
	 * this would blink so that we know the LCD is alive
	 */
	if (getTimeNowSeconds() % 2 == 0) {
		lcd_HD44780_print_char('R');
	} else {
		lcd_HD44780_print_char(' ');
	}
	lcd_HD44780_set_position(0, 10);

	char * ptr = itoa10(buffer, getRpm());
	ptr[0] = 0;
	int len = ptr - buffer;
	for (int i = 0; i < 6 - len; i++) {
		lcd_HD44780_print_char(' ');
	}
	lcd_HD44780_print_string(buffer);

	if (hasFirmwareError()) {
		memcpy(buffer, getFirmwareError(), LCD_WIDTH);
		buffer[LCD_WIDTH] = 0;
		lcd_HD44780_set_position(1, 0);
		lcd_HD44780_print_string(buffer);
		return;
	}

	lcd_HD44780_set_position(1, 0);
	memset(buffer, ' ', LCD_WIDTH);
	memcpy(buffer, getWarninig(), LCD_WIDTH);
	buffer[LCD_WIDTH] = 0;
	lcd_HD44780_print_string(buffer);

	if (engineConfiguration->HD44780height < 3) {
		return;
	}

	int index = (getTimeNowSeconds() / 2) % (NUMBER_OF_DIFFERENT_LINES / 2);

	prepareCurrentSecondLine(index);
	buffer[LCD_WIDTH] = 0;
	lcd_HD44780_set_position(2, 0);
	lcd_HD44780_print_string(buffer);

	prepareCurrentSecondLine(index + NUMBER_OF_DIFFERENT_LINES / 2);
	buffer[LCD_WIDTH] = 0;
	lcd_HD44780_set_position(3, 0);
	lcd_HD44780_print_string(buffer);

#if EFI_PROD_CODE
	dateToString(dateBuffer);
	lcd_HD44780_set_position(1, 0);
	lcd_HD44780_print_string(dateBuffer);
#endif /* EFI_PROD_CODE */
}
