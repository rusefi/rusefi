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

	EXPECT_EQ(0, state.getValue());

	state.onNewSignal(2);
	state.onNewSignal(2);
	state.onNewSignal(2);

	// value is still '0' until we signal end of engine cycle
	EXPECT_EQ(0, state.getValue());

	// this signals start of new cycle
	state.onNewSignal(4);
	EXPECT_EQ(3, state.getValue());


}


#endif /* TEST_ION_CPP_ */
