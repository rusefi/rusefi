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
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#if EFI_HD44780_LCD
#include "os_access.h"

#include "lcd_controller.h"
#include "HD44780.h"
#include "rtc_helper.h"
#include "svnversion.h"
#include "joystick.h"
#include "utlist.h"
#include "lcd_menu_tree.h"
#include "memstreams.h"
#include "settings.h"
#include "bench_test.h"
#include "mmc_card.h"
#include "idle_thread.h"
#include "fuel_math.h"

static MenuItem ROOT(NULL, NULL);

static MenuTree tree(&ROOT);

/**
 * todo: add some comment explaining how this works
 */
static MenuItem miRpm(tree.root, LL_RPM);
static MenuItem miSensors(tree.root, "SENSORS");
static MenuItem miFuelControl(tree.root, "FUEL CONTROL");
static MenuItem miBench(tree.root, "BENCH TEST");
static MenuItem miAbout(tree.root, "ABOUT");

static MenuItem miTriggerErrors(&miRpm, LL_TRIGGER_ERRORS);
static MenuItem miTriggerDuty(&miRpm, LL_TRIGGER_DUTY);

static MenuItem miFuelCltCorr(&miFuelControl, LL_FUEL_CLT_CORRECTION);
static MenuItem miFuelIatCorr(&miFuelControl, LL_FUEL_IAT_CORRECTION);
static MenuItem miFuelInjectorLag(&miFuelControl, LL_FUEL_INJECTOR_LAG);

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

static MenuItem miStopEngine(&miBench, "stop engine", scheduleStopEngine);
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

static void showLine(lcd_line_e line, int /*screenY*/) {
	static char buffer[_MAX_FILLER + 2];

	switch (line) {
	case LL_VERSION:
		lcdPrintf("ver %s %d", VCS_VERSION, getRusEfiVersion());
		return;
	case LL_CONFIG:
		lcdPrintf("config %s", getConfigurationName(engineConfiguration->engineType));
		return;
	case LL_RPM:
	{
		int seconds = minI(9999, getTimeNowSeconds());
		lcdPrintf("RPM %d %d ", GET_RPM(), seconds);
	}
#if EFI_FILE_LOGGING
		{
			char sdState;
			if (CONFIG(isSdCardEnabled)) {
				sdState = isSdCardAlive() ? 'L' : 'n';
			} else {
				sdState = 'D';
			}

			lcdPrintf("%c", sdState);
		}
#endif
		return;
	case LL_CLT_TEMPERATURE:
		lcdPrintf("Coolant %.2f", Sensor::get(SensorType::Clt).value_or(0));
		return;
	case LL_IAT_TEMPERATURE:
		lcdPrintf("Intake Air %.2f", Sensor::get(SensorType::Iat).value_or(0));
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
		getPinNameByAdcChannel("tps", engineConfiguration->tps1_1AdcChannel, buffer);

		lcdPrintf("Throttle %s %.2f%%", buffer, Sensor::get(SensorType::Tps1).value_or(0));
		return;
	case LL_FUEL_CLT_CORRECTION:
		lcdPrintf("CLT corr %.2f", getCltFuelCorrection(PASS_ENGINE_PARAMETER_SIGNATURE));
		return;
	case LL_FUEL_IAT_CORRECTION:
		lcdPrintf("IAT corr %.2f", getIatFuelCorrection(PASS_ENGINE_PARAMETER_SIGNATURE));
		return;
	case LL_FUEL_INJECTOR_LAG:
		lcdPrintf("ING LAG %.2f", engine->engineState.running.injectorLag);
		return;
	case LL_VBATT:
		lcdPrintf("Battery %.2fv", Sensor::get(SensorType::BatteryVoltage).value_or(0));
		return;
	case LL_KNOCK:
		getPinNameByAdcChannel("hip", engineConfiguration->hipOutputChannel, buffer);
		lcdPrintf("Knock %s %.2fv", buffer, engine->knockVolts);
		return;

#if	EFI_ANALOG_SENSORS
	case LL_BARO:
		if (Sensor::hasSensor(SensorType::BarometricPressure)) {
			lcdPrintf("Baro: %.2f", getBaroPressure());
		} else {
			lcdPrintf("Baro: none");
		}
		return;
#endif
	case LL_AFR:
		if (Sensor::hasSensor(SensorType::Lambda1)) {
			lcdPrintf("AFR: %.2f", Sensor::get(SensorType::Lambda1).value_or(0));
		} else {
			lcdPrintf("AFR: none");
		}
		return;
	case LL_MAP:
		if (hasMapSensor(PASS_ENGINE_PARAMETER_SIGNATURE)) {
			lcdPrintf("MAP %.2f", Sensor::get(SensorType::Map).value_or(0));
		} else {
			lcdPrintf("MAP: none");
		}
		return;
	case LL_MAF_V:
		if (Sensor::hasSensor(SensorType::Maf)) {
			lcdPrintf("MAF: %.2fv", Sensor::getRaw(SensorType::Maf));
		} else {
			lcdPrintf("MAF: none");
		}
		return;
	case LL_MAF_KG_HR:
		if (Sensor::hasSensor(SensorType::Maf)) {
			lcdPrintf("MAF: %.2f kg/hr", Sensor::get(SensorType::Maf).value_or(0));
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
	for (; screenY < tree.linesCount && p != nullptr; screenY++) {
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


	const char * message = hasFirmwareErrorFlag ? getFirmwareError() : getWarningMessage();
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
//	char * ptr = itoa10(buffer, GET_RPM());
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
//	memcpy(buffer, getWarningMessage(), LCD_WIDTH);
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
