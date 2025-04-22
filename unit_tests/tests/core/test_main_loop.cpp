//
// Created by FDSoftware on 21/04/25.
//
#include "pch.h"

#include "main_loop.h"

#define MAIN_LOOP_RATE 1000

//TODO: remove me after pr #7763
int m_cycleCounter = 0;

//TODO: move me to main_loop.cpp after pr #7763
template <LoopPeriod flag>
static constexpr int loopCounts() {
    constexpr auto hz = hzForPeriod(flag);

    // check that this cleanly divides
    static_assert(MAIN_LOOP_RATE % hz == 0);

    return MAIN_LOOP_RATE / hz;
}

//TODO: move me to main_loop.cpp after pr #7763
template <LoopPeriod TFlag>
LoopPeriod makePeriodFlag() {
    if (m_cycleCounter % loopCounts<TFlag>() == 0) {
        return TFlag;
    } else {
        return LoopPeriod::None;
    }
}

//TODO: replaced by MainLoop::makePeriodFlags() after pr #7763
LoopPeriod makePeriodFlags() {
    LoopPeriod lp = LoopPeriod::None;
    lp |= makePeriodFlag<LoopPeriod::Period1000hz>();
    lp |= makePeriodFlag<LoopPeriod::Period500hz>();
    lp |= makePeriodFlag<LoopPeriod::Period250hz>();
    lp |= makePeriodFlag<LoopPeriod::Period200hz>();
    lp |= makePeriodFlag<LoopPeriod::Period20hz>();

    m_cycleCounter++;

    return lp;
}

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
    m_cycleCounter = 501;
    EXPECT_EQ(makePeriodFlag<LoopPeriod::Period500hz>(), LoopPeriod::None);
    // valid value
    m_cycleCounter = 1000;
    EXPECT_EQ(makePeriodFlag<LoopPeriod::Period500hz>(), LoopPeriod::Period500hz);
    // valid value 200Hz
    m_cycleCounter = 1000;
    EXPECT_EQ(makePeriodFlag<LoopPeriod::Period200hz>(), LoopPeriod::Period200hz);
    // invalid value 200Hz
    m_cycleCounter = 343;
    EXPECT_EQ(makePeriodFlag<LoopPeriod::Period200hz>(), LoopPeriod::None);
}

TEST(MainLoop, operatorTest){
    m_cycleCounter = 500;
    LoopPeriod lp = makePeriodFlags();

    EXPECT_TRUE(lp & LoopPeriod::Period1000hz);
    EXPECT_TRUE(lp & LoopPeriod::Period500hz);
    EXPECT_TRUE(lp & LoopPeriod::Period250hz);
    EXPECT_TRUE(lp & LoopPeriod::Period200hz);
    EXPECT_TRUE(lp & LoopPeriod::Period20hz);

    // update time
    m_cycleCounter = 60;
    lp = makePeriodFlags();

    EXPECT_TRUE(lp & LoopPeriod::Period1000hz);
    EXPECT_TRUE(lp & LoopPeriod::Period500hz);
    EXPECT_TRUE(lp & LoopPeriod::Period250hz);
    EXPECT_TRUE(lp & LoopPeriod::Period200hz);
    EXPECT_FALSE(lp & LoopPeriod::Period20hz);

    // update time
    m_cycleCounter = 0;
    lp = makePeriodFlags();

    EXPECT_TRUE(lp & LoopPeriod::Period1000hz);
    EXPECT_TRUE(lp & LoopPeriod::Period500hz);
    EXPECT_TRUE(lp & LoopPeriod::Period250hz);
    EXPECT_TRUE(lp & LoopPeriod::Period200hz);
    EXPECT_TRUE(lp & LoopPeriod::Period20hz);

    // update time
    m_cycleCounter = 1000;
    lp = makePeriodFlags();

    EXPECT_TRUE(lp & LoopPeriod::Period1000hz);
    EXPECT_TRUE(lp & LoopPeriod::Period500hz);
    EXPECT_TRUE(lp & LoopPeriod::Period250hz);
    EXPECT_TRUE(lp & LoopPeriod::Period200hz);
    EXPECT_TRUE(lp & LoopPeriod::Period20hz);

    // update time
    m_cycleCounter = 423;
    lp = makePeriodFlags();

    EXPECT_TRUE(lp & LoopPeriod::Period1000hz);
    EXPECT_FALSE(lp & LoopPeriod::Period500hz);
    EXPECT_FALSE(lp & LoopPeriod::Period250hz);
    EXPECT_FALSE(lp & LoopPeriod::Period20hz);
    EXPECT_FALSE(lp & LoopPeriod::Period200hz);

    auto operatorResult = LoopPeriod::None & LoopPeriod::Period500hz;
    EXPECT_FALSE(operatorResult);
}