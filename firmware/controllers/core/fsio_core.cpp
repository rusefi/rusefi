/**
 * @file fsio_core.cpp
 * @brief core FSUI handling logic
 *
 * Here we parse and evaluate logical expressions in
 * http://en.wikipedia.org/wiki/Reverse_Polish_notation
 *
 * Once the expressions are parsed on startup (that's a heavy operation),
 * evaluating those is relatively efficient.
 *
 *
 * @date Oct 3, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#if EFI_FSIO
#include "os_access.h"

#include "fsio_core.h"
#include "fsio_impl.h"

LENameOrdinalPair * LE_FIRST = nullptr;

/**
 * the main point of these static fields is that their constructor would register
 * them in the magic list of operator name/ordinal pairs
 */
static LENameOrdinalPair leAnd(LE_OPERATOR_AND, "and");
static LENameOrdinalPair leAnd2(LE_OPERATOR_AND, "&");
static LENameOrdinalPair leOr(LE_OPERATOR_OR, "or");
static LENameOrdinalPair leOr2(LE_OPERATOR_OR, "|");
static LENameOrdinalPair leNot(LE_OPERATOR_NOT, "not");
static LENameOrdinalPair leNot2(LE_OPERATOR_NOT, "!");

static LENameOrdinalPair leAdd(LE_OPERATOR_ADDITION, "+");
static LENameOrdinalPair leSub(LE_OPERATOR_SUBTRACTION, "-");
static LENameOrdinalPair leMul(LE_OPERATOR_MULTIPLICATION, "*");
static LENameOrdinalPair leDiv(LE_OPERATOR_DIVISION, "/");

static LENameOrdinalPair leMore(LE_OPERATOR_MORE, ">");
static LENameOrdinalPair leMoreOrEqual(LE_OPERATOR_MORE_OR_EQUAL, ">=");

static LENameOrdinalPair leLess(LE_OPERATOR_LESS, "<");
static LENameOrdinalPair leLessOrEquals(LE_OPERATOR_LESS_OR_EQUAL, "<=");

static LENameOrdinalPair leMax(LE_METHOD_MAX, "max");
static LENameOrdinalPair leMin(LE_METHOD_MIN, "min");
static LENameOrdinalPair leIf(LE_METHOD_IF, "if");
static LENameOrdinalPair leSelf(LE_METHOD_SELF, "self");

LENameOrdinalPair::LENameOrdinalPair(le_action_e action, const char *name) {
	this->action = action;
	this->name = name;
	this->next = LE_FIRST;
	LE_FIRST = this;
}

LEElement::LEElement() {
	clear();
}

void LEElement::clear() {
	action = LE_UNDEFINED;
	fValue = NAN;
}

void LEElement::init(le_action_e action) {
	this->action = action;
}

void LEElement::init(le_action_e action, float fValue) {
	this->action = action;
	this->fValue = fValue;
}

void LEElement::init(le_action_e action, bool bValue) {
	this->action = action;
	this->fValue = bValue ? 1 : 0;
}

LECalculator::LECalculator() {
	reset();
}

void LECalculator::reset() {
	stack.reset();
	currentCalculationLogPosition = 0;
	memset(calcLogAction, 0, sizeof(calcLogAction));
}

bool float2bool(float v) {
	return v != 0;
}

float LECalculator::pop(le_action_e action) {
	if (stack.size() == 0) {
		warning(CUSTOM_EMPTY_FSIO_STACK, "empty stack for action=%d", action);
		return NAN;
	}
	return stack.pop();
}

void LECalculator::push(le_action_e action, float value) {
	stack.push(value);
	if (currentCalculationLogPosition < MAX_CALC_LOG) {
		calcLogAction[currentCalculationLogPosition] = action;
		calcLogValue[currentCalculationLogPosition] = value;
		currentCalculationLogPosition++;
	}
}

static FsioResult doBinaryBoolean(le_action_e action, float lhs, float rhs) {
	bool v1 = float2bool(lhs);
	bool v2 = float2bool(rhs);
	
	switch (action) {
		case LE_OPERATOR_AND:
			return v1 && v2;
		case LE_OPERATOR_OR:
			return v1 || v2;
		default:
			return unexpected;
	}
}

static FsioResult doBinaryNumeric(le_action_e action, float v1, float v2) {
	// Process based on the action type
	switch (action) {
		case LE_OPERATOR_ADDITION:
			return v1 + v2;
		case LE_OPERATOR_SUBTRACTION:
			return v1 - v2;
		case LE_OPERATOR_MULTIPLICATION:
			return v1 * v2;
		case LE_OPERATOR_DIVISION:
			return v1 / v2;
		case LE_OPERATOR_LESS:
			return v1 < v2;
		case LE_OPERATOR_MORE:
			return v1 > v2;
		case LE_OPERATOR_LESS_OR_EQUAL:
			return v1 <= v2;
		case LE_OPERATOR_MORE_OR_EQUAL:
			return v1 >= v2;
		case LE_METHOD_MIN:
			return minF(v1, v2);
		case LE_METHOD_MAX:
			return maxF(v1, v2);
		default:
			return unexpected;
	}
}

/**
 * @return true in case of error, false otherwise
 */
FsioResult LECalculator::processElement(const LEElement *element DECLARE_ENGINE_PARAMETER_SUFFIX) {
#if EFI_PROD_CODE
	efiAssert(CUSTOM_ERR_ASSERT, getCurrentRemainingStack() > 64, "FSIO logic", unexpected);
#endif
	switch (element->action) {
	// Literal values
	case LE_NUMERIC_VALUE:
		return element->fValue;
	case LE_BOOLEAN_VALUE:
		return element->fValue != 0;
	// Boolean input binary operators
	case LE_OPERATOR_AND:
	case LE_OPERATOR_OR: {
		float v1 = pop(LE_OPERATOR_OR);
		float v2 = pop(LE_OPERATOR_OR);

		return doBinaryBoolean(element->action, v1, v2);
	}
	// Numeric input binary operators
	case LE_OPERATOR_ADDITION:
	case LE_OPERATOR_SUBTRACTION:
	case LE_OPERATOR_MULTIPLICATION:
	case LE_OPERATOR_DIVISION:
	case LE_OPERATOR_LESS:
	case LE_OPERATOR_MORE:
	case LE_OPERATOR_LESS_OR_EQUAL:
	case LE_OPERATOR_MORE_OR_EQUAL:
	case LE_METHOD_MIN:
	case LE_METHOD_MAX: {
		// elements on stack are in reverse order
		float v2 = pop(element->action);
		float v1 = pop(element->action);

		return doBinaryNumeric(element->action, v1, v2);
	}
	// Boolean input unary operator
	case LE_OPERATOR_NOT: {
		float v = pop(LE_OPERATOR_NOT);
		return !float2bool(v) ? 1 : 0;
	}
	case LE_METHOD_IF: {
		// elements on stack are in reverse order
		float vFalse = pop(LE_METHOD_IF);
		float vTrue = pop(LE_METHOD_IF);
		float vCond = pop(LE_METHOD_IF);
		return vCond != 0 ? vTrue : vFalse;
	}
	case LE_METHOD_FSIO_SETTING: {
		return unexpected;
	}
	case LE_METHOD_SCRIPT_TABLE: {
		return unexpected;
	}
	case LE_METHOD_FSIO_DIGITAL_INPUT:
		// todo: implement code for digital input!!!
		return unexpected;
	case LE_METHOD_FSIO_ANALOG_INPUT:
	{
		int index = clampF(0, pop(LE_METHOD_FSIO_ANALOG_INPUT), AUX_ANALOG_INPUT_COUNT - 1);
		int sensorIdx = static_cast<int>(SensorType::Aux1) + index;
		return Sensor::get(static_cast<SensorType>(sensorIdx));
	}
	case LE_UNDEFINED:
		warning(CUSTOM_UNKNOWN_FSIO, "FSIO undefined action");
		return unexpected;
	default:
		return getEngineValue(element->action);
	}
}

float LECalculator::evaluate(const char * msg, float selfValue, const LEElement* element DECLARE_ENGINE_PARAMETER_SUFFIX) {
	if (!element) {
		warning(CUSTOM_NO_FSIO, "%s no FSIO code", msg);
		return NAN;
	}

	reset();

	int counter = 0;

	// while not a return statement, execute instructions
	while (element->action != LE_METHOD_RETURN) {
		efiAssert(CUSTOM_ERR_ASSERT, counter < 200, "FSIOcount", NAN); // just in case

		if (element->action == LE_METHOD_SELF) {
			push(element->action, selfValue);
		} else {
			FsioResult result = processElement(element);

			if (!result) {
				// error already reported
				return NAN;
			}

			push(element->action, result.Value);
		}

		// Step forward to the next instruction in sequence
		element++;
		counter++;
	}

	// The stack should have exactly one element on it
	if (stack.size() != 1) {
		warning(CUSTOM_FSIO_STACK_SIZE, "%s unexpected FSIO stack size at return: %d", msg, stack.size());
		return NAN;
	}
	return stack.pop();
}

LEElementPool::LEElementPool(LEElement *pool, int size) {
	this->m_pool = pool;
	this->size = size;
	reset();
}

void LEElementPool::reset() {
	// Next free element is the first one
	m_nextFree = m_pool;
}

int LEElementPool::getSize() const {
	return m_nextFree - m_pool;
}

bool isNumeric(const char* line) {
	return line[0] >= '0' && line[0] <= '9';
}

bool isBoolean(const char* line) {
	bool isTrue = 0 == strcmp(line, "true");
	bool isFalse = 0 == strcmp(line, "false");

	return isTrue || isFalse;
}

/**
 * @return pointer at the position after the consumed token, null if no token consumed
 */
const char *getNextToken(const char *line, char *buffer, const int bufferSize) {
	while (line[0] != 0 && line[0] == ' ') {
		line++;
	}
	if (line[0] == 0) {
		return NULL;
	}
	int tokenLen = indexOf(line, ' ');
	if (tokenLen == -1) {
		// no space - the whole remaining line is the token
		tokenLen = strlen(line);
	}
	efiAssert(CUSTOM_ERR_ASSERT, tokenLen < bufferSize, "token overflow", NULL);
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

LEElement* LEElementPool::parseExpression(const char * line) {
	LEElement* expressionHead = m_nextFree;
	LEElement* n = expressionHead;

	while (true) {
		line = getNextToken(line, parsingBuffer, sizeof(parsingBuffer));

		if (!line) {
			/**
			 * No more tokens in this line, parsing complete!
			 */

			// Push a return statement on the end
			n->init(LE_METHOD_RETURN);

			// The next available element is the one after the return
			m_nextFree = n + 1;
			return expressionHead;
		}

		if (isNumeric(parsingBuffer)) {
			n->init(LE_NUMERIC_VALUE, atoff(parsingBuffer));
		} else if (isBoolean(parsingBuffer)) {
			n->init(LE_BOOLEAN_VALUE, parsingBuffer[0] == 't');
		} else {
			le_action_e action = parseAction(parsingBuffer);
			if (action == LE_UNDEFINED) {
				/**
				 * Cannot recognize token
				 */
				firmwareError(CUSTOM_ERR_PARSING_ERROR, "unrecognized FSIO keyword [%s]", parsingBuffer);

				return nullptr;
			}
			n->init(action);
		}

		n++;
	}
}

FsioValue::FsioValue(float f)
{
	u.f32 = f;
	
	// The low bit represents whether this is a bool or not, clear it for float
	u.u32 &= 0xFFFFFFFE;
}

FsioValue::FsioValue(bool b)
{
	u.u32 = (b ? 2 : 0);	// second bit is the actual value of the bool

	// Low bit indicates this is a bool
	u.u32 |= 0x1;
}

bool FsioValue::isFloat() const {
	uint32_t typeBit = u.u32 & 0x1;

	return typeBit == 0;
}

float FsioValue::asFloat() const {
	return u.f32;
}

bool FsioValue::isBool() const {
	uint32_t typeBit = u.u32 & 0x1;

	return typeBit == 1;
}

bool FsioValue::asBool() const {
	uint32_t boolBit = u.u32 & 0x2;

	return boolBit != 0;
}

#endif /* EFI_FSIO */
