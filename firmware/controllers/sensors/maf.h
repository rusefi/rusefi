/**
 * @file    maf.h
 * @brief
 *
 *
 * @date Nov 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef MAF_H_
#define MAF_H_

#include "main.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#define getMafT(ec) (getVoltageDivided(ec->mafAdcChannel))

float getMaf(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
