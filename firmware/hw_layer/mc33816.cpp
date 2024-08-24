/*
 * @file mc33816.cpp
 *
 * TL,DR: GDI
 *
 * The NXP MC33816 is a programmable gate driver IC for precision solenoid control applications.
 *
 *
 * Useful wires:
 * 5v, 3(3.3v), Gnd, 12v, VccIO(3v) SPI, DRVEN, RSTB
 *
 * For MC33816 vs PT2000 differences see
 * https://www.nxp.com/docs/en/application-note/AN5203.pdf
 *
 * @date May 3, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#if EFI_MC33816

#include "hardware.h"
#include "mpu_util.h"
#include "ignition_controller.h"

static SPIConfig spiCfg = {
    .circular = false,
#ifdef _CHIBIOS_RT_CONF_VER_6_1_
	.end_cb = NULL,
#else
        .slave = false,
        .data_cb = NULL,
        .error_cb = NULL,
#endif
		.ssport = nullptr,
		.sspad = 0,
		.cr1 =
			SPI_CR1_MSTR |
			SPI_CR1_SSM | // Software Slave Management, the SSI bit will be internal reference
			SPI_CR1_SSI | // Internal Slave Select (active low) set High
			SPI_CR1_CPHA |
			//SPI_CR1_BR_1 // 5MHz
			SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2 |
			SPI_CR1_SPE,
		.cr2 = SPI_CR2_SSOE |
			SPI_CR2_16BIT_MODE
		 };

class Pt2001 : public Pt2001Base {
public:
	void init();
	void initIfNeeded();

protected:
	void select() override {
// revive MC33816 driver, also support bus sharing #6781
// should be somewhere but not here 	  spiStart(driver, &spiCfg);
  efiPrintf("mc select %s", hwOnChipPhysicalPinName(driver->config->ssport, driver->config->sspad));
		spiSelect(driver);
		//chris - "no implementation on stm32"
		chipSelect.setValue(0);
	}

	void deselect() override {
		spiUnselect(driver);
		//chris - "no implementation on stm32"
		chipSelect.setValue(1);
	}

	uint16_t sendRecv(uint16_t tx) override {
		return spiPolledExchange(driver, tx);
	}

	// Send `count` number of 16 bit words from `data`
	void sendLarge(const uint16_t* data, size_t count) override {
		spiSend(driver, count, data);
	}

	// GPIO reset and enable pins
	void setResetB(bool state) override {
		resetB.setValue(state);
	}

	void setDriveEN(bool state) override {
		driven.setValue(state);
	}

	// GPIO inputs for various pins we need
	bool readFlag0() const override {
		return efiReadPin(engineConfiguration->mc33816_flag0);
	}

	// Get battery voltage - only try to init chip when powered
	float getVbatt() const override {
	  // reminder that we do not have sensor subsystem right away
		return Sensor::get(SensorType::BatteryVoltage).value_or(0);
	}

	// CONFIGURATIONS: currents, timings, voltages
	float getBoostVoltage() const override {
		return engineConfiguration->mc33_hvolt;
	}

	// Currents in amps
	float getBoostCurrent() const override {
		return engineConfiguration->mc33_i_boost;
	}

	float getPeakCurrent() const override {
		return engineConfiguration->mc33_i_peak;
	}

	float getHoldCurrent() const override {
		return engineConfiguration->mc33_i_hold;
	}

	float getPumpPeakCurrent() const override {
		return engineConfiguration->mc33_hpfp_i_peak;
	}

	float getPumpHoldCurrent() const override {
		return engineConfiguration->mc33_hpfp_i_hold;
	}

	// Timings in microseconds
	uint16_t getTpeakOff() const override {
		return engineConfiguration->mc33_t_peak_off;
	}

	uint16_t getTpeakTot() const override {
		return engineConfiguration->mc33_t_peak_tot;
	}

	uint16_t getTbypass() const override {
		return engineConfiguration->mc33_t_bypass;
	}

	uint16_t getTholdOff() const override {
		return engineConfiguration->mc33_t_hold_off;
	}

	uint16_t getTHoldTot() const override {
		return engineConfiguration->mc33_t_hold_tot;
	}

	uint16_t getTBoostMin() const override {
		return engineConfiguration->mc33_t_min_boost;
	}

	uint16_t getTBoostMax() const override {
		return engineConfiguration->mc33_t_max_boost;
	}

	uint16_t getPumpTholdOff() const override {
		return engineConfiguration->mc33_hpfp_i_hold_off;
	}

	uint16_t getPumpTholdTot() const override {
		return engineConfiguration->mc33_hpfp_max_hold;
	}

	// Print out an error message
	void onError(const char* why) override {
		efiPrintf("PT2001 error: %s", why);
	}

  bool errorOnUnexpectedFlag() override {
    efiPrintf("****** unexpected mc33 flag state ******");
    return false;
  }

	void sleepMs(size_t ms) override {
		chThdSleepMilliseconds(ms);
	}

private:
	SPIDriver* driver;

	OutputPin resetB;
	OutputPin driven;
	OutputPin chipSelect;
};

static Pt2001 pt;

void Pt2001::init() {
	//
	// see setTest33816EngineConfiguration for default configuration
	// Pins
	if (!isBrainPinValid(engineConfiguration->mc33816_cs) ||
		!isBrainPinValid(engineConfiguration->mc33816_rstb) ||
		!isBrainPinValid(engineConfiguration->mc33816_driven)) {
		return;
	}
	if (isBrainPinValid(engineConfiguration->mc33816_flag0)) {
		efiSetPadMode("mc33816 flag0", engineConfiguration->mc33816_flag0, getInputMode(PI_DEFAULT));
	}

	chipSelect.initPin("mc33 CS", engineConfiguration->mc33816_cs /*, &engineConfiguration->csPinMode*/);
	chipSelect.setValue(1);

	// Initialize the chip via ResetB
	resetB.initPin("mc33 RESTB", engineConfiguration->mc33816_rstb);
	// High Voltage via DRIVEN
	driven.initPin("mc33 DRIVEN", engineConfiguration->mc33816_driven);

	spiCfg.ssport = getHwPort("mc33816", engineConfiguration->mc33816_cs);
	spiCfg.sspad = getHwPin("mc33816", engineConfiguration->mc33816_cs);
	// huh, looks like we never set AF for any CS _anywhere_? are we supposed to?
	palSetPadMode(spiCfg.ssport, spiCfg.sspad, PAL_MODE_ALTERNATE(5/* << magic for SPI2. todo make visible getSpiAf(driver)*/) | PAL_STM32_OSPEED_HIGHEST);

#if HW_MICRO_RUSEFI
	// hard-coded for now, just resolve the conflict with SD card!
	engineConfiguration->mc33816spiDevice = SPI_DEVICE_3;
#endif

  if (engineConfiguration->mc33816spiDevice == SPI_NONE) {
    criticalError("mc33816 needs SPI selection");
  }

	driver = getSpiDevice(engineConfiguration->mc33816spiDevice);
	if (driver == nullptr) {
		// error already reported
		return;
	}

	spiStart(driver, &spiCfg);
	efiPrintf("mc33 started SPI");

	// addConsoleAction("mc33_stats", showStats);
	addConsoleAction("mc33_restart", [](){
    pt.initIfNeeded();
  });

	// todo: too soon to read voltage, it has to fail, right?!
	initIfNeeded();
}

static bool isInitialized = false;

void Pt2001::initIfNeeded() {
	// chris
	//if (!isIgnVoltage()) {
	//	isInitialized = false;
	//  efiPrintf("unhappy mc33 due to battery voltage");
	//} else {
		if (!isInitialized) {
			isInitialized = restart();
			if (isInitialized) {
			  efiPrintf("happy mc33/PT2001!");
			} else {
			  efiPrintf("unhappy mc33 fault=%d", (int)fault);
			}
		}
	//}
}

void initMc33816() {
	pt.init();
}

#endif /* EFI_MC33816 */
