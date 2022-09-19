/*
 * @file test_hellen_board_id.cpp
 *
 * @date Jan 20, 2022
 * @author andreika <prometheus.pcb@gmail.com>
 * @author Andrey Belomutskiy, (c) 2012-2022
 */

#include "pch.h"
#include "gtest/gtest.h"
#include "hellen_meta.h"
#include "digital_input_exti.h"

#include "hellen_board_id.h"

TEST(hellen_board_id, testNewtonSolver) {
	// let's prove that our formula is independent of the threshold voltage:
	HellenBoardIdSolver solver;
	// 1.5V threshold
	EXPECT_NEAR(1100, solver.solve(666.74938f, 353.32522f, 1000.0f, 1.0f, 1.0f), 0.001);
	// 2.5V threshold
	EXPECT_NEAR(1100, solver.solve(1558.773f, 1335.563f, 1000.0f, 1.0f, 1.0f), 0.001);
}

TEST(hellen_board_id, testClosestResistor) {
	HellenBoardIdFinderBase finder;
	int rIdx;
	// use only major series
	EXPECT_FLOAT_EQ(1000, finder.findClosestResistor(876, true, &rIdx));

	// break point between 1000 and 1200 = ~1091
	EXPECT_FLOAT_EQ(1000, finder.findClosestResistor(1090, true, &rIdx));
	EXPECT_FLOAT_EQ(1200, finder.findClosestResistor(1091, true, &rIdx));
	// use full series
	EXPECT_FLOAT_EQ(1000, finder.findClosestResistor(1047, false, &rIdx));
	EXPECT_FLOAT_EQ(1100, finder.findClosestResistor(1049, false, &rIdx));
	EXPECT_FLOAT_EQ(1100, finder.findClosestResistor(1147, false, &rIdx));
	EXPECT_FLOAT_EQ(1200, finder.findClosestResistor(1149, false, &rIdx));

	EXPECT_FLOAT_EQ(510, finder.findClosestResistor(0, true, &rIdx));
	ASSERT_EQ(0, rIdx);
}

TEST(hellen_board_id, testEstimatedResistor) {
	HellenBoardIdFinderBase finder;
	EXPECT_NEAR(10000, finder.calcEstimatedResistance(6931.4718055995f, 1.0f), 0.001);
}

TEST(hellen_board_id, testCalc) {
	HellenBoardIdFinderBase finder;
	float Rmeasured, newC;
	int rIdx;
	float R = finder.calc(1024.714f, 724.639555f, 1099.0f, 1.0f, false, &Rmeasured, &newC, &rIdx);
	EXPECT_NEAR(1100, R, 0.001);
	EXPECT_NEAR(1099.998779, Rmeasured, 0.001);
	EXPECT_NEAR(0.980316, newC, 0.001);
	ASSERT_EQ(19, rIdx);
}
