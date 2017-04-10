/**
 * @file	rusEfiFunctionalTest.h
 *
 * @date Mar 1, 2014
 * @author Andrey Belomutskiy, (c) 2012-2016
 */

#ifndef RUSEFIFUNCTIONALTEST_H_
#define RUSEFIFUNCTIONALTEST_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void rusEfiFunctionalTest(void);
void printPendingMessages(void);
void logMsg(const char *fmt, ...);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* RUSEFIFUNCTIONALTEST_H_ */
