/**
 * @file test_logic_expression.cpp
 *
 * https://sourceforge.net/p/rusefi/tickets/102/
 *
 * @date Oct 3, 2014
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "global.h"
#include "fsio_impl.h"
#include "cli_registry.h"
#include "engine_test_helper.h"

#define TEST_POOL_SIZE 256

static float mockCoolant;
static float mockFan;
static float mockRpm;
static float mockCrankingRpm;
static float mockTimeSinceBoot;

float getEngineValue(le_action_e action DECLARE_ENGINE_PARAMETER_SUFFIX) {
	switch(action) {
	case LE_METHOD_FAN:
		return mockFan;
	case LE_METHOD_COOLANT:
		return mockCoolant;
	case LE_METHOD_RPM:
		return mockRpm;
	case LE_METHOD_CRANKING_RPM:
		return mockCrankingRpm;
	case LE_METHOD_TIME_SINCE_BOOT:
		return mockTimeSinceBoot;
	case LE_METHOD_FAN_ON_SETTING:
		return 0;
	case LE_METHOD_FAN_OFF_SETTING:
		return 0;
	default:
	firmwareError(OBD_PCM_Processor_Fault, "No mock value for %d", action);
		return NAN;
	}
}

static void testParsing(void) {
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

	LEElement thepool[TEST_POOL_SIZE];
	LEElementPool pool(thepool, TEST_POOL_SIZE);

	LEElement *element;
	element = pool.parseExpression("1 3 AND not");
	ASSERT_TRUE(element != NULL);

	assertEquals(element->action, LE_NUMERIC_VALUE);
	assertEquals(element->fValue, 1.0);

	element = element->next;
	assertEquals(element->action, LE_NUMERIC_VALUE);
	assertEquals(element->fValue, 3.0);

	element = element->next;
	assertEquals(element->action, LE_OPERATOR_AND);

	element = element->next;
	assertEquals(element->action, LE_OPERATOR_NOT);

	element = element->next;
	ASSERT_TRUE(element == NULL);
}

static void testExpression2(float selfValue, const char *line, float expected) {
	LEElement thepool[TEST_POOL_SIZE];
	LEElementPool pool(thepool, TEST_POOL_SIZE);
	LEElement * element = pool.parseExpression(line);
	print("Parsing [%s]", line);
	assertTrueM("Not NULL expected", element != NULL);
	LECalculator c;

	EngineTestHelper eth(FORD_INLINE_6_1995);
	EXPAND_EngineTestHelper;

	assertEqualsM(line, expected, c.getValue2(selfValue, element PASS_ENGINE_PARAMETER_SUFFIX));
}

static void testExpression(const char *line, float expected) {
	testExpression2(0, line, expected);
}

TEST(misc, testLogicExpressions) {
	printf("*************************************************** testLogicExpressions\r\n");

	testParsing();

	EngineTestHelper eth(FORD_INLINE_6_1995);
	EXPAND_EngineTestHelper;

	LECalculator c;

	LEElement value1;
	value1.init(LE_NUMERIC_VALUE, 123.0);
	c.add(&value1);

	assertEqualsM("123", 123.0, c.getValue(0 PASS_ENGINE_PARAMETER_SUFFIX));

	LEElement value2;
	value2.init(LE_NUMERIC_VALUE, 321.0);
	c.add(&value2);

	LEElement value3;
	value3.init(LE_OPERATOR_AND);
	c.add(&value3);
	assertEqualsM("123 and 321", 1.0, c.getValue(0 PASS_ENGINE_PARAMETER_SUFFIX));

	/**
	 * fuel_pump = (time_since_boot < 4 seconds) OR (rpm > 0)
	 * fuel_pump = time_since_boot 4 less rpm 0 > OR
	 */

	c.reset();

	LEElement thepool[TEST_POOL_SIZE];
	LEElementPool pool(thepool, TEST_POOL_SIZE);
	LEElement *e = pool.next();
	e->init(LE_METHOD_TIME_SINCE_BOOT);

	e = pool.next();
	e->init(LE_NUMERIC_VALUE, 4);

	e = pool.next();
	e->init(LE_OPERATOR_LESS);

	e = pool.next();
	e->init(LE_METHOD_RPM);

	e = pool.next();
	e->init(LE_NUMERIC_VALUE, 0);

	e = pool.next();
	e->init(LE_OPERATOR_MORE);

	e = pool.next();
	e->init(LE_OPERATOR_OR);

	pool.reset();
	LEElement *element;
	element = pool.parseExpression("fan no_such_method");
	assertTrueM("NULL expected", element == NULL);


	/**
	 * fan = (not fan && coolant > 90) OR (fan && coolant > 85)
	 * fan = fan NOT coolant 90 AND more fan coolant 85 more AND OR
	 */


	mockFan = 0;
	mockCoolant = 100;

	testExpression("coolant", 100);
	testExpression("fan", 0);
	testExpression("fan not", 1);
	testExpression("coolant 90 >", 1);
	testExpression("fan not coolant 90 > and", 1);

	testExpression("100 200 1 if", 200);
	testExpression("10 99 max", 99);

	testExpression2(123, "10 self max", 123);

	testExpression("fan NOT coolant 90 > AND fan coolant 85 > AND OR", 1);
	{
		LEElement thepool[TEST_POOL_SIZE];
		LEElementPool pool(thepool, TEST_POOL_SIZE);
		LEElement * element = pool.parseExpression("fan NOT coolant 90 > AND fan coolant 85 > AND OR");
		assertTrueM("Not NULL expected", element != NULL);
		LECalculator c;
		assertEqualsM("that expression", 1, c.getValue2(0, element PASS_ENGINE_PARAMETER_SUFFIX));

		assertEquals(12, c.currentCalculationLogPosition);
		assertEquals(102, c.calcLogAction[0]);
		assertEquals(0, c.calcLogValue[0]);
	}

	testExpression("coolant", 100);
	testExpression("fan_off_setting", 0);
	testExpression("coolant fan_off_setting >", 1);

	testExpression("0 1 &", 0);
	testExpression("0 1 |", 1);

	testExpression("0 1 >", 0);

	testExpression(FAN_CONTROL_LOGIC, 1);

	{
		mockRpm = 900;
		mockCrankingRpm = 200;
		testExpression("rpm", 900);
		testExpression("cranking_rpm", 200);
		testExpression(STARTER_BLOCK, 0);
		testExpression("rpm cranking_rpm > ", 1);

	}
	mockRpm = 900;
	testExpression("fan NOT coolant 90 > AND fan coolant 85 > AND OR", 1);

}
