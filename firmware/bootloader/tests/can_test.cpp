#include "pch.h"
#include <cassert>
extern "C" {
#include "types.h"
#include "shared_params.h"
void CanInit();
blt_bool CanReceivePacket(blt_int8u*, blt_int8u*);
void XcpSetCanBaudrateHook(blt_int8u);
void OpenBltCanApplyBaudrate();
}
extern blt_int8u bootBaudrateRequest;
int CAND1;
static systime_t now;
static unsigned resets, filters;
static uint8_t requestedRate;
static CANRxFrame rx;
static can_baudrate_e rate;
void palSetPadMode(int, int, int) {}
void canSTM32SetFilters(int*, int, int, void*) { ++filters; }
void canStart(int*, const CANConfig* cfg) { rate = cfg->rate; }
int canReceiveTimeout(int*, int, CANRxFrame* frame, int) { *frame = rx; return MSG_OK; }
int canTransmitTimeout(int*, int, const CANTxFrame*, int) { return MSG_OK; }
systime_t chVTGetSystemTime() { return now; }
void chThdSleepMilliseconds(int) {}
void NVIC_SystemReset() { ++resets; }
bool SharedParamsWriteByIndex(uint32_t index, uint8_t value) {
    assert(index == 4); requestedRate = value; return true;
}
const CANConfig* findCanConfig(can_baudrate_e r) {
    static const CANConfig configs[] = {{B500KBPS}, {B1MBPS}};
    return &configs[r];
}
static void start(uint8_t r, systime_t time) {
    now = time; bootBaudrateRequest = r; resets = filters = 0; CanInit();
#ifndef TEST_FDCAN
    assert(filters == 1);
#else
    assert(filters == 0);
#endif
}
int main() {
    start(0, 0);
    assert(rate == B500KBPS);
    XcpSetCanBaudrateHook(1);
    assert(resets == 0);
    OpenBltCanApplyBaudrate();
    assert(resets == 1 && requestedRate == 1);

    start(1, 0);
    assert(rate == B1MBPS);
    now = 5001;
    OpenBltCanApplyBaudrate();
    assert(resets == 1 && requestedRate == 0);

    start(1, 0);
    rx.IDE = 1; rx.EID = 0x10667; rx.DLC = 2; rx.data8[0] = 0xff;
    uint8_t data[8], len;
    now = 4000;
    assert(CanReceivePacket(data, &len));
    now = 5001;
    OpenBltCanApplyBaudrate();
    assert(resets == 0);
    now = 9001;
    OpenBltCanApplyBaudrate();
    assert(resets == 1 && requestedRate == 0);
    // Wrong IDs and remote/oversized frames must not keep the switched link alive.
    for (int malformed = 0; malformed < 4; ++malformed) {
        start(1, 0);
        rx = {}; rx.IDE = 1; rx.EID = 0x10667; rx.DLC = 2;
        if (malformed == 0) { rx.EID++; }
        if (malformed == 1) { rx.RTR = 1; }
        if (malformed == 2) { rx.DLC = 9; }
        if (malformed == 3) { rx.DLC = 0; }
        now = 4000;
        assert(!CanReceivePacket(data, &len));
        now = 5001;
        OpenBltCanApplyBaudrate();
        assert(resets == 1);
    }
    start(1, UINT32_MAX - 1000);
    now = 4001; // Elapsed time still works across tick wrap.
    OpenBltCanApplyBaudrate();
    assert(resets == 1);

}
