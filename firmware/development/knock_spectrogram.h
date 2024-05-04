/**
 * @file	knock_spectrogram.h
 *
 * @date Feb 20, 2023
 * @author Alexey Ershov, (c) 2012-2023
 */

#pragma once

#include "global.h"

#if KNOCK_SPECTROGRAM
void knockSpectorgramAddLine(float main_freq, float* data, size_t size);
#endif