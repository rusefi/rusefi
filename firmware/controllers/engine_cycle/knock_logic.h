/*
 * @file knock_logic.h
 *
 * @date Apr 04, 2021
 * @author Andrey Gusakov
 */

#pragma once

int getCylinderKnockBank(uint8_t cylinderIndex);

void onKnockSenseCompleted(uint8_t cylinderIndex, float levelDbv, efitick_t lastKnockTime);
