/*
 * @file chconf_common.h
 *
 * @date Apr 20, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#ifndef CONFIG_CHCONF_COMMON_H_
#define CONFIG_CHCONF_COMMON_H_

#if !defined(_FROM_ASM_)
#include "obd_error_codes.h"
#endif /* _FROM_ASM_ */


#if !defined(ENABLE_PERF_TRACE) || defined(__DOXYGEN__)
// looks like this value could not be defined in efifeatures.h - please define either externally or just change the value here
 #define ENABLE_PERF_TRACE FALSE
#endif /* ENABLE_PERF_TRACE */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
 #ifndef __ASSEMBLER__
 void firmwareError(obd_code_e code, const char *fmt, ...);
  #if ENABLE_PERF_TRACE
    void irqEnterHook(void);
    void irqExitHook(void);
    void contextSwitchHook(void);
  #else /* ENABLE_PERF_TRACE */
    #define irqEnterHook() {}
    #define irqExitHook() {}
    #define contextSwitchHook() {}
  #endif /*ENABLE_PERF_TRACE */
 #endif /* __ASSEMBLER__ */
#ifdef __cplusplus
}
#endif /* __cplusplus */

#define ON_LOCK_HOOK {}
#define ON_UNLOCK_HOOK {}

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
  void setBoardConfigurationOverrides(void);
  void setPinConfigurationOverrides(void);
  void setSerialConfigurationOverrides(void);
  void setSdCardConfigurationOverrides(void);
  void setAdcChannelOverrides(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */


#endif /* CONFIG_CHCONF_COMMON_H_ */
