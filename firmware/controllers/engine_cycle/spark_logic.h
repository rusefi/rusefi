/*
 * @file spark_logic.h
 *
 * @date Sep 15, 2016
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

void onTriggerEventSparkLogic(float rpm, efitick_t edgeTimestamp, float currentPhase, float nextPhase, float nextNextPhase);
void turnSparkPinHighStartCharging(IgnitionEvent *event);
void fireSparkAndPrepareNextSchedule(IgnitionEvent *event);
// Overdwell watchdog: fires when the angle-queued spark did not fire on its
// expected tooth. Exported so the executor can attribute dispatch-lateness
// telemetry per command class (see classifyAction in single_timer_executor.cpp).
void overFireSparkAndPrepareNextSchedule(IgnitionEvent *event);
int getNumberOfSparks(ignition_mode_e mode);
// fact: getInjectorDutyCycle is used by limpManager as cut reason but coil duty cycle is only logged not considered for control strategy
// see also maxAllowedDwellAngle which only produces a warning without cutting spark
percent_t getCoilDutyCycle(float rpm);
void initializeIgnitionActions();
