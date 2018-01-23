/*
 * @file biquad.cpp
 *
 * @date Sep 10, 2016
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "biquad.h"

EXTERN_ENGINE;

Biquad::Biquad() {
	a0 = a1 = a2 = b1 = b2;
	z1 = z2 = 0;
}

void Biquad::initValue(float input DECLARE_ENGINE_PARAMETER_SUFFIX) {
	a0 = engineConfiguration->biQuad.a0;
	a1 = engineConfiguration->biQuad.a1;
	a2 = engineConfiguration->biQuad.a2;
	b1 = engineConfiguration->biQuad.b1;
	b2 = engineConfiguration->biQuad.b2;

	z1 = input * (1 - a0);
	z2 = input * (1 - a0 - a1 + b1);
}

float Biquad::getValue(float input) {
    float result = input * a0 + z1;
    z1 = input * a1 + z2 - b1 * result;
    z2 = input * a2 - b2 * result;
    return result;
}

