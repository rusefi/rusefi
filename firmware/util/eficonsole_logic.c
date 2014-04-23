/*
 * @date Nov 15, 2012
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

/**
 * @file    eficonsole_logic.c
 * @brief   Console interactive shell code
 *
 * Here we have a data structure which holds all the dynamically-registered
 * command line interface action names & callback. This logic is invoked in
 * user context by the console thread - see consoleThreadThreadEntryPoint
 */

#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include "main.h"
#include "eficonsole_logic.h"
#include "efilib.h"
#if EFI_PROD_CODE || EFI_SIMULATOR
#include "eficonsole.h"
static Logging logging;
#endif /* EFI_PROD_CODE */

static int consoleActionCount = 0;
static TokenCallback consoleActions[CONSOLE_MAX_ACTIONS];

void resetConsoleActions(void) {
	consoleActionCount = 0;
}

static void doAddAction(char *token, int type, Void callback) {
	if (consoleActionCount == CONSOLE_MAX_ACTIONS) {
		fatal("Too many console actions\r\n");
		return;
	}
	TokenCallback *current = &consoleActions[consoleActionCount++];
	current->token = token;
	current->parameterType = type;
	current->callback = callback;
}

/**
 * @brief	Register console action without parameters
 */
void addConsoleAction(char *token, Void callback) {
	doAddAction(token, NO_PARAMETER, callback);
}

/**
 * @brief	Register a console command with one Integer parameter
 */
void addConsoleActionI(char *token, VoidInt callback) {
	doAddAction(token, ONE_PARAMETER, (Void) callback);
}

/**
 * @brief	Register a console command with two Integer parameters
 */
void addConsoleActionII(char *token, VoidIntInt callback) {
	doAddAction(token, TWO_INTS_PARAMETER, (Void) callback);
}

void addConsoleActionS(char *token, VoidCharPtr callback) {
	doAddAction(token, STRING_PARAMETER, (Void) callback);
}

void addConsoleActionSSS(char *token, VoidCharPtrCharPtrCharPtr callback) {
	doAddAction(token, STRING3_PARAMETER, (Void) callback);
}

void addConsoleActionF(char *token, VoidFloat callback) {
	doAddAction(token, FLOAT_PARAMETER, (Void) callback);
}

/**
 * @brief This function prints out a list of all available commands
 */
void helpCommand(void) {
#if EFI_PROD_CODE || EFI_SIMULATOR
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

	if (current->parameterType == STRING3_PARAMETER) {
		int spaceIndex = indexOf(parameter, ' ');
		if (spaceIndex == -1)
			return;
		parameter[spaceIndex] = 0;
		char * firstParam = parameter;

		parameter += spaceIndex + 1;
		spaceIndex = indexOf(parameter, ' ');
		if (spaceIndex == -1)
			return;
		parameter[spaceIndex] = 0;
		char * secondParam = parameter;
		parameter += spaceIndex + 1;
		char * thirdParam = parameter;

		VoidCharPtrCharPtrCharPtr callbackS = (VoidCharPtrCharPtrCharPtr) current->callback;
		(*callbackS)(firstParam, secondParam, thirdParam);
		return;

	}

	if (current->parameterType == TWO_INTS_PARAMETER) {
		int spaceIndex = indexOf(parameter, ' ');
		if (spaceIndex == -1)
			return;
		parameter[spaceIndex] = 0;
		int value1 = atoi(parameter);
		parameter += spaceIndex + 1;
		int value2 = atoi(parameter);
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
int tokenLength(char *msgp) {
	int result = 0;
	while (*msgp) {
		char ch = *msgp++;
		if (ch == ' ')
			break;
		result++;
	}
	return result;
}

int strEqual(char *str1, char *str2) {
	// todo: there must be a standard function?!
	int len1 = strlen(str1);
	int len2 = strlen(str2);
	if (len1 != len2)
		return FALSE;
	for (int i = 0; i < len1; i++)
		if (str1[i] != str2[i])
			return FALSE;
	return TRUE;
}

void initConsoleLogic() {
#if EFI_PROD_CODE || EFI_SIMULATOR
	initLogging(&logging, "rfi console");
#endif /* EFI_PROD_CODE */
	resetConsoleActions();
	addConsoleAction("help", helpCommand);
	addConsoleActionI("echo", echo);
}

char *validateSecureLine(char *line) {
	if (line == NULL)
		return NULL;
	if (strncmp("sec!", line, 4) == 0) {
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

void sendOutConfirmation(char *value, int i);

static bool_t handleConsoleLineInternal(char *line, int lineLength) {
	int firstTokenLength = tokenLength(line);

//	print("processing [%s] with %d actions\r\n", line, consoleActionCount);

	if (firstTokenLength == lineLength) {
		// no-param actions are processed here
		for (int i = 0; i < consoleActionCount; i++) {
			TokenCallback *current = &consoleActions[i];
			if (strEqual(line, current->token)) {
				// invoke callback function by reference
				(*current->callback)();
				return TRUE;
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
	return FALSE;
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
		print("Long line?\r\n");
		return;
	}

	strcpy(confirmation, "confirmation_");
	strcat(confirmation, line);
	strcat(confirmation, ":");

	bool_t isKnownComman = handleConsoleLineInternal(line, lineLength);

	// confirmation happens after the command to avoid conflict with command own output
	sendOutConfirmation(confirmation, lineLength);

	if (!isKnownComman)
		helpCommand();
}
