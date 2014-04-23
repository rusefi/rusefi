/*
 * @date Nov 15, 2012
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

/**
 * @file    eficonsole_logic.h
 * @brief   Console interactive shell header
 */

#ifndef RFICONSOLE_LOGIC_H_
#define RFICONSOLE_LOGIC_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#define CONSOLE_MAX_ACTIONS 96

#ifndef FALSE
#define FALSE       0
#endif
#ifndef TRUE
#define TRUE        (!FALSE)
#endif

typedef enum {
	NO_PARAMETER,
	ONE_PARAMETER,
	FLOAT_PARAMETER,
	STRING_PARAMETER,
	STRING3_PARAMETER,
	TWO_INTS_PARAMETER,
} ACTION_PARAMETER_TYPE;

typedef struct {
	char *token;
	int parameterType;
	void (*callback)(void);
} TokenCallback;

//void addDefaultConsoleActions(void);
//void handleActionWithParameter(TokenCallback *current, char *parameter);
int tokenLength(char *msgp);

typedef void (*Void)(void);
typedef void (*VoidInt)(int);
typedef void (*VoidFloat)(float);
typedef void (*VoidIntInt)(int, int);
typedef void (*VoidCharPtr)(char *);
typedef void (*VoidCharPtrCharPtrCharPtr)(char *, char *, char *);

char *validateSecureLine(char *line);
int strEqual(char *str1, char *str2);
void resetConsoleActions(void);
void helpCommand(void);
void initConsoleLogic(void);
void handleConsoleLine(char *line);
void addConsoleAction(char *token, Void callback);
void addConsoleActionI(char *token, VoidInt callback);
void addConsoleActionII(char *token, VoidIntInt callback);
void addConsoleActionF(char *token, VoidFloat callback);
void addConsoleActionS(char *token, VoidCharPtr callback);
void addConsoleActionSSS(char *token, VoidCharPtrCharPtrCharPtr callback);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* RFICONSOLE_H_ */
