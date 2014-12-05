/**
 * @file logic_expression.cpp
 * @brief Logical expressions handling logic
 *
 * Here we parse and evaluate logical expressions in
 * http://en.wikipedia.org/wiki/Reverse_Polish_notation
 *
 * Once the expressions are parsed on startup (that's a heavy operation),
 * evaluating those is relatively efficient.
 *
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
static LENameOrdinalPair leNot(LE_OPERATOR_NOT, "not");

static LENameOrdinalPair leAdd(LE_OPERATOR_ADDITION, "+");
static LENameOrdinalPair leSub(LE_OPERATOR_SUBSTRACTION, "-");
static LENameOrdinalPair leMul(LE_OPERATOR_MULTIPLICATION, "*");
static LENameOrdinalPair leDiv(LE_OPERATOR_DIVISION, "/");

static LENameOrdinalPair leMore(LE_OPERATOR_MORE, ">");
static LENameOrdinalPair leMoreOrEqual(LE_OPERATOR_MORE_OR_EQUAL, ">=");

static LENameOrdinalPair leLess(LE_OPERATOR_LESS, "<");
static LENameOrdinalPair leLessOrEquals(LE_OPERATOR_LESS_OR_EQUAL, "<=");

static LENameOrdinalPair leMax(LE_METHOD_MAX, "max");
static LENameOrdinalPair leMin(LE_METHOD_MIN, "min");

#define LE_EVAL_POOL_SIZE 32

#if EFI_PROD_CODE || EFI_SIMULATOR
static Logging logger;
#endif

static LECalculator evalCalc;
static LEElement evalPoolElements[LE_EVAL_POOL_SIZE];
static LEElementPool evalPool(evalPoolElements, LE_EVAL_POOL_SIZE);

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

float LECalculator::pop(le_action_e action) {
	if (stack.size() == 0) {
		firmwareError("empty stack for %d", action);
		return NAN;
	}
	return stack.pop();
}

void LECalculator::doJob(Engine *engine, LEElement *element) {
	switch (element->action) {

	case LE_NUMERIC_VALUE:
		stack.push(element->fValue);
		break;
	case LE_OPERATOR_AND: {
		float v1 = pop(LE_OPERATOR_AND);
		float v2 = pop(LE_OPERATOR_AND);

		stack.push(float2bool(v1) && float2bool(v2));
	}
		break;
	case LE_OPERATOR_OR: {
		float v1 = pop(LE_OPERATOR_OR);
		float v2 = pop(LE_OPERATOR_OR);

		stack.push(float2bool(v1) || float2bool(v2));
	}
		break;
	case LE_OPERATOR_LESS: {
		// elements on stack are in reverse order
		float v2 = pop(LE_OPERATOR_LESS);
		float v1 = pop(LE_OPERATOR_LESS);

		stack.push(v1 < v2);
	}
		break;
	case LE_OPERATOR_NOT: {
		float v = pop(LE_OPERATOR_NOT);
		stack.push(!float2bool(v));
	}
		break;
	case LE_OPERATOR_MORE: {
		// elements on stack are in reverse order
		float v2 = pop(LE_OPERATOR_MORE);
		float v1 = pop(LE_OPERATOR_MORE);

		stack.push(v1 > v2);
	}
	break;
	case LE_OPERATOR_ADDITION: {
		// elements on stack are in reverse order
		float v2 = pop(LE_OPERATOR_MORE);
		float v1 = pop(LE_OPERATOR_MORE);

		stack.push(v1 + v2);
	}
	break;
	case LE_OPERATOR_SUBSTRACTION: {
		// elements on stack are in reverse order
		float v2 = pop(LE_OPERATOR_MORE);
		float v1 = pop(LE_OPERATOR_MORE);

		stack.push(v1 - v2);
	}
	break;
	case LE_OPERATOR_MULTIPLICATION: {
		// elements on stack are in reverse order
		float v2 = pop(LE_OPERATOR_MORE);
		float v1 = pop(LE_OPERATOR_MORE);

		stack.push(v1 * v2);
	}
	break;
	case LE_OPERATOR_DIVISION: {
		// elements on stack are in reverse order
		float v2 = pop(LE_OPERATOR_MORE);
		float v1 = pop(LE_OPERATOR_MORE);

		stack.push(v1 / v2);
	}
		break;
	case LE_OPERATOR_LESS_OR_EQUAL: {
		// elements on stack are in reverse order
		float v2 = pop(LE_OPERATOR_LESS_OR_EQUAL);
		float v1 = pop(LE_OPERATOR_LESS_OR_EQUAL);

		stack.push(v1 <= v2);
	}
		break;
	case LE_OPERATOR_MORE_OR_EQUAL: {
		// elements on stack are in reverse order
		float v2 = pop(LE_OPERATOR_MORE_OR_EQUAL);
		float v1 = pop(LE_OPERATOR_MORE_OR_EQUAL);

		stack.push(v1 >= v2);
	}
		break;
	case LE_METHOD_MAX: {
		float v2 = pop(LE_METHOD_MAX);
		float v1 = pop(LE_METHOD_MAX);
		stack.push(maxF(v1, v2));
	}
	break;
	case LE_METHOD_MIN: {
		float v2 = pop(LE_METHOD_MIN);
		float v1 = pop(LE_METHOD_MIN);
		stack.push(minF(v1, v2));
	}
	break;

	case LE_UNDEFINED:
		firmwareError("Undefined not expected here");
		break;
	default:
		stack.push(getLEValue(engine, element->action));
	}
}

float LECalculator::getValue2(LEElement *element, Engine *engine) {
	reset(element);
	return getValue(engine);
}

float LECalculator::getValue(Engine *engine) {
	LEElement *element = first;

	stack.reset();

	while (element != NULL) {
		doJob(engine, element);
		element = element->next;
	}
	efiAssert(stack.size() == 1, "One value expected on stack", NAN);

	return stack.pop();
}

LEElementPool::LEElementPool(LEElement *pool, int size) {
	this->pool = pool;
	this->size = size;
	reset();
}

void LEElementPool::reset() {
	index = 0;
}

LEElement *LEElementPool::next() {
	if (index == size - 1) {
		// todo: this should not be a fatal error, just an error
		firmwareError("LE_ELEMENT_POOL_SIZE overflow");
		return NULL;
	}
	return &pool[index++];
}

bool isNumeric(const char* line) {
	return line[0] >= '0' && line[0] <= '9';
}

const char *getNextToken(const char *line, char *buffer) {
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

LEElement *LEElementPool::parseExpression(const char * line) {

	LEElement *first = NULL;
	LEElement *last = NULL;

	while (true) {
		line = getNextToken(line, parsingBuffer);

		if (line == NULL) {
			/**
			 * No more tokens in this line
			 */
			return first;
		}

		LEElement *n = next();

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

#if (EFI_PROD_CODE || EFI_SIMULATOR)

static void eval(char *line, Engine *engine) {
	line = unquote(line);
	scheduleMsg(&logger, "Parsing [%s]", line);
	evalPool.reset();
	LEElement * e = evalPool.parseExpression(line);
	if (e == NULL) {
		scheduleMsg(&logger, "parsing failed");
	} else {
		float result = evalCalc.getValue2(e, engine);
		scheduleMsg(&logger, "Eval result: %f", result);
	}
}

void initEval(Engine *engine) {
	initLogging(&logger, "le");
	addConsoleActionSP("eval", (VoidCharPtrVoidPtr) eval, engine);
}

#endif
