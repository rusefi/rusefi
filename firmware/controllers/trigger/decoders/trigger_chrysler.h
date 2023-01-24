/**
 * @file	trigger_chrysler.h
 *
 * @date Mar 24, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

class TriggerWaveform;

#define CHRYSLER_NGC4_GAP 1
#define CHRYSLER_NGC6_GAP 1.5

void configureNeon1995TriggerWaveformOnlyCrank(TriggerWaveform *s);

void configureNeon2003TriggerWaveformCam(TriggerWaveform *s);
void configureNeon2003TriggerWaveformCrank(TriggerWaveform *s);
void initDodgeRam(TriggerWaveform *s);

void configureDodgeStratusTriggerWaveform(TriggerWaveform *s);

void initJeep18_2_2_2(TriggerWaveform *s);
void initJeep_XJ_4cyl_2500(TriggerWaveform *s);

void configureChryslerNGC_36_2_2(TriggerWaveform *s);

