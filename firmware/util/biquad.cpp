/*
 * @file biquad.cpp
 *
 * @date Sep 10, 2016
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "biquad.h"

Biquad::Biquad() {
	a0 = a1 = a2 = b1 = b2 = 0;
	z1 = z2 = 0;
}

// todo: decouple from engine and just use bi_quard_s
void Biquad::initValue(float input, bi_quard_s *settings) {
	a0 = settings->a0;
	a1 = settings->a1;
	a2 = settings->a2;
	b1 = settings->b1;
	b2 = settings->b2;

	z1 = input * (1 - a0);
	z2 = input * (1 - a0 - a1 + b1);
}

float Biquad::getValue(float input) {
    float result = input * a0 + z1;
    z1 = input * a1 + z2 - b1 * result;
    z2 = input * a2 - b2 * result;
    return result;
}

