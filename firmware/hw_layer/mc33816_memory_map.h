/**
 * see mc33816/rusefi/readme.md
*/

typedef enum {
    // see dram1.def values
    Iboost = 0,
    Ipeak = 1,
    Ihold = 2,
    Tpeak_off = 3,
    Tpeak_tot = 4,
    Tbypass = 5,
    Thold_off = 6,
    Thold_tot = 7,
    // see dram2.def values, base 64 for channel 2
    Vboost_high = 64,
    Vboost_low = 65,
    Isense4_high = 66,
    Isense4_low = 67,
    HPFP_Ipeak = 69,
    HPFP_Ihold = 70,
    HPFP_Thold_off = 71,
    HPFP_Thold_tot = 72,
} MC33816Mem;
