/**
 * @file	trigger_mitsubishi.h
 *
 * @date Aug 5, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

class TriggerWaveform;

void initializeMitsubishi4g18(TriggerWaveform *s);
void initializeMitsubishi4g93_both_both(TriggerWaveform *s);
void initializeMitsubishi4g93_only_first_wheel_both_fronts(TriggerWaveform *s);
void configureFordAspireTriggerWaveform(TriggerWaveform * s);

void initialize36_2_1_1(TriggerWaveform *s);
void initialize36_2_1(TriggerWaveform *s);

void initializeVvt3A92(TriggerWaveform *s);
void initializeVvt6G75(TriggerWaveform *s);
