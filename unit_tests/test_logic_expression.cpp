/**
 * @file test_logic_expression.cpp
 *
 * https://sourceforge.net/p/rusefi/tickets/102/
 *
 * @date Oct 3, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "test_logic_expression.h"
#include "logic_expression.h"
#include "le_functions.h"
#include "cli_registry.h"
#include "engine.h"

#define TEST_POOL_SIZE 256

static float mockCoolant;
static float mockFan;
static float mockRpm;
static float mockTimeSinceBoot;

float getLEValue(Engine *engine, calc_stack_t *s, le_action_e action) {
	switch(action) {
	case LE_METHOD_FAN:
		return mockFan;
	case LE_METHOD_COOLANT:
		return mockCoolant;
	case LE_METHOD_RPM:
		return mockRpm;
	case LE_METHOD_TIME_SINCE_BOOT:
		return mockTimeSinceBoot;
	case LE_METHOD_FAN_ON_SETTING:
		return 0;
	case LE_METHOD_FAN_OFF_SETTING:
		return 0;
	default:
	firmwareError("No mock value for %d", action);
		return NAN;
	}
}

static void testParsing(void) {
	char buffer[64];

	assertTrue(strEqualCaseInsensitive("hello", "HELlo"));
	assertFalse(strEqualCaseInsensitive("hello", "HElo2"));

	const char *ptr;
	ptr = getNextToken("  hello  ", buffer);
	assertTrue(strEqual("hello", buffer));

	ptr = getNextToken("hello", buffer);
	assertTrue(strEqual("hello", buffer));

	ptr = getNextToken("  hello  world ", buffer);
	assertTrue(strEqual("hello", buffer));
	ptr = getNextToken(ptr, buffer);
	assertTrue(strEqual("world", buffer));

	assertTrue(isNumeric("123"));
	assertFalse(isNumeric("a123"));

	LEElement thepool[TEST_POOL_SIZE];
	LEElementPool pool(thepool, TEST_POOL_SIZE);

	LEElement *element;
	element = pool.parseExpression("1 3 AND not");
	assertTrue(element != NULL);

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
	assertTrue(element == NULL);
}

static void testExpression(const char *line, float expected) {
	LEElement thepool[TEST_POOL_SIZE];
	LEElementPool pool(thepool, TEST_POOL_SIZE);
	pool.reset();
	LEElement * element = pool.parseExpression(line);
	print("Parsing [%s]", line);
	assertTrueM("Not NULL expected", element != NULL);
	LECalculator c;
	c.add(element);
	assertEqualsM(line, expected, c.getValue(NULL));
}

void testLogicExpressions(void) {
	printf("*************************************************** testLogicExpressions\r\n");

	testParsing();

	LECalculator c;

	LEElement value1;
	value1.init(LE_NUMERIC_VALUE, 123.0);
	c.add(&value1);

	assertEqualsM("123", 123.0, c.getValue(NULL));

	LEElement value2;
	value2.init(LE_NUMERIC_VALUE, 321.0);
	c.add(&value2);

	LEElement value3;
	value3.init(LE_OPERATOR_AND);
	c.add(&value3);
	assertEqualsM("123 and 321", 1.0, c.getValue(NULL));

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

	testExpression("fan NOT coolant 90 > AND fan coolant 85 > AND OR", 1);
	testExpression("coolant", 100);
	testExpression("fan_off_setting", 0);
	testExpression("coolant fan_off_setting >", 1);

	testExpression("0 1 &", 0);
	testExpression("0 1 |", 1);

	testExpression("0 1 >", 0);

	testExpression(FAN_CONTROL_LOGIC, 1);

	mockRpm = 900;
	testExpression(FUEL_PUMP_LOGIC, 1);

}
