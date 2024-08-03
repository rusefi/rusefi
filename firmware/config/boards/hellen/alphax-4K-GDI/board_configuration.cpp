
#include "pch.h"
#include "defaults.h"
#include "hellen_meta.h"

void setBoardConfigOverrides() {
    setHellenSdCardSpi1Hardware();
	setHellenVbatt();
    setupTLE9201(/*PWM controlPin*/Gpio::H144_OUT_PWM2, Gpio::H144_OUT_PWM3, Gpio::H144_OUT_PWM1);
	setHellenCan();
	setHellenMMbaro();
	hellenWbo();
	setDefaultHellenAtPullUps();
}

void setBoardDefaultConfiguration() {

	engineConfiguration->clt.adcChannel = H144_IN_CLT;
	engineConfiguration->iat.adcChannel = H144_IN_IAT;
	engineConfiguration->tps1_1AdcChannel = H144_IN_TPS;
	engineConfiguration->tps1_2AdcChannel = H144_IN_TPS2;
	setPPSInputs(H144_IN_PPS, H144_IN_PPS2);
}

/*PUBLIC_API_WEAK*/ void boardInitHardware() {
    {
    	static OutputPin csDcEtb;
	    csDcEtb.initPin("cs-dc-etb", Gpio::H144_GP_IO5);
	    csDcEtb.setValue(1);
    }
    {
	    static OutputPin csDcAux;
	    csDcAux.initPin("cs-dc-aux", Gpio::H144_GP_IO6);
	    csDcAux.setValue(1);
    }

}
