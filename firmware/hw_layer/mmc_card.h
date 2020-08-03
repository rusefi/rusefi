/*
 * @file    mmc_card.h
 *
 *
 * @date Dec 30, 2013
 * @author Kot_dnz
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

void initMmcCard(void);
bool isSdCardAlive(void);
void appendToLog(const char *line, size_t length);

void readLogFileContent(char *buffer, short fileId, short offset, short length);

void handleTsR(char *input);
void handleTsW(char *input);
