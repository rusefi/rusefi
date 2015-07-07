///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:27 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\util\histogram. /
//                       c                                                    /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\util\histogram. /
//                       c -lCN F:\stuff\rusefi_sourceforge\firmware\iar\Debu /
//                       g\List\ -lA F:\stuff\rusefi_sourceforge\firmware\iar /
//                       \Debug\List\ -o F:\stuff\rusefi_sourceforge\firmware /
//                       \iar\Debug\Obj\ --no_cse --no_unroll --no_inline     /
//                       --no_code_motion --no_tbaa --no_clustering           /
//                       --no_scheduling --debug --endian=little              /
//                       --cpu=Cortex-M4 -e --fpu=VFPv4_sp --dlib_config      /
//                       "C:\Program Files (x86)\IAR Systems\Embedded         /
//                       Workbench 6.5\arm\INC\c\DLib_Config_Normal.h"        /
//                       --misrac2004=1.2-1.4,2.3,3.4,4.2,6.5-7,8.2-8.3,9.3,  /
//                       12.3,13.6-13,15.5,17.3,19.17,20.7-20.9,20.11 -I      /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\ -I      /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\kernel\include\ -I F:\stuff\rusefi_sourceforge\fi /
//                       rmware\iar\..\chibios\os\ports\common\ARMCMx\ -I     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\ports\common\ARMCMx\CMSIS\include\ -I             /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\ports\IAR\ARMCMx\ -I F:\stuff\rusefi_sourceforge\ /
//                       firmware\iar\..\chibios\os\ports\IAR\ARMCMx\STM32F4x /
//                       x\ -I F:\stuff\rusefi_sourceforge\firmware\iar\..\ch /
//                       ibios\os\hal\include\ -I F:\stuff\rusefi_sourceforge /
//                       \firmware\iar\..\chibios\os\hal\platforms\STM32\ -I  /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\DMAv1\ -I                     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\GPIOv2\ -I                    /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\I2Cv1\ -I                     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\SPIv1\ -I                     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\TIMv1\ -I                     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\RTCv2\ -I                     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\USARTv1\ -I                   /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\OTGv1\ -I                     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32F4xx\ -I                       /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\various\ -I F:\stuff\rusefi_sourceforge\firmware\ /
//                       iar\..\chibios\os\various\devices_lib\accel\ -I      /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       boards\ST_STM32F4_DISCOVERY\ -I                      /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\controll /
//                       ers\ -I F:\stuff\rusefi_sourceforge\firmware\iar\..\ /
//                       controllers\algo\ -I F:\stuff\rusefi_sourceforge\fir /
//                       mware\iar\..\controllers\core\ -I                    /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\controll /
//                       ers\system\ -I F:\stuff\rusefi_sourceforge\firmware\ /
//                       iar\..\controllers\sensors\ -I                       /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\controll /
//                       ers\math\ -I F:\stuff\rusefi_sourceforge\firmware\ia /
//                       r\..\controllers\trigger\ -I                         /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\console\ /
//                        -I F:\stuff\rusefi_sourceforge\firmware\iar\..\conf /
//                       ig\ -I F:\stuff\rusefi_sourceforge\firmware\iar\..\c /
//                       onfig\engines\ -I F:\stuff\rusefi_sourceforge\firmwa /
//                       re\iar\..\config\stm32f4ems\ -I                      /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\console\ /
//                       binary\ -I F:\stuff\rusefi_sourceforge\firmware\iar\ /
//                       ..\console_util\ -I F:\stuff\rusefi_sourceforge\firm /
//                       ware\iar\..\development\ -I                          /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\developm /
//                       ent\test\ -I F:\stuff\rusefi_sourceforge\firmware\ia /
//                       r\..\development\hw_layer\ -I                        /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\hw_layer /
//                       \algo\ -I F:\stuff\rusefi_sourceforge\firmware\iar\. /
//                       .\hw_layer\lcd\ -I F:\stuff\rusefi_sourceforge\firmw /
//                       are\iar\..\hw_layer\stm32f4\ -I                      /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\hw_layer /
//                       \serial_over_usb\ -I F:\stuff\rusefi_sourceforge\fir /
//                       mware\iar\..\hw_layer\ -I                            /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\ext\ -I  /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\ext_algo /
//                       \ -I F:\stuff\rusefi_sourceforge\firmware\iar\..\dev /
//                       elopment\ -I F:\stuff\rusefi_sourceforge\firmware\ia /
//                       r\..\development\hw_layer\ -I                        /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\util\    /
//                       -On                                                  /
//    MISRA C version =  2004                                                 /
//      Enabled       =  1.2-1.4 2.3 3.4 4.2 6.5 7 8.2 8.3 9.3 12.3 13.6      /
//                       13.7 15.5 17.3 19.17 20.7-20.9 20.11                 /
//      Checked       =  1.4 2.3 4.2 6.5 7 8.2 8.3 9.3 12.3 13.7 15.5         /
//                       20.7-20.9 20.11                                      /
//      Not checked   =  1.1-1.3 1.5-2.2 2.4-4.1 5-6.4 8.1 8.4-9.2 10-12.2    /
//                       12.4-13.6 14-15.4 16-20.6 20.10 20.12 21             /
//    List file       =  F:\stuff\rusefi_sourceforge\firmware\iar\Debug\List\ /
//                       histogram.s                                          /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME histogram

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1


        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        END
// F:\stuff\rusefi_sourceforge\firmware\util\histogram.c
//    1 /**
//    2  * @file	histogram.c
//    3  * @brief This data structure is used to analyze CPU performance
//    4  *
//    5  * Histogram is a data structure which simplifies CPU performance monitoring and trobleshooting by tracking the min, max
//    6  * and a couple of median values for a series of measurments.
//    7  *
//    8  * @date Dec 18, 2013
//    9  * @author Andrey Belomutskiy, (c) 2012-2015
//   10  */
//   11 
//   12 #include <string.h>
//   13 #include "histogram.h"
//   14 #include "main.h"
//   15 
//   16 #if EFI_HISTOGRAMS
//   17 
//   18 #define H_ACCURACY 0.05
//   19 #define H_CONFIDENCE 0.8
//   20 #define LONG_MAX_INT 0x7fffffffffffffffL
//   21 #define SBI_SIZE 1000
//   22 
//   23 static float confidence_bounds[] = { 0.5 - H_CONFIDENCE * 0.5, 0.5, 0.5 + H_CONFIDENCE * 0.5 };
//   24 
//   25 /**
//   26  * magic curve lookup table
//   27  */
//   28 static int64_t bounds[BOUND_LENGTH] CCM_OPTIONAL;
//   29 /**
//   30  * just an optimization - faster lookup for small values
//   31  */
//   32 static int small_bounds_index[SBI_SIZE];
//   33 
//   34 static int initialized = FALSE;
//   35 
//   36 /**
//   37  * @breif Internal histogram data structure
//   38  */
//   39 void initHistogramsModule(void) {
//   40 	bounds[0] = 0;
//   41 	for (int i = 1; i < BOUND_LENGTH; i++) {
//   42 		int64_t prev = bounds[i - 1];
//   43 		int64_t next = prev + (int64_t) ((double) prev * H_ACCURACY);
//   44 		if (next == prev) // Ensure minimum step for small numbers.
//   45 			next = prev + 1;
//   46 		if (next < prev) // Overflow over Long.MAX_VALUE occurred.
//   47 			next = LONG_MAX_INT;
//   48 		bounds[i] = next;
//   49 	}
//   50 	bounds[BOUND_LENGTH - 1] = LONG_MAX_INT;
//   51 
//   52 	for (int i = 0, j = 0; j < SBI_SIZE; i++)
//   53 		while (j < bounds[i + 1] && j < SBI_SIZE)
//   54 			small_bounds_index[j++] = i;
//   55 	initialized = TRUE;
//   56 }
//   57 
//   58 /**
//   59  * @brief This internal method is only public so that we can test it.
//   60  */
//   61 int histogramGetIndex(int64_t value) {
//   62 	efiAssert(initialized, "histo initialized", 0);
//   63 	if (value < 0)
//   64 		return 0;
//   65 	if (value < SBI_SIZE)
//   66 		return small_bounds_index[(int) value];
//   67 	int l = small_bounds_index[SBI_SIZE - 1];
//   68 	int r = BOUND_LENGTH - 1;
//   69 	while (l < r) {
//   70 		int m = (l + r) >> 1;
//   71 		if (bounds[m] > value)
//   72 			r = m - 1;
//   73 		else if (bounds[m + 1] <= value)
//   74 			l = m + 1;
//   75 		else
//   76 			return m;
//   77 	}
//   78 	return l;
//   79 }
//   80 
//   81 /**
//   82  * @brief Reset histogram_s to orignal state
//   83  */
//   84 void initHistogram(histogram_s *h, const char *name) {
//   85 	if (efiStrlen(name) > sizeof(h->name) - 1) {
//   86 		firmwareError("Histogram name [%s] too long", name);
//   87 	}
//   88 	strcpy(h->name, name);
//   89 	h->total_value = 0;
//   90 	h->total_count = 0;
//   91 	memset(h->values, 0, sizeof(h->values));
//   92 }
//   93 
//   94 /**
//   95  * @breif Add a new value into histogram_s
//   96  */
//   97 void hsAdd(histogram_s *h, int64_t value) {
//   98 	int index = histogramGetIndex(value);
//   99 	int count = 1;
//  100 	h->total_value += value;
//  101 	h->total_count += count;
//  102 	efiAssertVoid(index < BOUND_LENGTH, "histogram issue");
//  103 
//  104 	h->values[index] += count;
//  105 }
//  106 
//  107 /**
//  108  * @brief Prepare histogram report
//  109  * @note This report should be displayed using 'printHistogram' method
//  110  */
//  111 int hsReport(histogram_s *h, int* report) {
//  112 	int index = 0;
//  113 
//  114 	if (h->total_count <= 5) {
//  115 		for (int j = 0; j < BOUND_LENGTH; j++) {
//  116 			for (int k = 0; k < h->values[j]; k++) {
//  117 				report[index++] = (bounds[j] + bounds[j + 1]) / 2;
//  118 			}
//  119 		}
//  120 		return index;
//  121 	}
//  122 
//  123 	int minIndex = 0;
//  124 	while (h->values[minIndex] == 0) {
//  125 		minIndex++;
//  126 	}
//  127 	report[index++] = h->values[minIndex];
//  128 
//  129 	int64_t acc = 0;
//  130 	// 'acc' is accumulated number of samples in [0, min - 1].
//  131 	for (int j = 0; j < 3; j++) {
//  132 		int64_t k = confidence_bounds[j] * h->total_count;
//  133 		// Always drop at least 1 'non-confident' sample...
//  134 		if (k == 0) {
//  135 			k = 1;
//  136 		}
//  137 		if (k == h->total_count) {
//  138 			k = h->total_count - 1;
//  139 		}
//  140 		// 'k' is desired number of samples.
//  141 		while (acc + h->values[minIndex] < k)
//  142 			acc += h->values[minIndex++];
//  143 		if (k < h->total_count / 2) // Converge to median (from left).
//  144 			while (acc + h->values[minIndex] <= k)
//  145 				acc += h->values[minIndex++];
//  146 		// Now: acc <= k <= acc + st.histogram[min]
//  147 		// And desired number of samples is within [min, min + 1)
//  148 		float d = bounds[minIndex];
//  149 		if (acc != k)
//  150 			d += (bounds[minIndex + 1] - 1 - bounds[minIndex]) * (k - acc) / h->values[minIndex];
//  151 		report[index++] = (int) d;
//  152 	}
//  153 
//  154 	int maxIndex = BOUND_LENGTH - 1;
//  155 	while (h->values[maxIndex] == 0)
//  156 		maxIndex--;
//  157 	int64_t maxValue = bounds[maxIndex + 1] - 1;
//  158 	report[index++] = maxValue;
//  159 
//  160 	return index;
//  161 }
//  162 
//  163 #endif /* EFI_HISTOGRAMS */
// 
//
// 
//
//
//Errors: none
//Warnings: none
