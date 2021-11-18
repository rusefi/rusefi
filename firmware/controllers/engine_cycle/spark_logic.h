/*
 * @file spark_logic.h
 *
 * @date Sep 15, 2016
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

void onTriggerEventSparkLogic(bool limitedSpark, uint32_t trgEventIndex, int rpm, efitick_t edgeTimestamp);
void turnSparkPinHigh(IgnitionEvent *event);
void fireSparkAndPrepareNextSchedule(IgnitionEvent *event);
int getNumberOfSparks(ignition_mode_e mode);
percent_t getCoilDutyCycle(int rpm);
void initializeIgnitionActions();

int isIgnitionTimingError(void);
