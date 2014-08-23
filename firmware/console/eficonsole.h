/**
 * @file    eficonsole.h
 * @brief   Console package entry point header
 *
 * @date Nov 15, 2012
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#pragma once
#ifndef RFICONSOLE_H_
#define RFICONSOLE_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void initializeConsole(void);
void print(const char *fmt, ...);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* RFICONSOLE_H_ */
