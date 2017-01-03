/**
 * @file	histogram.h
 * @brief This data structure is used to analyze CPU performance
 *
 * User can report data into a histogram and later get an aggregated represenation, i.e.
 * a histogram, of these values.
 *
 * A typicsl use-case would be reporting the times it took to execure a particular section of
 * code - and later analyzing the histogram.
 *
 *
 * @date Dec 18, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef HISTOGRAM_H_
#define HISTOGRAM_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include <stdint.h>

#define BOUND_LENGTH 895

typedef struct {
	char name[16];
	int64_t total_value;
	int64_t total_count;
	int values[BOUND_LENGTH];
} histogram_s;

void initHistogramsModule(void);
int histogramGetIndex(int64_t value);
void initHistogram(histogram_s *h, const char *name);
void hsAdd(histogram_s *h, int64_t value);
int hsReport(histogram_s *h, int* report);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* HISTOGRAM_H_ */
