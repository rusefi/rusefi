#include "isr_duration_histogram.h"

// Runs inside the per-tooth trigger ISR on AT32 boards (m74_9), where the
// flash NZW area is too slow at 288 MHz - keep it in the zero-wait SRAM
// .fast_text section. Out-of-line on purpose: LTO clones of inline methods
// ignore the section attribute.
TRIGGER_RAM_CODE void IsrDurationHistogram::add(uint32_t ticks) {
	for (int i = 0; i < Buckets - 1; i++) {
		if (ticks < bounds[i]) {
			count[i]++;
			sumUs += ticks / US_TO_NT_MULTIPLIER;
			return;
		}
	}
	count[Buckets - 1]++;
	sumUs += ticks / US_TO_NT_MULTIPLIER;
}
