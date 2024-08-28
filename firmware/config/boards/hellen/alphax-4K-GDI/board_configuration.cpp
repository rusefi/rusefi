
#include "pch.h"
#include "defaults.h"
#include "hellen_meta.h"
#include "drivers/gpio/tle9104.h"

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
	engineConfiguration->injectionPins[0] = Gpio::TLE9104_0_OUT_0;
	engineConfiguration->injectionPins[1] = Gpio::TLE9104_0_OUT_1;
	engineConfiguration->injectionPins[2] = Gpio::TLE9104_0_OUT_2;
	engineConfiguration->injectionPins[3] = Gpio::TLE9104_0_OUT_3;

	engineConfiguration->mainRelayPin = Gpio::TLE9104_1_OUT_0;
	engineConfiguration->fuelPumpPin = Gpio::TLE9104_1_OUT_1;
	engineConfiguration->fanPin = Gpio::TLE9104_1_OUT_2;


	engineConfiguration->map.sensor.hwChannel = H144_IN_MAP1;
	engineConfiguration->clt.adcChannel = H144_IN_CLT;
	engineConfiguration->iat.adcChannel = H144_IN_IAT;
	engineConfiguration->tps1_1AdcChannel = H144_IN_TPS;
	engineConfiguration->tps1_2AdcChannel = H144_IN_TPS2;
	setPPSInputs(H144_IN_PPS, H144_IN_PPS2);
}

static const tle9104_config tle9104_cfg[BOARD_TLE9104_COUNT] = {
	{
		.spi_bus = &SPID2,
		.spi_config = {
			.circular = false,
			.end_cb = nullptr,
			.ssport = GPIOD, // H144_GP_IO1
			.sspad = 4,
			.cr1 =
				SPI_CR1_16BIT_MODE |
				SPI_CR1_SSM |
				SPI_CR1_SSI |
				((3 << SPI_CR1_BR_Pos) & SPI_CR1_BR) |	// div = 16
				SPI_CR1_MSTR |
				SPI_CR1_CPHA |
				0,
			.cr2 = SPI_CR2_16BIT_MODE
		},
		.direct_io = {
			{ .port = GPIOD, .pad = 3 }, // H144_OUT_IO1
			{ .port = GPIOA, .pad = 9 }, // H144_OUT_IO2
			{ .port = GPIOG, .pad = 14 }, // H144_OUT_IO3
			{ .port = GPIOG, .pad = 5 } // H144_OUT_IO4
		},
		.resn = Gpio::Unassigned,
		.en   = Gpio::Unassigned
	},
	{
		.spi_bus = &SPID2,
		.spi_config = {
			.circular = false,
			.end_cb = nullptr,
			.ssport = GPIOD, // H144_GP_IO2
			.sspad = 7,
			.cr1 =
				SPI_CR1_16BIT_MODE |
				SPI_CR1_SSM |
				SPI_CR1_SSI |
				((3 << SPI_CR1_BR_Pos) & SPI_CR1_BR) |	// div = 16
				SPI_CR1_MSTR |
				SPI_CR1_CPHA |
				0,
			.cr2 = SPI_CR2_16BIT_MODE
		},
		.direct_io = {
			{ .port = GPIOD, .pad = 2 }, // H144_OUT_IO5
			{ .port = GPIOG, .pad = 11 }, // H144_OUT_IO6
			{ .port = GPIOG, .pad = 3 }, // H144_OUT_IO7
			{ .port = GPIOG, .pad = 4 } // H144_OUT_IO8
		},
		.resn = Gpio::Unassigned,
		.en   = Gpio::Unassigned
	},
	{
		.spi_bus = &SPID2,
		.spi_config = {
			.circular = false,
			.end_cb = nullptr,
			.ssport = GPIOG, // H144_GP_IO3
			.sspad = 10,
			.cr1 =
				SPI_CR1_16BIT_MODE |
				SPI_CR1_SSM |
				SPI_CR1_SSI |
				((3 << SPI_CR1_BR_Pos) & SPI_CR1_BR) |	// div = 16
				SPI_CR1_MSTR |
				SPI_CR1_CPHA |
				0,
			.cr2 = SPI_CR2_16BIT_MODE
		},
		.direct_io = {
			{ .port = GPIOG, .pad = 13 }, // H144_OUT_IO9
			{ .port = GPIOG, .pad = 12 }, // H144_OUT_IO10
			{ .port = GPIOG, .pad = 2 }, // H144_OUT_IO11
			{ .port = GPIOA, .pad = 8 } // H144_OUT_IO12
		},
		.resn = Gpio::Unassigned,
		.en   = Gpio::Unassigned
	},
   	{
   		.spi_bus = &SPID2,
   		.spi_config = {
   			.circular = false,
   			.end_cb = nullptr,
   			.ssport = GPIOG, // H144_GP_IO4
   			.sspad = 9,
   			.cr1 =
   				SPI_CR1_16BIT_MODE |
   				SPI_CR1_SSM |
   				SPI_CR1_SSI |
   				((3 << SPI_CR1_BR_Pos) & SPI_CR1_BR) |	// div = 16
   				SPI_CR1_MSTR |
   				SPI_CR1_CPHA |
   				0,
   			.cr2 = SPI_CR2_16BIT_MODE
   		},
   		.direct_io = {
   			{ .port = GPIOG, .pad = 6 }, // H144_OUT_IO13
   			{ .port = GPIOF, .pad = 15 }, // H144_GP_IO7
   			{ .port = nullptr, .pad = 0 }, // unused
   			{ .port = nullptr, .pad = 0 }  // unused
   		},
   		.resn = Gpio::Unassigned,
   		.en   = Gpio::Unassigned
   	}
	};

/*PUBLIC_API_WEAK*/ void boardInitHardware() {
  setHellenMegaEnPin();
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
	  static OutputPin TleEn;
	  TleEn.initPin("TLE9104 En", H144_UART8_RX);
	  TleEn.setValue(1);
	  }
	  {
	    static OutputPin TleReset;
	    TleReset.initPin("TLE9104 Reset", H144_UART8_TX);
	    TleReset.setValue(1);
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

  initAll9104(tle9104_cfg);
}

static Gpio OUTPUTS[] = {
	Gpio::TLE9104_0_OUT_0, // 4B Port injector 1
	Gpio::TLE9104_0_OUT_1, // 3B Port injector 2
	Gpio::TLE9104_0_OUT_2, // 2B Port injector 3
	Gpio::TLE9104_0_OUT_3, // 1B Port injector 4
	Gpio::TLE9104_1_OUT_0, // 5B Main Relay Control
	Gpio::TLE9104_1_OUT_1, // 7B Fuel Pump
	Gpio::TLE9104_1_OUT_2, // 8B Fan 1
	Gpio::TLE9104_1_OUT_3, // 9B Fan 2
	Gpio::TLE9104_2_OUT_0, // 29B VVT1
	Gpio::TLE9104_2_OUT_1, // 30B VVT2
	Gpio::TLE9104_2_OUT_2, // 31B VVT3
	Gpio::TLE9104_2_OUT_3, // 32B VVT4
	Gpio::TLE9104_3_OUT_0,
	Gpio::TLE9104_3_OUT_1,
};

int getBoardMetaOutputsCount() {
    return efi::size(OUTPUTS);
}

Gpio* getBoardMetaOutputs() {
    return OUTPUTS;
}

int getBoardMetaDcOutputsCount() {
    return 1;
}
