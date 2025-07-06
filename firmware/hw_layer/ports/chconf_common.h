/*
 * @file chconf_common.h
 *
 * @date Apr 20, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

/*
 * __process_stack_size__ and __process_stack_size__ defaults are each hard-coded as 0x400 in ChibiOS rules.mk files
 * rusEFI does not override these defaults.
 *
 * http://www.chibios.com/forum/viewtopic.php?t=309
 * "__main_stack_size__ is the size of INTERRUPTS stack"
 * "__process_stack_size__ is the stack of the C-runtime, in ChibiOS the "main" thread uses the C-runtime stack."
 *
 */

#pragma once

#define _CHIBIOS_RT_CONF_
#define _CHIBIOS_RT_CONF_VER_7_0_

#define PORT_IDLE_THREAD_STACK_SIZE     32

// See global_shared.h notes about stack requirements
// see also http://www.chibios.org/dokuwiki/doku.php?id=chibios:kb:stacks
#define PORT_INT_REQUIRED_STACK 	128

#define CHPRINTF_USE_FLOAT          	TRUE

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
 #ifndef __ASSEMBLER__
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
 * @brief   System structure extension.
 * @details User fields added to the end of the @p ch_system_t structure.
 */
#define CH_CFG_SYSTEM_EXTRA_FIELDS                                          \
  /* Add threads custom fields here.*/

/**
 * @brief   System initialization hook.
 * @details User initialization code added to the @p chSysInit() function
 *          just before interrupts are enabled globally.
 */
#define CH_CFG_SYSTEM_INIT_HOOK() {                                         \
  /* Add system initialization code here.*/                                 \
}

/**
 * @brief   OS instance structure extension.
 * @details User fields added to the end of the @p os_instance_t structure.
 */
#define CH_CFG_OS_INSTANCE_EXTRA_FIELDS                                     \
  /* Add OS instance custom fields here.*/

/**
 * @brief   OS instance initialization hook.
 *
 * @param[in] oip       pointer to the @p os_instance_t structure
 */
#define CH_CFG_OS_INSTANCE_INIT_HOOK(oip) {                                 \
  /* Add OS instance initialization code here.*/                            \
}

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
 * @brief   Threads finalization hook.
 * @details User finalization code added to the @p chThdExit() API.
 *
 * @param[in] tp        pointer to the @p thread_t structure
 */
#define CH_CFG_THREAD_EXIT_HOOK(tp) {                                       \
  /* Add threads finalization code here.*/                                  \
}

/**
 * @brief   Context switch hook.
 * @details This hook is invoked just before switching between threads.
 *
 * @param[in] ntp       thread being switched in
 * @param[in] otp       thread being switched out
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
 * @brief   Idle thread enter hook.
 * @note    This hook is invoked within a critical zone, no OS functions
 *          should be invoked from here.
 * @note    This macro can be used to activate a power saving mode.
 */
#define CH_CFG_IDLE_ENTER_HOOK() {                                          \
  /* Idle-enter code here.*/                                                \
}

/**
 * @brief   Idle thread leave hook.
 * @note    This hook is invoked within a critical zone, no OS functions
 *          should be invoked from here.
 * @note    This macro can be used to deactivate a power saving mode.
 */
#define CH_CFG_IDLE_LEAVE_HOOK() {                                          \
  /* Idle-leave code here.*/                                                \
}

/**
 * @brief   Idle Loop hook.
 * @details This hook is continuously invoked by the idle thread loop.
 */
#define CH_CFG_IDLE_LOOP_HOOK() {                                           \
  /* Idle loop code here.*/                                                 \
}

/**
 * @brief   System tick event hook.
 * @details This hook is invoked in the system tick handler immediately
 *          after processing the virtual timers queue.
 */
#define CH_CFG_SYSTEM_TICK_HOOK() {                                         \
  /* System tick event code here.*/                                         \
}

/**
 * @brief   System halt hook.
 * @details This hook is invoked in case to a system halting error before
 *          the system is halted.
 */
#define CH_CFG_SYSTEM_HALT_HOOK(reason) {                                   \
  /* System halt code here.*/                                               \
  chDbgPanic3(reason, __FILE__, __LINE__); \
}

/**
 * @brief   Trace hook.
 * @details This hook is invoked each time a new record is written in the
 *          trace buffer.
 */
#define CH_CFG_TRACE_HOOK(tep) {                                            \
  /* Trace code here.*/                                                     \
}

/**
 * @brief   Runtime Faults Collection Unit hook.
 * @details This hook is invoked each time new faults are collected and stored.
 */
#define CH_CFG_RUNTIME_FAULTS_HOOK(mask) {                                  \
  /* Faults handling code here.*/                                           \
}

/** @} */

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
