/**
 * @file	trigger_ford.h
 *
 * @author Andrey Belomutskiy, (c) rusEFI LLC 2012-2023
 */

#pragma once

class TriggerWaveform;

void configureFordCoyote(TriggerWaveform *s);
void configureFordPip6(TriggerWaveform * s);
void configureFordPip8(TriggerWaveform * s);
void configureFordST170(TriggerWaveform * s);
