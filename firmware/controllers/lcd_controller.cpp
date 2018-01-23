/**
 * @file lcd_controller.cpp
 *
 * LCD is a tree-like set of menues controlled by a joystick. At the moment three actions are supported:
 *  "next item"
 *  "enter sub-menu"
 *  "return one level up"
 *
 * In case of a 20x4 display top three lines are three menu items you can scroll through and
 * bottom line is error or warning line.
 *
 * By default the first line shows current RPM and time since boot in seconds.
 * The "more" symbol points at the currently selected menu item.
 *
 * @date Aug 14, 2014
 * @author Andrey Belomutskiy, (c) 2012-2018
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
#include "injector_central.h"
#include "engine_controller.h"
#include "mmc_card.h"
#include "idle_thread.h"
#include "fuel_math.h"

#if EFI_HD44780_LCD || defined(__DOXYGEN__)

EXTERN_ENGINE
;
extern bool hasFirmwareErrorFlag;
extern fatal_msg_t errorMessageBuffer;

static MenuItem ROOT(NULL, NULL);

static MenuTree tree(&ROOT);

/**
 * todo: add some comment explaining how this works
 */
static MenuItem miRpm(tree.root, LL_RPM);
static MenuItem miSensors(tree.root, "sensors");
static MenuItem miFuelControl(tree.root, "fuel");
static MenuItem miBench(tree.root, "bench test");
static MenuItem miAbout(tree.root, "about");

static MenuItem miTriggerErrors(&miRpm, LL_TRIGGER_ERRORS);
static MenuItem miTriggerDuty(&miRpm, LL_TRIGGER_DUTY);

static MenuItem miFuelCltCorr(&miFuelControl, LL_FUEL_CLT_CORRECTION);

static MenuItem miClt(&miSensors, LL_CLT_TEMPERATURE);
static MenuItem miIat(&miSensors, LL_IAT_TEMPERATURE);
static MenuItem miTps(&miSensors, LL_TPS);
static MenuItem miVBatt(&miSensors, LL_VBATT);
static MenuItem miMap(&miSensors, LL_MAP);
static MenuItem miAfr(&miSensors, LL_AFR);
static MenuItem miBaro(&miSensors, LL_BARO);
static MenuItem miMapV(&miSensors, LL_MAF_V);
static MenuItem miMapKgHr(&miSensors, LL_MAF_KG_HR);
static MenuItem miKnock(&miSensors, LL_KNOCK);

static MenuItem miStopEngine(&miBench, "stop engine", stopEngine);
static MenuItem miTestFan(&miBench, "test fan", fanBench);
static MenuItem miTestFuelPump(&miBench, "test pump", fuelPumpBench);
static MenuItem miTestMIL(&miBench, "test MIL", milBench);
static MenuItem miTestIAC(&miBench, "test IAC", startIdleBench);
// todo: looks like these are not finished yet?
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
static MenuItem miInjection(&miAbout, LL_INJECTION);
static MenuItem miIgnition(&miAbout, LL_IGNITION);
static MenuItem miInjFlow(&miAbout, LL_ING_FLOW);

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

static const char* ignitionModeStr[] = { "1C", "IND", "WS" };
static const char* injectionModeStr[] = { "Sim", "Seq", "Bch" };
static const char* idleModeStr[] = { "I:A", "I:M" };

//static const char *getPinShortName(io_pin_e pin) {
//	switch (pin) {
//	case ALTERNATOR_SWITCH:
//		return "AL";
//	case FUEL_PUMP_RELAY:
//		return "FP";
//	case FAN_RELAY:
//		return "FN";
//	case O2_HEATER:
//		return "O2H";
//	default:
//		firmwareError(OBD_PCM_Processor_Fault, "No short name for %d", (int) pin);
//		return "";
//	}
//}

//char * appendPinStatus(char *buffer, io_pin_e pin) {
//	char *ptr = appendStr(buffer, getPinShortName(pin));
//	int state = getOutputPinValue(pin);
//	// todo: should we handle INITIAL_PIN_STATE?
//	if (state) {
//		return appendStr(ptr, ":Y ");
//	} else {
//		return appendStr(ptr, ":n ");
//	}
//}

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

//static char * prepareStatusLine(char *buffer) {
//	char *ptr = buffer;
//
//	ptr = appendPinStatus(ptr, FUEL_PUMP_RELAY);
//	ptr = appendPinStatus(ptr, FAN_RELAY);
//	ptr = appendPinStatus(ptr, O2_HEATER);
//	return ptr;
//}

static char buffer[MAX_LCD_WIDTH + 4];

static void lcdPrintf(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	// todo: migrate to chsnprintf
	lcdLineStream.eos = 0; // reset
	chvprintf((BaseSequentialStream *) &lcdLineStream, fmt, ap);
	lcdLineStream.buffer[lcdLineStream.eos] = 0; // terminator
	va_end(ap);
	lcd_HD44780_print_string(lcdLineBuffer);
}

static void showLine(lcd_line_e line, int screenY) {
	static char buffer[10];

	switch (line) {
	case LL_VERSION:
		lcdPrintf("version %s", VCS_VERSION);
		return;
	case LL_CONFIG:
		lcdPrintf("config %s", getConfigurationName(engineConfiguration->engineType));
		return;
	case LL_RPM:
		lcdPrintf("RPM %d", getRpmE(engine));
#if EFI_FILE_LOGGING || defined(__DOXYGEN__)
		{
			char sdState;
			if (boardConfiguration->isSdCardEnabled) {
				sdState = isSdCardAlive() ? 'L' : 'n';
			} else {
				sdState = 'D';
			}
			efitimesec_t seconds = getTimeNowSeconds();
			if (seconds < 10000) {
				lcdPrintf("  %d%c", seconds, sdState);
			}
		}
#endif
		return;
	case LL_CLT_TEMPERATURE:
		lcdPrintf("Coolant %.2f", getCoolantTemperature(PASS_ENGINE_PARAMETER_SIGNATURE));
		return;
	case LL_IAT_TEMPERATURE:
		lcdPrintf("Intake Air %.2f", getIntakeAirTemperature(PASS_ENGINE_PARAMETER_SIGNATURE));
		return;
	case LL_ALGORITHM:
		lcdPrintf(getEngine_load_mode_e(engineConfiguration->fuelAlgorithm));
		return;
	case LL_INJECTION:
		lcdPrintf(getInjection_mode_e(engineConfiguration->injectionMode));
		return;
	case LL_ING_FLOW:
		lcdPrintf("Inj %.2fcc", engineConfiguration->injector.flow);
		return;
	case LL_IGNITION:
		lcdPrintf(getIgnition_mode_e(engineConfiguration->ignitionMode));
		return;
	case LL_TPS:
		getPinNameByAdcChannel("tps", engineConfiguration->tpsAdcChannel, buffer);

		lcdPrintf("Throttle %s %.2f%%", buffer, getTPS());
		return;
	case LL_FUEL_CLT_CORRECTION:
		lcdPrintf("CLT corr %.2fv", getCltFuelCorrection(PASS_ENGINE_PARAMETER_SIGNATURE));
		return;

	case LL_VBATT:
		lcdPrintf("Battery %.2fv", getVBatt(PASS_ENGINE_PARAMETER_SIGNATURE));
		return;
	case LL_KNOCK:
		getPinNameByAdcChannel("hip", engineConfiguration->hipOutputChannel, buffer);
		lcdPrintf("Knock %s %.2fv", buffer, engine->knockVolts);
		return;

#if	EFI_ANALOG_SENSORS || defined(__DOXYGEN__)
	case LL_BARO:
		if (hasBaroSensor()) {
			lcdPrintf("Baro: %.2f", getBaroPressure());
		} else {
			lcdPrintf("Baro: none");
		}
		return;
#endif
	case LL_AFR:
		if (hasAfrSensor(PASS_ENGINE_PARAMETER_SIGNATURE)) {
			lcdPrintf("AFR: %.2f", getAfr());
		} else {
			lcdPrintf("AFR: none");
		}
		return;
	case LL_MAP:
		if (hasMapSensor(PASS_ENGINE_PARAMETER_SIGNATURE)) {
			lcdPrintf("MAP %.2f", getMap());
		} else {
			lcdPrintf("MAP: none");
		}
		return;
	case LL_MAF_V:
		if (hasMafSensor()) {
			lcdPrintf("MAF: %.2fv", getMaf(PASS_ENGINE_PARAMETER_SIGNATURE));
		} else {
			lcdPrintf("MAF: none");
		}
		return;
	case LL_MAF_KG_HR:
		if (hasMafSensor()) {
			lcdPrintf("MAF: %.2f kg/hr", getRealMaf(PASS_ENGINE_PARAMETER_SIGNATURE));
		} else {
			lcdPrintf("MAF: none");
		}
		return;
	case LL_TRIGGER_ERRORS:
		lcdPrintf("Errors");
		return;
	case LL_TRIGGER_DUTY:
		lcdPrintf("Duty");
		return;
	default:
		lcdPrintf("()");
	}
}

static void fillWithSpaces(void) {
	int column = getCurrentHD44780column();
	for (int r = column; r < 20; r++) {
		lcd_HD44780_print_char(' ');
	}
}

void updateHD44780lcd(void) {
	MenuItem *p = tree.topVisible;
	int screenY = 0;
	for (; screenY < tree.linesCount && p != NULL; screenY++) {
		lcd_HD44780_set_position(screenY, 0);
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
			showLine(p->lcdLine, screenY);
		}
		fillWithSpaces();
		p = p->next;
	}

	for (; screenY < tree.linesCount; screenY++) {
		lcd_HD44780_set_position(screenY, 0);
		fillWithSpaces();
	}


	const char * message = hasFirmwareErrorFlag ? getFirmwareError() : getWarning();
	memcpy(buffer, message, engineConfiguration->HD44780width);
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
//	memcpy(buffer, getWarning(), LCD_WIDTH);
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

#endif
