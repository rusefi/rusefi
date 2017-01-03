/**
 * @file	microsecond_timer.h
 *
 * @date Apr 14, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef SIGNAL_TEMP_H_
#define SIGNAL_TEMP_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void initMicrosecondTimer(void);
void setHardwareUsTimer(int32_t timeUs);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SIGNAL_TEMP_H_ */
