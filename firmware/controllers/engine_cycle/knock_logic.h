/*
 * @file knock_logic.h
 *
 * @date Apr 04, 2021
 * @author Andrey Gusakov
 */

#pragma once

int getCylinderKnockBank(uint8_t cylinderIndex);

void onSparkFireKnockSense(uint8_t cylinderIndex, efitick_t nowNt);
void onKnockSenseCompleted(uint8_t cylinderIndex, float levelDbv, efitick_t lastKnockTime);
