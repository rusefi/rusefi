/**
 * @file	histogram.c
 * @brief This data structure is used to analyze CPU performance
 *
 * Histogram is a data structure which simplifies CPU performance monitoring and trobleshooting by tracking the min, max
 * and a couple of median values for a series of measurments.
 *
 * @date Dec 18, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include <string.h>
#include "histogram.h"

#if defined(HAS_OS_ACCESS)
#error "Unexpected OS ACCESS HERE"
#endif

#if EFI_HISTOGRAMS || EFI_UNIT_TEST

#define H_ACCURACY 0.05
#define H_CONFIDENCE 0.8
#define LONG_MAX_INT 0x7fffffffffffffffL
#define SBI_SIZE 1000

static float confidence_bounds[] = { 0.5 - H_CONFIDENCE * 0.5, 0.5, 0.5 + H_CONFIDENCE * 0.5 };

/**
 * magic curve lookup table
 */
static int64_t bounds[BOUND_LENGTH] CCM_OPTIONAL;
/**
 * just an optimization - faster lookup for small values
 */
static int small_bounds_index[SBI_SIZE];

static int initialized = FALSE;

/**
 * @breif Internal histogram data structure
 */
void initHistogramsModule(void) {
	bounds[0] = 0;
	for (int i = 1; i < BOUND_LENGTH; i++) {
		int64_t prev = bounds[i - 1];
		int64_t next = prev + (int64_t) ((double) prev * H_ACCURACY);
		if (next == prev) // Ensure minimum step for small numbers.
			next = prev + 1;
		if (next < prev) // Overflow over Long.MAX_VALUE occurred.
			next = LONG_MAX_INT;
		bounds[i] = next;
	}
	bounds[BOUND_LENGTH - 1] = LONG_MAX_INT;

	for (int i = 0, j = 0; j < SBI_SIZE; i++)
		while (j < bounds[i + 1] && j < SBI_SIZE)
			small_bounds_index[j++] = i;
	initialized = TRUE;
}

/**
 * @brief This internal method is only public so that we can test it.
 */
int histogramGetIndex(int64_t value) {
	efiAssert(CUSTOM_ERR_ASSERT, initialized, "histo initialized", 0);
	if (value < 0)
		return 0;
	if (value < SBI_SIZE)
		return small_bounds_index[(int) value];
	int l = small_bounds_index[SBI_SIZE - 1];
	int r = BOUND_LENGTH - 1;
	while (l < r) {
		int m = (l + r) >> 1;
		if (bounds[m] > value)
			r = m - 1;
		else if (bounds[m + 1] <= value)
			l = m + 1;
		else
			return m;
	}
	return l;
}

/**
 * @brief Reset histogram_s to orignal state
 */
void initHistogram(histogram_s *h, const char *name) {
	if (efiStrlen(name) > sizeof(h->name) - 1) {
		firmwareError(ERROR_HISTO_NAME, "Histogram name [%s] too long", name);
	}
	strcpy(h->name, name);
	h->total_value = 0;
	h->total_count = 0;
	memset(h->values, 0, sizeof(h->values));
}

/**
 * @breif Add a new value into histogram_s
 */
void hsAdd(histogram_s *h, int64_t value) {
	int index = histogramGetIndex(value);
	int count = 1;
	h->total_value += value;
	h->total_count += count;
	efiAssertVoid(CUSTOM_ERR_6670, index < BOUND_LENGTH, "histogram issue");

	h->values[index] += count;
}

/**
 * @brief Prepare histogram report
 * @note This report should be displayed using 'printHistogram' method
 */
int hsReport(histogram_s *h, int* report) {
	int index = 0;

	if (h->total_count <= 5) {
		for (int j = 0; j < BOUND_LENGTH; j++) {
			for (int k = 0; k < h->values[j]; k++) {
				report[index++] = (bounds[j] + bounds[j + 1]) / 2;
			}
		}
		return index;
	}

	int minIndex = 0;
	while (h->values[minIndex] == 0) {
		minIndex++;
	}
	report[index++] = h->values[minIndex];

	int64_t acc = 0;
	// 'acc' is accumulated number of samples in [0, min - 1].
	for (int j = 0; j < 3; j++) {
		int64_t k = confidence_bounds[j] * h->total_count;
		// Always drop at least 1 'non-confident' sample...
		if (k == 0) {
			k = 1;
		}
		if (k == h->total_count) {
			k = h->total_count - 1;
		}
		// 'k' is desired number of samples.
		while (acc + h->values[minIndex] < k)
			acc += h->values[minIndex++];
		if (k < h->total_count / 2) // Converge to median (from left).
			while (acc + h->values[minIndex] <= k)
				acc += h->values[minIndex++];
		// Now: acc <= k <= acc + st.histogram[min]
		// And desired number of samples is within [min, min + 1)
		float d = bounds[minIndex];
		if (acc != k)
			d += (bounds[minIndex + 1] - 1 - bounds[minIndex]) * (k - acc) / h->values[minIndex];
		report[index++] = (int) d;
	}

	int maxIndex = BOUND_LENGTH - 1;
	while (h->values[maxIndex] == 0)
		maxIndex--;
	int64_t maxValue = bounds[maxIndex + 1] - 1;
	report[index++] = maxValue;

	return index;
}

#endif /* EFI_HISTOGRAMS */
