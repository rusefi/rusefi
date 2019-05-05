/*
 * test_ion.cpp
 *
 *  Created on: Jan 4, 2019
 *      Author: user
 */

#ifndef TEST_ION_CPP_
#define TEST_ION_CPP_

#include "gtest/gtest.h"
#include "cdm_ion_sense.h"

TEST(ion, signalCounter) {
	CdmState state;

	EXPECT_EQ(0, state.getValue(0));

	state.onNewSignal(2);
	state.onNewSignal(2);
	state.onNewSignal(2);

	// value is still '0' until we signal end of engine cycle
	EXPECT_EQ(0, state.getValue(2));

	// this invocation would flush current accumulation
	EXPECT_EQ(3, state.getValue(3));

	state.onNewSignal(3);
	// returning previous full cycle value
	EXPECT_EQ(3, state.getValue(3));


	EXPECT_EQ(1, state.getValue(4));
	EXPECT_EQ(0, state.getValue(5));




}


#endif /* TEST_ION_CPP_ */
