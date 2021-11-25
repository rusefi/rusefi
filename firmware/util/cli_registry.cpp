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

#include "pch.h"
#include "cli_registry.h"

#if ! EFI_UNIT_TEST
#include "eficonsole.h"
#endif /* ! EFI_UNIT_TEST */

// todo: support \t as well
#define SPACE_CHAR ' '

static int consoleActionCount = 0;
static TokenCallback consoleActions[CONSOLE_MAX_ACTIONS] CCM_OPTIONAL;

#define SECURE_LINE_PREFIX "sec!"
#define SECURE_LINE_PREFIX_LENGTH 4

void resetConsoleActions(void) {
	consoleActionCount = 0;
}

static void doAddAction(const char *token, action_type_e type, Void callback, void *param) {
#if !defined(EFI_DISABLE_CONSOLE_ACTIONS)
	for (uint32_t i = 0; i < efiStrlen(token);i++) {
		char ch = token[i];
		if (ch != mytolower(ch)) {
			firmwareError(CUSTOM_ERR_COMMAND_LOWER_CASE_EXPECTED, "lowerCase expected [%s]", token);
		}
	}
	for (int i = 0; i < consoleActionCount; i++) {
		if (strcmp(token, consoleActions[i].token) == 0 /* zero result means strings are equal */) {
			firmwareError(CUSTOM_SAME_TWICE, "Same action twice [%s]", token);
		}
	}

	efiAssertVoid(CUSTOM_CONSOLE_TOO_MANY, consoleActionCount < CONSOLE_MAX_ACTIONS, "Too many console actions");
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
		return 3;
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

#if EFI_PROD_CODE || EFI_SIMULATOR
	efiPrintf("%d actions available", consoleActionCount);
	for (int i = 0; i < consoleActionCount; i++) {
		TokenCallback *current = &consoleActions[i];
		efiPrintf("  %s: %d parameters", current->token, getParameterCount(current->parameterType));
	}
#endif
	efiPrintf("For more visit http://rusefi.com/wiki/index.php?title=Manual:Software:dev_console_commands");
}

/**
 * @brief This is just a test function
 */
static void echo(int value) {
	efiPrintf("got value: %d", value);
}

char *unquote(char *line) {
	if (line[0] == '"') {
		int len = strlen(line);
		if (line[len - 1] == '"') {
			line[len - 1] = 0;
			return line + 1;
		}
	}
	return line;
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

#define REPLACE_SPACES_WITH_ZERO { \
	    while (parameter[spaceIndex + 1] == SPACE_CHAR) { \
			parameter[spaceIndex++] = 0; \
		} \
		parameter[spaceIndex] = 0; \
}

void handleActionWithParameter(TokenCallback *current, char *parameter) {
    while (parameter[0] == SPACE_CHAR) {
		parameter[0] = 0;
		parameter++;
	}

    switch (current->parameterType) {
	case STRING_PARAMETER:
	{
		VoidCharPtr callbackS = (VoidCharPtr) current->callback;
		(*callbackS)(parameter);
		return;
	}
	case STRING_PARAMETER_P:
	{
			VoidCharPtrVoidPtr callbackS = (VoidCharPtrVoidPtr) current->callback;
			(*callbackS)(parameter, current->param);
			return;
	}
	default:
		// todo: handle all cases explicitly
		break;
	}



	// todo: refactor this hell!
	if (current->parameterType == STRING2_PARAMETER || current->parameterType == STRING2_PARAMETER_P) {
		int spaceIndex = findEndOfToken(parameter);
		if (spaceIndex == -1) {
			return;
		}
		REPLACE_SPACES_WITH_ZERO;
		char * param0 = parameter;

		parameter += spaceIndex + 1;
		char * param1 = parameter;

		if (current->parameterType == STRING2_PARAMETER) {
			VoidCharPtrCharPtr callbackS = (VoidCharPtrCharPtr) current->callback;
			(*callbackS)(param0, param1);
		} else {
			VoidCharPtrCharPtrVoidPtr callbackS = (VoidCharPtrCharPtrVoidPtr) current->callback;
			(*callbackS)(param0, param1, current->param);
		}
		return;
	}

	if (current->parameterType == STRING3_PARAMETER) {
		int spaceIndex = findEndOfToken(parameter);
		if (spaceIndex == -1) {
			return;
		}
		REPLACE_SPACES_WITH_ZERO;
		char * param0 = parameter;

		parameter += spaceIndex + 1;
		spaceIndex = findEndOfToken(parameter);
		if (spaceIndex == -1)
			return;
		REPLACE_SPACES_WITH_ZERO;
		char * param1 = parameter;
		parameter += spaceIndex + 1;
		char * param2 = parameter;

		VoidCharPtrCharPtrCharPtr callbackS = (VoidCharPtrCharPtrCharPtr) current->callback;
		(*callbackS)(param0, param1, param2);
		return;

	}

	// todo: refactor this hell!
	if (current->parameterType == STRING5_PARAMETER) {
		int spaceIndex = findEndOfToken(parameter);
		if (spaceIndex == -1) {
			return;
		}
		REPLACE_SPACES_WITH_ZERO;
		char * param0 = parameter;

		parameter += spaceIndex + 1;
		spaceIndex = findEndOfToken(parameter);
		if (spaceIndex == -1)
			return;
		REPLACE_SPACES_WITH_ZERO;
		char * param1 = parameter;

		parameter += spaceIndex + 1;
		spaceIndex = findEndOfToken(parameter);
		if (spaceIndex == -1)
			return;
		REPLACE_SPACES_WITH_ZERO;
		char * param2 = parameter;

		parameter += spaceIndex + 1;
		spaceIndex = findEndOfToken(parameter);
		if (spaceIndex == -1)
			return;
		REPLACE_SPACES_WITH_ZERO;
		char * param3 = parameter;

		parameter += spaceIndex + 1;
		char * param4 = parameter;

		VoidCharPtrCharPtrCharPtrCharPtrCharPtr callbackS = (VoidCharPtrCharPtrCharPtrCharPtrCharPtr) current->callback;
		(*callbackS)(param0, param1, param2, param3, param4);
		return;

	}

	if (current->parameterType == TWO_INTS_PARAMETER) {
		int spaceIndex = findEndOfToken(parameter);
		if (spaceIndex == -1)
			return;
		REPLACE_SPACES_WITH_ZERO;
		int value1 = atoi(parameter);
		if (absI(value1) == ERROR_CODE) {
#if EFI_PROD_CODE || EFI_SIMULATOR
			efiPrintf("not an integer [%s]", parameter);
#endif
			return;
		}
		parameter += spaceIndex + 1;
		int value2 = atoi(parameter);
		if (absI(value2) == ERROR_CODE) {
#if EFI_PROD_CODE || EFI_SIMULATOR
			efiPrintf("not an integer [%s]", parameter);
#endif
			return;
		}
		VoidIntInt callbackS = (VoidIntInt) current->callback;
		(*callbackS)(value1, value2);
		return;
	}

	if (current->parameterType == FLOAT_PARAMETER_NAN_ALLOWED) {
		float value = atoff(parameter);
		VoidFloat callbackF = (VoidFloat) current->callback;

		// invoke callback function by reference
		(*callbackF)(value);
		return;
	}

	if (current->parameterType == FLOAT_PARAMETER) {
		float value = atoff(parameter);
		if (cisnan(value)) {
			efiPrintf("invalid float [%s]", parameter);
			return;
		}
		VoidFloat callbackF = (VoidFloat) current->callback;

		// invoke callback function by reference
		(*callbackF)(value);
		return;
	}

	if (current->parameterType == FLOAT_FLOAT_PARAMETER || current->parameterType == FLOAT_FLOAT_PARAMETER_P) {
		int spaceIndex = findEndOfToken(parameter);
		if (spaceIndex == -1)
			return;
		REPLACE_SPACES_WITH_ZERO;
		float value1 = atoff(parameter);
		if (cisnan(value1)) {
			efiPrintf("invalid float [%s]", parameter);
			return;
		}

		parameter += spaceIndex + 1;
		float value2 = atoff(parameter);

		if (cisnan(value2)) {
			efiPrintf("invalid float [%s]", parameter);
			return;
		}
	if (current->parameterType == FLOAT_FLOAT_PARAMETER) {
			VoidFloatFloat callbackS = (VoidFloatFloat) current->callback;
			(*callbackS)(value1, value2);
		} else {
			VoidFloatFloatVoidPtr callbackS = (VoidFloatFloatVoidPtr) current->callback;
			(*callbackS)(value1, value2, current->param);
		}
		return;
	}

	if (current->parameterType == INT_FLOAT_PARAMETER) {
		int spaceIndex = findEndOfToken(parameter);
		if (spaceIndex == -1)
			return;
		REPLACE_SPACES_WITH_ZERO;
		int value1 = atoi(parameter);
		if (absI(value1) == ERROR_CODE) {
#if EFI_PROD_CODE || EFI_SIMULATOR
			efiPrintf("not an integer [%s]", parameter);
#endif
			return;
		}
		parameter += spaceIndex + 1;
		float value2 = atoff(parameter);

		if (cisnan(value2)) {
			efiPrintf("invalid float [%s]", parameter);
			return;
		}
		
		VoidIntFloat callback = (VoidIntFloat) current->callback;
		callback(value1, value2);

		return;
	}

	int value = atoi(parameter);
	if (absI(value) == ERROR_CODE) {
		efiPrintf("invalid integer [%s]", parameter);
		return;
	}

	if (current->parameterType == ONE_PARAMETER_P) {
		VoidIntVoidPtr callback1 = (VoidIntVoidPtr) current->callback;
		// invoke callback function by reference
		(*callback1)(value, current->param);

	} else {
		VoidInt callback1 = (VoidInt) current->callback;
		// invoke callback function by reference
		(*callback1)(value);
	}

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

void initConsoleLogic() {
//	resetConsoleActions();
	addConsoleAction("help", helpCommand);
	addConsoleActionI("echo", echo);
}

/**
 * @return NULL if input line validation failed, reference to line payload if validation succeeded.
 * @see sendOutConfirmation() for command confirmation processing.
 */
char *validateSecureLine(char *line) {
	if (line == NULL)
		return NULL;
	if (strncmp(SECURE_LINE_PREFIX, line, SECURE_LINE_PREFIX_LENGTH) == 0) {
		// COM protocol looses bytes, this is a super-naive error detection

//		print("Got secure mode request header [%s]\r\n", line);
		line += SECURE_LINE_PREFIX_LENGTH;
//		print("Got secure mode request command [%s]\r\n", line);

		char *divider = line;
		while (*divider != '!') {
			if (*divider == '\0') {
				efiPrintf("Divider not found [%s]", line);
				return NULL;
			}
			divider++;
		}
		*divider++ = 0; // replacing divider symbol with zero
		int expectedLength = atoi(line);
		line = divider;
		int actualLength = strlen(line);
		if (expectedLength != actualLength) {
			efiPrintf("Error detected: expected %d but got %d in [%s]", expectedLength, actualLength, line);
			return NULL;
		}
	}
	return line;
}

static char handleBuffer[200];

static bool handleConsoleLineInternal(const char *commandLine, int lineLength) {
	strncpy(handleBuffer, commandLine, sizeof(handleBuffer) - 1);
	handleBuffer[sizeof(handleBuffer) - 1] = 0; // we want this to be null-terminated for sure
	char *line = handleBuffer;
	int firstTokenLength = tokenLength(line);

//	print("processing [%s] with %d actions\r\n", line, consoleActionCount);

	if (firstTokenLength == lineLength) {
		// no-param actions are processed here
		for (int i = 0; i < consoleActionCount; i++) {
			TokenCallback *current = &consoleActions[i];
			if (strEqual(line, current->token)) {
				if (current->parameterType == NO_PARAMETER) {
					(*current->callback)();
				} else if (current->parameterType == NO_PARAMETER_P) {
					VoidPtr cb = (VoidPtr) current->callback;
					(*cb)(current->param);
				}
				return true;
			}
		}
	} else {
		char *ptr = line + firstTokenLength;
		ptr[0] = 0; // change space into line end
		ptr++; // start from next symbol

		for (int i = 0; i < consoleActionCount; i++) {
			TokenCallback *current = &consoleActions[i];
			if (strEqual(line, current->token)) {
				handleActionWithParameter(current, ptr);
				return true;
			}
		}
	}
	return false;
}

/**
 * @brief This function takes care of one command line once we have it
 */
void handleConsoleLine(char *line) {
	line = validateSecureLine(line);
	if (line == NULL)
		return; // error detected

	int lineLength = strlen(line);
	if (lineLength > 100) {
		// todo: better max size logic
		// todo: better reaction to excessive line
		efiPrintf("Long line?");
		return;
	}

	bool isKnownComman = handleConsoleLineInternal(line, lineLength);

	if (!isKnownComman) {
		efiPrintf("unknown command [%s]", line);
		return;
	}

#if EFI_PROD_CODE || EFI_SIMULATOR
	efiPrintf("confirmation_%s:%d", line, lineLength);
#endif
}
