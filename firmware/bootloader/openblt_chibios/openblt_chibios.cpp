#include "pch.h"

extern "C" {
	#include "boot.h"
	#include "flash.h"
}

void CpuInit() { }
void CopInit() { }

void TimerInit() { }
void TimerReset() { }

void CopService() { }
void TimerUpdate() { }

// See also STM32_NOCACHE_ENABLE option will enable MPU init
extern "C" void __cpu_init() {
  // *** this is about specifically stm32f7 ***
	// This overrides the built-in __cpu_init() function
	// We do this to avoid enabling the D/I caches, which
	// we'll immediately have to turn back off when jumping
	// to the main firmware (which will then enable them itself)
}

blt_int32u TimerGet() {
	return 0;
}

void CpuMemCopy(blt_addr dest, blt_addr src, blt_int16u len)
{
	memcpy((void*)dest, (void*)src, len);
}

void CpuMemSet(blt_addr dest, blt_int8u value, blt_int16u len)
{
	memset((void*)dest, value, len);
}

/** \brief Pointer to the user program's reset vector. */
#define CPU_USER_PROGRAM_STARTADDR_PTR    ((blt_addr)(NvmGetUserProgBaseAddress() + 0x00000004))
/** \brief Pointer to the user program's vector table. */
#define CPU_USER_PROGRAM_VECTABLE_OFFSET  ((blt_int32u)NvmGetUserProgBaseAddress())

void CpuStartUserProgram(void)
{
  void (*pProgResetHandler)(void);

  /* check if a user program is present by verifying the checksum */
  if (NvmVerifyChecksum() == BLT_FALSE)
  {
#if (BOOT_COM_DEFERRED_INIT_ENABLE > 0) && (BOOT_COM_ENABLE > 0)
    /* bootloader will stay active so perform deferred initialization to make sure
     * the communication interface that were not yet initialized are now initialized.
     * this is needed to make sure firmware updates via these communication interfaces
     * will be possible.
     */
    ComDeferredInit();
#endif
    /* not a valid user program so it cannot be started */
    return;
  }
#if (BOOT_CPU_USER_PROGRAM_START_HOOK > 0)
  /* invoke callback */
  if (CpuUserProgramStartHook() == BLT_FALSE)
  {
  #if (BOOT_COM_DEFERRED_INIT_ENABLE > 0) && (BOOT_COM_ENABLE > 0)
    /* bootloader will stay active so perform deferred initialization to make sure
     * the communication interface that were not yet initialized are now initialized.
     * this is needed to make sure firmware updates via these communication interfaces
     * will be possible.
     */
    ComDeferredInit();
  #endif
    /* callback requests the user program to not be started */
    return;
  }
#endif
#if (BOOT_COM_ENABLE > 0)
  /* release the communication interface */
  ComFree();
#endif
  /* reset the HAL */
  chSysDisable();
  /* reset the timer */
  TimerReset();
  /* remap user program's vector table */
  SCB->VTOR = CPU_USER_PROGRAM_VECTABLE_OFFSET & (blt_int32u)0x1FFFFF80;
  /* set the address where the bootloader needs to jump to. this is the address of
   * the 2nd entry in the user program's vector table. this address points to the
   * user program's reset handler.
   */
  pProgResetHandler = (void(*)(void))(*((blt_addr *)CPU_USER_PROGRAM_STARTADDR_PTR));
  /* The Cortex-M4 core has interrupts enabled out of reset. the bootloader
   * explicitly disables these for security reasons. Enable them here again, so it does
   * not have to be done by the user program.
   */
  /* start the user program by activating its reset interrupt service routine */
  pProgResetHandler();
#if (BOOT_COM_DEFERRED_INIT_ENABLE > 0) && (BOOT_COM_ENABLE > 0)
  /* theoretically, the code never gets here because the user program should now be
   * running and the previous function call should not return. In case it did return
   * for whatever reason, make sure all communication interfaces are initialized so that
   * firmware updates can be started.
   */
  ComDeferredInit();
#endif
} /*** end of CpuStartUserProgram ***/


