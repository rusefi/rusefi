/**
 * @file	can_dash.h
 *
 * @date Mar 19, 2020
 * @author Matthew Kennedy, (c) 2020
 */

#pragma once

#define CAM_5ms         (1<<0)
#define CAM_10ms        (1<<1)
#define CAM_20ms        (1<<2)
#define CAM_50ms        (1<<3)
#define CAM_100ms       (1<<4)
#define CAM_200ms       (1<<5)
#define CAM_250ms       (1<<6)
#define CAM_500ms       (1<<7)
#define CAM_1000ms      (1<<8)

void canDashboardBMW(uint16_t cycle);
void canDashboardFiat(uint16_t cycle);
void canDashboardVAG(uint16_t cycle);
void canMazdaRX8(uint16_t cycle);
void canDashboardW202(uint16_t cycle);
void canDashboardBMWE90(uint16_t cycle);
void canDashboardHaltech(uint16_t cycle);
void canDashboardVagMqb();
