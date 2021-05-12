/**
 * This file defines the API for the microsecond timer that a port needs to implement
 *
 * Do not call these functions directly, they should only be called by microsecond_timer.cpp
 */

#pragma once

void portInitMicrosecondTimer();
void portSetHardwareSchedulerTimer(efitick_t nowNt, efitick_t setTimeNt);

// The port should call this callback when the timer expires
void portMicrosecondTimerCallback();
