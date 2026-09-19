/*
 * @file spark_logic.h
 *
 * @date Sep 15, 2016
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

void onTriggerEventSparkLogic(float rpm, efitick_t edgeTimestamp, float currentPhase, float nextPhase, float nextNextPhase);
void scheduleSparkEvent(bool limitedSpark, IgnitionEvent *event,
		float rpm, float dwellMs, float dwellAngle, float sparkAngle,
		efitick_t edgeTimestamp, float currentPhase, float nextPhase,
		bool earlyWindow);
#if EFI_ANGLE_CLOCK
// Called EARLY in mainTriggerCallback (before handleFuel, ~20-30 µs after the
// tooth edge) to arm dwell starts on TMR2 while handoff elapsed time is still
// small. At 7000 rpm the scheduleEarly window (6°=143 µs) easily exceeds the
// 30 µs elapsed here; by the end of onTriggerEventSparkLogic (~250 µs elapsed)
// the same window falls below the arm margin and all arms fail.
void scheduleDwellEarlyIfDue(float rpm, efitick_t edgeTimestamp, float currentPhase,
                               float nextPhase, float nextNextPhase);
#endif
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
