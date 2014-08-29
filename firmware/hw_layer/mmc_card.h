/*
 * @file    mmc_card.h
 *
 *
 * @date Dec 30, 2013
 * @author Kot_dnz
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef MMC_CARD_H_
#define MMC_CARD_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void initMmcCard(void);
bool isSdCardAlive(void);
void appendToLog(char *line);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MMC_CARD_H_ */
