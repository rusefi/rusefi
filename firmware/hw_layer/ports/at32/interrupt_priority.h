#pragma once

#ifdef EFI_BOOTLOADER
#define EFI_IRQ_EXTI_PRIORITY             6
#else
#define EFI_IRQ_EXTI_PRIORITY             CORTEX_MAXIMUM_PRIORITY
#endif

// The scheduling timer (TIM5 CC1) is the event executor: spark/dwell/injection
// commands fire from its ISR and must have a fixed dispatch latency. It must
// therefore be the HIGHEST-PRIORITY kernel IRQ (CORTEX_MAX_KERNEL_PRIORITY is 3
// on ARMv7-M) - above the trigger handoff and the ADC. This is load-bearing:
// the AT32 mcuconf defines no STM32_PWM_TIM5_IRQ_PRIORITY, so the shared TIMv1
// LLD defaults it to 7 - BELOW the trigger handoff decode (3) and the ADC ISR
// (6). With the default, every trigger decode (44 us average, tails to ~1 ms,
// ~2200/s at 1900 rpm) and every ADC completion delayed the executor: 11-30%
// of all scheduled commands went out >=10 us late (up to 389 us) - the
// measured "floating command execution" behind the stable-rpm misfires and
// coil overcharges (m74_9, 2026-08-23).
// STM32_PWM_TIM5_IRQ_PRIORITY is consumed by the TIMv1 LLD pwm_lld_init ->
// nvicEnableVector(STM32_TIM5_NUMBER, ...); the OSAL validity check requires
// >= CORTEX_MAX_KERNEL_PRIORITY, which 3 satisfies.
#define EFI_IRQ_SCHEDULING_TIMER_PRIORITY 3
#define STM32_PWM_TIM5_IRQ_PRIORITY       EFI_IRQ_SCHEDULING_TIMER_PRIORITY

// The TMR2 angle clock (hardware compare firing for one-tooth-ahead engine
// events) runs at the SAME priority as the TIM5 executor: its ISR executes
// the spark/injection actions directly, so it needs the same fixed entry
// latency and must preempt the trigger handoff. Same-priority means the two
// ISRs never nest into each other, which keeps the channel pool race-free.
#define EFI_IRQ_ANGLE_CLOCK_PRIORITY      3

// Trigger decode runs BELOW the executor on purpose: tooth timestamps are
// captured in the raw EXTI ISR (fast IRQ 0) into a queue before the decode
// starts, so the decode being preempted by due events does not affect tooth
// timing measurements - while the decode's variable duration no longer delays
// command dispatch. With the executor at 3, the handoff at 4 still preempts
// ADC/UART/SysTick/CAN as before.
#define EFI_IRQ_EXTI_HANDOFF_PRIORITY     4
#define EFI_IRQ_ADC_PRIORITY 5

// The L9779 WDA feed (one-shot TIM10 GPT): BELOW the trigger handoff on
// purpose - its ~100 us polled-SPI burst used to run inside the priority-3
// executor ISR, which preempted the handoff every ~105 ms and added up to
// 100 us to the decode chain. The answer window is ~12.6 ms wide, so even a
// 1 ms handoff tail preempting the burst cannot push RESP_BYTE0 out of it.
// It stays ISR-context (never a thread): a thread wakeup under cranking load
// was the original wd_timing_miss/EC>4 kill mode.
#define EFI_IRQ_L9779_WDA_PRIORITY        5
#define EFI_IRQ_UART_PRIORITY 6
#define EFI_IRQ_SYSTICK_PRIORITY 8
#define EFI_IRQ_SDMMC_PRIORITY 10
#define EFI_IRQ_SPI_PRIORITY 10
#define EFI_IRQ_CAN_PRIORITY 11
#define EFI_IRQ_USB_PRIORITY 14
