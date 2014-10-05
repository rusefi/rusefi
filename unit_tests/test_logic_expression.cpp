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
#include "cli_registry.h"
#include "engine.h"

static float mockCoolant;
static float mockFan;

float getLEValue(Engine *engine, le_action_e action) {
	switch(action) {
	case LE_METHOD_FAN:
		return mockFan;
	case LE_METHOD_COOLANT:
		return mockCoolant;
	default:
		firmwareError("No value for %d", action);
	}
}


static void testParsing(void) {
	char buffer[64];

	assertTrue(strEqualCaseInsensitive("hello", "HELlo"));
	assertFalse(strEqualCaseInsensitive("hello", "HElo2"));

	const char *ptr;
	ptr = processToken("  hello  ", buffer);
	assertTrue(strEqual("hello", buffer));

	ptr = processToken("hello", buffer);
	assertTrue(strEqual("hello", buffer));

	ptr = processToken("  hello  world ", buffer);
	assertTrue(strEqual("hello", buffer));
	ptr = processToken(ptr, buffer);
	assertTrue(strEqual("world", buffer));

	assertTrue(isNumeric("123"));
	assertFalse(isNumeric("a123"));

	LEElementPool pool;

	LEElement *element;
	element = parseExpression(&pool, "1 3 AND not");
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
	LEElementPool pool;
	pool.reset();
	LEElement * element = parseExpression(&pool, line);
	assertTrueM("Not NULL expected", element != NULL);
	LECalculator c;
	c.add(element);
	assertEqualsM(line, expected, c.getValue());
}

void testLogicExpressions(void) {
	printf("*************************************************** testLogicExpressions\r\n");

	testParsing();

	LECalculator c;

	LEElement value1;
	value1.init(LE_NUMERIC_VALUE, 123.0);
	c.add(&value1);

	assertEqualsM("123", 123.0, c.getValue());

	LEElement value2;
	value2.init(LE_NUMERIC_VALUE, 321.0);
	c.add(&value2);

	LEElement value3;
	value3.init(LE_OPERATOR_AND);
	c.add(&value3);
	assertEqualsM("123 and 321", 1.0, c.getValue());

	/**
	 * fuel_pump = (time_since_boot < 4 seconds) OR (rpm > 0)
	 * fuel_pump = time_since_boot 4 less rpm 0 more OR
	 */

	c.reset();

	LEElementPool pool;
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
	element = parseExpression(&pool, "fan no_such_method");
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


	testExpression("fan NOT coolant 90 > AND fan coolant 85 > AND OR", 1);

}
