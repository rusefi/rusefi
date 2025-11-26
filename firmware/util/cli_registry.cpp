/**
 * @file    cli_registry.cpp
 * @brief   Command-line interface commands registry
 *
 * Here we have a data structure which holds all the dynamically-registered
 * command line interface action names & callback. This logic is invoked in
 * user context by the console thread - see consoleThreadEntryPoint
 *
 * TODO: there is too much copy-paste here, this class needs some refactoring :)
 *
 * see testConsoleLogic()
 *
 * @date Nov 15, 2012
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include <cstring>
#include <cstdint>

// looks like some technical debt here?! that's about error: ‘isnan’ is not a member of ‘std’
#include <cmath>
#include <rusefi/rusefi_math.h>

#include "efiprintf.h"
#include "rusefi/efistringutil.h"
#include "cli_registry.h"

/* for isspace() */
#include <cctype>

#ifndef CONSOLE_MAX_ACTIONS
#define CONSOLE_MAX_ACTIONS 256
#endif

#ifndef MAX_CMD_LINE_LENGTH
#define MAX_CMD_LINE_LENGTH		100
#endif

// todo: support \t as well
#define SPACE_CHAR ' '

using namespace rusefi::stringutil;

static int consoleActionCount = 0;
static TokenCallback consoleActions[CONSOLE_MAX_ACTIONS];

void resetConsoleActions(void) {
	consoleActionCount = 0;
}

static void doAddAction(const char *token, action_type_e type, Void callback, void *param) {
#if !defined(EFI_DISABLE_CONSOLE_ACTIONS)
	for (uint32_t i = 0; i < std::strlen(token); i++) {
		char ch = token[i];
		if (isupper(ch)) {
		    onCliCaseError(token);
		    return;
		}
	}
	for (int i = 0; i < consoleActionCount; i++) {
		if (strcmp(token, consoleActions[i].token) == 0 /* zero result means strings are equal */) {
			onCliDuplicateError(token);
		    return;
		}
	}

    if (consoleActionCount >= CONSOLE_MAX_ACTIONS) {
		onCliOverflowError();
		return;
    }

	TokenCallback *current = &consoleActions[consoleActionCount++];
	current->token = token;
	current->parameterType = type;
	current->callback = callback;
	current->param = param;
#endif /* EFI_DISABLE_CONSOLE_ACTIONS */
}

void addConsoleActionP(const char *token, VoidPtr callback, void *param) {
	doAddAction(token, NO_PARAMETER_P, (Void) callback, param);
}

void addConsoleActionSSP(const char *token, VoidCharPtrCharPtrVoidPtr callback, void *param) {
	doAddAction(token, STRING2_PARAMETER_P, (Void) callback, param);
}

/**
 * @brief	Register console action without parameters
 */
void addConsoleAction(const char *token, Void callback) {
	doAddAction(token, NO_PARAMETER, callback, NULL);
}

/**
 * @brief	Register a console command with one Integer parameter
 */
void addConsoleActionI(const char *token, VoidInt callback) {
	doAddAction(token, ONE_PARAMETER, (Void) callback, NULL);
}

void addConsoleActionIP(const char *token, VoidIntVoidPtr callback, void *param) {
	doAddAction(token, ONE_PARAMETER_P, (Void) callback, param);
}

/**
 * @brief	Register a console command with two Integer parameters
 */
void addConsoleActionII(const char *token, VoidIntInt callback) {
	doAddAction(token, TWO_INTS_PARAMETER, (Void) callback, NULL);
}

void addConsoleActionIIP(const char *token, VoidIntIntVoidPtr callback, void *param) {
	doAddAction(token, TWO_INTS_PARAMETER_P, (Void) callback, param);
}

void addConsoleActionIF(const char *token, VoidIntFloat callback) {
	doAddAction(token, INT_FLOAT_PARAMETER, (Void) callback, NULL);
}

void addConsoleActionS(const char *token, VoidCharPtr callback) {
	doAddAction(token, STRING_PARAMETER, (Void) callback, NULL);
}

void addConsoleActionSP(const char *token, VoidCharPtrVoidPtr callback, void *param) {
	doAddAction(token, STRING_PARAMETER_P, (Void) callback, param);
}

void addConsoleActionSS(const char *token, VoidCharPtrCharPtr callback) {
	doAddAction(token, STRING2_PARAMETER, (Void) callback, NULL);
}

void addConsoleActionSSS(const char *token, VoidCharPtrCharPtrCharPtr callback) {
	doAddAction(token, STRING3_PARAMETER, (Void) callback, NULL);
}

void addConsoleActionSSSSS(const char *token, VoidCharPtrCharPtrCharPtrCharPtrCharPtr callback) {
	doAddAction(token, STRING5_PARAMETER, (Void) callback, NULL);
}

void addConsoleActionNANF(const char *token, VoidFloat callback) {
	doAddAction(token, FLOAT_PARAMETER_NAN_ALLOWED, (Void) callback, NULL);
}

void addConsoleActionF(const char *token, VoidFloat callback) {
	doAddAction(token, FLOAT_PARAMETER, (Void) callback, NULL);
}

void addConsoleActionFF(const char *token, VoidFloatFloat callback) {
	doAddAction(token, FLOAT_FLOAT_PARAMETER, (Void) callback, NULL);
}

void addConsoleActionFFF(const char *token, VoidFloatFloatFloat callback) {
	doAddAction(token, FLOAT_FLOAT_FLOAT_PARAMETER, (Void) callback, NULL);
}

void addConsoleActionFFFF(const char *token, VoidFloatFloatFloatFloat callback) {
	doAddAction(token, FLOAT_FLOAT_FLOAT_FLOAT_PARAMETER, (Void) callback, NULL);
}

void addConsoleActionFFP(const char *token, VoidFloatFloatVoidPtr callback, void *param) {
	doAddAction(token, FLOAT_FLOAT_PARAMETER_P, (Void) callback, param);
}

static int getParameterCount(action_type_e parameterType) {
	switch (parameterType) {
	case NO_PARAMETER:
	case NO_PARAMETER_P:
		return 0;
	case ONE_PARAMETER:
	case ONE_PARAMETER_P:
	case FLOAT_PARAMETER:
	case STRING_PARAMETER:
		return 1;
	case FLOAT_FLOAT_PARAMETER:
	case FLOAT_FLOAT_PARAMETER_P:
	case STRING2_PARAMETER:
	case STRING2_PARAMETER_P:
	case TWO_INTS_PARAMETER:
	case TWO_INTS_PARAMETER_P:
	case INT_FLOAT_PARAMETER:
		return 2;
	case STRING3_PARAMETER:
	case FLOAT_FLOAT_FLOAT_PARAMETER:
		return 3;
	case FLOAT_FLOAT_FLOAT_FLOAT_PARAMETER:
		return 4;
	case STRING5_PARAMETER:
		return 5;
	default:
		return -1;
	}
}

/**
 * @brief This function prints out a list of all available commands
 */
void helpCommand(void) {
	efiPrintf("%d actions available", consoleActionCount);
	for (int i = 0; i < consoleActionCount; i++) {
		TokenCallback *current = &consoleActions[i];
		efiPrintf("  %s: %d parameters", current->token, getParameterCount(current->parameterType));
	}
	efiPrintf("For more visit https://wiki.rusefi.com/Dev-Console-Commands");
}

int findEndOfToken(const char *line) {
	if (line[0] == '"') {
		/**
		 * Looks like this is a quoted token
		 */
		int v = indexOf(line + 1, '"');
		if (v == -1) {
			/**
			 * Matching closing quote not found
			 */
			return -1;
		}
		/**
		 * Skipping first quote and the symbol after closing quote
		 */
		return v + 2;
	}
	return indexOf(line, SPACE_CHAR);
}

/**
 * @return Number of space-separated tokens in the string
 */
int tokenLength(const char *msgp) {
	int result = 0;
	while (*msgp) {
		char ch = *msgp++;
		if (ch == SPACE_CHAR) {
			break;
		}
		result++;
	}
	return result;
}

char *unquote(char *line) {
	if (line[0] == '"') {
		int len = std::strlen(line);
		if (line[len - 1] == '"') {
			line[len - 1] = 0;
			return line + 1;
		}
	}
	return line;
}

static int setargs(char *args, char **argv, int max_args)
{
	int count = 0;

	while (isspace(*args)) {
		*args = '\0';
		args++;
	}
	while ((*args) && (count < max_args)) {
		if (argv) {
			argv[count] = args;
		}
		while ((*args) && (!isspace(*args))) {
			if (*args == '"') {
				args++;
				/* find closing quote */
				while ((*args) && (*args != '"')) {
					args++;
				}
				/* failed? */
				if (*args == '\0') {
					return -1;
				}
			}
			args++;
		}
		if (*args) {
			*args = '\0';
			args++;
		}
		while (isspace(*args)) {
			*args = '\0';
			args++;
		}
		count++;
	}
	return count;
}

int handleActionWithParameter(TokenCallback *current, char *argv[], int argc) {
	(void)argc;

	switch (current->parameterType) {
	case NO_PARAMETER:
	{
		(*current->callback)();
		return 0;
	}
	case NO_PARAMETER_P:
	{
		VoidPtr callbackS = (VoidPtr) current->callback;
		(*callbackS)(current->param);
		return 0;
	}
	case STRING_PARAMETER:
	{
		VoidCharPtr callbackS = (VoidCharPtr) current->callback;
		(*callbackS)(argv[0]);
		return 0;
	}
	case STRING_PARAMETER_P:
	{
		VoidCharPtrVoidPtr callbackS = (VoidCharPtrVoidPtr) current->callback;
		(*callbackS)(argv[0], current->param);
		return 0;
	}
	case STRING2_PARAMETER:
	{
		VoidCharPtrCharPtr callbackS = (VoidCharPtrCharPtr) current->callback;
		(*callbackS)(argv[0], argv[1]);
		return 0;
	}
	case STRING2_PARAMETER_P:
	{
		VoidCharPtrCharPtrVoidPtr callbackS = (VoidCharPtrCharPtrVoidPtr) current->callback;
		(*callbackS)(argv[0], argv[1], current->param);
		return 0;
	}
	case STRING3_PARAMETER:
	{
		VoidCharPtrCharPtrCharPtr callbackS = (VoidCharPtrCharPtrCharPtr) current->callback;
		(*callbackS)(argv[0], argv[1], argv[2]);
		return 0;
	}
	case STRING5_PARAMETER:
	{
		VoidCharPtrCharPtrCharPtrCharPtrCharPtr callbackS = (VoidCharPtrCharPtrCharPtrCharPtrCharPtr) current->callback;
		(*callbackS)(argv[0], argv[1], argv[2], argv[3], argv[4]);
		return 0;
	}
	case TWO_INTS_PARAMETER:
	{
		int value[2];
		for (int i = 0; i < 2; i++) {
			value[i] = atoi(argv[i]);
			if (absI(value[i]) == ATOI_ERROR_CODE) {
				#if EFI_PROD_CODE || EFI_SIMULATOR
					efiPrintf("not an integer [%s]", argv[0]);
				#endif
				return -1;
			}
		}
		VoidIntInt callbackS = (VoidIntInt) current->callback;
		(*callbackS)(value[0], value[1]);
		return 0;
	}
	case FLOAT_PARAMETER_NAN_ALLOWED:
	{
		float value = atoff(argv[0]);
		VoidFloat callbackF = (VoidFloat) current->callback;
		(*callbackF)(value);
		return 0;
	}

	case FLOAT_PARAMETER:
	{
		float value = atoff(argv[0]);
		if (std::isnan(value)) {
			efiPrintf("invalid float [%s]", argv[0]);
			return -1;
		}
		VoidFloat callbackF = (VoidFloat) current->callback;
		(*callbackF)(value);
		return 0;
	}

	case FLOAT_FLOAT_PARAMETER:
	case FLOAT_FLOAT_PARAMETER_P:
	{
		float value[2];
		for (int i = 0; i < 2; i++) {
			value[i] = atoff(argv[i]);
			if (std::isnan(value[i])) {
				efiPrintf("invalid float [%s]", argv[i]);
				return -1;
			}
		}
		if (current->parameterType == FLOAT_FLOAT_PARAMETER) {
			VoidFloatFloat callbackS = (VoidFloatFloat) current->callback;
			(*callbackS)(value[0], value[1]);
		} else {
			VoidFloatFloatVoidPtr callbackS = (VoidFloatFloatVoidPtr) current->callback;
			(*callbackS)(value[0], value[1], current->param);
		}
		return 0;
	}
	case FLOAT_FLOAT_FLOAT_PARAMETER:
	{
		float value[3];
		for (int i = 0; i < 3; i++) {
			value[i] = atoff(argv[i]);
			if (std::isnan(value[i])) {
				efiPrintf("invalid float [%s]", argv[i]);
				return -1;
			}
		}
		VoidFloatFloatFloat callbackS = (VoidFloatFloatFloat) current->callback;
		(*callbackS)(value[0], value[1], value[2]);
		return 0;
	}
	case FLOAT_FLOAT_FLOAT_FLOAT_PARAMETER:
	{
		float value[4];
		for (int i = 0; i < 4; i++) {
			value[i] = atoff(argv[i]);
			if (std::isnan(value[i])) {
				efiPrintf("invalid float [%s]", argv[i]);
				return -1;
			}
		}
		VoidFloatFloatFloatFloat callbackS = (VoidFloatFloatFloatFloat) current->callback;
		(*callbackS)(value[0], value[1], value[2], value[3]);
		return 0;
	}
	case INT_FLOAT_PARAMETER:
	{
		int value1 = atoi(argv[0]);
		if (absI(value1) == ATOI_ERROR_CODE) {
			#if EFI_PROD_CODE || EFI_SIMULATOR
				efiPrintf("not an integer [%s]", argv[0]);
			#endif
			return -1;
		}
		float value2 = atoff(argv[1]);
		if (std::isnan(value2)) {
			efiPrintf("invalid float [%s]", argv[1]);
			return -1;
		}
		VoidIntFloat callback = (VoidIntFloat) current->callback;
		callback(value1, value2);
		return 0;
	}
	case ONE_PARAMETER_P:
	case ONE_PARAMETER:
	{
		int value = atoi(argv[0]);
		if (absI(value) == ATOI_ERROR_CODE) {
			#if EFI_PROD_CODE || EFI_SIMULATOR
				efiPrintf("not an integer [%s]", argv[0]);
			#endif
			return -1;
		}
		if (current->parameterType == ONE_PARAMETER_P) {
			VoidIntVoidPtr callback1 = (VoidIntVoidPtr) current->callback;
			(*callback1)(value, current->param);

		} else {
			VoidInt callback1 = (VoidInt) current->callback;
			(*callback1)(value);
		}
		return 0;
	}
	default:
		efiPrintf("Unsupported parameterType %d", current->parameterType);
		break;
	}
	return -1;
}

void initConsoleLogic() {
	addConsoleAction("help", helpCommand);
}

static char handleBuffer[MAX_CMD_LINE_LENGTH + 1];

static int handleConsoleLineInternal(const char *commandLine) {
	strncpy(handleBuffer, commandLine, sizeof(handleBuffer) - 1);

	char *argv[10];
	int argc = setargs(handleBuffer, argv, 10);

	if (argc <= 0) {
		efiPrintf("invalid input");
		return -1;
	}

	for (int i = 0; i < consoleActionCount; i++) {
		TokenCallback *current = &consoleActions[i];
		if (strEqual(argv[0], current->token)) {
			if ((argc - 1) != getParameterCount(current->parameterType)) {
				efiPrintf("Incorrect argument count %d, expected %d",
					(argc - 1), getParameterCount(current->parameterType));
				return -1;
			}

			/* skip commant name */
			return handleActionWithParameter(current, argv + 1, argc - 1);
		}
	}

	efiPrintf("unknown command [%s]", commandLine);
	return -1;
}

/**
 * @brief This function takes care of one command line once we have it
 */
void handleConsoleLine(char *line) {
	if (line == NULL)
		return; // error detected

	int lineLength = std::strlen(line);
	if (lineLength > MAX_CMD_LINE_LENGTH) {
		// todo: better reaction to excessive line
		efiPrintf("Long line?");
		return;
	}

	int ret = handleConsoleLineInternal(line);

	if (ret < 0) {
		efiPrintf("failed to handle command [%s]", line);
		return;
	}

	efiPrintf("confirmation_%s:%d", line, lineLength);
}
