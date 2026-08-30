/**
 * @file boards/m74_9/efifeatures.h
 *
 * @brief In this header we can override efifeatures.h.
 *
 * @date Jan 01, 2023
 * @author Andrey Gusakov, 2023
 */

/* Override some settings */
#define HAL_USE_MMC_SPI FALSE
#define EFI_FILE_LOGGING FALSE

/* The application runs on top of the OpenBLT bootloader (USE_OPENBLT=yes in
 * meta-info.env). Without this define the app-side OpenBLT machinery is
 * compiled out entirely: the canOpenBLT CAN trigger, the reboot_openblt
 * console command and show_blt_version. Must be defined BEFORE the
 * stm32f4ems include - that header defaults the flag to FALSE. */
#define EFI_USE_OPENBLT TRUE

/* AT32F435 NZW flash is too slow for the per-tooth trigger path at high rpm
 * (~25 cycles per 7-instruction iteration at 288 MHz). Run the trigger decode
 * and event scheduling from zero-wait SRAM: the .fast_text linker section is
 * copied from flash at boot (see AT32F435ZMxx.ld and board.c __early_init). */
#define EFI_TRIGGER_IN_RAM TRUE

/* TMR2 hardware angle clock (OC channels as software comparators, events
 * programmed one tooth ahead, fired by a priority-3 ISR): removes the
 * trigger-decode/handoff latency from the spark/injection timing chain.
 * Must be defined BEFORE the stm32f4ems include - that header defaults the
 * flag to FALSE. See hw_layer/angle_clock/angle_clock.h.
 *
 * DISABLED 2026-08-30 after the first car start blew the 15A fuse (all
 * four coils overcharged 4.5-8.2 ms at the catch, C9012 out-of-order coil
 * off). The TMR2 driver itself is datasheet-correct and the clock measures
 * right (40000/40000 vs NT), so the defect is in the one-tooth-ahead
 * scheduling logic - pending the bench investigation. With the flag FALSE
 * the early windows compile out and ignition/fuel return to the proven
 * time-based paths. */
#define EFI_ANGLE_CLOCK FALSE

/* Console log buffer (double-buffered, 2x this, static). The stm32f4ems
 * default of 6500 truncates the 'pins' output mid-line: the pin report +
 * L9779 SPI frame history exceed it and LogBuffer::writeInternal writes
 * partial lines when full. 16384 x 2 = 32 KB static - fine on the 384 KB
 * AT32F435 (the memcore pool still has ~200 KB left). */
#define DL_OUTPUT_BUFFER 16384

#include "../../stm32f4ems/efifeatures.h"

/* This board has no USB wired out */
#undef EFI_USB_SERIAL
#define EFI_USB_SERIAL              FALSE

/* Fast ADC (TIM6 -> ADC2, MAP sampling) is enabled (default TRUE from
 * stm32f4ems) and functional: the AT32 DMA1/DMA2 DMAMUX (TBL_SEL flexible
 * mapping) is fixed in the ChibiOS fork - see
 * ChibiOS/os/hal/ports/AT32/AT32F4xx/hal_lld.c (DMA clocks before dmaInit)
 * and at32_registry.h (STM32_DMA_HAS_DMAMUXSEL). */

/* CLT/IAT (and O2/AC-pressure) live on ADC3-only pins (PF5/PF6/F10/F3) - the
 * slow ADC (ADC1) cannot see them. Sample ADC3 channels EFI_ADC_32..39 in the
 * slow loop (stm32_adc_v2.cpp convGroupSlowAdc3). */
#define EFI_ADC3_SLOW                TRUE
