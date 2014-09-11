/**
 * @file    cli_registry.c
 * @brief   Command-line interface commands registry
 *
 * Here we have a data structure which holds all the dynamically-registered
 * command line interface action names & callback. This logic is invoked in
 * user context by the console thread - see consoleThreadThreadEntryPoint
 *
 * TODO: there is too much copy-paste here, this class needs some refactoring :)
 *
 * @date Nov 15, 2012
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include "main.h"
#include "cli_registry.h"
#include "efilib.h"

#if EFI_PROD_CODE
#include "board_test.h"
#endif

#if EFI_PROD_CODE || EFI_SIMULATOR
#include "eficonsole.h"
static Logging logging;
#endif /* EFI_PROD_CODE */

static int consoleActionCount = 0;
static TokenCallback consoleActions[CONSOLE_MAX_ACTIONS];

#define SECURE_LINE_PREFIX "sec!"

void resetConsoleActions(void) {
	consoleActionCount = 0;
}

static void doAddAction(const char *token, int type, Void callback) {
	efiAssertVoid(consoleActionCount < CONSOLE_MAX_ACTIONS, "Too many console actions");
	TokenCallback *current = &consoleActions[consoleActionCount++];
	current->token = token;
	current->parameterType = type;
	current->callback = callback;
}

/**
 * @brief	Register console action without parameters
 */
void addConsoleAction(const char *token, Void callback) {
	doAddAction(token, NO_PARAMETER, callback);
}

/**
 * @brief	Register a console command with one Integer parameter
 */
void addConsoleActionI(const char *token, VoidInt callback) {
	doAddAction(token, ONE_PARAMETER, (Void) callback);
}

/**
 * @brief	Register a console command with two Integer parameters
 */
void addConsoleActionII(const char *token, VoidIntInt callback) {
	doAddAction(token, TWO_INTS_PARAMETER, (Void) callback);
}

void addConsoleActionS(const char *token, VoidCharPtr callback) {
	doAddAction(token, STRING_PARAMETER, (Void) callback);
}

void addConsoleActionSS(const char *token, VoidCharPtrCharPtr callback) {
	doAddAction(token, STRING2_PARAMETER, (Void) callback);
}

void addConsoleActionSSS(const char *token, VoidCharPtrCharPtrCharPtr callback) {
	doAddAction(token, STRING3_PARAMETER, (Void) callback);
}

void addConsoleActionSSSSS(const char *token, VoidCharPtrCharPtrCharPtrCharPtrCharPtr callback) {
	doAddAction(token, STRING5_PARAMETER, (Void) callback);
}

void addConsoleActionF(const char *token, VoidFloat callback) {
	doAddAction(token, FLOAT_PARAMETER, (Void) callback);
}

void addConsoleActionFF(const char *token, VoidFloatFloat callback) {
	doAddAction(token, FLOAT_FLOAT_PARAMETER, (Void) callback);
}

/**
 * @brief This function prints out a list of all available commands
 */
void helpCommand(void) {
#if EFI_PROD_CODE
	if (isBoardTestMode()) {
		printBoardTestState();
		return;
	}
#endif /* EFI_PROD_CODE */

#if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)
	scheduleMsg(&logging, "%d actions available", consoleActionCount);
	for (int i = 0; i < consoleActionCount; i++) {
		TokenCallback *current = &consoleActions[i];
		scheduleMsg(&logging, "  %s: %d parameters", current->token, current->parameterType);
	}
#endif
}

/**
 * @brief This is just a test function
 */
static void echo(int value) {
	print("got value: %d\r\n", value);
}

void handleActionWithParameter(TokenCallback *current, char *parameter) {
	if (current->parameterType == STRING_PARAMETER) {
		VoidCharPtr callbackS = (VoidCharPtr) current->callback;
		(*callbackS)(parameter);
		return;
	}

	// todo: refactor this hell!
	if (current->parameterType == STRING2_PARAMETER) {
		int spaceIndex = indexOf(parameter, ' ');
		if (spaceIndex == -1) {
			return;
		}
		parameter[spaceIndex] = 0;
		char * param0 = parameter;

		parameter += spaceIndex + 1;
		char * param1 = parameter;

		VoidCharPtrCharPtr callbackS = (VoidCharPtrCharPtr) current->callback;
		(*callbackS)(param0, param1);
		return;

	}

	if (current->parameterType == STRING3_PARAMETER) {
		int spaceIndex = indexOf(parameter, ' ');
		if (spaceIndex == -1) {
			return;
		}
		parameter[spaceIndex] = 0;
		char * param0 = parameter;

		parameter += spaceIndex + 1;
		spaceIndex = indexOf(parameter, ' ');
		if (spaceIndex == -1)
			return;
		parameter[spaceIndex] = 0;
		char * param1 = parameter;
		parameter += spaceIndex + 1;
		char * param2 = parameter;

		VoidCharPtrCharPtrCharPtr callbackS = (VoidCharPtrCharPtrCharPtr) current->callback;
		(*callbackS)(param0, param1, param2);
		return;

	}

	// todo: refactor this hell!
	if (current->parameterType == STRING5_PARAMETER) {
		int spaceIndex = indexOf(parameter, ' ');
		if (spaceIndex == -1) {
			return;
		}
		parameter[spaceIndex] = 0;
		char * param0 = parameter;

		parameter += spaceIndex + 1;
		spaceIndex = indexOf(parameter, ' ');
		if (spaceIndex == -1)
			return;
		parameter[spaceIndex] = 0;
		char * param1 = parameter;

		parameter += spaceIndex + 1;
		spaceIndex = indexOf(parameter, ' ');
		if (spaceIndex == -1)
			return;
		parameter[spaceIndex] = 0;
		char * param2 = parameter;

		parameter += spaceIndex + 1;
		spaceIndex = indexOf(parameter, ' ');
		if (spaceIndex == -1)
			return;
		parameter[spaceIndex] = 0;
		char * param3 = parameter;

		parameter += spaceIndex + 1;
		char * param4 = parameter;

		VoidCharPtrCharPtrCharPtrCharPtrCharPtr callbackS = (VoidCharPtrCharPtrCharPtrCharPtrCharPtr) current->callback;
		(*callbackS)(param0, param1, param2, param3, param4);
		return;

	}

	if (current->parameterType == TWO_INTS_PARAMETER) {
		int spaceIndex = indexOf(parameter, ' ');
		if (spaceIndex == -1)
			return;
		parameter[spaceIndex] = 0;
		int value1 = atoi(parameter);
		if (absI(value1) == absI(ERROR_CODE)) {
#if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)
			scheduleMsg(&logging, "not an integer [%s]", parameter);
#endif
			return;
		}
		parameter += spaceIndex + 1;
		int value2 = atoi(parameter);
		if (absI(value2) == absI(ERROR_CODE)) {
#if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)
			scheduleMsg(&logging, "not an integer [%s]", parameter);
#endif
			return;
		}
		VoidIntInt callbackS = (VoidIntInt) current->callback;
		(*callbackS)(value1, value2);
		return;
	}

	if (current->parameterType == FLOAT_PARAMETER) {
		float value = atoff(parameter);
		VoidFloat callbackF = (VoidFloat) current->callback;

		// invoke callback function by reference
		(*callbackF)(value);
		return;
	}

	if (current->parameterType == FLOAT_FLOAT_PARAMETER) {
		int spaceIndex = indexOf(parameter, ' ');
		if (spaceIndex == -1)
			return;
		parameter[spaceIndex] = 0;
		float value1 = atoff(parameter);
		parameter += spaceIndex + 1;
		float value2 = atoff(parameter);
		VoidFloatFloat callbackS = (VoidFloatFloat) current->callback;
		(*callbackS)(value1, value2);
		return;
	}

	int value = atoi(parameter);
	if (value == ERROR_CODE) {
		print("invalid integer [%s]\r\n", parameter);
		return;
	}

	VoidInt callback1 = (VoidInt) current->callback;

	// invoke callback function by reference
	(*callback1)(value);

}

/**
 * @return Number of space-separated tokens in the string
 */
int tokenLength(const char *msgp) {
	int result = 0;
	while (*msgp) {
		char ch = *msgp++;
		if (ch == ' ') {
			break;
		}
		result++;
	}
	return result;
}

bool strEqualCaseInsensitive(const char *str1, const char *str2) {
	// todo: actual implementation!
	return strEqual(str1, str2);
}

bool strEqual(const char *str1, const char *str2) {
	// todo: there must be a standard function?!
	int len1 = strlen(str1);
	int len2 = strlen(str2);
	if (len1 != len2) {
		return false;
	}
	for (int i = 0; i < len1; i++)
		if (str1[i] != str2[i])
			return false;
	return true;
}

void initConsoleLogic() {
#if EFI_PROD_CODE || EFI_SIMULATOR
	initLogging(&logging, "rfi console");
#endif /* EFI_PROD_CODE */
	resetConsoleActions();
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
	if (strncmp(SECURE_LINE_PREFIX, line, 4) == 0) {
		// COM protocol looses bytes, this is a super-naive error detection

//		print("Got secure mode request header [%s]\r\n", line);
		line += 4;
//		print("Got secure mode request command [%s]\r\n", line);

		char *divider = line;
		while (*divider != '!') {
			if (*divider == '\0') {
				print("Divider not found [%s]\r\n", line);
				return NULL;
			}
			divider++;
		}
		*divider++ = 0; // replacing divider symbol with zero
		int expectedLength = atoi(line);
		line = divider;
		int actualLength = strlen(line);
		if (expectedLength != actualLength) {
			print("Error detected: expected %d but got %d in [%s]\r\n", expectedLength, actualLength, line);
			return NULL;
		}
	}
	return line;
}

static char confirmation[200];

static bool handleConsoleLineInternal(char *line, int lineLength) {
	int firstTokenLength = tokenLength(line);

//	print("processing [%s] with %d actions\r\n", line, consoleActionCount);

	if (firstTokenLength == lineLength) {
		// no-param actions are processed here
		for (int i = 0; i < consoleActionCount; i++) {
			TokenCallback *current = &consoleActions[i];
			if (strEqual(line, current->token)) {
				// invoke callback function by reference
				(*current->callback)();
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
				return TRUE;
			}
		}
	}
	return false;
}

#if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)
static void sendOutConfirmation(const char *command, int length) {
	scheduleMsg(&logging, "%s%d", command, length);
}
#endif

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
		print("Long line?\r\n");
		return;
	}

	strcpy(confirmation, "confirmation_");
	strcat(confirmation, line);
	strcat(confirmation, ":");

#if EFI_PROD_CODE || EFI_SIMULATOR
	sendOutConfirmation(confirmation, lineLength);
#endif

	bool isKnownComman = handleConsoleLineInternal(line, lineLength);

	if (!isKnownComman)
		helpCommand();
}
