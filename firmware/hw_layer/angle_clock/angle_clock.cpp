/**
 * @file angle_clock.cpp
 *
 * Three-timer hardware angle clock.  See angle_clock.h for architecture.
 *
 * Register notes (same TIM peripheral on STM32F4 and AT32F435):
 *  - OCxM = 1 with CCxE = 0: pure software comparator; CCxIF fires on
 *    CNT == CCR without driving any output pin.
 *  - CCxIF is rc_w0: write ~flag to clear.
 *  - All three timers run at 4 MHz (APB1, PSC = 71 @ 288 MHz TIMCLK1).
 *    The rate is MEASURED at init against TIM5 (NT domain, physics-validated)
 *    and the PSC is re-programmed from the measurement - prevents the
 *    2026-08-30 fuse incident (wrong assumed clock rate doubled every dwell).
 *  - rccEnableTIMx(true): lp=true keeps the timer clocked during WFI sleep.
 *    lp=false would clear APB1LPENR and stop the counter in idle - the root
 *    cause of the 2026-08-31 "2x period" saga for TMR7 and the earlier TMR2
 *    offset drift.
 *
 * Concurrency (same analysis as the single-timer design):
 *  - Handoff ISR (prio 4) arms/refreshes/cancels channels.
 *  - TMR ISRs (prio 3) fire them.
 *  - A TMR ISR can preempt the handoff mid-arm; the arm sequence is ordered:
 *    1. clear stale CCxIF
 *    2. write future CCR  (CNT cannot reach it within this ISR window)
 *    3. store action
 *    4. enable CCxIE
 *    A preempting ISR sees no flag and no action, so it does nothing.
 *  - Fixed channels: the ISR releases a channel before calling action.execute()
 *    so a re-arm from inside the action is safe.
 */

#include "pch.h"

#if EFI_ANGLE_CLOCK && EFI_PROD_CODE

#include "angle_clock.h"

// ── Timer aliases ─────────────────────────────────────────────────────────
#define DWELL_TIMER  TIM2   // 32-bit APB1, VectorB0, IRQ 28
#define SPARK_TIMER  TIM4   // 16-bit APB1, VectorB8, IRQ 30
#define INJ_TIMER    TIM3   // 16-bit APB1, VectorB4, IRQ 29

// ── Shared constants ──────────────────────────────────────────────────────
// Armed tick must be at least ARM_MARGIN_TICKS in the future so the CCR
// write and CCxIE enable complete before the match.  4 us mirrors TIM5.
static constexpr uint32_t ARM_MARGIN_TICKS = US2NT(4);

// Reject targets farther than MAX_LEAD_DEG degrees from s_currentPhase:
// the early windows arm at most 2 teeth ahead (~12.4 deg on 60-2); anything
// beyond this means the phase basis jumped (desync/re-sync) and the angle
// is no longer meaningful.
static constexpr float MAX_LEAD_DEG = 30.0f;

// Absolute ceiling on the angle->time basis fed by angleClockOnTooth.
// 2 ms/deg ~ 32 rpm; bounds the armed delay so a garbage basis (e.g., the
// 10 s decoder clamp at the first teeth) cannot stick a channel for the run.
static constexpr float MAX_TICKS_PER_DEGREE = US2NT(2000);

static constexpr uint32_t CC_IF_MASK =
    STM32_TIM_SR_CC1IF | STM32_TIM_SR_CC2IF |
    STM32_TIM_SR_CC3IF | STM32_TIM_SR_CC4IF;

// ── Per-channel state ─────────────────────────────────────────────────────
struct AngleClockChannel {
    action_s action;
    float    targetAngle = 0.0f;
    uint32_t ccr = 0;   // shadow: full 32-bit for TMR2, lower 16 for TMR3/4
};

static AngleClockChannel s_dwell[4];  // TMR2 ch 0..3 = cylinder 0..3
static AngleClockChannel s_spark[4];  // TMR4 ch 0..3 = cylinder 0..3
static AngleClockChannel s_inj[4];    // TMR3 ch 0..3 = cylinder 0..3

// ── NT <-> timer tick offsets ─────────────────────────────────────────────
// All three counters run at the same 4 MHz APB1 clock; the offset is constant.
// TMR2 (32-bit): s_dwell_off = (uint32_t)(TMR2->CNT - NT_lower32)
// TMR4 (16-bit): s_spark_off = (uint16_t)(TMR4->CNT - NT_lower16)
// TMR3 (16-bit): s_inj_off   = (uint16_t)(TMR3->CNT - NT_lower16)
static uint32_t s_dwell_off;
static uint16_t s_spark_off;
static uint16_t s_inj_off;

// ── Per-tooth state (shared, written only by handoff prio 4) ─────────────
static efitick_t s_edgeTimestamp  = 0;
static float     s_currentPhase   = 0.0f;
static float     s_cycleDeg       = 720.0f;
static float     s_ticksPerDegree = 0.0f;

// ── Telemetry ─────────────────────────────────────────────────────────────
static uint32_t s_firedDwell   = 0;
static uint32_t s_firedSpark   = 0;
static uint32_t s_firedInj     = 0;
static uint32_t s_lateArmDwell = 0;
static uint32_t s_lateArmSpark = 0;
static uint32_t s_lateArmInj   = 0;
static uint32_t s_lateArmDwellGuard    = 0;  // armGuard refusal (no basis / stale phase)
static uint32_t s_lateArmDwellTickPast = 0;  // target tick already in the past
static uint32_t s_maxLateTicks = 0;
static uint32_t s_immediate    = 0;

// Most recent failed dwell arm (diagnostic snapshot, read by lockstats).
static DwellArmRefusal s_dwellRefusal;

static uint32_t s_initAcDelta = 0;
static uint32_t s_initNtDelta = 0;
static uint32_t s_initPsc     = 0;

// ── CCR register pointer ──────────────────────────────────────────────────
static volatile uint32_t* ccrReg(TIM_TypeDef* tmr, int ch) {
    switch (ch) {
    case 0:  return &tmr->CCR1;
    case 1:  return &tmr->CCR2;
    case 2:  return &tmr->CCR3;
    default: return &tmr->CCR4;
    }
}

// ── Tick conversions ──────────────────────────────────────────────────────
uint32_t angleClockTickForNt(efitick_t nt) {
    return static_cast<uint32_t>(nt) + s_dwell_off;
}

static uint16_t sparkTickForNt(efitick_t nt, uint32_t delay) {
    return static_cast<uint16_t>(static_cast<uint32_t>(nt) + s_spark_off + delay);
}

static uint16_t injTickForNt(efitick_t nt, uint32_t delay) {
    return static_cast<uint16_t>(static_cast<uint32_t>(nt) + s_inj_off + delay);
}

static float remainingAngle(float targetAngle) {
    float r = targetAngle - s_currentPhase;
    if (r < 0.0f) r += s_cycleDeg;
    return r;
}

uint32_t angleClockNow() {
    return DWELL_TIMER->CNT;
}

// ── ISR dispatch helpers ──────────────────────────────────────────────────
// 32-bit version (TMR2, dwell)
static void dispatch32(TIM_TypeDef* tmr,
                                         AngleClockChannel* chs,
                                         uint32_t& firedCount) {
    uint32_t sr = tmr->SR & CC_IF_MASK;
    for (int ch = 0; ch < 4 && sr; ch++) {
        const uint32_t flag = STM32_TIM_SR_CC1IF << ch;
        if (!(sr & flag)) continue;
        sr &= ~flag;

        tmr->SR    = ~flag;
        tmr->DIER &= ~(STM32_TIM_DIER_CC1IE << ch);

        auto& c = chs[ch];
        action_s act = c.action;
        uint32_t ccr = c.ccr;
        c.action = {};

        if (act) {
            const int32_t late =
                static_cast<int32_t>(static_cast<uint32_t>(tmr->CNT) - ccr);
            firedCount++;
            if (late > 0 && static_cast<uint32_t>(late) > s_maxLateTicks)
                s_maxLateTicks = static_cast<uint32_t>(late);
            act.execute();
        }
    }
}

// 16-bit version (TMR3/TMR4, spark/injection)
static void dispatch16(TIM_TypeDef* tmr,
                                         AngleClockChannel* chs,
                                         uint32_t& firedCount) {
    uint32_t sr = tmr->SR & CC_IF_MASK;
    for (int ch = 0; ch < 4 && sr; ch++) {
        const uint32_t flag = STM32_TIM_SR_CC1IF << ch;
        if (!(sr & flag)) continue;
        sr &= ~flag;

        tmr->SR    = ~flag;
        tmr->DIER &= ~(STM32_TIM_DIER_CC1IE << ch);

        auto& c = chs[ch];
        action_s act = c.action;
        const uint16_t ccr = static_cast<uint16_t>(c.ccr);
        c.action = {};

        if (act) {
            const int16_t late =
                static_cast<int16_t>(static_cast<uint16_t>(tmr->CNT) - ccr);
            firedCount++;
            if (late > 0 && static_cast<uint32_t>(late) > s_maxLateTicks)
                s_maxLateTicks = static_cast<uint32_t>(late);
            act.execute();
        }
    }
    assertInterruptPriority(__func__, EFI_IRQ_ANGLE_CLOCK_PRIORITY);
}

// ── ISR handlers ─────────────────────────────────────────────────────────
extern "C" {

void STM32_TIM2_HANDLER(void) {   // dwell
    OSAL_IRQ_PROLOGUE();
    dispatch32(DWELL_TIMER, s_dwell, s_firedDwell);
    assertInterruptPriority(__func__, EFI_IRQ_ANGLE_CLOCK_PRIORITY);
    OSAL_IRQ_EPILOGUE();
}

void STM32_TIM4_HANDLER(void) {   // spark
    OSAL_IRQ_PROLOGUE();
    dispatch16(SPARK_TIMER, s_spark, s_firedSpark);
    OSAL_IRQ_EPILOGUE();
}

void STM32_TIM3_HANDLER(void) {   // injection
    OSAL_IRQ_PROLOGUE();
    dispatch16(INJ_TIMER, s_inj, s_firedInj);
    OSAL_IRQ_EPILOGUE();
}

} // extern "C"

// ── Initialization ────────────────────────────────────────────────────────
static void initTimerRegs(TIM_TypeDef* tmr, uint32_t psc) {
    tmr->PSC   = psc;
    tmr->ARR   = 0xFFFFFFFFU;  // 16-bit timers naturally truncate CNT/ARR
    tmr->CR1   = 0;
    tmr->CCMR1 = STM32_TIM_CCMR1_OC1M(1) | STM32_TIM_CCMR1_OC2M(1);
    tmr->CCMR2 = STM32_TIM_CCMR1_OC1M(1) | STM32_TIM_CCMR1_OC2M(1);
    tmr->CCER  = 0;
    tmr->DIER  = 0;
    tmr->EGR   = STM32_TIM_EGR_UG;   // latch PSC/ARR, clear CNT
    tmr->SR    = 0;
    tmr->CR1   = STM32_TIM_CR1_CEN;
}

void initAngleClock() {
    // lp=true: keep all three clocked during WFI sleep (APB1LPENR).
    // lp=false would gate the clock and drift the NT<->timer offset on
    // every idle period - the 2026-08-31 "2x period" root cause.
    rccEnableTIM2(true);
    rccEnableTIM4(true);
    rccEnableTIM3(true);

    // Freeze alongside TIM5 on debugger halt so the NT<->timer offsets
    // stay constant across core halts.
#if defined(STM32F4XX) || defined(STM32F7XX)
    DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_TIM2_STOP |
                      DBGMCU_APB1_FZ_DBG_TIM3_STOP |
                      DBGMCU_APB1_FZ_DBG_TIM4_STOP;
#endif

    // Start all three at the provisional PSC = 71 (assumes TIMCLK1 = 288 MHz
    // -> 4 MHz).  The measured rate below corrects it.
    initTimerRegs(DWELL_TIMER, 71);
    initTimerRegs(SPARK_TIMER, 71);
    initTimerRegs(INJ_TIMER,   71);

    // Measure the real TMR2 rate against NT (TIM5).  All three timers share
    // the same APB1 clock so one measurement corrects all three PSCs.
    // This is load-bearing: a 2x-wrong PSC doubled every dwell and blew the
    // 15A fuse on 2026-08-30.
    const uint32_t ac0 = DWELL_TIMER->CNT;
    const uint32_t nt0 = getTimeNowLowerNt();
    do { } while (getTimeNowLowerNt() - nt0 < US2NT(10000));  // 10 ms
    const uint32_t acDelta = DWELL_TIMER->CNT - ac0;
    const uint32_t ntDelta = getTimeNowLowerNt() - nt0;

    // Round-half-up to nearest integer: (72*ac + nt/2) / nt - 1.
    // Plain truncating division (72*ac/nt - 1) rounds DOWN, which when
    // acDelta < ntDelta by even 1 tick (measurement noise) gives PSC=70
    // instead of 71. With PSC=70, TMR2 runs 1.4% faster than NT (TIM5),
    // the NT<->TMR2 offset drifts ~56 k ticks/s, and all arm checks fail
    // after ~10 ms -> the car stops starting (angclk psc=70 was observed
    // in the field, 2026-09-01). TMR2/3/4 share the same APB1 TIMCLK1 as
    // TIM5, so with the same PSC the rates are identical; the measurement
    // is only a safety net and must never overshoot by rounding error.
    uint32_t newPsc = (72U * acDelta + ntDelta / 2U) / ntDelta - 1U;
    if (newPsc > 0xFFFFU) newPsc = 0xFFFFU;

    // Re-program all three with the measured PSC.
    DWELL_TIMER->CR1 = 0; DWELL_TIMER->PSC = newPsc;
    SPARK_TIMER->CR1 = 0; SPARK_TIMER->PSC = newPsc;
    INJ_TIMER->CR1   = 0; INJ_TIMER->PSC   = newPsc;
    DWELL_TIMER->CNT = 0;
    SPARK_TIMER->CNT = 0;
    INJ_TIMER->CNT   = 0;
    DWELL_TIMER->EGR = STM32_TIM_EGR_UG;
    SPARK_TIMER->EGR = STM32_TIM_EGR_UG;
    INJ_TIMER->EGR   = STM32_TIM_EGR_UG;
    DWELL_TIMER->SR  = 0; SPARK_TIMER->SR = 0; INJ_TIMER->SR = 0;
    DWELL_TIMER->CR1 = STM32_TIM_CR1_CEN;
    SPARK_TIMER->CR1 = STM32_TIM_CR1_CEN;
    INJ_TIMER->CR1   = STM32_TIM_CR1_CEN;

    // Measure NT <-> each timer offset AFTER the PSC re-programming.
    // All three run at exactly 4 MHz (same TIMCLK1), so the offset is constant.
    {
        const uint32_t nowNt = getTimeNowLowerNt();
        s_dwell_off = DWELL_TIMER->CNT - nowNt;
        s_spark_off = static_cast<uint16_t>(SPARK_TIMER->CNT - nowNt);
        s_inj_off   = static_cast<uint16_t>(INJ_TIMER->CNT   - nowNt);
    }

    nvicEnableVector(STM32_TIM2_NUMBER, EFI_IRQ_ANGLE_CLOCK_PRIORITY);
    nvicEnableVector(STM32_TIM4_NUMBER, EFI_IRQ_ANGLE_CLOCK_PRIORITY);
    nvicEnableVector(STM32_TIM3_NUMBER, EFI_IRQ_ANGLE_CLOCK_PRIORITY);

    s_initAcDelta = acDelta;
    s_initNtDelta = ntDelta;
    s_initPsc     = newPsc;

    efiPrintf("angle clock: TMR2/3/4 init PSC 71->%lu (measured %lu/%lu ticks)",
        static_cast<unsigned long>(newPsc),
        static_cast<unsigned long>(acDelta),
        static_cast<unsigned long>(ntDelta));
}

// ── Per-tooth state update ────────────────────────────────────────────────
void angleClockOnTooth(efitick_t edgeTimestamp,
                                         float currentPhase,
                                         float cycleDeg,
                                         float ticksPerDegree) {
    s_edgeTimestamp = edgeTimestamp;
    s_currentPhase  = currentPhase;
    s_cycleDeg      = cycleDeg;

    // Keep the last good basis when the feed is NaN/zero (rpm==0 storm flap).
    // Zeroing the basis would collapse every armed tick onto the tooth edge
    // and fire everything immediately at the worst moment.
    if (!(ticksPerDegree > 0.0f)) {
        return;
    }
    if (ticksPerDegree > MAX_TICKS_PER_DEGREE) {
        s_ticksPerDegree = MAX_TICKS_PER_DEGREE;
    } else {
        s_ticksPerDegree = ticksPerDegree;
    }
}

// ── Arm functions ─────────────────────────────────────────────────────────
// Common arm guard: check lead window and return delay in ticks.
// Returns false when the target is too far ahead (stale phase) or the
// basis is not yet valid.
static bool armGuard(float targetAngle,
                                       float& outRemaining,
                                       uint32_t& outDelay) {
    if (s_ticksPerDegree <= 0.0f) return false;
    outRemaining = remainingAngle(targetAngle);
    if (outRemaining > MAX_LEAD_DEG) return false;
    outDelay = angleClockDelayTicks(outRemaining, s_ticksPerDegree);
    return true;
}

// Record the most recent dwell arm refusal for lockstats diagnostics. Runs
// only on the failure path, so the flash-resident body is not a hot-path cost.
static void recordDwellRefusal(uint8_t branch, uint8_t cyl, bool earlyWindow,
                               float targetAngle, float callerPhase, float callerNextPhase,
                               float remaining, uint32_t atTick, uint32_t ccrCnt) {
    s_dwellRefusal.branch = branch;
    s_dwellRefusal.window = earlyWindow ? 0 : 1;
    s_dwellRefusal.cyl = cyl;
    s_dwellRefusal.targetAngle = targetAngle;
    s_dwellRefusal.callerPhase = callerPhase;
    s_dwellRefusal.callerNextPhase = callerNextPhase;
    s_dwellRefusal.currentPhase = s_currentPhase;
    s_dwellRefusal.cycleDeg = s_cycleDeg;
    s_dwellRefusal.ticksPerDegree = s_ticksPerDegree;
    s_dwellRefusal.remaining = remaining;
    s_dwellRefusal.atTick = atTick;
    s_dwellRefusal.ccrCnt = ccrCnt;
    s_dwellRefusal.lateArmTotal = s_lateArmDwell;
}

bool angleClockArmDwell(int cyl, float targetAngle,
                                          action_s action,
                                          float callerPhase,
                                          float callerNextPhase,
                                          bool earlyWindow) {
    const float remaining = remainingAngle(targetAngle);

    // armGuard, inlined so each refusal is attributed to its exact branch in
    // the snapshot (the shared armGuard() is kept for spark/injection).
    if (s_ticksPerDegree <= 0.0f) {
        s_lateArmDwell++;
        s_lateArmDwellGuard++;
        recordDwellRefusal(0, static_cast<uint8_t>(cyl), earlyWindow, targetAngle,
                           callerPhase, callerNextPhase, remaining, 0, DWELL_TIMER->CNT);
        return false;
    }
    if (remaining > MAX_LEAD_DEG) {
        s_lateArmDwell++;
        s_lateArmDwellGuard++;
        recordDwellRefusal(1, static_cast<uint8_t>(cyl), earlyWindow, targetAngle,
                           callerPhase, callerNextPhase, remaining, 0, DWELL_TIMER->CNT);
        return false;
    }

    const uint32_t delay  = angleClockDelayTicks(remaining, s_ticksPerDegree);
    const uint32_t atTick = angleClockTickForNt(s_edgeTimestamp) + delay;
    const uint32_t cnt    = DWELL_TIMER->CNT;

    if (static_cast<int32_t>(atTick - cnt) <
        static_cast<int32_t>(ARM_MARGIN_TICKS)) {
        s_lateArmDwell++;
        s_lateArmDwellTickPast++;
        recordDwellRefusal(2, static_cast<uint8_t>(cyl), earlyWindow, targetAngle,
                           callerPhase, callerNextPhase, remaining, atTick, cnt);
        return false;
    }

    auto& c = s_dwell[cyl];
    const uint32_t flag = STM32_TIM_SR_CC1IF << cyl;
    DWELL_TIMER->SR    = ~flag;
    *ccrReg(DWELL_TIMER, cyl) = atTick;
    c.ccr         = atTick;
    c.targetAngle = targetAngle;
    c.action      = action;
    DWELL_TIMER->DIER |= STM32_TIM_DIER_CC1IE << cyl;
    return true;
}

bool angleClockArmSpark(int cyl, float targetAngle,
                                          action_s action,
                                          float /*callerPhase*/,
                                          float /*callerNextPhase*/) {
    float remaining; uint32_t delay;
    if (!armGuard(targetAngle, remaining, delay)) {
        s_lateArmSpark++;
        return false;
    }

    const uint16_t atTick = sparkTickForNt(s_edgeTimestamp, delay);
    const uint16_t cnt16  = static_cast<uint16_t>(SPARK_TIMER->CNT);

    if (static_cast<int16_t>(atTick - cnt16) <
        static_cast<int16_t>(ARM_MARGIN_TICKS)) {
        s_lateArmSpark++;
        return false;
    }

    auto& c = s_spark[cyl];
    const uint32_t flag = STM32_TIM_SR_CC1IF << cyl;
    SPARK_TIMER->SR    = ~flag;
    *ccrReg(SPARK_TIMER, cyl) = atTick;
    c.ccr         = atTick;
    c.targetAngle = targetAngle;
    c.action      = action;
    SPARK_TIMER->DIER |= STM32_TIM_DIER_CC1IE << cyl;
    return true;
}

bool angleClockArmInjection(int cyl, float targetAngle,
                                              action_s action,
                                              float /*callerPhase*/,
                                              float /*callerNextPhase*/) {
    float remaining; uint32_t delay;
    if (!armGuard(targetAngle, remaining, delay)) {
        s_lateArmInj++;
        return false;
    }

    const uint16_t atTick = injTickForNt(s_edgeTimestamp, delay);
    const uint16_t cnt16  = static_cast<uint16_t>(INJ_TIMER->CNT);

    if (static_cast<int16_t>(atTick - cnt16) <
        static_cast<int16_t>(ARM_MARGIN_TICKS)) {
        s_lateArmInj++;
        return false;
    }

    auto& c = s_inj[cyl];
    const uint32_t flag = STM32_TIM_SR_CC1IF << cyl;
    INJ_TIMER->SR    = ~flag;
    *ccrReg(INJ_TIMER, cyl) = atTick;
    c.ccr         = atTick;
    c.targetAngle = targetAngle;
    c.action      = action;
    INJ_TIMER->DIER |= STM32_TIM_DIER_CC1IE << cyl;
    return true;
}

// ── Refresh ───────────────────────────────────────────────────────────────
// Re-anchor one 32-bit timer channel from freshest tooth data.
static void refreshChannel32(TIM_TypeDef* tmr,
                                               AngleClockChannel& c, int ch) {
    if (!c.action) return;

    const uint32_t flag = STM32_TIM_SR_CC1IF << ch;
    // If CCxIF is pending the ISR is about to fire with the current CCR -
    // don't touch it (re-anchoring would give a ~2^32 late reading).
    if (tmr->SR & flag) return;

    const float remaining = remainingAngle(c.targetAngle);
    // Phase basis jumped (desync): leave armed to fire by time, exactly
    // like the FALSE build which fires time-scheduled events regardless of sync.
    if (remaining > MAX_LEAD_DEG) return;

    const uint32_t oldCcr = c.ccr;
    const uint32_t newTick = angleClockTickForNt(s_edgeTimestamp) +
        angleClockDelayTicks(remaining, s_ticksPerDegree);

    // Only re-anchor EARLIER: the armed tick is the best prediction from the
    // arm tooth; a later-tooth basis cannot push it out (that was the pile-up
    // root cause at the catch with the stale ~250 rpm basis).
    const uint32_t anchor = (static_cast<int32_t>(newTick - oldCcr) > 0)
                            ? oldCcr : newTick;

    if (static_cast<int32_t>(anchor - static_cast<uint32_t>(tmr->CNT)) <
        static_cast<int32_t>(ARM_MARGIN_TICKS)) {
        // Due now: arm for immediate ISR fire.
        const uint32_t nowTick = static_cast<uint32_t>(tmr->CNT) + ARM_MARGIN_TICKS;
        *ccrReg(tmr, ch) = nowTick;
        // Race: old CCR may have matched in the few cycles between the flag
        // check above and this write.  If so, the pending ISR fires the action
        // with the old CCR - restore it so the lateness measurement is correct.
        if (tmr->SR & flag) {
            *ccrReg(tmr, ch) = oldCcr;
        } else {
            c.ccr = nowTick;
            s_immediate++;
        }
        return;
    }

    // Normal re-anchor.
    *ccrReg(tmr, ch) = anchor;
    if (tmr->SR & flag) {
        *ccrReg(tmr, ch) = oldCcr;
    } else {
        c.ccr = anchor;
    }
}

// Re-anchor one 16-bit timer channel from freshest tooth data.
static void refreshChannel16(TIM_TypeDef* tmr,
                                               AngleClockChannel& c, int ch,
                                               uint16_t ntOff) {
    if (!c.action) return;

    const uint32_t flag = STM32_TIM_SR_CC1IF << ch;
    if (tmr->SR & flag) return;

    const float remaining = remainingAngle(c.targetAngle);
    if (remaining > MAX_LEAD_DEG) return;

    const uint16_t oldCcr = static_cast<uint16_t>(c.ccr);
    const uint32_t delay  = angleClockDelayTicks(remaining, s_ticksPerDegree);
    const uint16_t newTick =
        static_cast<uint16_t>(static_cast<uint32_t>(s_edgeTimestamp) + ntOff + delay);

    const uint16_t anchor = (static_cast<int16_t>(newTick - oldCcr) > 0)
                            ? oldCcr : newTick;

    const uint16_t cnt16 = static_cast<uint16_t>(tmr->CNT);
    if (static_cast<int16_t>(anchor - cnt16) < static_cast<int16_t>(ARM_MARGIN_TICKS)) {
        const uint16_t nowTick = cnt16 + static_cast<uint16_t>(ARM_MARGIN_TICKS);
        *ccrReg(tmr, ch) = nowTick;
        if (tmr->SR & flag) {
            *ccrReg(tmr, ch) = oldCcr;
        } else {
            c.ccr = nowTick;
            s_immediate++;
        }
        return;
    }

    *ccrReg(tmr, ch) = anchor;
    if (tmr->SR & flag) {
        *ccrReg(tmr, ch) = oldCcr;
    } else {
        c.ccr = anchor;
    }
}

void angleClockRefresh() {
    for (int ch = 0; ch < 4; ch++) {
        refreshChannel32(DWELL_TIMER, s_dwell[ch], ch);
        refreshChannel16(SPARK_TIMER, s_spark[ch], ch, s_spark_off);
        refreshChannel16(INJ_TIMER,   s_inj[ch],   ch, s_inj_off);
    }
}

// ── Cancel ────────────────────────────────────────────────────────────────
void angleClockCancelDwell(int cyl) {
    s_dwell[cyl].action = {};
    DWELL_TIMER->DIER &= ~(STM32_TIM_DIER_CC1IE << cyl);
}

void angleClockCancelSpark(int cyl) {
    s_spark[cyl].action = {};
    SPARK_TIMER->DIER &= ~(STM32_TIM_DIER_CC1IE << cyl);
}

bool angleClockArmSparkFromNow(int cyl, efitick_t nowNt,
                                                  uint32_t delayNt, action_s action) {
    // Compute CCR in TMR4 16-bit domain directly from NT time + delay.
    // Uses the same conversion as sparkTickForNt but without the angle-domain
    // guard (targetAngle is set to 780 deg so the refresh always skips it).
    const uint16_t atTick = sparkTickForNt(nowNt, delayNt);
    const uint16_t cnt16  = static_cast<uint16_t>(SPARK_TIMER->CNT);

    if (static_cast<int16_t>(atTick - cnt16) < static_cast<int16_t>(ARM_MARGIN_TICKS)) {
        s_lateArmSpark++;
        return false;
    }

    auto& c = s_spark[cyl];
    const uint32_t flag = STM32_TIM_SR_CC1IF << cyl;
    SPARK_TIMER->SR   = ~flag;
    *ccrReg(SPARK_TIMER, cyl) = atTick;
    c.ccr         = atTick;
    // Sentinel: 780 deg > cycleDeg(720) + MAX_LEAD_DEG(30) = 750.
    // remainingAngle(780) = 780 - currentPhase > 60 > MAX_LEAD_DEG for ANY
    // currentPhase in [0, 720) -> refresh always skips this channel.
    c.targetAngle = 780.0f;
    c.action      = action;
    SPARK_TIMER->DIER |= STM32_TIM_DIER_CC1IE << cyl;
    return true;
}

void angleClockCancelInjection(int cyl) {
    s_inj[cyl].action = {};
    INJ_TIMER->DIER &= ~(STM32_TIM_DIER_CC1IE << cyl);
}

bool angleClockArmInjectionFromNow(int cyl, efitick_t nowNt,
                                                      uint32_t delayNt, action_s action) {
    const uint16_t atTick = injTickForNt(nowNt, delayNt);
    const uint16_t cnt16  = static_cast<uint16_t>(INJ_TIMER->CNT);

    if (static_cast<int16_t>(atTick - cnt16) < static_cast<int16_t>(ARM_MARGIN_TICKS)) {
        s_lateArmInj++;
        return false;
    }

    auto& c = s_inj[cyl];
    const uint32_t flag = STM32_TIM_SR_CC1IF << cyl;
    INJ_TIMER->SR   = ~flag;
    *ccrReg(INJ_TIMER, cyl) = atTick;
    c.ccr         = atTick;
    // Sentinel: 780 deg > cycleDeg(720) + MAX_LEAD_DEG(30) = 750 for any
    // currentPhase in [0, 720) -> refresh always skips this channel.
    c.targetAngle = 780.0f;
    c.action      = action;
    INJ_TIMER->DIER |= STM32_TIM_DIER_CC1IE << cyl;
    return true;
}

void angleClockCancelAll() {
    for (int ch = 0; ch < 4; ch++) {
        angleClockCancelDwell(ch);
        angleClockCancelSpark(ch);
        angleClockCancelInjection(ch);
    }
}

// ── Telemetry accessors ───────────────────────────────────────────────────
uint32_t angleClockFiredDwell()      { return s_firedDwell; }
uint32_t angleClockFiredSpark()      { return s_firedSpark; }
uint32_t angleClockFiredInj()        { return s_firedInj; }
uint32_t angleClockLateArmDwell()    { return s_lateArmDwell; }
uint32_t angleClockLateArmSpark()    { return s_lateArmSpark; }
uint32_t angleClockLateArmInj()      { return s_lateArmInj; }
uint32_t angleClockLateArmDwellGuard()    { return s_lateArmDwellGuard; }
uint32_t angleClockLateArmDwellTickPast() { return s_lateArmDwellTickPast; }
const DwellArmRefusal& angleClockDwellRefusal() { return s_dwellRefusal; }
uint32_t angleClockMaxLateTicks()    { return s_maxLateTicks; }
uint32_t angleClockImmediateFireCount() { return s_immediate; }
uint32_t angleClockInitAcDelta()     { return s_initAcDelta; }
uint32_t angleClockInitNtDelta()     { return s_initNtDelta; }
uint32_t angleClockInitPsc()         { return s_initPsc; }

void angleClockResetStats() {
    s_firedDwell = s_firedSpark = s_firedInj = 0;
    s_lateArmDwell = s_lateArmSpark = s_lateArmInj = 0;
    s_lateArmDwellGuard = s_lateArmDwellTickPast = 0;
    s_maxLateTicks = s_immediate = 0;
    s_dwellRefusal.branch = 0xFF;  // sentinel: no refusal since last reset
}

#endif // EFI_ANGLE_CLOCK && EFI_PROD_CODE
