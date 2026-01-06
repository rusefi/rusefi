// DTCM memory is 128k
#define CCM_OPTIONAL __attribute__((section(".ram3")))
//TODO: update LD file!
#define SDRAM_OPTIONAL __attribute__((section(".ram7")))

// SRAM2 is 16k and set to disable dcache (see STM32_NOCACHE_ENABLE in mcuconf.h)
// we have another way to put something in no cache area - __nocache_ prefix in name
#define NO_CACHE __attribute__((aligned(4))) __attribute__((section(".ram2")))

// TODO: test and switch to this
// Current ChibiOS puts nocache data into SRAM3/DTCM that is not chached by design
//#define NO_CACHE __attribute__((section(".ram3")))

#define BKUP_RAM_NOINIT __attribute__((section(".bkup_ram_noinit")))
