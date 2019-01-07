/*
 * test_efilib.cpp
 *
 *  Created on: Jan 6, 2019
 * @author Matthew Kennedy, (c) 2019
 */

#include "gtest/gtest.h"

#include "efilib.h"
#include <math.h>

TEST(EfiLibTest, ExpTaylor)
{
    float x = -2;

    // test from -2 < x < 0
    for(float x = -2; x < 0; x += 0.05)
    {
        // Compare taylor to libc implementation
        EXPECT_NEAR(expf_taylor(x), expf(x), 0.01f);
    }
}
