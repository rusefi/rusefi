/**
 * @file trigger_nissan.h
 *
 * @date Sep 19, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

class TriggerWaveform;

void initializeNissanSR20VE_4(TriggerWaveform *s);

void initializeNissanVQvvt(TriggerWaveform *s);
void initializeNissanMRvvt(TriggerWaveform *s);
void initializeNissanVQ35crank(TriggerWaveform *s);
void initializeNissanMR18crank(TriggerWaveform *s);
void initializeNissanQR25crank(TriggerWaveform *s);

void initializeNissanK11(TriggerWaveform *s);

void initializeNissanVQ30cam(TriggerWaveform *s);

// not vq35hr but https://en.wikipedia.org/wiki/Nissan_HR_engine
void initializeNissanHRcrank(TriggerWaveform *s);
void initializeNissanHRvvtIn(TriggerWaveform *s);
