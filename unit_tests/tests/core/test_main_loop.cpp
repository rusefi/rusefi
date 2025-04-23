//
// Created by FDSoftware on 21/04/25.
//
#include "pch.h"

#include "main_loop.h"
#include "main_loop_controller.h"
#define MAIN_LOOP_RATE 1000

TEST(MainLoop, hzForPeriodTest){
    // Invalid value, should return 0
    EXPECT_EQ(hzForPeriod((LoopPeriod)-1), 0);
    // valid value
    EXPECT_EQ(hzForPeriod(LoopPeriod::Period500hz), 500);
}

TEST(MainLoop, loopCountsTest){
    // valid value
    EXPECT_EQ(loopCounts<LoopPeriod::Period500hz>(), 2);
    // valid value
    EXPECT_EQ(loopCounts<LoopPeriod::Period1000hz>(), 1);
}

TEST(MainLoop, makePeriodFlagTest){
    // no in exec time, should be None
    mainLoop.m_cycleCounter = 501;
    EXPECT_EQ(mainLoop.makePeriodFlag<LoopPeriod::Period500hz>(), LoopPeriod::None);
    // valid value
    mainLoop.m_cycleCounter = 1000;
    EXPECT_EQ(mainLoop.makePeriodFlag<LoopPeriod::Period500hz>(), LoopPeriod::Period500hz);
    // valid value 200Hz
    mainLoop.m_cycleCounter = 1000;
    EXPECT_EQ(mainLoop.makePeriodFlag<LoopPeriod::Period200hz>(), LoopPeriod::Period200hz);
    // invalid value 200Hz
    mainLoop.m_cycleCounter = 343;
    EXPECT_EQ(mainLoop.makePeriodFlag<LoopPeriod::Period200hz>(), LoopPeriod::None);
}

TEST(MainLoop, operatorTest){
    mainLoop.m_cycleCounter = 500;
    LoopPeriod lp = mainLoop.makePeriodFlags();

    EXPECT_TRUE(lp & LoopPeriod::Period1000hz);
    EXPECT_TRUE(lp & LoopPeriod::Period500hz);
    EXPECT_TRUE(lp & LoopPeriod::Period250hz);
    EXPECT_TRUE(lp & LoopPeriod::Period200hz);
    EXPECT_TRUE(lp & LoopPeriod::Period20hz);

    // update time
    mainLoop.m_cycleCounter = 60;
    lp = mainLoop.makePeriodFlags();

    EXPECT_TRUE(lp & LoopPeriod::Period1000hz);
    EXPECT_TRUE(lp & LoopPeriod::Period500hz);
    EXPECT_TRUE(lp & LoopPeriod::Period250hz);
    EXPECT_TRUE(lp & LoopPeriod::Period200hz);
    EXPECT_FALSE(lp & LoopPeriod::Period20hz);

    // update time
    mainLoop.m_cycleCounter = 0;
    lp = mainLoop.makePeriodFlags();

    EXPECT_TRUE(lp & LoopPeriod::Period1000hz);
    EXPECT_TRUE(lp & LoopPeriod::Period500hz);
    EXPECT_TRUE(lp & LoopPeriod::Period250hz);
    EXPECT_TRUE(lp & LoopPeriod::Period200hz);
    EXPECT_TRUE(lp & LoopPeriod::Period20hz);

    // update time
    mainLoop.m_cycleCounter = 1000;
    lp = mainLoop.makePeriodFlags();

    EXPECT_TRUE(lp & LoopPeriod::Period1000hz);
    EXPECT_TRUE(lp & LoopPeriod::Period500hz);
    EXPECT_TRUE(lp & LoopPeriod::Period250hz);
    EXPECT_TRUE(lp & LoopPeriod::Period200hz);
    EXPECT_TRUE(lp & LoopPeriod::Period20hz);

    // update time
    mainLoop.m_cycleCounter = 423;
    lp = mainLoop.makePeriodFlags();

    EXPECT_TRUE(lp & LoopPeriod::Period1000hz);
    EXPECT_FALSE(lp & LoopPeriod::Period500hz);
    EXPECT_FALSE(lp & LoopPeriod::Period250hz);
    EXPECT_FALSE(lp & LoopPeriod::Period20hz);
    EXPECT_FALSE(lp & LoopPeriod::Period200hz);

    auto operatorResult = LoopPeriod::None & LoopPeriod::Period500hz;
    EXPECT_FALSE(operatorResult);
}