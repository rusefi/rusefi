/**
 * @file	at32_common.cpp
 * @brief	Low level common Artery AT32 code
 *
 * @date Oct 29, 2023
 * @author Andrey Gusakov, (c) 2023
 */

#include "pch.h"

int at32GetMcuType(uint32_t id, const char **pn, const char **package, uint32_t *flashSize)
{
    const struct {
        uint32_t uid;
        const char *pn;
        uint32_t flashSize;
        const char *package;
    } at32f43x_types[] = {
        { 0x70084540, "AT32F435ZMT7", 4032, "LQFP144" },
        { 0x70083341, "AT32F435ZGT7", 1024, "LQFP144" },
        { 0x70084598, "AT32F435ZDT7",  448, "LQFP144" },
        { 0x70083242, "AT32F435ZCT7",  256, "LQFP144" },
        { 0x70084543, "AT32F435VMT7", 4032, "LQFP100" },
        { 0x70083344, "AT32F435VGT7", 1024, "LQFP100" },
        { 0x70084599, "AT32F435VDT7",  448, "LQFP100" },
        { 0x70083245, "AT32F435VCT7",  256, "LQFP100" },
        { 0x70084546, "AT32F435RMT7", 4032,  "LQFP64" },
        { 0x70083347, "AT32F435RGT7", 1024,  "LQFP64" },
        { 0x7008459A, "AT32F435RDT7",  448,  "LQFP64" },
        { 0x70083248, "AT32F435RCT7",  256,  "LQFP64" },
        { 0x70084549, "AT32F435CMT7", 4032,  "LQFP48" },
        { 0x7008334A, "AT32F435CGT7", 1024,  "LQFP48" },
        { 0x7008459B, "AT32F435CDT7",  448,  "LQFP48" },
        { 0x7008324B, "AT32F435CCT7",  256,  "LQFP48" },
        { 0x7008454C, "AT32F435CMU7", 4032,   "QFN48" },
        { 0x7008334D, "AT32F435CGU7", 1024,   "QFN48" },
        { 0x7008459C, "AT32F435CDU7",  448,   "QFN48" },
        { 0x7008324E, "AT32F435CCU7",  256,   "QFN48" },
        { 0x7008454F, "AT32F437ZMT7", 4032, "LQFP144" },
        { 0x70083350, "AT32F437ZGT7", 1024, "LQFP144" },
        { 0x7008459D, "AT32F437ZDT7",  448, "LQFP144" },
        { 0x70083251, "AT32F437ZCT7",  256, "LQFP144" },
        { 0x70084552, "AT32F437VMT7", 4032, "LQFP100" },
        { 0x70083353, "AT32F437VGT7", 1024, "LQFP100" },
        { 0x7008459E, "AT32F437VDT7",  448, "LQFP100" },
        { 0x70083254, "AT32F437VCT7",  256, "LQFP100" },
        { 0x70084555, "AT32F437RMT7", 4032,  "LQFP64" },
        { 0x70083356, "AT32F437RGT7", 1024,  "LQFP64" },
        { 0x7008459F, "AT32F437RDT7",  448,  "LQFP64" },
        { 0x70083257, "AT32F437RCT7",  256,  "LQFP64" },
    };

    for (size_t i = 0; i < efi::size(at32f43x_types); i++) {
        if (id == at32f43x_types[i].uid) {
            if (pn)
                *pn = at32f43x_types[i].pn;
            if (package)
                *package = at32f43x_types[i].package;
            if (flashSize)
                *flashSize = at32f43x_types[i].flashSize;
            return 0;
        }
    }

    /* unknown */
    return -1;
}

int at32GetRamSizeKb(void)
{
    uint8_t EOPB0 = *(__IO uint16_t *) (0x1FFFC010);
    /* TODO: check inverted value */

    switch ((*(__IO uint16_t *) (FLASHSIZE_BASE))) {
    case 256:
        EOPB0 &= 0x03;
        if (EOPB0 == 3)
            EOPB0 = 2;
        return 512 - (64 * EOPB0);
    case 448:
        EOPB0 &= 0x07;
        if (EOPB0 > 5)
            EOPB0 = 5;
        return 512 - (64 * EOPB0);
    case 1024:
    case 4032:
        EOPB0 &= 0x07;
        if (EOPB0 > 6)
            EOPB0 = 6;
        return 512 - (64 * EOPB0);
    default:
        return 0;
    }
    return 0;
}

#if EFI_PROD_CODE

static void reset_and_jump(void) {
    // and now reboot
    NVIC_SystemReset();
}

#if EFI_DFU_JUMP
void jump_to_bootloader() {
    // leave DFU breadcrumb which assembly startup code would check, see [rusefi][DFU] section in assembly code

    *((unsigned long *)0x2001FFF0) = 0xDEADBEEF; // End of RAM

    reset_and_jump();
}
#endif

void jump_to_openblt() {
#if EFI_USE_OPENBLT
    /* safe to call on already inited shares area */
    SharedParamsInit();
    /* Store sing to stay in OpenBLT */
    SharedParamsWriteByIndex(0, 0x01);

    reset_and_jump();
#endif
}

BOR_Level_t BOR_Get(void) {
    /* TODO: Artery */
    /* Fake */
    return BOR_Level_None;
}

BOR_Result_t BOR_Set(BOR_Level_t BORValue) {
    /* NOP */
    return BOR_Result_Ok;
}

void startWatchdog(int) {
}

void tryResetWatchdog() {
}

void setWatchdogResetPeriod(int) {
}

void baseMCUInit(void) {
    // looks like this holds a random value on start? Let's set a nice clean zero
    DWT->CYCCNT = 0;

    BOR_Set(BOR_Level_1); // one step above default value
}

/* used to detect additional RAM available for LUA
 * TODO: find RAM on AT32
 * TODO: better name */
bool isStm32F42x(void) {
    return false;
}

extern uint32_t __main_stack_base__;

typedef struct port_intctx intctx_t;

EXTERNC int getRemainingStack(thread_t *otp) {
#if CH_DBG_ENABLE_STACK_CHECK
    // this would dismiss coverity warning - see http://rusefi.com/forum/viewtopic.php?f=5&t=655
    // coverity[uninit_use]
    register intctx_t *r13 asm ("r13");
    otp->activeStack = r13;

    int remainingStack;
    if (ch0.dbg.isr_cnt > 0) {
        // ISR context
        remainingStack = (int)(r13 - 1) - (int)&__main_stack_base__;
    } else {
        remainingStack = (int)(r13 - 1) - (int)otp->wabase;
    }
    otp->remainingStack = remainingStack;
    return remainingStack;
#else
    UNUSED(otp);
    return 99999;
#endif /* CH_DBG_ENABLE_STACK_CHECK */
}

__attribute__((weak)) void boardPrepareForStandby() {
}

Reset_Cause_t getMCUResetCause() {
	return Reset_Cause_Unknown;
}

const char *getMCUResetCause(Reset_Cause_t) {
	return "Unknown";
}

#endif /* EFI_PROD_CODE */
