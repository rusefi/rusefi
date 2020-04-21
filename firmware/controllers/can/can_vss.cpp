/**
 * @file	can_vss.cpp
 *
 * This file handles incomming vss values from can.
 *
 * @date Apr 19, 2020
 * @author Alex Miculescu, (c) 2020
 */

#include "globalaccess.h"
#if EFI_CAN_SUPPORT
#include "engine_configuration.h"
#include "engine.h"
#include "can.h"
#include "vehicle_speed.h"

EXTERN_ENGINE;

static Logging *logger;
static bool isInit = false;
static uint16_t filterCanID = 0;
static efitick_t frameTime;
static float vssSpeed = 0;
/* keep 16 bit address since till now we do not have 28bit address request */
uint32_t canIDs[] = { 0x01F0 /* BMW e46 ABS Message        */,
                      0x0200 /* W202 C180 ABS signal       */
                    };

#define look_up_can_id(X)   canIDs[X]

/* Module specitifc processing functions */
/* source: http://z4evconversion.blogspot.com/2016/07/completely-forgot-but-it-does-live-on.html */
void processBMW_e46(const CANRxFrame& frame) {
    
    uint16_t tmp;
    
    /* left front wheel speed is used here */
    tmp = ((frame.data8[1] & 0x0f) << 8 );
    tmp |= frame.data8[0];

    vssSpeed = tmp / 16;


}

void processW202(const CANRxFrame& frame) {
}

/* End of specific processing functions */

void canVssInfo(void) {
    scheduleMsg(logger, "vss using can option selected %x", CONFIG(canVssNbcType) );
}

void processCanRxVss(const CANRxFrame& frame, efitick_t nowNt) {
    if (( CONFIG(enableCanVss) == false ) || (isInit == false )) {
        return;
    }

    //filter it we need to process the can message or not
    if ( frame.SID != filterCanID ) {
        return;
    }

    frameTime = nowNt;
    switch ( CONFIG(canVssNbcType) ){
        case BMW_e46:
            processBMW_e46(frame);
            break;
        case W202:
            processW202(frame);
            break;
        default:
            scheduleMsg(logger, "vss unsupported can option selected %x", CONFIG(canVssNbcType) );
            break;
    }

}

float getVehicleCanSpeed(void) {

    efitick_t nowNt = getTimeNowNt();

    if ( (nowNt - frameTime ) > NT_PER_SECOND ) {
        return 0; /* can timeout? */
    } else {
        return vssSpeed;
    }
}

void initCanVssSupport(Logging *l) {

    if( CONFIG(enableCanVss) == true ) {
        logger = l;
        isInit = true;
        filterCanID = look_up_can_id(CONFIG(canVssNbcType));
    }
    
}

void setCanVss(int type) {
	engineConfiguration->canVssNbcType = (can_vss_nbc_e)type;
	canVssInfo();
}

#endif // EFI_CAN_SUPPORT