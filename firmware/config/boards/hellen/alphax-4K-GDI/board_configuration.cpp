
#include "pch.h"
#include "defaults.h"
#include "hellen_meta.h"

void setBoardConfigOverrides() {
    setHellenSdCardSpi1Hardware();
	setHellenVbatt();
    enableHellenSpi2();
    setupTLE9201(/*PWM controlPin*/Gpio::H144_OUT_PWM2, Gpio::H144_OUT_PWM3, Gpio::H144_OUT_PWM1);
	setHellenCan();
	setHellenMMbaro();
	hellenWbo();
	setDefaultHellenAtPullUps();


  engineConfiguration->mc33816spiDevice = SPI_DEVICE_2;
	engineConfiguration->mc33816_cs = H_SPI2_CS;
	engineConfiguration->mc33816_rstb = Gpio::H144_LS_8;
	engineConfiguration->mc33816_driven = Gpio::H144_LS_7;
	engineConfiguration->mc33816_flag0 = Gpio::H144_IN_RES3;
}

void setBoardDefaultConfiguration() {
	engineConfiguration->injectionPins[0] = Gpio::H144_LS_1;
	engineConfiguration->injectionPins[1] = Gpio::H144_LS_2;
	engineConfiguration->injectionPins[2] = Gpio::H144_LS_3;
	engineConfiguration->injectionPins[3] = Gpio::H144_LS_4;

	engineConfiguration->clt.adcChannel = H144_IN_CLT;
	engineConfiguration->iat.adcChannel = H144_IN_IAT;
	engineConfiguration->tps1_1AdcChannel = H144_IN_TPS;
	engineConfiguration->tps1_2AdcChannel = H144_IN_TPS2;
	setPPSInputs(H144_IN_PPS, H144_IN_PPS2);
}

/*PUBLIC_API_WEAK*/ void boardInitHardware() {
    {
    	static OutputPin csLs1;
	    csLs1.initPin("csLs1", Gpio::H144_GP_IO1);
	    csLs1.setValue(1);
    }
    {
    	static OutputPin csLs2;
	    csLs2.initPin("csLs2", Gpio::H144_GP_IO2);
	    csLs2.setValue(1);
    }
    {
    	static OutputPin csLs3;
	    csLs3.initPin("csLs3", Gpio::H144_GP_IO3);
	    csLs3.setValue(1);
    }
    {
    	static OutputPin csLs4;
	    csLs4.initPin("csLs4", Gpio::H144_GP_IO4);
	    csLs4.setValue(1);
    }
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
