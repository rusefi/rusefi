/**
 * @file	trigger_mitsubishi.h
 *
 * @date Aug 5, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

class TriggerWaveform;

void initializeMitsubishi4G93(TriggerWaveform *s);
void initializeMitsubishi4g63Cam(TriggerWaveform *s);
void initializeMitsubishi4gMess(TriggerWaveform *s);

// 4G92/93/94
void initializeMitsubishi4g9xCam(TriggerWaveform *s);
void configureFordAspireTriggerWaveform(TriggerWaveform * s);

void initialize36_2_1_1(TriggerWaveform *s);
void initialize36_2_1(TriggerWaveform *s);

void initializeVvt3A92(TriggerWaveform *s);
void initializeVvt6G75(TriggerWaveform *s);
