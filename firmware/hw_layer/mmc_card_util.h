// file mmc_card_util.h

#pragma once

bool incLogFileName(FIL *fd);
const char *getFatFsErrorDescription(FRESULT f_error);
void printFatFsError(const char *str, FRESULT f_error);
void writeErrorReportFile();
