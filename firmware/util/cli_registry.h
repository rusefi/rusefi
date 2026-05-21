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
	NO_PARAMETER_P,
	ONE_PARAMETER,
	ONE_PARAMETER_P,
	FLOAT_PARAMETER_NAN_ALLOWED,
	FLOAT_PARAMETER,
	FLOAT_PARAMETER_P,
	STRING_PARAMETER,
	STRING_PARAMETER_P,
	STRING2_PARAMETER,
	STRING2_PARAMETER_P,
	STRING3_PARAMETER,
	STRING5_PARAMETER,
	TWO_INTS_PARAMETER,
	TWO_INTS_PARAMETER_P,
	FLOAT_FLOAT_PARAMETER,
	FLOAT_FLOAT_FLOAT_PARAMETER,
	FLOAT_FLOAT_FLOAT_FLOAT_PARAMETER,
	FLOAT_FLOAT_PARAMETER_P,
	INT_FLOAT_PARAMETER,
} action_type_e;

typedef struct {
	const char *token;
	action_type_e parameterType;
	void *callback;
	void *param;
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
typedef void (*VoidIntVoidPtr)(int, void*);
typedef void (*VoidFloat)(float);
typedef void (*VoidFloatFloat)(float, float);
typedef void (*VoidFloatFloatFloat)(float, float, float);
typedef void (*VoidFloatFloatFloatFloat)(float, float, float, float);
typedef void (*VoidFloatFloatVoidPtr)(float, float, void*);
typedef void (*VoidIntInt)(int, int);
typedef void (*VoidIntIntVoidPtr)(int, int, void*);
typedef void (*VoidIntFloat)(int, float);

typedef void (*VoidCharPtr)(const char *);
typedef void (*VoidCharPtrVoidPtr)(const char *, void*);

typedef void (*VoidCharPtrCharPtr)(const char *, const char *);
typedef void (*VoidCharPtrCharPtrVoidPtr)(const char *, const char *, void*);

typedef void (*VoidCharPtrCharPtrCharPtr)(const char *, const char *, const char *);
typedef void (*VoidCharPtrCharPtrCharPtrCharPtrCharPtr)(const char *, const char *, const char *, const char *, const char *);

void resetConsoleActions(void);
void helpCommand(void);
void initConsoleLogic();
void handleConsoleLine(char *line);

void doAddAction(const TokenCallback *t);

#if !defined(EFI_DISABLE_CONSOLE_ACTIONS)
#define addTypedConsoleAction(token, type, callback, param) \
	do { \
		static const TokenCallback t = { token, type, (void*)+callback, param }; \
		doAddAction(&t); \
	} while(0)
#else
#define addTypedConsoleAction(token, type, callback, param)
#endif

#define ASSERT_TYPE(TYPE, VALUE) \
	static_assert(std::is_same_v<decltype(+VALUE), TYPE>, "Incorrect action callback")

#define addConsoleAction(token, callback) \
	ASSERT_TYPE(Void, callback); \
	addTypedConsoleAction(token, NO_PARAMETER, callback, nullptr)
#define addConsoleActionI(token, callback) \
	ASSERT_TYPE(VoidInt, callback); \
	addTypedConsoleAction(token, ONE_PARAMETER, callback, nullptr)
#define addConsoleActionII(token, callback) \
	ASSERT_TYPE(VoidIntInt, callback); \
	addTypedConsoleAction(token, TWO_INTS_PARAMETER, callback, nullptr)
#define addConsoleActionIF(token, callback) \
	ASSERT_TYPE(VoidIntFloat, callback); \
	addTypedConsoleAction(token, INT_FLOAT_PARAMETER, callback, nullptr)
#define addConsoleActionF(token, callback) \
	ASSERT_TYPE(VoidFloat, callback); \
	addTypedConsoleAction(token, FLOAT_PARAMETER, callback, nullptr)
#define addConsoleActionNANF(token, callback) \
	ASSERT_TYPE(VoidFloat, callback); \
	addTypedConsoleAction(token, FLOAT_PARAMETER_NAN_ALLOWED, callback, nullptr)
#define addConsoleActionFF(token, callback) \
	ASSERT_TYPE(VoidFloatFloat, callback); \
	addTypedConsoleAction(token, FLOAT_FLOAT_PARAMETER, callback, nullptr)
#define addConsoleActionFFF(token, callback) \
	ASSERT_TYPE(VoidFloatFloatFloat, callback); \
	addTypedConsoleAction(token, FLOAT_FLOAT_FLOAT_PARAMETER, callback, nullptr)
#define addConsoleActionFFFF(token, callback) \
	ASSERT_TYPE(VoidFloatFloatFloatFloat, callback); \
	addTypedConsoleAction(token, FLOAT_FLOAT_FLOAT_FLOAT_PARAMETER, callback, nullptr)
#define addConsoleActionS(token, callback) \
	ASSERT_TYPE(VoidCharPtr, callback); \
	addTypedConsoleAction(token, STRING_PARAMETER, callback, nullptr)
#define addConsoleActionSS(token, callback) \
	ASSERT_TYPE(VoidCharPtrCharPtr, callback); \
	addTypedConsoleAction(token, STRING2_PARAMETER, callback, nullptr)
#define addConsoleActionSSS(token, callback) \
	ASSERT_TYPE(VoidCharPtrCharPtrCharPtr, callback); \
	addTypedConsoleAction(token, STRING3_PARAMETER, callback, nullptr)
#define addConsoleActionSSSSS(token, callback) \
	ASSERT_TYPE(VoidCharPtrCharPtrCharPtrCharPtrCharPtr, callback); \
	addTypedConsoleAction(token, STRING5_PARAMETER, callback, nullptr)

#define addConsoleActionP(token, callback, param) \
	ASSERT_TYPE(VoidPtr, callback); \
	addTypedConsoleAction(token, NO_PARAMETER_P, callback, param)
#define addConsoleActionIP(token, callback, param) \
	ASSERT_TYPE(VoidIntVoidPtr, callback); \
	addTypedConsoleAction(token, ONE_PARAMETER_P, callback, param)
#define addConsoleActionIIP(token, callback, param) \
	ASSERT_TYPE(VoidIntIntVoidPtr, callback); \
	addTypedConsoleAction(token, TWO_INTS_PARAMETER_P, callback, param)
#define addConsoleActionFFP(token, callback, param) \
	ASSERT_TYPE(VoidFloatFloatVoidPtr, callback); \
	addTypedConsoleAction(token, FLOAT_FLOAT_PARAMETER_P, callback, param)
#define addConsoleActionSP(token, callback, param) \
	ASSERT_TYPE(VoidCharPtrVoidPtr, callback); \
	addTypedConsoleAction(token, STRING_PARAMETER_P, callback, param)
#define addConsoleActionSSP(token, callback, param) \
	ASSERT_TYPE(VoidCharPtrCharPtrVoidPtr, callback); \
	addTypedConsoleAction(token, STRING2_PARAMETER_P, callback, param)

void onCliCaseError(const char *token);
void onCliDuplicateError(const char *token);
void onCliOverflowError();

#ifdef __cplusplus
}
#endif /* __cplusplus */
