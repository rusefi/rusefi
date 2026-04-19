/**
 * @brief   Sleep at idle.
 * @details This option enables call to __WFI() from idle thread to save power.
 */
#ifndef CORTEX_ENABLE_WFI_IDLE
#define CORTEX_ENABLE_WFI_IDLE FALSE
#endif

/* Use common ST32 ChibiOS config */
#include "chconf_stm32.h"
