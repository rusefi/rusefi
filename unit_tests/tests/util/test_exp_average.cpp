#include "pch.h"

#include "exp_average.h"

TEST(exp_average, length1) {
    ExpAverage ea;
    ea.init(1);
    ASSERT_NEAR(ea.average(3), 3, EPS2D);
    ASSERT_NEAR(ea.average(8), 8, EPS2D);
    ASSERT_NEAR(ea.average(3), 3, EPS2D);
    ASSERT_NEAR(ea.average(8), 8, EPS2D);
}

TEST(exp_average, length2) {
    ExpAverage ea;
    ea.init(2);
    ASSERT_NEAR(ea.average(3), 2, EPS2D);
    ASSERT_NEAR(ea.average(8), 6, EPS2D);
    ASSERT_NEAR(ea.average(3), 4, EPS2D);
    ASSERT_NEAR(ea.average(8), 6.6666, EPS2D);
}

TEST(exp_average, length12) {
    ExpAverage ea;
    ea.init(12);
    ASSERT_NEAR(ea.average(3), 0.4615, EPS2D);
    ASSERT_NEAR(ea.average(8), 1.6213, EPS2D);
    ASSERT_NEAR(ea.average(3), 1.8334, EPS2D);
    ASSERT_NEAR(ea.average(8), 2.7821, EPS2D);

    for (int i = 0; i < 300; i++) {
    	ea.average(3);
    	ea.average(8);
    }
    ASSERT_NEAR(ea.average(3), 5.2916, EPS2D);
}
