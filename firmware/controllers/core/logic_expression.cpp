/**
 * @file logic_expression.cpp
 *
 * @date Oct 3, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "logic_expression.h"
#include "le_functions.h"

LENameOrdinalPair * LE_FIRST = NULL;

/**
 * the main point of these static fields is that their constructor would register
 * them in the magic list of operator name/ordinal pairs
 */
static LENameOrdinalPair leAnd(LE_OPERATOR_AND, "and");
static LENameOrdinalPair leOr(LE_OPERATOR_OR, "or");
static LENameOrdinalPair leMore(LE_OPERATOR_MORE, ">");
static LENameOrdinalPair leMoreOrEqual(LE_OPERATOR_MORE_OR_EQUAL, ">=");
static LENameOrdinalPair leNot(LE_OPERATOR_NOT, "not");

LENameOrdinalPair::LENameOrdinalPair(le_action_e action, const char *name) {
	this->action = action;
	this->name = name;
	this->next = LE_FIRST;
	LE_FIRST = this;
}

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
	reset();
}

void LECalculator::reset() {
	first = NULL;
	stack.reset();
}

void LECalculator::reset(LEElement *element) {
	first = NULL;
	stack.reset();
	add(element);
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
		// elements on stack are in reverse order
		float v2 = stack.pop();
		float v1 = stack.pop();

		stack.push(v1 < v2);
	}
		break;
	case LE_OPERATOR_NOT: {
		float v = stack.pop();
		stack.push(!float2bool(v));
	}
		break;
	case LE_OPERATOR_MORE: {
		// elements on stack are in reverse order
		float v2 = stack.pop();
		float v1 = stack.pop();

		stack.push(v1 > v2);
	}
		break;
	case LE_OPERATOR_LESS_OR_EQUAL: {
		// elements on stack are in reverse order
		float v2 = stack.pop();
		float v1 = stack.pop();

		stack.push(v1 <= v2);
	}
		break;
	case LE_OPERATOR_MORE_OR_EQUAL: {
		// elements on stack are in reverse order
		float v2 = stack.pop();
		float v1 = stack.pop();

		stack.push(v1 >= v2);
	}
		break;
	case LE_UNDEFINED:
		firmwareError("Undefined not expected here");
		break;
	default:
		stack.push(getLEValue(NULL, element->action));
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

LEElementPool::LEElementPool() {
	reset();
}

void LEElementPool::reset() {
	index = 0;
}

LEElement *LEElementPool::next() {
	if (index == LE_ELEMENT_POOL_SIZE - 1) {
		firmwareError("LE_ELEMENT_POOL_SIZE overflow");
		return NULL;
	}
	return &pool[index++];
}

bool isNumeric(const char* line) {
	return line[0] >= '0' && line[0] <= '9';
}

const char *processToken(const char *line, char *buffer) {
	while (line[0] != 0 && line[0] == ' ') {
		line++;
	}
	if (line[0] == 0) {
		return NULL;
	}
	int tokenLen = indexOf(line, ' ');
	if (tokenLen == -1) {
		// no space - the whole remaining line is the token
		strcpy(buffer, line);
		return line + strlen(buffer);
	}
	strncpy(buffer, line, tokenLen);
	buffer[tokenLen] = 0;
	line += tokenLen;
	return line;
}

le_action_e parseAction(const char * line) {
	LENameOrdinalPair *pair = LE_FIRST;
	while (pair != NULL) {
		if (strEqualCaseInsensitive(pair->name, line)) {
			return pair->action;
		}
		pair = pair->next;
	}
	return LE_UNDEFINED;
}

static char parsingBuffer[64];

LEElement * parseExpression(LEElementPool *pool, const char * line) {

	LEElement *first = NULL;
	LEElement *last = NULL;

	while (true) {
		line = processToken(line, parsingBuffer);

		if (line == NULL) {
			/**
			 * No more tokens in this line
			 */
			return first;
		}

		LEElement *n = pool->next();

		if (isNumeric(parsingBuffer)) {
			n->init(LE_NUMERIC_VALUE, atoff(parsingBuffer));
		} else {
			le_action_e action = parseAction(parsingBuffer);
			if (action == LE_UNDEFINED) {
				/**
				 * Cannot recognize token
				 */
				warning((obd_code_e) 0, "unrecognized [%s]", parsingBuffer);
				return NULL;
			}
			n->init(action);
		}

		if (first == NULL) {
			first = n;
			last = n;
		} else {
			last->next = n;
			last = last->next;
		}
	}
	return first;
}
