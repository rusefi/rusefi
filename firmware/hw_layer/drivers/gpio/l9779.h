/*
 * l9779.h
 * ST L9779WD-SPI
 *
 *
 *  Created on: Jan 10, 2022
 */

#pragma once

#include "global.h"

#include <hal.h>
#include "efifeatures.h"

#define L9779_OUTPUTS_IGN       (4)
/* 4 x IGNI, IN1..IN6, PWM (IN7) */
#define L9779_DIRECT_OUTPUTS    (L9779_OUTPUTS_IGN + 7)
#define L9779_OUTPUTS           (L9779_OUTPUTS_IGN + 28 + 1)
#define L9779_INPUTS            (1)

#define L9779_SIGNALS           (L9779_OUTPUTS + L9779_INPUTS)

struct l9779_config {
#if HAL_USE_SPI
    SPIDriver       *spi_bus;
    SPIConfig       spi_config;
#endif
    /* MCU port-pin routed to IGN1..IGN4, IN1..7 */
    struct {
        ioportid_t      port;
        uint_fast8_t    pad;
    } direct_gpio[L9779_DIRECT_OUTPUTS];
    /* PWM(IN8) */
    struct {
        ioportid_t      port;
        uint_fast8_t    pad;
    } pwm_gpio;
};

int l9779_add(brain_pin_e base, unsigned int index, const l9779_config *cfg);

/* Snapshot the VDA 2.0 watchdog state for diagnostics. */
bool l9779_getWdaCounters(uint8_t *ec, bool *wdaInt, int *ok, int *fail,
	int *timingMiss, uint8_t *dia10, int *delayMs, int *deferCount,
	int *killCount, uint8_t *requhi, int *wrongCount, int *countBad);

/* Request an ignition-gated power-stage transition. The flag may be changed
 * from an interrupt; all SPI work is deferred to the L9779 driver thread. */
void l9779_setPowerStage(bool on);
