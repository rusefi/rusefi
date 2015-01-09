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
#include "svnversion.h"
#include "joystick.h"
#include "utlist.h"
#include "lcd_menu_tree.h"
#include "memstreams.h"
#include "settings.h"

EXTERN_ENGINE
;

static MenuItem ROOT(NULL, NULL);

static MenuTree tree(&ROOT);

static MenuItem miRpm(tree.root, LL_RPM);
static MenuItem miSensors(tree.root, "sensors");
static MenuItem miBench(tree.root, "bench test");
static MenuItem miAbout(tree.root, "about");

static MenuItem miTriggerErrors(&miRpm, LL_TRIGGER_ERRORS);
static MenuItem miTriggerDuty(&miRpm, LL_TRIGGER_DUTY);

static MenuItem miClt(&miSensors, LL_CLT_TEMPERATURE);
static MenuItem miIat(&miSensors, LL_IAT_TEMPERATURE);
static MenuItem miTps(&miSensors, LL_TPS);
static MenuItem miVBatt(&miSensors, LL_VBATT);
static MenuItem miMap(&miSensors, LL_MAP);
static MenuItem miBaro(&miSensors, LL_BARO);

static MenuItem miStopEngine(&miBench, "stop engine", stopEngine);
static MenuItem miTestFan(&miBench, "test fan");
static MenuItem miTestFuelPump(&miBench, "test pump");
static MenuItem miTestSpark1(&miBench, "test spark1");
static MenuItem miTestSpark2(&miBench, "test spark2");
static MenuItem miTestSpark3(&miBench, "test spark3");
static MenuItem miTestSpark4(&miBench, "test spark4");
static MenuItem miTestInj1(&miBench, "test injector1");
static MenuItem miTestInj2(&miBench, "test injector2");
static MenuItem miTestInj3(&miBench, "test injector3");
static MenuItem miTestInj4(&miBench, "test injector4");

static MenuItem miVersion(&miAbout, LL_VERSION);
static MenuItem miConfig(&miAbout, LL_CONFIG);
static MenuItem miAlgo(&miAbout, LL_ALGORITHM);

#define DISP_LINES (engineConfiguration->HD44780height - 1)

static char lcdLineBuffer[30];
static MemoryStream lcdLineStream;

void onJoystick(joystick_button_e button) {
	/**
	 * this method is invoked on EXTI IRQ thread
	 */
	if (button == JB_CENTER) {
		tree.enterSubMenu();
	} else if (button == JB_BUTTON_D) {
		tree.nextItem();
	} else if (button == JB_BUTTON_A) {
		tree.back();
	}
	// actual repaint happends in the repaint loop
}

char * appendStr(char *ptr, const char *suffix) {
	for (uint32_t i = 0; i < efiStrlen(suffix); i++) {
		*ptr++ = suffix[i];
	}
	return ptr;
}

void initLcdController(void) {
	tree.init(&miRpm, engineConfiguration->HD44780height - 1);
	msObjectInit(&lcdLineStream, (uint8_t *) lcdLineBuffer, sizeof(lcdLineBuffer), 0);
}

static char * prepareVBattMapLine(engine_configuration_s *engineConfiguration, char *buffer) {
	char *ptr = buffer;
	*ptr++ = 'V';
	ptr = ftoa(ptr, getVBatt(engineConfiguration), 10.0f);

	ptr = appendStr(ptr, " M");
	ptr = ftoa(ptr, getRawMap(), 10.0f);
	return ptr;
}

static char * prepareCltIatTpsLine(Engine *engine, char *buffer) {
	char *ptr = buffer;
	*ptr++ = 'C';

	ptr = appendStr(ptr, " TP");
	ptr = itoa10(ptr, (int) getTPS(PASS_ENGINE_PARAMETER_F));
	return ptr;
}

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

static char * prepareInfoLine(engine_configuration_s *engineConfiguration, char *buffer) {
	char *ptr = buffer;

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

static char buffer[MAX_LCD_WIDTH + 4];
static char dateBuffer[30];

static void lcdPrintf(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	lcdLineStream.eos = 0; // reset
	chvprintf((BaseSequentialStream *) &lcdLineStream, fmt, ap);
	lcdLineStream.buffer[lcdLineStream.eos] = 0; // terminator
	va_end(ap);
	lcd_HD44780_print_string(lcdLineBuffer);
}

static void showLine(lcd_line_e line) {

	switch (line) {
	case LL_VERSION:
		lcdPrintf("version %s", VCS_VERSION);
		return;
	case LL_CONFIG:
		lcdPrintf("config %s", getConfigurationName(engine->engineConfiguration->engineType));
		return;
	case LL_RPM:
		lcdPrintf("RPM %d", getRpmE(engine));
		return;
	case LL_CLT_TEMPERATURE:
		lcdPrintf("Coolant %f", getCoolantTemperature(engine));
		return;
	case LL_IAT_TEMPERATURE:
		lcdPrintf("Intake Air %f", getIntakeAirTemperature(engine));
		return;
	case LL_ALGORITHM:
		lcdPrintf(getEngine_load_mode_e(engineConfiguration->algorithm));
		return;
	case LL_TPS:
		lcdPrintf("Throttle %f%%", getTPS());
		return;
	case LL_VBATT:
		lcdPrintf("Battery %fv", getVBatt(engineConfiguration));
		return;
	case LL_BARO:
		if (engineConfiguration->hasBaroSensor) {
			lcdPrintf("Baro: none");
		} else {
			lcdPrintf("Baro: %f", getBaroPressure());
		}
		return;
	case LL_MAP:
		if (engineConfiguration->hasMapSensor) {
			lcdPrintf("MAP: none");
		} else {
			lcdPrintf("MAP %s", getMap());
		}
		return;
	case LL_TRIGGER_ERRORS:
		lcdPrintf("Errors");
		return;
	case LL_TRIGGER_DUTY:
		lcdPrintf("Duty");
		return;
	}
}

static void fillWithSpaces(void) {
	int column = getCurrentHD44780column();
	for (int r = column; r < 20; r++) {
		lcd_HD44780_print_char(' ');
	}
}

void updateHD44780lcd(Engine *engine) {
	MenuItem *p = tree.topVisible;
	int count = 0;
	for (; count < tree.linesCount && p != NULL; count++) {
		lcd_HD44780_set_position(count, 0);
		char firstChar;
		if (p == tree.current) {
			if (p->callback != NULL) {
				firstChar = '!';
			} else {
				firstChar = p->firstChild == NULL ? '*' : '>';
			}
		} else {
			firstChar = ' ';
		}
		lcd_HD44780_print_char(firstChar);
		if (p->lcdLine == LL_STRING) {
			lcd_HD44780_print_string(p->text);
		} else {
			showLine(p->lcdLine);
		}
		fillWithSpaces();
		p = p->next;
	}

	for (; count < tree.linesCount; count++) {
		lcd_HD44780_set_position(count, 0);
		fillWithSpaces();
	}

	memcpy(buffer, getWarninig(), engineConfiguration->HD44780width);
	buffer[engineConfiguration->HD44780width] = 0;
	lcd_HD44780_set_position(engineConfiguration->HD44780height - 1, 0);
	lcd_HD44780_print_string(buffer);
	fillWithSpaces();

//
//	lcd_HD44780_set_position(0, 9);
//	/**
//	 * this would blink so that we know the LCD is alive
//	 */
//	if (isEven) {
//		lcd_HD44780_print_char('R');
//	} else {
//		lcd_HD44780_print_char(' ');
//	}
//	lcd_HD44780_set_position(0, 10);
//
//	char * ptr = itoa10(buffer, getRpmE(engine));
//	ptr[0] = 0;
//	int len = ptr - buffer;
//	for (int i = 0; i < 6 - len; i++) {
//		lcd_HD44780_print_char(' ');
//	}
//	lcd_HD44780_print_string(buffer);
//
//	if (hasFirmwareError()) {
//		memcpy(buffer, getFirmwareError(), LCD_WIDTH);
//		buffer[LCD_WIDTH] = 0;
//		lcd_HD44780_set_position(1, 0);
//		lcd_HD44780_print_string(buffer);
//		return;
//	}
//
//	lcd_HD44780_set_position(1, 0);
//	memset(buffer, ' ', LCD_WIDTH);
//	memcpy(buffer, getWarninig(), LCD_WIDTH);
//	buffer[LCD_WIDTH] = 0;
//	lcd_HD44780_print_string(buffer);
//
//	if (engineConfiguration->HD44780height < 3) {
//		return;
//	}
//
//	int index = (getTimeNowSeconds() / 2) % (NUMBER_OF_DIFFERENT_LINES / 2);
//
//	prepareCurrentSecondLine(engine, index);
//	buffer[LCD_WIDTH] = 0;
//	lcd_HD44780_set_position(2, 0);
//	lcd_HD44780_print_string(buffer);
//
//	prepareCurrentSecondLine(engine, index + NUMBER_OF_DIFFERENT_LINES / 2);
//	buffer[LCD_WIDTH] = 0;
//	lcd_HD44780_set_position(3, 0);
//	lcd_HD44780_print_string(buffer);
//
//#if EFI_PROD_CODE
//	dateToString(dateBuffer);
//	lcd_HD44780_set_position(1, 0);
//	lcd_HD44780_print_string(dateBuffer);
//#endif /* EFI_PROD_CODE */
}
