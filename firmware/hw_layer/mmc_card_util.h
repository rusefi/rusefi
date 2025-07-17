// file mmc_card_util.h

#pragma once

void incLogFileName(FIL *fd);
void printFatFsError(const char *str, FRESULT f_error);
void writeErrorReportFile();
