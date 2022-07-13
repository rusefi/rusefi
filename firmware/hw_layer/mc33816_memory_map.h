/**
 * see mc33816/rusefi/readme.md
*/

#include "mc33816/rusefi/sample_code/PT2001_dram.h"

typedef enum {
    // see dram1.def values
    Iboost = PT2001_D1_Iboost,
    Ipeak = PT2001_D1_Ipeak,
    Ihold = PT2001_D1_Ihold,
    Tpeak_off = PT2001_D1_Tpeak_off,
    Tpeak_tot = PT2001_D1_Tpeak_tot,
    Tbypass = PT2001_D1_Tbypass,
    Thold_off = PT2001_D1_Thold_off,
    Thold_tot = PT2001_D1_Thold_tot,
    // see dram2.def values, base 64 for channel 2
    Vboost_high = PT2001_D2_Vboost_high,
    Vboost_low = PT2001_D2_Vboost_low,
    Isense4_high = 66,
    Isense4_low = 67,
    HPFP_Ipeak = 69,
    HPFP_Ihold = 70,
    HPFP_Thold_off = 71,
    HPFP_Thold_tot = 72,
} MC33816Mem;
