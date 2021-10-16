/*
 * @file chconf_common.h
 *
 * @date Apr 20, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#ifndef CONFIG_CHCONF_COMMON_H_
#define CONFIG_CHCONF_COMMON_H_

#define PORT_IDLE_THREAD_STACK_SIZE     32

// See global_shared.h notes about stack requirements
// see also http://www.chibios.org/dokuwiki/doku.php?id=chibios:kb:stacks
#define PORT_INT_REQUIRED_STACK 	128

#define CHPRINTF_USE_FLOAT          	TRUE

#if !defined(_FROM_ASM_)
#include "obd_error_codes.h"
#endif /* _FROM_ASM_ */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
 #ifndef __ASSEMBLER__
 void firmwareError(obd_code_e code, const char *fmt, ...);
 void irqEnterHook(void);
 void irqExitHook(void);
 void contextSwitchHook(void);
 void threadInitHook(void* tp);
 void onLockHook(void);
 void onUnlockHook(void);
 #endif /* __ASSEMBLER__ */
#ifdef __cplusplus
}
#endif /* __cplusplus */

#define ON_LOCK_HOOK onLockHook()
#define ON_UNLOCK_HOOK onUnlockHook()

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
  void setPinConfigurationOverrides(void);
  void setSerialConfigurationOverrides(void);
  void setSdCardConfigurationOverrides(void);
  void setAdcChannelOverrides(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

/*===========================================================================*/
/**
 * @name Kernel hooks
 * @{
 */
/*===========================================================================*/

/**
 * @brief   Threads descriptor structure extension.
 * @details User fields added to the end of the @p thread_t structure.
 */
#define CH_CFG_THREAD_EXTRA_FIELDS                                          \
  void *activeStack; \
  int remainingStack; \
  unsigned char threadId; \
  /* Add threads custom fields here.*/

/**
 * @brief   Threads initialization hook.
 * @details User initialization code added to the @p chThdInit() API.
 *
 * @note    It is invoked from within @p chThdInit() and implicitly from all
 *          the threads creation APIs.
 */
#define CH_CFG_THREAD_INIT_HOOK(tp) {                                       \
  threadInitHook(tp);                                                       \
}

/**
 * @brief   Context switch hook.
 * @details This hook is invoked just before switching between threads.
 */
#define CH_CFG_CONTEXT_SWITCH_HOOK(ntp, otp) {                              \
  contextSwitchHook();                                                      \
}

/**
 * @brief   ISR enter hook.
 */
#define CH_CFG_IRQ_PROLOGUE_HOOK() {                                        \
  /* IRQ prologue code here.*/                                              \
  irqEnterHook();                                                           \
}

/**
 * @brief   ISR exit hook.
 */
#define CH_CFG_IRQ_EPILOGUE_HOOK() {                                        \
  /* IRQ epilogue code here.*/                                              \
  irqExitHook();                                                            \
}

/**
 * declared as a macro so that this code does not use stack
 * so that it would not crash the error handler in case of stack issues
 */
#if CH_DBG_SYSTEM_STATE_CHECK
#define hasOsPanicError() (ch.dbg.panic_msg != NULL)
#else
#define hasOsPanicError() (FALSE)
#endif

#ifndef __ASSEMBLER__
 #ifdef __cplusplus
  extern "C"
 #endif // __cplusplus
 void chDbgPanic3(const char *msg, const char * file, int line);
#endif // __ASSEMBLER__

#endif /* CONFIG_CHCONF_COMMON_H_ */
