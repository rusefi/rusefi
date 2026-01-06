// DTCM memory is 128k
#define CCM_OPTIONAL __attribute__((section(".ram5")))
//TODO: update LD file!
#define SDRAM_OPTIONAL __attribute__((section(".ram8")))
// SRAM3 is 32k and set to disable dcache
#define NO_CACHE __attribute__((aligned(4))) __attribute__((section(".ram3")))

#define BKUP_RAM_NOINIT __attribute__((section(".bkup_ram_noinit")))
