/*
 * test_cj125.cpp
 *
 *  Created on: Jan 3, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#include "gtest/gtest.h"
#include "cj125_logic.h"
#include "engine_test_helper.h"

static void applyHeaterPinState(PwmConfig *state, int stateIndex) {

}

class TestSpi : public Cj125SpiStream {
public:
	MOCK_METHOD1(ReadRegister, uint8_t(uint8_t));
	MOCK_METHOD2(WriteRegister, void(uint8_t, uint8_t));
};

TEST(testCJ125, testInitialState) {
	CJ125 cj;

	ASSERT_EQ(cj.state, CJ125_INIT);
	ASSERT_FALSE(cj.isWorkingState());
	ASSERT_EQ(cj.heaterDuty, 0);

	WITH_ENGINE_TEST_HELPER(FORD_ASPIRE_1996);
	ASSERT_EQ(engine->sensors.vBatt, 0);

	cj.StartHeaterControl(&applyHeaterPinState PASS_ENGINE_PARAMETER_SUFFIX);
	ASSERT_EQ(cj.heaterDuty, CJ125_HEATER_IDLE_RATE);

	TestSpi mock;
	cj.spi = &mock;

	EXPECT_CALL(mock, ReadRegister(IDENT_REG_RD)).Times(1);
	EXPECT_CALL(mock, ReadRegister(INIT_REG1_RD)).Times(1);
	EXPECT_CALL(mock, ReadRegister(INIT_REG2_RD)).Times(1);
	EXPECT_CALL(mock, ReadRegister(DIAG_REG_RD)).Times(1);

	cj.cjIdentify();

}


