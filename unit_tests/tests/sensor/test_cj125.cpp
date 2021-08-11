/*
 * @file test_cj125.cpp
 *
 *  Created on: Jan 3, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "cj125_logic.h"

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

	cj.StartHeaterControl(PASS_ENGINE_PARAMETER_SIGNATURE);
	ASSERT_EQ(cj.heaterDuty, CJ125_HEATER_IDLE_RATE);

	TestSpi mock;
	cj.spi = &mock;

	EXPECT_CALL(mock, ReadRegister(IDENT_REG_RD)).Times(1).WillOnce(Return(CJ125_IDENT));
	EXPECT_CALL(mock, ReadRegister(INIT_REG1_RD)).Times(1).WillOnce(Return(CJ125_INIT1_NORMAL_17));
	EXPECT_CALL(mock, ReadRegister(INIT_REG2_RD)).Times(1).WillOnce(Return(CJ125_INIT2_DIAG));
	EXPECT_CALL(mock, ReadRegister(DIAG_REG_RD)).Times(1);

	cj.cjIdentify(PASS_ENGINE_PARAMETER_SIGNATURE);
	// validate that error state was not set
	ASSERT_EQ(cj.state, CJ125_INIT);
}


TEST(testCJ125, testFailedIdentify) {
	CJ125 cj;

	ASSERT_EQ(cj.state, CJ125_INIT);

	TestSpi mock;
	cj.spi = &mock;

	WITH_ENGINE_TEST_HELPER(FORD_ASPIRE_1996);

	cj.cjIdentify(PASS_ENGINE_PARAMETER_SIGNATURE);
	ASSERT_EQ(cj.errorCode, CJ125_ERROR_WRONG_IDENT);
	ASSERT_EQ(cj.state, CJ125_ERROR);
}

TEST(testCJ125, testMode) {



}
