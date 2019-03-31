/*
 * @file biquad.h
 *
 * @date Sep 10, 2016
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef CONTROLLERS_MATH_BIQUAD_H_
#define CONTROLLERS_MATH_BIQUAD_H_

// todo: narrow this dependency further? only 'bi_quard_s' is needed, should it be extracted / moved to a smaller header?
// todo: do we need to make code generation smarted and produce a larger number of smaller headers instead of one monster header?
#include "engine_configuration.h"

class Biquad {
public:
    Biquad();
    void initValue(float input, bi_quard_s *settings);
    float getValue(float input);

    float a0, a1, a2, b1, b2;
    float z1, z2;
};

#endif /* CONTROLLERS_MATH_BIQUAD_H_ */
