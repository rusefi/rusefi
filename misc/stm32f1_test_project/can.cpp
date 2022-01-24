#include "can.h"
#include "hal.h"

#include <cstdint>
#include <cstring>

const CANConfig canConfig500 =
{
    CAN_MCR_ABOM | CAN_MCR_AWUM | CAN_MCR_TXFP,
    /*
    Settings from https://forum.chibios.org/viewtopic.php?t=2990
     For 36MHz http://www.bittiming.can-wiki.info/ does not give that option :( Subtract '1' for register values
    */
    CAN_BTR_SJW(0) | CAN_BTR_BRP(5)  | CAN_BTR_TS1(5) | CAN_BTR_TS2(4),
};


#define SWAP_UINT16(x) (((x) << 8) | ((x) >> 8))

void SendSomething()
{
    auto baseAddress = 0x156;

    {
        CANTxFrame m_frame;

	    m_frame.IDE = CAN_IDE_STD;
	    m_frame.EID = 0;
	    m_frame.SID = baseAddress;
	    m_frame.RTR = CAN_RTR_DATA;
	    m_frame.DLC = 8;
	    memset(m_frame.data8, 0, sizeof(m_frame.data8));
	    m_frame.data8[3] = 0x33;
	    m_frame.data8[6] = 0x66;

    	canTransmitTimeout(&CAND1, CAN_ANY_MAILBOX, &m_frame, TIME_IMMEDIATE);
    }

}

static THD_WORKING_AREA(waCanTxThread, 256);
void CanTxThread(void*)
{
    while(1) {
        SendSomething();

        chThdSleepMilliseconds(10);
    }
}

static THD_WORKING_AREA(waCanRxThread, 256);
void CanRxThread(void*)
{
    while(1)
    {
        CANRxFrame frame;
        msg_t msg = canReceiveTimeout(&CAND1, CAN_ANY_MAILBOX, &frame, TIME_INFINITE);

        // Ignore non-ok results...
        if (msg != MSG_OK)
        {
            continue;
        }

        // Ignore std frames, only listen to ext
        if (frame.IDE != CAN_IDE_EXT)
        {
            continue;
        }

    }
}

void InitCan() {
    canStart(&CAND1, &canConfig500);
    // CAN TX
    palSetPadMode(GPIOA,12, PAL_MODE_STM32_ALTERNATE_PUSHPULL );
    // CAN RX
    palSetPadMode(GPIOA,11, PAL_MODE_INPUT_PULLUP );

    chThdCreateStatic(waCanTxThread, sizeof(waCanTxThread), NORMALPRIO, CanTxThread, nullptr);
    chThdCreateStatic(waCanRxThread, sizeof(waCanRxThread), NORMALPRIO - 4, CanRxThread, nullptr);
}
