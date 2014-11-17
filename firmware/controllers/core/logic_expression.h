/**
 * @file logic_expression.h
 *
 * @date Oct 3, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef LOGIC_EXPRESSION_H_
#define LOGIC_EXPRESSION_H_

#include "rusefi_enums.h"
#include "fl_stack.h"
#include "engine.h"

typedef enum {

	LE_UNDEFINED = 0 ,
	LE_NUMERIC_VALUE = 1,
	LE_OPERATOR_LESS = 2,
	LE_OPERATOR_MORE = 3,
	LE_OPERATOR_LESS_OR_EQUAL = 4,
	LE_OPERATOR_MORE_OR_EQUAL = 5,
	LE_OPERATOR_AND = 6,
	LE_OPERATOR_OR = 7,
	LE_OPERATOR_NOT = 8,

	LE_METHOD_RPM = 100,
	LE_METHOD_COOLANT = 101,
	LE_METHOD_FAN = 102,
	LE_METHOD_TIME_SINCE_BOOT = 103,
	LE_METHOD_FAN_ON_SETTING = 104,
	LE_METHOD_FAN_OFF_SETTING = 105,
	LE_METHOD_TPS = 106,
	LE_METHOD_MAF = 107,
	LE_METHOD_INTAKE_AIR = 108,
	LE_METHOD_VBATT = 109,

	Force_4b_le_action = ENUM_SIZE_HACK,

} le_action_e;

class LEElement {
public:
	LEElement();
//	void init(le_action_e action, int iValue);
	void init(le_action_e action);
	void init(le_action_e action, float fValue);

	le_action_e action;
	float fValue;
	int iValue;

	LEElement *next;
};

#define LE_ELEMENT_POOL_SIZE 256

class LEElementPool {
public:
	LEElementPool(int size);
	LEElement thepool[LE_ELEMENT_POOL_SIZE];
	LEElement *pool;
	LEElement *next();
	void reset();
private:
	int index;
	int size;
};


#define MAX_STACK_DEPTH 32

class LECalculator {
public:
	LECalculator();
	float getValue(Engine *engine);
	float getValue2(LEElement *element, Engine *engine);
	void add(LEElement *element);
	void reset();
	void reset(LEElement *element);

private:
	void doJob(Engine *engine, LEElement *element);
	float pop(le_action_e action);
	LEElement *first;
	FLStack<float, MAX_STACK_DEPTH> stack;
};

class LENameOrdinalPair {
public:
	LENameOrdinalPair(le_action_e action, const char *name);
	LENameOrdinalPair *next;
	le_action_e action;
	const char *name;
};

/**
 * This method extract the first token on the line into the specified buffer
 *
 * @return pointer after the token
 */
const char *getNextToken(const char *line, char *buffer);
bool isNumeric(const char* line);
le_action_e parseAction(const char * line);
LEElement * parseExpression(LEElementPool *pool, const char * line);

#endif /* LOGIC_EXPRESSION_H_ */
