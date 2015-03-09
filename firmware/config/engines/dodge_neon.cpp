/**
 * @file	dodge_neon.cpp
 *
 * DODGE_NEON_1995 = 2
 *
 * DODGE_NEON_2003 = 23
 * set_engine_type 23
 *
 * This config overrides some values of the default configuration which is set by setDefaultConfiguration() method
 *
 * @date Dec 16, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "main.h"

#if EFI_SUPPORT_DODGE_NEON || defined(__DOXYGEN__)

#include "dodge_neon.h"
#include "engine_configuration.h"
#include "thermistors.h"
#include "engine_math.h"
#include "fsio_impl.h"
#if EFI_PROD_CODE
#include "HIP9011.h"
#endif

// setFrankenso_01_LCD
#include "honda_accord.h"

#define xxxxx 12

static const fuel_table_t alphaNfuel = {
		{/*0  engineLoad=0.00*/   /*0 800.0*/003.7, /*1 1213.0*/003.6, /*2 1626.0*/003.6, /*3 2040.0*/004.0, /*4 2453.0*/004.0, /*5 2866.0*/004.0, /*6 3280.0*/006.0, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
		{/*1  engineLoad=6.66*/   /*0 800.0*/004.0, /*1 1213.0*/004.0, /*2 1626.0*/004.0, /*3 2040.0*/005.0, /*4 2453.0*/004.5, /*5 2866.0*/004.5, /*6 3280.0*/005.0, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
		{/*2  engineLoad=13.33*/  /*0 800.0*/005.0, /*1 1213.0*/005.0, /*2 1626.0*/006.0, /*3 2040.0*/007.0, /*4 2453.0*/004.5, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
		{/*3  engineLoad=20.00*/  /*0 800.0*/006.0, /*1 1213.0*/007.0, /*2 1626.0*/007.0, /*3 2040.0*/007.0, /*4 2453.0*/005.0, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
		{/*4  engineLoad=26.66*/  /*0 800.0*/007.0, /*1 1213.0*/008.0, /*2 1626.0*/008.0, /*3 2040.0*/008.0, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
		{/*5  engineLoad=33.33*/  /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
		{/*6  engineLoad=40.00*/  /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
		{/*7  engineLoad=46.66*/  /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
		{/*8  engineLoad=53.33*/  /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
		{/*9  engineLoad=60.00*/  /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
		{/*10 engineLoad=66.66*/  /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
		{/*11 engineLoad=73.33*/  /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
		{/*12 engineLoad=80.00*/  /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
		{/*13 engineLoad=86.66*/  /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
		{/*14 engineLoad=93.33*/  /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
		{/*15 engineLoad=100.00*/ /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx}
		};

static const ignition_table_t fromDyno = {
		{/* 0 0.000	*//* 0 800.0*/12.000,	/* 1 1213.3281*/12.000,	/* 2 1626.6562*/12.000,	/* 3 2040.0*/26.000,	/* 4 2453.3438*/29.000,	/* 5 2866.6562*/31.000,	/* 6 3280.0*/31.000,	/* 7 3693.3438*/38.000,	/* 8 4106.6875*/40.000,	/* 9 4520.0*/40.000,	/* 10 4933.3125*/40.000,	/* 11 5346.6875*/42.000,	/* 12 5760.0*/42.000,	/* 13 6173.3125*/42.000,	/* 14 6586.6875*/42.000,	/* 15 7000.0*/42.000,	},
		{/* 1 6.660	*//* 0 800.0*/12.000,	/* 1 1213.3281*/12.000,	/* 2 1626.6562*/12.000,	/* 3 2040.0*/26.000,	/* 4 2453.3438*/29.000,	/* 5 2866.6562*/31.000,	/* 6 3280.0*/31.000,	/* 7 3693.3438*/38.000,	/* 8 4106.6875*/40.000,	/* 9 4520.0*/40.000,	/* 10 4933.3125*/40.000,	/* 11 5346.6875*/42.000,	/* 12 5760.0*/42.000,	/* 13 6173.3125*/42.000,	/* 14 6586.6875*/42.000,	/* 15 7000.0*/42.000,	},
		{/* 2 13.330	*//* 0 800.0*/12.000,	/* 1 1213.3281*/12.000,	/* 2 1626.6562*/12.000,	/* 3 2040.0*/26.000,	/* 4 2453.3438*/29.000,	/* 5 2866.6562*/31.000,	/* 6 3280.0*/31.000,	/* 7 3693.3438*/38.000,	/* 8 4106.6875*/40.000,	/* 9 4520.0*/40.000,	/* 10 4933.3125*/40.000,	/* 11 5346.6875*/42.000,	/* 12 5760.0*/42.000,	/* 13 6173.3125*/42.000,	/* 14 6586.6875*/42.000,	/* 15 7000.0*/42.000,	},
		{/* 3 20.000	*//* 0 800.0*/12.000,	/* 1 1213.3281*/12.000,	/* 2 1626.6562*/12.000,	/* 3 2040.0*/26.000,	/* 4 2453.3438*/29.000,	/* 5 2866.6562*/31.000,	/* 6 3280.0*/31.000,	/* 7 3693.3438*/38.000,	/* 8 4106.6875*/40.000,	/* 9 4520.0*/40.000,	/* 10 4933.3125*/40.000,	/* 11 5346.6875*/42.000,	/* 12 5760.0*/42.000,	/* 13 6173.3125*/42.000,	/* 14 6586.6875*/42.000,	/* 15 7000.0*/42.000,	},
		{/* 4 26.660	*//* 0 800.0*/12.000,	/* 1 1213.3281*/12.000,	/* 2 1626.6562*/12.000,	/* 3 2040.0*/26.000,	/* 4 2453.3438*/29.000,	/* 5 2866.6562*/31.000,	/* 6 3280.0*/31.000,	/* 7 3693.3438*/38.000,	/* 8 4106.6875*/40.000,	/* 9 4520.0*/40.000,	/* 10 4933.3125*/40.000,	/* 11 5346.6875*/42.000,	/* 12 5760.0*/42.000,	/* 13 6173.3125*/42.000,	/* 14 6586.6875*/42.000,	/* 15 7000.0*/42.000,	},
		{/* 5 33.330	*//* 0 800.0*/12.000,	/* 1 1213.3281*/12.000,	/* 2 1626.6562*/12.000,	/* 3 2040.0*/26.000,	/* 4 2453.3438*/29.000,	/* 5 2866.6562*/31.000,	/* 6 3280.0*/31.000,	/* 7 3693.3438*/38.000,	/* 8 4106.6875*/40.000,	/* 9 4520.0*/40.000,	/* 10 4933.3125*/40.000,	/* 11 5346.6875*/42.000,	/* 12 5760.0*/42.000,	/* 13 6173.3125*/42.000,	/* 14 6586.6875*/42.000,	/* 15 7000.0*/42.000,	},
		{/* 6 40.000	*//* 0 800.0*/12.000,	/* 1 1213.3281*/12.000,	/* 2 1626.6562*/12.000,	/* 3 2040.0*/26.000,	/* 4 2453.3438*/29.000,	/* 5 2866.6562*/31.000,	/* 6 3280.0*/31.000,	/* 7 3693.3438*/38.000,	/* 8 4106.6875*/40.000,	/* 9 4520.0*/40.000,	/* 10 4933.3125*/40.000,	/* 11 5346.6875*/42.000,	/* 12 5760.0*/42.000,	/* 13 6173.3125*/42.000,	/* 14 6586.6875*/42.000,	/* 15 7000.0*/42.000,	},
		{/* 7 46.660	*//* 0 800.0*/12.000,	/* 1 1213.3281*/12.000,	/* 2 1626.6562*/12.000,	/* 3 2040.0*/26.000,	/* 4 2453.3438*/29.000,	/* 5 2866.6562*/31.000,	/* 6 3280.0*/31.000,	/* 7 3693.3438*/38.000,	/* 8 4106.6875*/40.000,	/* 9 4520.0*/40.000,	/* 10 4933.3125*/40.000,	/* 11 5346.6875*/42.000,	/* 12 5760.0*/42.000,	/* 13 6173.3125*/42.000,	/* 14 6586.6875*/42.000,	/* 15 7000.0*/42.000,	},
		{/* 8 53.330	*//* 0 800.0*/12.000,	/* 1 1213.3281*/12.000,	/* 2 1626.6562*/12.000,	/* 3 2040.0*/26.000,	/* 4 2453.3438*/29.000,	/* 5 2866.6562*/31.000,	/* 6 3280.0*/31.000,	/* 7 3693.3438*/38.000,	/* 8 4106.6875*/40.000,	/* 9 4520.0*/40.000,	/* 10 4933.3125*/40.000,	/* 11 5346.6875*/42.000,	/* 12 5760.0*/42.000,	/* 13 6173.3125*/42.000,	/* 14 6586.6875*/42.000,	/* 15 7000.0*/42.000,	},
		{/* 9 60.000	*//* 0 800.0*/12.000,	/* 1 1213.3281*/12.000,	/* 2 1626.6562*/12.000,	/* 3 2040.0*/26.000,	/* 4 2453.3438*/29.000,	/* 5 2866.6562*/31.000,	/* 6 3280.0*/31.000,	/* 7 3693.3438*/38.000,	/* 8 4106.6875*/40.000,	/* 9 4520.0*/40.000,	/* 10 4933.3125*/40.000,	/* 11 5346.6875*/42.000,	/* 12 5760.0*/42.000,	/* 13 6173.3125*/42.000,	/* 14 6586.6875*/42.000,	/* 15 7000.0*/42.000,	},
		{/* 10 66.660	*//* 0 800.0*/12.000,	/* 1 1213.3281*/12.000,	/* 2 1626.6562*/12.000,	/* 3 2040.0*/26.000,	/* 4 2453.3438*/29.000,	/* 5 2866.6562*/31.000,	/* 6 3280.0*/31.000,	/* 7 3693.3438*/38.000,	/* 8 4106.6875*/40.000,	/* 9 4520.0*/40.000,	/* 10 4933.3125*/40.000,	/* 11 5346.6875*/42.000,	/* 12 5760.0*/42.000,	/* 13 6173.3125*/42.000,	/* 14 6586.6875*/42.000,	/* 15 7000.0*/42.000,	},
		{/* 11 73.330	*//* 0 800.0*/12.000,	/* 1 1213.3281*/12.000,	/* 2 1626.6562*/12.000,	/* 3 2040.0*/26.000,	/* 4 2453.3438*/29.000,	/* 5 2866.6562*/31.000,	/* 6 3280.0*/31.000,	/* 7 3693.3438*/38.000,	/* 8 4106.6875*/40.000,	/* 9 4520.0*/40.000,	/* 10 4933.3125*/40.000,	/* 11 5346.6875*/42.000,	/* 12 5760.0*/42.000,	/* 13 6173.3125*/42.000,	/* 14 6586.6875*/42.000,	/* 15 7000.0*/42.000,	},
		{/* 12 80.000	*//* 0 800.0*/12.000,	/* 1 1213.3281*/12.000,	/* 2 1626.6562*/12.000,	/* 3 2040.0*/26.000,	/* 4 2453.3438*/29.000,	/* 5 2866.6562*/31.000,	/* 6 3280.0*/31.000,	/* 7 3693.3438*/38.000,	/* 8 4106.6875*/40.000,	/* 9 4520.0*/40.000,	/* 10 4933.3125*/40.000,	/* 11 5346.6875*/42.000,	/* 12 5760.0*/42.000,	/* 13 6173.3125*/42.000,	/* 14 6586.6875*/42.000,	/* 15 7000.0*/42.000,	},
		{/* 13 86.660	*//* 0 800.0*/12.000,	/* 1 1213.3281*/12.000,	/* 2 1626.6562*/12.000,	/* 3 2040.0*/26.000,	/* 4 2453.3438*/29.000,	/* 5 2866.6562*/31.000,	/* 6 3280.0*/31.000,	/* 7 3693.3438*/38.000,	/* 8 4106.6875*/40.000,	/* 9 4520.0*/40.000,	/* 10 4933.3125*/40.000,	/* 11 5346.6875*/42.000,	/* 12 5760.0*/42.000,	/* 13 6173.3125*/42.000,	/* 14 6586.6875*/42.000,	/* 15 7000.0*/42.000,	},
		{/* 14 93.330	*//* 0 800.0*/12.000,	/* 1 1213.3281*/12.000,	/* 2 1626.6562*/12.000,	/* 3 2040.0*/26.000,	/* 4 2453.3438*/29.000,	/* 5 2866.6562*/31.000,	/* 6 3280.0*/31.000,	/* 7 3693.3438*/38.000,	/* 8 4106.6875*/40.000,	/* 9 4520.0*/40.000,	/* 10 4933.3125*/40.000,	/* 11 5346.6875*/42.000,	/* 12 5760.0*/42.000,	/* 13 6173.3125*/42.000,	/* 14 6586.6875*/42.000,	/* 15 7000.0*/42.000,	},
		{/* 15 100.000	*//* 0 800.0*/12.000,	/* 1 1213.3281*/12.000,	/* 2 1626.6562*/12.000,	/* 3 2040.0*/26.000,	/* 4 2453.3438*/29.000,	/* 5 2866.6562*/31.000,	/* 6 3280.0*/31.000,	/* 7 3693.3438*/38.000,	/* 8 4106.6875*/40.000,	/* 9 4520.0*/40.000,	/* 10 4933.3125*/40.000,	/* 11 5346.6875*/42.000,	/* 12 5760.0*/42.000,	/* 13 6173.3125*/42.000,	/* 14 6586.6875*/42.000,	/* 15 7000.0*/42.000,	},
};


// http://rusefi.com/forum/viewtopic.php?f=3&t=360&start=40
static float dodge_map_advance_table[16][16] = {
{/*0 engineLoad=1.2*//*0 800.0*/-4.498, /*1 1213.0*/-11.905, /*2 1626.0*/-23.418, /*3 2040.0*/-25.357, /*4 2453.0*/-25.441, /*5 2866.0*/-25.468, /*6 3280.0*/-29.425, /*7 3693.0*/-32.713, /*8 4106.0*/-35.556, /*9 4520.0*/-37.594, /*10 4933.0*/-36.165, /*11 5346.0*/-30.578, /*12 5760.0*/-29.145, /*13 6173.0*/-29.065, /*14 6586.0*/-27.071, /*15 7000.0*/-28.282},
{/*1 engineLoad=1.413333*//*0 800.0*/-4.87, /*1 1213.0*/-12.138, /*2 1626.0*/-23.389, /*3 2040.0*/-25.501, /*4 2453.0*/-25.441, /*5 2866.0*/-25.468, /*6 3280.0*/-29.125, /*7 3693.0*/-33.074, /*8 4106.0*/-34.203, /*9 4520.0*/-37.769, /*10 4933.0*/-35.899, /*11 5346.0*/-30.519, /*12 5760.0*/-28.88, /*13 6173.0*/-28.74, /*14 6586.0*/-27.189, /*15 7000.0*/-27.826},
{/*2 engineLoad=1.626666*//*0 800.0*/-4.817, /*1 1213.0*/-12.262, /*2 1626.0*/-23.925, /*3 2040.0*/-25.501, /*4 2453.0*/-25.5, /*5 2866.0*/-25.468, /*6 3280.0*/-29.364, /*7 3693.0*/-33.489, /*8 4106.0*/-34.839, /*9 4520.0*/-37.545, /*10 4933.0*/-35.875, /*11 5346.0*/-30.353, /*12 5760.0*/-29.052, /*13 6173.0*/-28.37, /*14 6586.0*/-27.072, /*15 7000.0*/-26.828},
{/*3 engineLoad=1.839999*//*0 800.0*/-4.537, /*1 1213.0*/-12.421, /*2 1626.0*/-23.214, /*3 2040.0*/-25.394, /*4 2453.0*/-25.412, /*5 2866.0*/-25.485, /*6 3280.0*/-29.425, /*7 3693.0*/-33.427, /*8 4106.0*/-34.091, /*9 4520.0*/-36.887, /*10 4933.0*/-36.047, /*11 5346.0*/-30.079, /*12 5760.0*/-28.453, /*13 6173.0*/-28.074, /*14 6586.0*/-27.189, /*15 7000.0*/-26.641},
{/*4 engineLoad=2.053332*//*0 800.0*/-4.522, /*1 1213.0*/-11.76, /*2 1626.0*/-23.915, /*3 2040.0*/-25.415, /*4 2453.0*/-25.551, /*5 2866.0*/-25.14, /*6 3280.0*/-29.346, /*7 3693.0*/-32.917, /*8 4106.0*/-34.815, /*9 4520.0*/-37.211, /*10 4933.0*/-35.817, /*11 5346.0*/-29.694, /*12 5760.0*/-28.799, /*13 6173.0*/-27.818, /*14 6586.0*/-28.098, /*15 7000.0*/-27.662},
{/*5 engineLoad=2.266665*//*0 800.0*/-4.678, /*1 1213.0*/-11.912, /*2 1626.0*/-23.486, /*3 2040.0*/-25.379, /*4 2453.0*/-25.551, /*5 2866.0*/-25.527, /*6 3280.0*/-29.856, /*7 3693.0*/-33.511, /*8 4106.0*/-34.786, /*9 4520.0*/-37.963, /*10 4933.0*/-35.917, /*11 5346.0*/-31.073, /*12 5760.0*/-28.361, /*13 6173.0*/-28.468, /*14 6586.0*/-27.188, /*15 7000.0*/-26.729},
{/*6 engineLoad=2.479998*//*0 800.0*/-4.517, /*1 1213.0*/-12.029, /*2 1626.0*/-23.477, /*3 2040.0*/-25.455, /*4 2453.0*/-25.382, /*5 2866.0*/-25.898, /*6 3280.0*/-29.147, /*7 3693.0*/-33.578, /*8 4106.0*/-34.12, /*9 4520.0*/-36.279, /*10 4933.0*/-36.432, /*11 5346.0*/-31.362, /*12 5760.0*/-28.084, /*13 6173.0*/-28.463, /*14 6586.0*/-27.691, /*15 7000.0*/-27.83},
{/*7 engineLoad=2.693331*//*0 800.0*/-4.532, /*1 1213.0*/-12.262, /*2 1626.0*/-23.935, /*3 2040.0*/-25.489, /*4 2453.0*/-25.595, /*5 2866.0*/-26.816, /*6 3280.0*/-30.251, /*7 3693.0*/-33.533, /*8 4106.0*/-34.794, /*9 4520.0*/-37.882, /*10 4933.0*/-36.104, /*11 5346.0*/-30.079, /*12 5760.0*/-28.545, /*13 6173.0*/-29.304, /*14 6586.0*/-27.07, /*15 7000.0*/-28.324},
{/*8 engineLoad=2.906664*//*0 800.0*/-4.532, /*1 1213.0*/-12.036, /*2 1626.0*/-23.418, /*3 2040.0*/-25.513, /*4 2453.0*/-25.382, /*5 2866.0*/-25.357, /*6 3280.0*/-29.934, /*7 3693.0*/-33.467, /*8 4106.0*/-34.748, /*9 4520.0*/-37.288, /*10 4933.0*/-36.38, /*11 5346.0*/-29.516, /*12 5760.0*/-28.799, /*13 6173.0*/-28.407, /*14 6586.0*/-26.951, /*15 7000.0*/-28.203},
{/*9 engineLoad=3.119997*//*0 800.0*/-4.532, /*1 1213.0*/-11.978, /*2 1626.0*/-23.73, /*3 2040.0*/-25.501, /*4 2453.0*/-25.624, /*5 2866.0*/-26.328, /*6 3280.0*/-30.015, /*7 3693.0*/-33.187, /*8 4106.0*/-34.881, /*9 4520.0*/-38.044, /*10 4933.0*/-35.81, /*11 5346.0*/-29.843, /*12 5760.0*/-29.306, /*13 6173.0*/-28.997, /*14 6586.0*/-27.109, /*15 7000.0*/-29.339},
{/*10 engineLoad=3.33333*//*0 800.0*/-4.527, /*1 1213.0*/-12.131, /*2 1626.0*/-23.486, /*3 2040.0*/-25.43, /*4 2453.0*/-25.551, /*5 2866.0*/-26.276, /*6 3280.0*/-29.639, /*7 3693.0*/-33.005, /*8 4106.0*/-34.253, /*9 4520.0*/-37.788, /*10 4933.0*/-36.077, /*11 5346.0*/-30.188, /*12 5760.0*/-29.087, /*13 6173.0*/-28.481, /*14 6586.0*/-27.348, /*15 7000.0*/-27.777},
{/*11 engineLoad=3.546663*//*0 800.0*/-4.889, /*1 1213.0*/-12.175, /*2 1626.0*/-23.271, /*3 2040.0*/-25.357, /*4 2453.0*/-25.551, /*5 2866.0*/-25.485, /*6 3280.0*/-29.899, /*7 3693.0*/-32.802, /*8 4106.0*/-34.786, /*9 4520.0*/-38.686, /*10 4933.0*/-35.722, /*11 5346.0*/-31.347, /*12 5760.0*/-28.891, /*13 6173.0*/-28.333, /*14 6586.0*/-27.149, /*15 7000.0*/-27.236},
{/*12 engineLoad=3.759996*//*0 800.0*/-4.537, /*1 1213.0*/-12.073, /*2 1626.0*/-23.896, /*3 2040.0*/-25.525, /*4 2453.0*/-25.595, /*5 2866.0*/-25.451, /*6 3280.0*/-30.428, /*7 3693.0*/-33.714, /*8 4106.0*/-34.08, /*9 4520.0*/-37.526, /*10 4933.0*/-35.817, /*11 5346.0*/-30.733, /*12 5760.0*/-28.718, /*13 6173.0*/-28.518, /*14 6586.0*/-27.518, /*15 7000.0*/-26.561},
{/*13 engineLoad=3.973329*//*0 800.0*/-4.86, /*1 1213.0*/-11.883, /*2 1626.0*/-23.428, /*3 2040.0*/-25.489, /*4 2453.0*/-25.536, /*5 2866.0*/-25.613, /*6 3280.0*/-29.895, /*7 3693.0*/-33.648, /*8 4106.0*/-34.758, /*9 4520.0*/-37.988, /*10 4933.0*/-36.047, /*11 5346.0*/-30.225, /*12 5760.0*/-28.698, /*13 6173.0*/-28.487, /*14 6586.0*/-27.111, /*15 7000.0*/-27.708},
{/*14 engineLoad=4.186662*//*0 800.0*/-4.683, /*1 1213.0*/-11.898, /*2 1626.0*/-23.506, /*3 2040.0*/-25.562, /*4 2453.0*/-25.61, /*5 2866.0*/-25.519, /*6 3280.0*/-29.95, /*7 3693.0*/-33.582, /*8 4106.0*/-34.548, /*9 4520.0*/-36.201, /*10 4933.0*/-35.788, /*11 5346.0*/-30.053, /*12 5760.0*/-28.292, /*13 6173.0*/-28.259, /*14 6586.0*/-27.269, /*15 7000.0*/-26.863},
{/*15 engineLoad=4.3999950000000005*//*0 800.0*/-4.85, /*1 1213.0*/-12.24, /*2 1626.0*/-24.091, /*3 2040.0*/-25.394, /*4 2453.0*/-25.323, /*5 2866.0*/-25.544, /*6 3280.0*/-29.915, /*7 3693.0*/-33.104, /*8 4106.0*/-36.016, /*9 4520.0*/-37.933, /*10 4933.0*/-36.254, /*11 5346.0*/-29.712, /*12 5760.0*/-28.651, /*13 6173.0*/-28.045, /*14 6586.0*/-27.228, /*15 7000.0*/-27.784}
};

EXTERN_ENGINE;

void setDodgeNeon1995EngineConfiguration(DECLARE_ENGINE_PARAMETER_F) {
	board_configuration_s * boardConfiguration = &engineConfiguration->bc;

	engineConfiguration->trigger.type = TT_DODGE_NEON_1995;

	engineConfiguration->algorithm = LM_ALPHA_N;

	engineConfiguration->hasIatSensor = false;

	// set_rpm_hard_limit 4000
	engineConfiguration->rpmHardLimit = 4000; // yes, 4k. let's play it safe for now
	// set_cranking_rpm 550
	engineConfiguration->cranking.rpm = 550;

	// set_cranking_fuel 5
	engineConfiguration->cranking.baseFuel = 5;

	/**
	 * that's 1995 config
	 */

	// set_whole_fuel_map 9
	setWholeFuelMap(9 PASS_ENGINE_PARAMETER);
	setWholeTimingTable(12 PASS_ENGINE_PARAMETER);

	// set_cranking_injection_mode 0
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	// set_injection_mode 1
	engineConfiguration->injectionMode = IM_SEQUENTIAL;

	// set_ignition_mode 2
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	// set_firing_order 2
	engineConfiguration->specs.firingOrder = FO_1_THEN_3_THEN_4_THEN2;

	// set_global_trigger_offset_angle 497
	engineConfiguration->globalTriggerAngleOffset = 497;
	// set_ignition_offset 350
	engineConfiguration->ignitionBaseAngle = 350;
	 // set_injection_offset 510
	engineConfiguration->injectionAngle = 510 + 497;

	/**
	 * that's 1995 config
	 */

	// set_cranking_charge_angle 70
	engineConfiguration->crankingChargeAngle = 70;
	// set_cranking_timing_angle 0
	engineConfiguration->crankingTimingAngle = 0;

	// Frankenstein: low side - out #1: PC14
	// Frankenstein: low side - out #2: PC15
	// Frankenstein: low side - out #3: PE6
	// Frankenstein: low side - out #4: PC13
	// Frankenstein: low side - out #5: PE4
	// Frankenstein: low side - out #6: PE5
	// Frankenstein: low side - out #7: PE2
	// Frankenstein: low side - out #8: PE3
	// Frankenstein: low side - out #9: PE0
	// Frankenstein: low side - out #10: PE1
	// Frankenstein: low side - out #11: PB8
	// Frankenstein: low side - out #12: PB9

	boardConfiguration->injectionPins[0] = GPIOB_9; // Frankenstein: low side - out #12
	boardConfiguration->injectionPins[1] = GPIOB_8; // Frankenstein: low side - out #11
	boardConfiguration->injectionPins[2] = GPIOE_3; // Frankenstein: low side - out #8
	boardConfiguration->injectionPins[3] = GPIOE_5; // Frankenstein: low side - out #6

	boardConfiguration->fuelPumpPin = GPIOC_13; // Frankenstein: low side - out #4
	boardConfiguration->fuelPumpPinMode = OM_DEFAULT;

	// set_injection_pin_mode 0
	boardConfiguration->injectionPinMode = OM_DEFAULT;

	// Frankenstein: high side #1: PE8
	// Frankenstein: high side #2: PE10

	boardConfiguration->ignitionPins[0] = GPIOE_8; // Frankenstein: high side #1
	boardConfiguration->ignitionPins[1] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPins[2] = GPIOE_10; // // Frankenstein: high side #2

	// set_ignition_pin_mode 0
	boardConfiguration->ignitionPinMode = OM_DEFAULT;

	setThermistorConfiguration(&engineConfiguration->clt, 0, 32500, 30, 7550, 100, 700);
	engineConfiguration->clt.bias_resistor = 2700;

	engineConfiguration->analogChartFrequency = 7;
}

void setDodgeNeonNGCEngineConfiguration(DECLARE_ENGINE_PARAMETER_F) {
	board_configuration_s * boardConfiguration = &engineConfiguration->bc;
	engineConfiguration->trigger.type = TT_DODGE_NEON_2003;
	setFrankenso_01_LCD(boardConfiguration);
	setFrankenso0_1_joystick(engineConfiguration);

	// set_global_trigger_offset_angle 50
	engineConfiguration->globalTriggerAngleOffset = 50;
	// set_injection_offset 0
	engineConfiguration->injectionAngle = 0;

	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_SEQUENTIAL;
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	engineConfiguration->specs.displacement = 1.996;
	engineConfiguration->specs.cylindersCount = 4;

	/**
	 * that's NGC config
	 *
	 * set_cranking_fuel 5
	 */
	engineConfiguration->cranking.baseFuel = 5;

	/**
	 * 77C
	 * 1200 rpm
	 * fuel 3
	 *
	 * 88C
	 * fuel 2.8
	 *
	 * set_whole_fuel_map 12
	 */
	//setWholeFuelMap(12 PASS_ENGINE_PARAMETER);
	copyFuelTable(alphaNfuel, engineConfiguration->fuelTable);
	//setWholeTimingTable(12 PASS_ENGINE_PARAMETER);
	copyTimingTable(fromDyno, engineConfiguration->ignitionTable);

	// set_cranking_charge_angle 70
	engineConfiguration->crankingChargeAngle = 70;
	// set_cranking_timing_angle 710
	engineConfiguration->crankingTimingAngle = -710;


	/**
	 * bosch 4G1139
	 * http://forum.2gn.org/viewtopic.php?t=21657
	 */
	engineConfiguration->injector.flow = 199;

	// I want to start with a simple Alpha-N
	engineConfiguration->algorithm = LM_ALPHA_N;
//	engineConfiguration->algorithm = LM_SPEED_DENSITY;
	setFuelLoadBin(0, 100 PASS_ENGINE_PARAMETER);
	setTimingLoadBin(0, 100 PASS_ENGINE_PARAMETER);

	boardConfiguration->malfunctionIndicatorPin = GPIO_UNASSIGNED;

	/**
	 * PA4 Wideband O2 Sensor
	 */
	engineConfiguration->afr.hwChannel = EFI_ADC_4;

	commonFrankensoAnalogInputs(engineConfiguration);
	engineConfiguration->vbattDividerCoeff = ((float) (8.2 + 33)) / 8.2 * 2;

	/**
	 * http://rusefi.com/wiki/index.php?title=Manual:Hardware_Frankenso_board
	 */
	// Frankenso low out #1: PE6 main relay
	// Frankenso low out #2: PE5 alternator field control
	// Frankenso low out #3: PD7 coolant fan relay
	// Frankenso low out #4: PC13 idle valve solenoid
	// Frankenso low out #5: PE3 fuel pump relay
	// Frankenso low out #6: PE4
	// Frankenso low out #7: PE1 (do not use with discovery!)
	// Frankenso low out #8: PE2 injector #3
	// Frankenso low out #9: PB9 injector #2
	// Frankenso low out #10: PE0 (do not use with discovery!)
	// Frankenso low out #11: PB8 injector #1
	// Frankenso low out #12: PB7 injector #4

	boardConfiguration->fanPin = GPIOD_7;

	boardConfiguration->injectionPins[0] = GPIOB_8;
	boardConfiguration->injectionPins[1] = GPIOB_9;
	boardConfiguration->injectionPins[2] = GPIOE_2;
	boardConfiguration->injectionPins[3] = GPIOB_7;

	boardConfiguration->ignitionPins[0] = GPIOC_9;
	boardConfiguration->ignitionPins[1] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPins[2] = GPIOE_8;
	boardConfiguration->ignitionPins[3] = GPIO_UNASSIGNED;

	boardConfiguration->mainRelayPin = GPIOE_6;

	boardConfiguration->idleValvePin = GPIOC_13;
	boardConfiguration->idleSolenoidFrequency = 300;

	boardConfiguration->fuelPumpPin = GPIOE_3;
	boardConfiguration->fuelPumpPinMode = OM_DEFAULT;

	boardConfiguration->triggerInputPins[0] = GPIOA_5;
	boardConfiguration->triggerInputPins[1] = GPIOC_6;

	/**
	 * Frankenso analog #1 PC2 ADC12 CLT
	 * Frankenso analog #2 PC1 ADC11 IAT
	 * Frankenso analog #3 PA0 ADC0
	 * Frankenso analog #4 PC3 ADC13
	 * Frankenso analog #5 PA2 ADC2
	 * Frankenso analog #6 PA1 ADC1
	 * Frankenso analog #7 PA4 ADC4 WBO AFR
	 * Frankenso analog #8 PA3 ADC3
	 * Frankenso analog #9 PA7 ADC7
	 * Frankenso analog #10 PA6 ADC6 MAP
	 * Frankenso analog #11 PC5 ADC15 TPS
	 * Frankenso analog #12 PC4 ADC14 VBatt
	 */


	setThermistorConfiguration(&engineConfiguration->clt, 0, 32500, 30, 7550, 100, 700);
	engineConfiguration->clt.bias_resistor = 10000;

	setThermistorConfiguration(&engineConfiguration->iat, 0, 32500, 30, 7550, 100, 700);
	engineConfiguration->iat.bias_resistor = 10000;

	/**
	 * MAP PA6
	 */
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_6;

	boardConfiguration->adcHwChannelEnabled[0] = ADC_SLOW; // ADC0 - PA0
	boardConfiguration->adcHwChannelEnabled[1] = ADC_SLOW; // just for test
	boardConfiguration->adcHwChannelEnabled[2] = ADC_SLOW; // just for test
	boardConfiguration->adcHwChannelEnabled[3] = ADC_SLOW; // just for test

	boardConfiguration->adcHwChannelEnabled[4] = ADC_SLOW;
	boardConfiguration->adcHwChannelEnabled[5] = ADC_OFF;
	boardConfiguration->adcHwChannelEnabled[6] = ADC_FAST;
	boardConfiguration->adcHwChannelEnabled[7] = ADC_SLOW; // just for test

	boardConfiguration->adcHwChannelEnabled[8] = ADC_OFF;
	boardConfiguration->adcHwChannelEnabled[9] = ADC_OFF;
	boardConfiguration->adcHwChannelEnabled[10] = ADC_OFF;
	boardConfiguration->adcHwChannelEnabled[11] = ADC_SLOW; // IAT

	boardConfiguration->adcHwChannelEnabled[12] = ADC_SLOW; // CLT
	boardConfiguration->adcHwChannelEnabled[13] = ADC_SLOW; // AFR
	boardConfiguration->adcHwChannelEnabled[14] = ADC_SLOW; // VBatt
	boardConfiguration->adcHwChannelEnabled[15] = ADC_SLOW; // TPS


	/**
	 * TPS
	 */
	engineConfiguration->tpsAdcChannel = EFI_ADC_15;
	engineConfiguration->tpsMin = 125; // convert 12to10 bit (ADC/4)
	engineConfiguration->tpsMax = 625; // convert 12to10 bit (ADC/4)

	/**
	 * IAT D15/W7
	 */
	engineConfiguration->iatAdcChannel = EFI_ADC_11;

	/**
	 * CLT D13/W9
	 */
	engineConfiguration->cltAdcChannel = EFI_ADC_12;


	boardConfiguration->analogChartMode = AC_MAP;
	boardConfiguration->isFastAdcEnabled = true;
	engineConfiguration->map.sensor.type = MT_DODGE_NEON_2003;

#if EFI_HIP_9011 || defined(__DOXYGEN__)
	setHip9011FrankensoPinout();
#endif
	engineConfiguration->cylinderBore = 87.5;

//	boardConfiguration->clutchDownPin = GPIOC_12;
	boardConfiguration->clutchDownPinMode = PI_PULLUP;
	engineConfiguration->clutchUpPin = GPIOD_3;
	engineConfiguration->clutchUpPinMode = PI_PULLUP;

	// alt GPIOC_12
	
	/**
	 * set_fsio_setting 0 0.11
	 */
	engineConfiguration->bc.fsio_setting[0] = 0.2;
#if EFI_FSIO || defined(__DOXYGEN__)
	boardConfiguration->fsio_setting[0] = 0.55;
	setFsioExt(engineConfiguration, 0, GPIOE_5, "0 fsio_setting", 400);
#endif

	engineConfiguration->vehicleSpeedSensorInputPin = GPIOA_8;
	engineConfiguration->hasVehicleSpeedSensor = true;

	engineConfiguration->fanOnTemperature = 85;
	engineConfiguration->fanOffTemperature = 80;

	boardConfiguration->tunerStudioSerialSpeed = 9600;
	engineConfiguration->algorithm = LM_SPEED_DENSITY;

	boardConfiguration->alternatorControlPin = GPIOD_5;

//	engineConfiguration->isCanEnabled = true;
	boardConfiguration->canTxPin = GPIOB_6;
	boardConfiguration->canRxPin = GPIOB_12;
	engineConfiguration->canWriteEnabled = true;
	engineConfiguration->canReadEnabled = false;
	engineConfiguration->canNbcType = CAN_BUS_NBC_BMW;
//	engineConfiguration->canNbcType = CAN_BUS_MAZDA_RX8;
}

#endif /* EFI_SUPPORT_DODGE_NEON */

