/**
 * @file	mazda_626.cpp
 * MAZDA_626
 * set engine_type 28
 *
 * @date Jan 16, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "mazda_626.h"
#include "custom_engine.h"
#include "allsensors.h"
#include "fsio_impl.h"
#include "settings.h"

void setMazda626EngineConfiguration(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	setFrankensoConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);

	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
//	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;
	engineConfiguration->trigger.type = TT_60_2_VW;
//	engineConfiguration->trigger.type = TT_MAZDA_DOHC_1_4;

	// with this complex trigger we do not need this by default
	engineConfiguration->sensorChartMode = SC_OFF;

	engineConfiguration->useOnlyRisingEdgeForTrigger = true;

	engineConfiguration->injectionMode = IM_BATCH;
	// enable two_wire_batch_injection
	engineConfiguration->twoWireBatchInjection = true;
	// enable two_wire_wasted_spark
	engineConfiguration->twoWireBatchIgnition = true;

	// chartsize 600
	engineConfiguration->engineChartSize = 600;

	engineConfiguration->injector.flow = 330;
	engineConfiguration->specs.displacement = 2.0;

	// set global_trigger_offset_angle 106
	engineConfiguration->globalTriggerAngleOffset = 106;
	// set ignition_mode 2
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;

	// set cranking_timing_angle 15
	engineConfiguration->crankingTimingAngle = 8;

	// set cranking_fuel 9
	engineConfiguration->cranking.baseFuel = 30;

	engineConfiguration->rpmHardLimit = 7400;

	engineConfiguration->tpsMin = 80;
	engineConfiguration->tpsMax = 764;

	setAlgorithm(LM_SPEED_DENSITY PASS_CONFIG_PARAMETER_SUFFIX);

// set_whole_timing_map 10
	setWholeTimingTable_d(10 PASS_CONFIG_PARAMETER_SUFFIX);

	// http://s1.micp.ru/tOb0U.png
	engineConfiguration->clt.config = {-15, 23, 97, 5900, 1750, 165, 2700};
	// http://s2.micp.ru/I6Cfe.png
	engineConfiguration->iat.config = {23, 41, 97, 1750, 810, 165, 1820};

//	engineConfiguration->map.sensor.hwChannel = EFI_ADC_4;
	engineConfiguration->mafAdcChannel = EFI_ADC_NONE;

	// todo: 8.2 or 10k?
	engineConfiguration->vbattDividerCoeff = ((float) (10 + 33)) / 10 * 2;

	engineConfiguration->map.sensor.type = MT_SUBY_DENSO;

	// http://s2.micp.ru/vJ9Sd.png
	engineConfiguration->engineLoadAccelLength = 12;
	engineConfiguration->engineLoadAccelEnrichmentThreshold = 1;
	engineConfiguration->engineLoadAccelEnrichmentMultiplier = 0.25;

	engineConfiguration->tpsAccelLength = 12;
	engineConfiguration->tpsAccelEnrichmentThreshold = 10;


// todo	engineConfiguration->afr.hwChannel = EFI_ADC_3;
	setEgoSensor(ES_Innovate_MTX_L PASS_CONFIG_PARAMETER_SUFFIX);


//#if EFI_FSIO
//	// backup main relay pin
//	setFsio(0, GPIOE_6, "1" PASS_CONFIG_PARAMETER_SUFFIX);
//#endif

	engineConfiguration->externalKnockSenseAdc = EFI_ADC_4;

	engineConfiguration->triggerInputPins[1] = GPIO_UNASSIGNED;
	engineConfiguration->camInputs[0] = GPIOA_5;
}
