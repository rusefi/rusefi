/**
 * @file    cli_registry.h
 * @brief   Command-line interface commands registry
 *
 * @date Nov 15, 2012
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

typedef enum {
	NO_PARAMETER,
	ONE_PARAMETER,
	FLOAT_PARAMETER_NAN_ALLOWED,
	FLOAT_PARAMETER,
	STRING_PARAMETER,
	STRING2_PARAMETER,
	STRING3_PARAMETER,
	STRING5_PARAMETER,
	TWO_INTS_PARAMETER,
	FLOAT_FLOAT_PARAMETER,
	FLOAT_FLOAT_FLOAT_PARAMETER,
	FLOAT_FLOAT_FLOAT_FLOAT_PARAMETER,
	INT_FLOAT_PARAMETER,
} action_type_e;

typedef struct {
	const char *token;
	action_type_e parameterType;
	void *callback;
} TokenCallback;


int tokenLength(const char *msgp);
int findEndOfToken(const char *line);
char *unquote(char *line);


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

typedef void (*Void)(void);
typedef void (*VoidPtr)(void*);
typedef void (*VoidInt)(int);
typedef void (*VoidFloat)(float);
typedef void (*VoidFloatFloat)(float, float);
typedef void (*VoidFloatFloatFloat)(float, float, float);
typedef void (*VoidFloatFloatFloatFloat)(float, float, float, float);
typedef void (*VoidIntInt)(int, int);
typedef void (*VoidIntFloat)(int, float);

typedef void (*VoidCharPtr)(const char *);
typedef void (*VoidCharPtrCharPtr)(const char *, const char *);
typedef void (*VoidCharPtrCharPtrCharPtr)(const char *, const char *, const char *);
typedef void (*VoidCharPtrCharPtrCharPtrCharPtrCharPtr)(const char *, const char *, const char *, const char *, const char *);

void resetConsoleActions(void);
void helpCommand(void);
void initConsoleLogic();
void handleConsoleLine(char *line);

void doAddAction(const TokenCallback *t);

#if !defined(EFI_DISABLE_CONSOLE_ACTIONS)
#define addTypedConsoleAction(token, type, callback) \
	do { \
		static const TokenCallback t = { token, type, (void*)+callback }; \
		doAddAction(&t); \
	} while(0)
#else
#define addTypedConsoleAction(token, type, callback, param)
#endif

#define ASSERT_TYPE(TYPE, VALUE) \
	static_assert(std::is_same_v<decltype(+VALUE), TYPE>, "Incorrect action callback")

#define addConsoleAction(token, callback) \
	ASSERT_TYPE(Void, callback); \
	addTypedConsoleAction(token, NO_PARAMETER, callback)
#define addConsoleActionI(token, callback) \
	ASSERT_TYPE(VoidInt, callback); \
	addTypedConsoleAction(token, ONE_PARAMETER, callback)
#define addConsoleActionII(token, callback) \
	ASSERT_TYPE(VoidIntInt, callback); \
	addTypedConsoleAction(token, TWO_INTS_PARAMETER, callback)
#define addConsoleActionIF(token, callback) \
	ASSERT_TYPE(VoidIntFloat, callback); \
	addTypedConsoleAction(token, INT_FLOAT_PARAMETER, callback)
#define addConsoleActionF(token, callback) \
	ASSERT_TYPE(VoidFloat, callback); \
	addTypedConsoleAction(token, FLOAT_PARAMETER, callback)
#define addConsoleActionNANF(token, callback) \
	ASSERT_TYPE(VoidFloat, callback); \
	addTypedConsoleAction(token, FLOAT_PARAMETER_NAN_ALLOWED, callback)
#define addConsoleActionFF(token, callback) \
	ASSERT_TYPE(VoidFloatFloat, callback); \
	addTypedConsoleAction(token, FLOAT_FLOAT_PARAMETER, callback)
#define addConsoleActionFFF(token, callback) \
	ASSERT_TYPE(VoidFloatFloatFloat, callback); \
	addTypedConsoleAction(token, FLOAT_FLOAT_FLOAT_PARAMETER, callback)
#define addConsoleActionFFFF(token, callback) \
	ASSERT_TYPE(VoidFloatFloatFloatFloat, callback); \
	addTypedConsoleAction(token, FLOAT_FLOAT_FLOAT_FLOAT_PARAMETER, callback)
#define addConsoleActionS(token, callback) \
	ASSERT_TYPE(VoidCharPtr, callback); \
	addTypedConsoleAction(token, STRING_PARAMETER, callback)
#define addConsoleActionSS(token, callback) \
	ASSERT_TYPE(VoidCharPtrCharPtr, callback); \
	addTypedConsoleAction(token, STRING2_PARAMETER, callback)
#define addConsoleActionSSS(token, callback) \
	ASSERT_TYPE(VoidCharPtrCharPtrCharPtr, callback); \
	addTypedConsoleAction(token, STRING3_PARAMETER, callback)
#define addConsoleActionSSSSS(token, callback) \
	ASSERT_TYPE(VoidCharPtrCharPtrCharPtrCharPtrCharPtr, callback); \
	addTypedConsoleAction(token, STRING5_PARAMETER, callback)

void onCliCaseError(const char *token);
void onCliDuplicateError(const char *token);
void onCliOverflowError();

#ifdef __cplusplus
}
#endif /* __cplusplus */
