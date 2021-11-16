/**
 * @file test_logic_expression.cpp
 *
 * https://sourceforge.net/p/rusefi/tickets/102/
 *
 * @date Oct 3, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "fsio_impl.h"
#include "cli_registry.h"

#define TEST_POOL_SIZE 256

FsioResult getEngineValue(le_action_e action) {
	switch(action) {
	case LE_METHOD_FAN:
		return engine->fsioState.mockFan;
	case LE_METHOD_COOLANT:
		return Sensor::getOrZero(SensorType::Clt);
	case LE_METHOD_RPM:
		return engine->fsioState.mockRpm;
	case LE_METHOD_CRANKING_RPM:
		return engine->fsioState.mockCrankingRpm;
	case LE_METHOD_TIME_SINCE_BOOT:
		return engine->fsioState.mockTimeSinceBoot;
	case LE_METHOD_STARTUP_FUEL_PUMP_DURATION:
		return 2.0f;
	case LE_METHOD_TIME_SINCE_TRIGGER_EVENT:
		return engine->fsioState.mockTimeSinceTrigger;
	case LE_METHOD_VBATT:
		return 12;
	case LE_METHOD_AC_TOGGLE:
		return getAcToggle();
	case LE_METHOD_IS_COOLANT_BROKEN:
		return 0;
#include "fsio_getters.def"
	default:
	firmwareError(OBD_PCM_Processor_Fault, "FSIO: No mock value for %d", action);
		return unexpected;
	}
}

TEST(fsio, testTokenizer) {
	char buffer[64];

	ASSERT_TRUE(strEqualCaseInsensitive("hello", "HELlo"));
	ASSERT_FALSE(strEqualCaseInsensitive("hello", "HElo2"));

	const char *ptr;
	ptr = getNextToken("  hello  ", buffer, sizeof(buffer));
	ASSERT_TRUE(strEqual("hello", buffer));

	ptr = getNextToken("hello", buffer, sizeof(buffer));
	ASSERT_TRUE(strEqual("hello", buffer));

	ptr = getNextToken("  hello  world ", buffer, sizeof(buffer));
	ASSERT_TRUE(strEqual("hello", buffer));
	ptr = getNextToken(ptr, buffer, sizeof(buffer));
	ASSERT_TRUE(strEqual("world", buffer));

	ASSERT_TRUE(isNumeric("123"));
	ASSERT_FALSE(isNumeric("a123"));
}

TEST(fsio, testParsing) {
	LEElement thepool[TEST_POOL_SIZE];
	LEElementPool pool(thepool, TEST_POOL_SIZE);

	LEElement *element = pool.parseExpression("1 3 AND not");
	ASSERT_TRUE(element != NULL);

	ASSERT_EQ(element[0].action, LE_NUMERIC_VALUE);
	ASSERT_EQ(element[0].fValue, 1.0);

	ASSERT_EQ(element[1].action, LE_NUMERIC_VALUE);
	ASSERT_EQ(element[1].fValue, 3.0);

	ASSERT_EQ(element[2].action, LE_OPERATOR_AND);

	ASSERT_EQ(element[3].action, LE_OPERATOR_NOT);

	// last should be a return instruction
	ASSERT_EQ(element[4].action, LE_METHOD_RETURN);

	ASSERT_EQ(pool.getSize(), 5);
}

TEST(fsio, parsingMultiple) {
	LEElement poolArr[TEST_POOL_SIZE];
	LEElementPool pool(poolArr, TEST_POOL_SIZE);

	LEElement* p1 = pool.parseExpression("2");
	ASSERT_EQ(p1[0].action, LE_NUMERIC_VALUE);
	ASSERT_EQ(p1[0].fValue, 2);
	ASSERT_EQ(p1[1].action, LE_METHOD_RETURN);

	LEElement* p2 = pool.parseExpression("4");
	ASSERT_EQ(p2[0].action, LE_NUMERIC_VALUE);
	ASSERT_EQ(p2[0].fValue, 4);
	ASSERT_EQ(p2[1].action, LE_METHOD_RETURN);

	// Check that they got allocated sequentially without overlap
	ASSERT_EQ(p2 - p1, 2);
}

static void testExpression2(float selfValue, const char *line, float expected, Engine *engine) {
	LEElement thepool[TEST_POOL_SIZE];
	LEElementPool pool(thepool, TEST_POOL_SIZE);
	LEElement * element = pool.parseExpression(line);
	printf("Parsing [%s]\n", line);
	ASSERT_TRUE(element != NULL) << "Not NULL expected";
	LECalculator c;

	EXPAND_Engine;

	ASSERT_NEAR(expected, c.evaluate("test", selfValue, element), EPS4D) << line;
}

static void testExpression2(float selfValue, const char *line, float expected, const std::unordered_map<SensorType, float>& sensorVals = {}) {
	WITH_ENGINE_TEST_HELPER_SENS(FORD_INLINE_6_1995, sensorVals);
	testExpression2(selfValue, line, expected, engine);
}

static void testExpression(const char *line, float expectedValue, const std::unordered_map<SensorType, float>& sensorVals = {}) {
	testExpression2(0, line, expectedValue, sensorVals);
}

TEST(fsio, testHysteresisSelf) {
	WITH_ENGINE_TEST_HELPER(FORD_INLINE_6_1995);

	LEElement thepool[TEST_POOL_SIZE];
	LEElementPool pool(thepool, TEST_POOL_SIZE);
	// value ON: 450
	// value OFF: 400
	// Human formula: (self and (rpm > 400)) | (rpm > 450)
	LEElement * element = pool.parseExpression("self rpm 400 > and rpm 450 > |");
	ASSERT_TRUE(element != NULL) << "Not NULL expected";

	LECalculator c;
	double selfValue = 0;

	engine->fsioState.mockRpm = 0;
	selfValue = c.evaluate("test", selfValue, element);
	ASSERT_EQ(0, selfValue);

	engine->fsioState.mockRpm = 430;
	selfValue = c.evaluate("test", selfValue, element);
	// OFF since not ON yet
	ASSERT_EQ(0, selfValue);

	engine->fsioState.mockRpm = 460;
	selfValue = c.evaluate("test", selfValue, element);
	ASSERT_EQ(1, selfValue);

	engine->fsioState.mockRpm = 430;
	selfValue = c.evaluate("test", selfValue, element);
	// OFF since was ON yet
	ASSERT_EQ(1, selfValue);
}

TEST(fsio, testLiterals) {
	// Constants - single token
	testExpression("123", 123.0f);
	testExpression("true", 1);
	testExpression("false", 0);
}

TEST(fsio, mathOperators) {
	// Test basic operations
	testExpression("123 456 +", 579);
	testExpression("123 456 -", -333);
	testExpression("123 456 *", 56088);
	testExpression("123 456 /", 0.269737f);
}

TEST(fsio, comparisonOperators) {
	// Comparison operators
	testExpression("123 456 >", 0);
	testExpression("123 456 >=", 0);
	testExpression("123 456 <", 1);
	testExpression("123 456 <=", 1);
	testExpression("123 456 min", 123);
	testExpression("123 456 max", 456);
}

TEST(fsio, booleanOperators) {
	// Boolean operators
	testExpression("true true and", 1);
	testExpression("true false and", 0);
	testExpression("true false or", 1);
	testExpression("false false or", 0);
	// (both ways to write and/or)
	testExpression("true true &", 1);
	testExpression("true false &", 0);
	testExpression("true false |", 1);
	testExpression("false false |", 0);

	// not operator
	testExpression("true not", 0);
	testExpression("false not", 1);
}

TEST(fsio, extraOperators) {
	// Self operator
	testExpression2(123, "self 1 +", 124);

	// ternary operator
	testExpression("1 22 33 if", 22);
	testExpression("0 22 33 if", 33);
}

TEST(fsio, invalidFunction) {
	EXPECT_FATAL_ERROR(testExpression("bogus_function", 0));
	EXPECT_FATAL_ERROR(testExpression("1 2 + bogus_expression *", 0));
}

TEST(fsio, testLogicExpressions) {
	/**
	 * fan = (not fan && coolant > 90) OR (fan && coolant > 85)
	 * fan = fan NOT coolant 90 AND more fan coolant 85 more AND OR
	 */

	std::unordered_map<SensorType, float> sensorVals = {{SensorType::Clt, 100}};
	testExpression("coolant 1 +", 101, sensorVals);

	testExpression("fan", 0, sensorVals);
	testExpression("fan not", 1, sensorVals);
	testExpression("coolant 90 >", 1, sensorVals);
	testExpression("fan not coolant 90 > and", 1, sensorVals);

	testExpression("fan NOT coolant 90 > AND fan coolant 85 > AND OR", 1, sensorVals);

	{
		WITH_ENGINE_TEST_HELPER_SENS(FORD_INLINE_6_1995, sensorVals);
		LEElement thepool[TEST_POOL_SIZE];
		LEElementPool pool(thepool, TEST_POOL_SIZE);
		LEElement * element = pool.parseExpression("fan NOT coolant 90 > AND fan coolant 85 > AND OR");
		ASSERT_TRUE(element != NULL) << "Not NULL expected";
		LECalculator c;
		ASSERT_EQ( 1,  c.evaluate("test", 0, element)) << "that expression";

		ASSERT_EQ(12, c.currentCalculationLogPosition);
		ASSERT_EQ(102, c.calcLogAction[0]);
		ASSERT_EQ(0, c.calcLogValue[0]);
	}

	{
		WITH_ENGINE_TEST_HELPER_SENS(FORD_INLINE_6_1995, sensorVals);
		engine->fsioState.mockRpm = 900;
		engine->fsioState.mockCrankingRpm = 200;
		testExpression2(0, "rpm", 900, engine);
		testExpression2(0, "cranking_rpm", 200, engine);
		testExpression2(0, STARTER_RELAY_LOGIC, 0, engine);
		testExpression2(0, "rpm cranking_rpm > ", 1, engine);
	}
}

TEST(fsio, fuelPump) {
	// this will init fuel pump fsio logic
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	// Mock a fuel pump pin
	CONFIG(fuelPumpPin) = GPIOA_0;
	// Re-init so it picks up the new config
	enginePins.fuelPumpRelay.init();

	// ECU just started, haven't seen trigger yet
	engine->fsioState.mockTimeSinceBoot = 0.5f;
	engine->fsioState.mockTimeSinceTrigger = 100;
	runFsio();
	// Pump should be on!
	EXPECT_TRUE(efiReadPin(GPIOA_0));

	// Long time since ecu start, haven't seen trigger yet
	engine->fsioState.mockTimeSinceBoot = 60;
	engine->fsioState.mockTimeSinceTrigger = 100;
	runFsio();
	// Pump should be off!
	EXPECT_FALSE(efiReadPin(GPIOA_0));

	// Long time since ecu start, just saw a trigger!
	engine->fsioState.mockTimeSinceBoot = 60;
	engine->fsioState.mockTimeSinceTrigger = 0.1f;
	runFsio();
	// Pump should be on!
	EXPECT_TRUE(efiReadPin(GPIOA_0));

	// ECU just started, and we just saw a trigger!
	engine->fsioState.mockTimeSinceBoot = 0.5f;
	engine->fsioState.mockTimeSinceTrigger = 0.1f;
	runFsio();
	// Pump should be on!
	EXPECT_TRUE(efiReadPin(GPIOA_0));
}

TEST(fsio, fsioValueFloat) {
	FsioValue floatVal(3.5f);

	EXPECT_TRUE(floatVal.isFloat());
	EXPECT_FALSE(floatVal.isBool());

	EXPECT_FLOAT_EQ(floatVal.asFloat(), 3.5f);
}

TEST(fsio, fsioValueFloatZero) {
	FsioValue floatVal(0.0f);

	EXPECT_TRUE(floatVal.isFloat());
	EXPECT_FALSE(floatVal.isBool());

	EXPECT_FLOAT_EQ(floatVal.asFloat(), 0);
}

TEST(fsio, fsioValueBoolTrue) {
	FsioValue boolVal(true);

	EXPECT_TRUE(boolVal.isBool());
	EXPECT_FALSE(boolVal.isFloat());

	EXPECT_TRUE(boolVal.asBool());
}

TEST(fsio, fsioValueBoolFalse) {
	FsioValue boolVal(false);

	EXPECT_TRUE(boolVal.isBool());
	EXPECT_FALSE(boolVal.isFloat());

	EXPECT_FALSE(boolVal.asBool());
}
