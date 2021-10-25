/*
 * @file dodge_ram.cpp
 *
 * set engine_type 31
 *
 * @date Apr 22, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "dodge_ram.h"
#include "custom_engine.h"

void setDodgeRam1996(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	setFrankensoConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);

	setOperationMode(engineConfiguration, FOUR_STROKE_CAM_SENSOR);
	engineConfiguration->trigger.type = TT_DODGE_RAM;
	engineConfiguration->injector.flow = 243.6; // 23.2lb/h
	engineConfiguration->map.sensor.type = MT_DODGE_NEON_2003;

	//Base engine setting
	engineConfiguration->specs.cylindersCount = 8;
	engineConfiguration->specs.firingOrder = FO_1_8_4_3_6_5_7_2;
	engineConfiguration->specs.displacement = 5.2;

	// set global_trigger_offset_angle -50
	engineConfiguration->globalTriggerAngleOffset = -50;

	setDodgeSensor(&engineConfiguration->clt, 2700);
	setDodgeSensor(&engineConfiguration->iat, 2700);

	engineConfiguration->useStepperIdle = true;
}
