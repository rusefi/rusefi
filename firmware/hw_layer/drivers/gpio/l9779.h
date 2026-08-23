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

/* WDA (VDA 2.0) watchdog counters of the first registered L9779: the last
 * error counter value, the WDA_INT flag (EC > 4 -> WDA output pin low), and
 * the totals of answered/missed response cycles. Cross-driver diagnostics -
 * e.g. the TLE9201 drop warning on boards where the L9779 WDA output kills
 * the ETB bridge (m74_9 ETC_WD chain). timing_miss counts responses that
 * landed outside the answer window (REQUHI flags) - NOT the same as fail,
 * which only counts SPI-level errors; the EC climbs on timing misses too.
 * dia10 returns the cached DIA_REG10 byte (CRK_RST=0x20, V3V3_UV=0x04,
 * OV_RST=0x01, OUT_DIS=0x02, ...) - the chip's own power-event flags.
 * Returns false when no L9779 chip is registered on this board. */
bool l9779_getWdaCounters(uint8_t *ec, bool *wda_int, int *ok, int *fail, int *timing_miss, uint8_t *dia10);
