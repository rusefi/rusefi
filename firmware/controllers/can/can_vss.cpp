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
#include "can.h"
#include "vehicle_speed.h"
#include "dynoview.h"

static bool isInit = false;
static uint16_t filterCanID = 0;
static efitick_t frameTime;
static float vssSpeed = 0;


uint16_t look_up_can_id(can_vss_nbc_e type) {
    
    uint16_t retCanID;
    switch (type) {
        case BMW_e46:
            retCanID = 0x01F0; /* BMW e46 ABS Message */
            break;
        case W202:
            retCanID = 0x0200; /* W202 C180 ABS signal */
            break;
        default:
            firmwareError(OBD_Vehicle_Speed_SensorB, "Wrong Can DBC selected: %d", type);
            retCanID = 0xffff;
            break;
    }
    return retCanID;
}


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

    uint16_t tmp;
    
    tmp = (frame.data8[2] << 8);
    tmp |= frame.data8[3];
    vssSpeed = ((float)tmp) * 0.0625;
}

/* End of specific processing functions */

void canVssInfo(void) {
    efiPrintf("vss using can option selected %x", CONFIG(canVssNbcType));
    efiPrintf("vss filter for %x canID", filterCanID);
    efiPrintf("Vss module is %d", isInit);
    efiPrintf("CONFIG_enableCanVss is %d", CONFIG(enableCanVss));
}

void processCanRxVss(const CANRxFrame& frame, efitick_t nowNt) {
    if ((!CONFIG(enableCanVss)) || (!isInit)) {
        return;
    }

    //filter it we need to process the can message or not
    if (CAN_SID(frame) != filterCanID ) {
        return;
    }

    frameTime = nowNt;
    switch (CONFIG(canVssNbcType)){
        case BMW_e46:
            processBMW_e46(frame);
            break;
        case W202:
            processW202(frame);
            break;
        default:
            efiPrintf("vss unsupported can option selected %x", CONFIG(canVssNbcType) );
            break;
    }

#if EFI_DYNO_VIEW
    updateDynoViewCan(PASS_ENGINE_PARAMETER_SIGNATURE);
#endif
}

float getVehicleCanSpeed(void) {

    efitick_t nowNt = getTimeNowNt();

    if ((nowNt - frameTime ) > NT_PER_SECOND) {
        return 0; /* can timeout? */
    } else {
        return vssSpeed;
    }
}

void initCanVssSupport() {
    addConsoleAction("canvssinfo", canVssInfo);

    if (CONFIG(enableCanVss)) {

        isInit = true;
        filterCanID = look_up_can_id(CONFIG(canVssNbcType));

        if (filterCanID == 0xffff) {
            isInit = false;
        }
    }
    
}

void setCanVss(int type) {
	engineConfiguration->canVssNbcType = (can_vss_nbc_e)type;
	canVssInfo();
}

#endif // EFI_CAN_SUPPORT