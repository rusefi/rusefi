/**
 * rusEfi is placing some of data structures into CCM memory simply
 * in order to use that memory - no magic about which RAM is faster etc.
 * That said, CCM/TCM could be faster as there will be less bus contention
 * with DMA.
 *
 * Please note that DMA does not work with CCM memory
 */

#if defined(AT32F4XX)
#define CCM_OPTIONAL
#define SDRAM_OPTIONAL
#define NO_CACHE __attribute__((aligned(4)))
#define BKUP_RAM_NOINIT
#else
// CCM memory is 64k
#define CCM_OPTIONAL __attribute__((section(".ram4")))
#define SDRAM_OPTIONAL __attribute__((section(".ram7")))
#define NO_CACHE __attribute__((aligned(4)))	// F4 has no cache, do nothing
#define BKUP_RAM_NOINIT __attribute__((section(".bkup_ram_noinit")))
#define MCU_HAS_CCM_RAM	TRUE
#endif
