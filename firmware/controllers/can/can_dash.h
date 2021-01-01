/**
 * @file	can_dash.h
 *
 * @date Mar 19, 2020
 * @author Matthew Kennedy, (c) 2020
 */

#pragma once

#define CAM_10ms        0x01
#define CAM_20ms        0x02
#define CAM_50ms        0x04
#define CAM_100ms       0x08
#define CAM_250ms       0x10
#define CAM_500ms       0x20
#define CAM_1000ms      0x40

void canDashboardBMW(uint8_t cycle);
void canDashboardFiat(uint8_t cycle);
void canDashboardVAG(uint8_t cycle);
void canMazdaRX8(uint8_t cycle);
void canDashboardW202(uint8_t cycle);
void canDashboardBMWE90(uint8_t cycle);