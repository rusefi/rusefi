/**
 * @file    flash_main.h
 * @brief
 *
 * @date Sep 19, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef FLASH_MAIN_H_
#define FLASH_MAIN_H_

#include "engine_configuration.h"

#define FLASH_DATA_VERSION 3880

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void readFromFlash(void);
void initFlash(void);
void writeToFlash(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FLASH_MAIN_H_ */
