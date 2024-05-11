/*
 * @file spark_logic.h
 *
 * @date Sep 15, 2016
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

void onTriggerEventSparkLogic(int rpm, efitick_t edgeTimestamp, float currentPhase, float nextPhase);
void turnSparkPinHighStartCharging(IgnitionEvent *event);
void fireSparkAndPrepareNextSchedule(IgnitionEvent *event);
int getNumberOfSparks(ignition_mode_e mode);
// fact: getInjectorDutyCycle is used by limpManager as cut reason but coil duty cycle is only logged not considered for control strategy
// see also maxAllowedDwellAngle which only produces a warning without cutting spark
percent_t getCoilDutyCycle(int rpm);
void initializeIgnitionActions();
