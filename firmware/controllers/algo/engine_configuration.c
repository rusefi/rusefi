/**
 * @file	engine_controller.c
 * @brief	Utility method related to the engine configuration data structure.
 *
 * @date Nov 22, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 *
 * This file is part of rusEfi - see http://rusefi.com
 *
 * rusEfi is free software; you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "main.h"
#include "engine_configuration.h"
#include "allsensors.h"
#include "interpolation.h"
#include "trigger_decoder.h"

#if EFI_PROD_CODE
#include "tunerstudio.h"
#endif

#include "audi_aan.h"
#include "dodge_neon.h"
#include "ford_aspire.h"
#include "ford_fiesta.h"
#include "ford_1995_inline_6.h"
#include "snow_blower.h"
#include "nissan_primera.h"
#include "honda_accord.h"
#include "GY6_139QMB.h"
#include "mazda_miata_nb.h"
#include "mazda_323.h"

#define ADC_CHANNEL_FAST_ADC 256

static volatile int globalConfigurationVersion = 0;

int getGlobalConfigurationVersion(void) {
	return globalConfigurationVersion;
}

void incrementGlobalConfigurationVersion(void) {
	globalConfigurationVersion++;
}


/**
 * @brief Sets the same dwell time across the whole getRpm() range
 */
void setConstantDwell(engine_configuration_s *engineConfiguration, float dwellMs) {
	for (int i = 0; i < DWELL_CURVE_SIZE; i++) {
		engineConfiguration->sparkDwellBins[i] = 1000 * i;
		engineConfiguration->sparkDwell[i] = dwellMs;
	}
}

void initBpsxD1Sensor(afr_sensor_s *sensor) {
	/**
	 * This decodes BPSX D1 Wideband Controller analog signal
	 */
	sensor->v1 = 0;
	sensor->value1 = 9;
	sensor->v2 = 5;
	sensor->value2 = 19;
}


/**
 * @brief	Global default engine configuration
 * This method sets the default global engine configuration. These values are later overridden by engine-specific defaults
 * and the settings saves in flash memory.
 */
void setDefaultConfiguration(engine_configuration_s *engineConfiguration) {
	engineConfiguration->injectorLag = 0.0;

	for (int i = 0; i < IAT_CURVE_SIZE; i++) {
		engineConfiguration->iatFuelCorrBins[i] = -40 + i * 10;
		engineConfiguration->iatFuelCorr[i] = 1; // this correction is a multiplier
	}

	for (int i = 0; i < CLT_CURVE_SIZE; i++) {
		engineConfiguration->cltFuelCorrBins[i] = -40 + i * 10;
		engineConfiguration->cltFuelCorr[i] = 1; // this correction is a multiplier
	}

//	setTableValue(engineConfiguration->cltFuelCorrBins, engineConfiguration->cltFuelCorr, CLT_CURVE_SIZE, -40, 1.5);
//	setTableValue(engineConfiguration->cltFuelCorrBins, engineConfiguration->cltFuelCorr, CLT_CURVE_SIZE, -30, 1.5);
//	setTableValue(engineConfiguration->cltFuelCorrBins, engineConfiguration->cltFuelCorr, CLT_CURVE_SIZE, -20, 1.42);
//	setTableValue(engineConfiguration->cltFuelCorrBins, engineConfiguration->cltFuelCorr, CLT_CURVE_SIZE, -10, 1.36);
//	setTableValue(engineConfiguration->cltFuelCorrBins, engineConfiguration->cltFuelCorr, CLT_CURVE_SIZE, 0, 1.28);
//	setTableValue(engineConfiguration->cltFuelCorrBins, engineConfiguration->cltFuelCorr, CLT_CURVE_SIZE, 10, 1.19);
//	setTableValue(engineConfiguration->cltFuelCorrBins, engineConfiguration->cltFuelCorr, CLT_CURVE_SIZE, 20, 1.12);
//	setTableValue(engineConfiguration->cltFuelCorrBins, engineConfiguration->cltFuelCorr, CLT_CURVE_SIZE, 30, 1.10);
//	setTableValue(engineConfiguration->cltFuelCorrBins, engineConfiguration->cltFuelCorr, CLT_CURVE_SIZE, 40, 1.06);
//	setTableValue(engineConfiguration->cltFuelCorrBins, engineConfiguration->cltFuelCorr, CLT_CURVE_SIZE, 50, 1.06);
//	setTableValue(engineConfiguration->cltFuelCorrBins, engineConfiguration->cltFuelCorr, CLT_CURVE_SIZE, 60, 1.03);
//	setTableValue(engineConfiguration->cltFuelCorrBins, engineConfiguration->cltFuelCorr, CLT_CURVE_SIZE, 70, 1.01);

	for (int i = 0; i < VBAT_INJECTOR_CURVE_SIZE; i++) {
		engineConfiguration->battInjectorLagCorrBins[i] = 12 - VBAT_INJECTOR_CURVE_SIZE / 2 + i;
		engineConfiguration->battInjectorLagCorr[i] = 0; // zero extra time by default
	}

	setConstantDwell(engineConfiguration, 4); // 4ms is global default dwell

	for (int k = 0; k < FUEL_LOAD_COUNT; k++) {
		for (int r = 0; r < FUEL_RPM_COUNT; r++) {
			// 3ms would be the global default
			engineConfiguration->fuelTable[k][r] = 3;
		}
	}

	setThermistorConfiguration(&engineConfiguration->cltThermistorConf, 0, 9500, 23.8889, 2100, 48.8889, 1000);
	engineConfiguration->cltThermistorConf.bias_resistor =  1500;

	setThermistorConfiguration(&engineConfiguration->iatThermistorConf, 32, 9500, 75, 2100, 120, 1000);
// todo: this value is way off! I am pretty sure temp coeffs are off also
	engineConfiguration->iatThermistorConf.bias_resistor = 2700;

	engineConfiguration->rpmHardLimit = 7000;
	engineConfiguration->crankingSettings.crankingRpm = 550;

	engineConfiguration->analogInputDividerCoefficient = 2;

	engineConfiguration->crankingChargeAngle = 70;
	engineConfiguration->timingMode = TM_DYNAMIC;
	engineConfiguration->fixedModeTiming = 50;

	engineConfiguration->analogChartMode = AC_TRIGGER;

	engineConfiguration->map.channel = ADC_CHANNEL_FAST_ADC;

	engineConfiguration->injectionPinMode = OM_DEFAULT;
	engineConfiguration->ignitionPinMode = OM_DEFAULT;
	engineConfiguration->idlePinMode = OM_DEFAULT;
	engineConfiguration->fuelPumpPinMode = OM_DEFAULT;
	engineConfiguration->malfunctionIndicatorPinMode = OM_DEFAULT;

	engineConfiguration->globalTriggerAngleOffset = 0;
	engineConfiguration->injectionOffset = 0;
	engineConfiguration->ignitionOffset = 0;


	engineConfiguration->engineLoadMode = LM_MAF;

	engineConfiguration->vbattDividerCoeff = ((float)(15 + 65)) / 15;

	engineConfiguration->fanOnTemperature = 75;
	engineConfiguration->fanOffTemperature = 70;

	engineConfiguration->can_nbc_type = CAN_BUS_NBC_BMW;
	engineConfiguration->can_sleep_period = 50;
	engineConfiguration->canReadEnabled = TRUE;
	engineConfiguration->canWriteEnabled = FALSE;

	/**
	 * 0.5 means primary position sensor is on a camshaft
	 */
	engineConfiguration->rpmMultiplier = 0.5;
	engineConfiguration->cylindersCount = 4;


	engineConfiguration->displayMode = DM_HD44780;

	engineConfiguration->logFormat = LF_NATIVE;

	engineConfiguration->triggerConfig.triggerType = TT_TOOTHED_WHEEL;
	engineConfiguration->triggerConfig.syncRatioFrom = 1.5;
	engineConfiguration->triggerConfig.syncRatioTo = 3;
	engineConfiguration->triggerConfig.isSynchronizationNeeded = TRUE;
	engineConfiguration->triggerConfig.useRiseEdge = TRUE;


	engineConfiguration->HD44780width = 16;
	engineConfiguration->HD44780height = 2;


	engineConfiguration->tpsAdcChannel = 3;
	engineConfiguration->vBattAdcChannel = 5;
	engineConfiguration->cltAdcChannel = 6;
	engineConfiguration->iatAdcChannel = 7;
	engineConfiguration->mafAdcChannel = 0;
	engineConfiguration->afrSensor.afrAdcChannel = 14;

	initBpsxD1Sensor(&engineConfiguration->afrSensor);

	engineConfiguration->globalFuelCorrection = 1;

	engineConfiguration->needSecondTriggerInput = TRUE;

	engineConfiguration->injectionPins[0] = GPIOB_9;
	engineConfiguration->injectionPins[1] = GPIOB_8;
	engineConfiguration->injectionPins[2] = GPIOE_3;
	engineConfiguration->injectionPins[3] = GPIOE_5;
	engineConfiguration->injectionPins[4] = GPIOE_6;
//	engineConfiguration->injectionPins[5] = GPIOE_5;

	engineConfiguration->ignitionPins[0] = GPIOC_7;
	engineConfiguration->ignitionPins[1] = GPIOE_4; // todo: update this value
	engineConfiguration->ignitionPins[2] = GPIOE_0; // todo: update this value
	engineConfiguration->ignitionPins[3] = GPIOE_1; // todo: update this value

	engineConfiguration->map.config.mapType = MT_CUSTOM;
	engineConfiguration->map.config.Min = 0;
	engineConfiguration->map.config.Max = 500;
}

void setDefaultNonPersistentConfiguration(engine_configuration2_s *engineConfiguration2) {
	/**
	 * 720 is the range for four stroke
	 */
	engineConfiguration2->crankAngleRange = 720;

	engineConfiguration2->hasMapSensor = TRUE;
	engineConfiguration2->hasCltSensor = TRUE;
}

void resetConfigurationExt(engine_type_e engineType, engine_configuration_s *engineConfiguration, engine_configuration2_s *engineConfiguration2) {
	/**
	 * Let's apply global defaults first
	 */
	setDefaultConfiguration(engineConfiguration);
	engineConfiguration->engineType = engineType;
	/**
	 * And override them with engine-specific defaults
	 */
	switch (engineType) {
#if EFI_SUPPORT_DODGE_NEON || defined(__DOXYGEN__)
	case DODGE_NEON_1995:
		setDodgeNeonEngineConfiguration(engineConfiguration);
		break;
#endif /* EFI_SUPPORT_DODGE_NEON */
#if EFI_SUPPORT_FORD_ASPIRE || defined(__DOXYGEN__)
	case FORD_ASPIRE_1996:
		setFordAspireEngineConfiguration(engineConfiguration);
		break;
#endif /* EFI_SUPPORT_FORD_ASPIRE */
#if EFI_SUPPORT_FORD_FIESTA || defined(__DOXYGEN__)
	case FORD_FIESTA:
		setFordFiestaDefaultEngineConfiguration(engineConfiguration);
		break;
#endif /* EFI_SUPPORT_FORD_FIESTA */
	case HONDA_ACCORD:
		setHondaAccordConfiguration(engineConfiguration);
		break;
#if EFI_SUPPORT_1995_FORD_INLINE_6 || defined(__DOXYGEN__)
	case FORD_INLINE_6_1995:
		setFordInline6(engineConfiguration);
		break;
#endif /* EFI_SUPPORT_1995_FORD_INLINE_6 */
	case GY6_139QMB:
		setGy6139qmbDefaultEngineConfiguration(engineConfiguration);
		break;
	case MAZDA_MIATA_NB:
		setMazdaMiataNbEngineConfiguration(engineConfiguration);
		break;
	case MAZDA_323:
		setMazda323EngineConfiguration(engineConfiguration);
		break;
	default:
		firmwareError("Unexpected engine type: %d", engineType);

	}
	applyNonPersistentConfiguration(engineConfiguration, engineConfiguration2, engineType);

#if EFI_TUNER_STUDIO
	syncTunerStudioCopy();
#endif
}

void applyNonPersistentConfiguration(engine_configuration_s *engineConfiguration, engine_configuration2_s *engineConfiguration2, engine_type_e engineType) {
	initializeTriggerShape(engineConfiguration, engineConfiguration2);
	chDbgCheck(engineConfiguration2->triggerShape.size != 0, "size is zero");
	chDbgCheck(engineConfiguration2->triggerShape.shaftPositionEventCount, "shaftPositionEventCount is zero");

	switch (engineConfiguration->engineType) {
#if EFI_SUPPORT_DODGE_NEON
	case DODGE_NEON_1995:
		setDodgeNeonengine_configuration2_s(engineConfiguration, engineConfiguration2);
		break;
#endif /* EFI_SUPPORT_DODGE_NEON */
#if EFI_SUPPORT_FORD_ASPIRE
	case FORD_ASPIRE_1996:
		setFordAspireengine_configuration2_s(engineConfiguration, engineConfiguration2);
		break;
#endif /* EFI_SUPPORT_FORD_ASPIRE */
#if EFI_SUPPORT_FORD_FIESTA
	case FORD_FIESTA:
		setFordFiestaengine_configuration2_s(engineConfiguration, engineConfiguration2);
		break;
#endif /* EFI_SUPPORT_FORD_FIESTA */
#if EFI_SUPPORT_NISSAN_PRIMERA
		case NISSAN_PRIMERA:
		setNissanPrimeraengine_configuration2_s(engineConfiguration, engineConfiguration2);
		break;
#endif /* EFI_SUPPORT_NISSAN_PRIMERA */
	case HONDA_ACCORD:
		setHondaAccordConfiguration2(engineConfiguration, engineConfiguration2);
		break;
#if EFI_SUPPORT_1995_FORD_INLINE_6 || defined(__DOXYGEN__)
	case FORD_INLINE_6_1995:
		setFordInline6_2(engineConfiguration, engineConfiguration2);
		break;
#endif /* EFI_SUPPORT_1995_FORD_INLINE_6 */
	case GY6_139QMB:
		setGy6139qmbengine_configuration2_s(engineConfiguration, engineConfiguration2);
		break;
	case MAZDA_MIATA_NB:
		setMazdaMiataNb_configuration2_s(engineConfiguration, engineConfiguration2);
		break;
	case MAZDA_323:
		setMazda323configuration2_s(engineConfiguration, engineConfiguration2);
		break;
	default:
		fatal("Unexpected engine type")
		;
	}

}
