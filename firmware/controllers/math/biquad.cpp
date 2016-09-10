/*
 * @file biquad.cpp
 *
 * @date Sep 10, 2016
 * @author Andrey Belomutskiy, (c) 2012-2016
 */

#include "biquad.h"

Biquad::Biquad() {
	a0 = 0.0000024635293743901;
	a1 = 0.00000492705874878021;
	a2 = 0.0000024635293743901;
	b1 = -1.9968534854;
	b2 = 0.9968633396;

	z1 = z2 = 0;
}

float Biquad::getValue(float input) {
    float result = input * a0 + z1;
    z1 = input * a1 + z2 - b1 * result;
    z2 = input * a2 - b2 * result;
    return result;
}

