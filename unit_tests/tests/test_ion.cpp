/*
 * @file test_ion.cpp
 *
 * @date Jan 4, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "gtest/gtest.h"
#include "cdm_ion_sense.h"

TEST(ion, signalCounter) {
	CdmState state;

	EXPECT_EQ(0, state.getValue(0));

	state.onNewSignal(/* currentRevolution= */ 2);
	state.onNewSignal(/* currentRevolution= */ 2);
	state.onNewSignal(/* currentRevolution= */ 2);

	// value is still '0' until we signal end of engine cycle
	EXPECT_EQ(0, state.getValue(/* currentRevolution= */2));

	// this invocation would flush current accumulation
	EXPECT_EQ(3, state.getValue(/* currentRevolution= */3));

	state.onNewSignal(/* currentRevolution= */3);
	// returning previous full cycle value
	EXPECT_EQ(3, state.getValue(3));


	EXPECT_EQ(1, state.getValue(/* currentRevolution= */4));
	EXPECT_EQ(0, state.getValue(/* currentRevolution= */5));
}

