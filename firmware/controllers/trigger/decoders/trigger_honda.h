/*
 * @file	trigger_honda.h
 *
 * @date May 27, 2016
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "trigger_structure.h"

void configureHondaAccordCDDip(TriggerWaveform *s);
void configureHondaAccordShifted(TriggerWaveform *s);

void configureHonda_1_4_24(TriggerWaveform *s, bool withOneEventSignal, bool withFourEventSignal,
		trigger_wheel_e const oneEventWave,
		trigger_wheel_e const fourEventWave,
		float d);

void configureOnePlus16(TriggerWaveform *s);

void configureHondaCbr600(TriggerWaveform *s);

void configureHondaK_12_1(TriggerWaveform *s);
void configureHondaK_4_1(TriggerWaveform *s);
