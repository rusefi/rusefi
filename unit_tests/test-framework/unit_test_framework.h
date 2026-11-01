/*
 * @file unit_test_framework.h
 *
 * @date Mar 4, 2018
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

using ::testing::Return;

// This lets us inspect private state from unit tests
// cool hack but probably should not be used, even conditional private/public compilation is more obvious
//#define private public

/**
 * These are usually used with ASSERT_NEAR(val1, val2, abs_error)
 */
#define EPS0D 1
#define EPS1D 0.1f
#define EPS2D 0.01f
#define EPS3D 0.001f
#define EPS4D 0.0001f
#define EPS5D 0.00001f

#define EXPECT_NO_FATAL_ERROR EXPECT_NO_THROW
#define EXPECT_FATAL_ERROR(expr) EXPECT_THROW((expr), std::logic_error)
