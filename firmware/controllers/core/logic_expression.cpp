/**
 * @file logic_expression.cpp
 *
 * @date Oct 3, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "logic_expression.h"

LEElement::LEElement() {
	action = LE_UNDEFINED;
	next = NULL;
}

//void LEElement::init(le_action_e action, int iValue) {
//	this->action = action;
//	this->iValue = iValue;
//}

void LEElement::init(le_action_e action) {
	this->action = action;
}

void LEElement::init(le_action_e action, float fValue) {
	this->action = action;
	this->fValue = fValue;
}

LECalculator::LECalculator() {
	first = NULL;
}

static bool float2bool(float v) {
	return v != 0;
}

void LECalculator::doJob(LEElement *element) {
	switch (element->action) {

	case LE_NUMERIC_VALUE:
		stack.push(element->fValue);
		break;
	case LE_OPERATOR_AND: {
		float v1 = stack.pop();
		float v2 = stack.pop();

		stack.push(float2bool(v1) && float2bool(v2));
	}
		break;
	case LE_OPERATOR_OR: {
		float v1 = stack.pop();
		float v2 = stack.pop();

		stack.push(float2bool(v1) || float2bool(v2));
	}
		break;
	case LE_OPERATOR_LESS: {
		float v1 = stack.pop();
		float v2 = stack.pop();

		stack.push(v1 < v2);
	}
		break;
	case LE_OPERATOR_MORE: {
		float v1 = stack.pop();
		float v2 = stack.pop();

		stack.push(v1 > v2);
	}
		break;
	case LE_OPERATOR_LESS_OR_EQUAL: {
		float v1 = stack.pop();
		float v2 = stack.pop();

		stack.push(v1 <= v2);
	}
		break;
	case LE_OPERATOR_MORE_OR_EQUAL: {
		float v1 = stack.pop();
		float v2 = stack.pop();

		stack.push(v1 >= v2);
	}
		break;
	default:
		firmwareError("Not implemented: %d", element->action);

	}

}

float LECalculator::getValue() {
	LEElement *element = first;

	stack.reset();

	while (element != NULL) {
		doJob(element);
		element = element->next;
	}

	return stack.pop();
}

void LECalculator::add(LEElement *element) {
	if (first == NULL) {
		first = element;
	} else {
		LEElement *last = first;
		while (last->next != NULL) {
			last = last->next;
		}
		last->next = element;
	}
}
