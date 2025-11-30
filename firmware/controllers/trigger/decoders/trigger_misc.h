/*
 * trigger_misc.h
 *
 *  Created on: Oct 30, 2018
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

class TriggerWaveform;

void configureFiatIAQ_P8(TriggerWaveform * s);
void configureDaihatsu3cyl(TriggerWaveform * s);
void configureDaihatsu4cyl(TriggerWaveform * s);
void configureFordPip(TriggerWaveform * s);
void configureFordST170(TriggerWaveform * s);
void configureTriTach(TriggerWaveform * s);
// TT_VVT_BARRA_3_PLUS_1
void configureBarra3plus1cam(TriggerWaveform *s);
void configureBenelli(TriggerWaveform *s);

/**
 * @brief Configure trigger as Sovek or Saruman ignition
 * @param s Trigger waveform visitor to config
 */
void configure60degSingleTooth(TriggerWaveform *s);
void configureArcticCat(TriggerWaveform *s);
/**
 * @brief Audi 5 cylinder 135-tooth crank with reference pin and CAM HALL gating
 * @param s Trigger waveform visitor to config
 */
void configureAudi5Cyl135_1_1(TriggerWaveform *s);
