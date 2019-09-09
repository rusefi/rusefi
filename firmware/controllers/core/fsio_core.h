/**
 * @file fsio_core.h
 *
 * @date Oct 3, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef LOGIC_EXPRESSION_H_
#define LOGIC_EXPRESSION_H_

#include "rusefi_enums.h"
#include "fl_stack.h"
#include "engine.h"

#define MAX_TABLE_INDEX 4

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
	LE_OPERATOR_ADDITION = 9,
	LE_OPERATOR_SUBTRACTION = 10,
	LE_OPERATOR_MULTIPLICATION = 11,
	LE_OPERATOR_DIVISION = 12,
	LE_METHOD_MAX = 13,
	LE_METHOD_MIN = 14,
	LE_METHOD_IF = 15,

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
	LE_METHOD_AC_TOGGLE = 110,
	LE_METHOD_TIME_SINCE_AC_TOGGLE = 111,
	LE_METHOD_KNOCK = 112,
	LE_METHOD_FSIO_TABLE = 113,
	LE_METHOD_SELF = 114,
	LE_METHOD_MAP = 115,
	LE_METHOD_FSIO_ANALOG_INPUT = 116,
	LE_METHOD_INTAKE_VVT = 117,
	LE_METHOD_EXHAUST_VVT = 118,
	LE_METHOD_IS_COOLANT_BROKEN = 119,
	LE_METHOD_CRANKING_RPM = 120,
	LE_METHOD_STARTUP_FUEL_PUMP_DURATION = 121,
	LE_METHOD_IN_SHUTDOWN = 122,
	LE_METHOD_FSIO_DIGITAL_INPUT = 123,
	LE_METHOD_FSIO_SETTING = 124,

	Force_4b_le_action = ENUM_32_BITS,

} le_action_e;

class LEElement {
public:
	LEElement();
	void clear();
//	void init(le_action_e action, int iValue);
	void init(le_action_e action);
	void init(le_action_e action, float fValue);

	le_action_e action;
	float fValue;
	int iValue;

	LEElement *next;
};

class LEElementPool {
public:
	LEElementPool(LEElement *pool, int size);
	LEElement *pool;
	LEElement *next();
	void reset();
	LEElement * parseExpression(const char * line);
	int getSize() const;
private:
	int index;
	int size;
};


#define MAX_STACK_DEPTH 32

typedef FLStack<float, MAX_STACK_DEPTH> calc_stack_t;

#define MAX_CALC_LOG 64

class LECalculator {
public:
	LECalculator();
	float getValue(float selfValue DECLARE_ENGINE_PARAMETER_SUFFIX);
	float getValue2(float selfValue, LEElement *fistElementInList DECLARE_ENGINE_PARAMETER_SUFFIX);
	void add(LEElement *element);
	bool isEmpty() const;
	void reset();
	void reset(LEElement *element);
	le_action_e calcLogAction[MAX_CALC_LOG];
	float calcLogValue[MAX_CALC_LOG];
	int currentCalculationLogPosition;
private:
	void push(le_action_e action, float value);
	bool processElement(LEElement *element DECLARE_ENGINE_PARAMETER_SUFFIX);
	float pop(le_action_e action);
	LEElement *first;
	calc_stack_t stack;
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
const char *getNextToken(const char *line, char *buffer, const int bufferSize);
bool isNumeric(const char* line);
le_action_e parseAction(const char * line);
bool float2bool(float v);

#endif /* LOGIC_EXPRESSION_H_ */
