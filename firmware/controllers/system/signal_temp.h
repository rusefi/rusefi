/**
 * @file	signal_temp.h
 *
 * @date Apr 14, 2014
 * @author Andrey Belomutskiy, (c) 2012-2013
 */

#ifndef SIGNAL_TEMP_H_
#define SIGNAL_TEMP_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void TIM_Init(void);
void setTimer(int timeUs);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SIGNAL_TEMP_H_ */
