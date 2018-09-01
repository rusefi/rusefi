/**
 * @file	loggingcentral.h
 *
 * @date Mar 8, 2015
 * @author Andrey Belomutskiy, (c) 2012-2017
 */
#ifndef UTIL_LOGGINGCENTRAL_H_
#define UTIL_LOGGINGCENTRAL_H_

void initLoggingCentral(void);
char * swapOutputBuffers(int *actualOutputBufferSize);
void scheduleMsg(Logging *logging, const char *fmt, ...);

#endif /* UTIL_LOGGINGCENTRAL_H_ */
