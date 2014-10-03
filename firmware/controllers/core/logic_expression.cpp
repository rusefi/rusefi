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

float LECalculator::getValue() {
	LEElement *element = first;

	stack.reset();

	while(element != NULL) {

		stack.push(element->fValue);


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
