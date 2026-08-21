#pragma once

/**
 * @file isr_duration_histogram.h
 *
 * Coarse histogram of ISR execution durations, for diagnosing which
 * interrupt handler starves the periodic SysTick (m74_9: the trigger EXTI
 * ISR runs at the highest priority, and any handler staying >1 ms makes
 * the lowest-priority periodic SysTick lose ticks permanently).
 *
 * Bounds are NT ticks: 50 us, 100 us, 250 us, 500 us, 1000 us.
 */

#include "global.h"
#include "loggingcentral.h"

struct IsrDurationHistogram {
	static constexpr int Buckets = 6;
	static constexpr uint32_t bounds[Buckets - 1] = { 200, 400, 1000, 2000, 4000 };
	uint32_t count[Buckets] = {0};
	uint32_t sumUs = 0;

	// Out-of-line in isr_duration_histogram.cpp: LTO clones of inline methods
	// ignore the TRIGGER_RAM_CODE section attribute, so this per-ISR method
	// must not stay inline or it runs from the slow flash NZW area.
	TRIGGER_RAM_CODE void add(uint32_t ticks);

	void reset() {
		for (size_t i = 0; i < Buckets; i++) {
			count[i] = 0;
		}
		sumUs = 0;
	}

	void print(const char* name) const {
		efiPrintf("%s hist: <50us=%lu 50-100=%lu 100-250=%lu 250-500=%lu 500-1000=%lu >1000us=%lu sumUs=%lu",
			name,
			(unsigned long)count[0], (unsigned long)count[1], (unsigned long)count[2],
			(unsigned long)count[3], (unsigned long)count[4], (unsigned long)count[5],
			(unsigned long)sumUs);
	}
};
