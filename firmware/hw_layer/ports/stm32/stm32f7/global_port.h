// DTCM memory is 128k
#define CCM_OPTIONAL __attribute__((section(".ram3")))
//TODO: update LD file!
#define SDRAM_OPTIONAL __attribute__((section(".ram7")))
// SRAM2 is 16k and set to disable dcache
#define NO_CACHE __attribute__((section(".ram2")))

#define BKUP_RAM_NOINIT __attribute__((section(".bkup_ram_noinit")))
