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

/* TMR2 hardware angle clock (OC channels as software comparators, events
 * programmed one tooth ahead, fired by a priority-3 ISR): removes the
 * trigger-decode/handoff latency from the spark/injection timing chain.
 * Must be defined BEFORE the stm32f4ems include - that header defaults the
 * flag to FALSE. See hw_layer/angle_clock/angle_clock.h.
 *
 * HISTORY: disabled 2026-08-30 after the first car start blew the 15A fuse
 * (C9012 out-of-order coil off + C9351-4 overcharge 4.5-8.2 ms at the
 * catch). Re-enabled 2026-08-31 on the sleep-gate theory (rccEnableTIM2
 * false->true) - the fuse blew AGAIN at the first crank attempt, so the
 * sleep gate was NOT the cause (or not the only one). Re-enabled again
 * 2026-08-31 after the redesign (angle-domain arming + per-tooth refresh,
 * charge-anchored overdwell rescue) and the car defect fix (no
 * angleClockCancelAll on the rpm==0 storm flap - see CLAUDE.md "THE CAR
 * DEFECT (2026-08-31 evening)"). With the flag FALSE the early windows
 * compile out and ignition/fuel return to the proven time-based paths. */
#define EFI_ANGLE_CLOCK TRUE

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
