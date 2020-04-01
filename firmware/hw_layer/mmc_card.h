/*
 * @file    mmc_card.h
 *
 *
 * @date Dec 30, 2013
 * @author Kot_dnz
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void initMmcCard(void);
bool isSdCardAlive(void);
void appendToLog(const char *line);

void readLogFileContent(char *buffer, short fileId, short offset, short length);

#ifdef __cplusplus
}
#endif /* __cplusplus */

