// DTCM memory is 128k
#define CCM_OPTIONAL __attribute__((section(".ram3")))
//TODO: update LD file!
#define SDRAM_OPTIONAL __attribute__((section(".ram7")))
// SRAM3/DTCM is 128K and not cached by design
#define NO_CACHE __attribute__((section(".ram3")))

#define BKUP_RAM_NOINIT __attribute__((section(".bkup_ram_noinit")))
