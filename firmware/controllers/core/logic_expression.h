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

typedef enum {
	LE_UNDEFINED,
	LE_NUMERIC_VALUE,
	LE_OPERATOR_LESS,
	LE_OPERATOR_MORE,
	LE_OPERATOR_LESS_OR_EQUAL,
	LE_OPERATOR_MORE_OR_EQUAL,
	LE_OPERATOR_AND,
	LE_OPERATOR_OR,

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


#define MAX_STACK_DEPTH 32

class LECalculator {
public:
	LECalculator();
	float getValue();
	void add(LEElement *element);
	LEElement *first;

private:
	void doJob(LEElement *element);
	FLStack<float, MAX_STACK_DEPTH> stack;
};

#endif /* LOGIC_EXPRESSION_H_ */
