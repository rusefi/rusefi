// file mmc_card_util.h

#pragma once

void incLogFileName();
void printError(const char *str, FRESULT f_error);
void writeErrorReportFile();
