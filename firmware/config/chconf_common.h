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

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
 #ifndef __ASSEMBLER__
 void firmwareError(obd_code_e code, const char *fmt, ...);
  #if EFI_CLOCK_LOCKS
    void irqEnterHook(void);
    void irqExitHook(void);
  #else /* EFI_CLOCK_LOCKS */
    #define irqEnterHook() {}
    #define irqExitHook() {}
  #endif /*EFI_CLOCK_LOCKS */
 #endif /* __ASSEMBLER__ */
#ifdef __cplusplus
}
#endif /* __cplusplus */

#if EFI_CLOCK_LOCKS
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
#ifndef __ASSEMBLER__
  void onLockHook(void);
  void onUnlockHook(void);
#endif /* __ASSEMBLER__ */
#ifdef __cplusplus
}
#endif /* __cplusplus */
  #define ON_LOCK_HOOK onLockHook()
  #define ON_UNLOCK_HOOK onUnlockHook()
#else /* EFI_CLOCK_LOCKS */
  #define ON_LOCK_HOOK
  #define ON_UNLOCK_HOOK
#endif /* EFI_CLOCK_LOCKS */

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
