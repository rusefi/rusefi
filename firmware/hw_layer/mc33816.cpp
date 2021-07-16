/*
 * @file mc33816.cpp
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

#include "global.h"

#if EFI_MC33816

#include "mc33816.h"
#include "mc33816_memory_map.h"
#include "engine.h"
#include "efi_gpio.h"
#include "pin_repository.h"
#include "hardware.h"
#include "mc33816_data.h"
#include "mpu_util.h"
#include "allsensors.h"

static bool isInitializaed = false;

static OutputPin chipSelect;
static OutputPin resetB;
static OutputPin driven;

static bool flag0before = false;
static bool flag0after = false;

static unsigned short mcChipId;
static unsigned short mcDriverStatus;

static SPIConfig spiCfg = { .circular = false,
		.end_cb = NULL,
		.ssport = NULL,
		.sspad = 0,
		.cr1 =
				SPI_CR1_16BIT_MODE |
				SPI_CR1_MSTR |
//SPI_CR1_BR_1 // 5MHz
		SPI_CR1_CPHA | SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2 | SPI_CR1_SPE,
		.cr2 = SPI_CR2_SSOE};

static SPIDriver *driver;

static bool validateChipId() {
	return (mcChipId  >> 8) == 0x9D;
}

static void showStats() {
	if (!isInitializaed) {
		efiPrintf("WAITINIG FOR VBatt...");
	}
	// x9D is product code or something, and 43 is the revision?
	efiPrintf("MC 0x%x %s", mcChipId, validateChipId() ? "hooray!" : "not hooray :(");

    if (isBrainPinValid(CONFIG(mc33816_flag0))) {
    	efiPrintf("flag0 before %d after %d", flag0before, flag0after);

    	efiPrintf("flag0 right now %d", efiReadPin(CONFIG(mc33816_flag0)));

    } else {
    	efiPrintf("No flag0 pin selected");
    }
    efiPrintf("MC voltage %d", CONFIG(mc33_hvolt));
    efiPrintf("MC driver status 0x%x", mcDriverStatus);
}

static void mcRestart();


// Receive 16bits
unsigned short recv_16bit_spi() {
	return spiPolledExchange(driver, 0xFFFF);
}

// This could be used to detect if check byte is wrong.. or use a FLAG after init
unsigned short txrx_16bit_spi(const unsigned short param) {
	return spiPolledExchange(driver, param);
}

// Send 16bits
static void spi_writew(unsigned short param) {
	//spiSelect(driver);
	spiPolledExchange(driver, param);
	//spiUnselect(driver);
}

static void setup_spi() {
	spiSelect(driver);
	// Select Channel command
	spi_writew(0x7FE1);
    // Common Page
	spi_writew(0x0004);


	// Configure SPI command
	spi_writew(0x3901);
	// Mode A + Watchdog timer full
    //spi_writew(0x001F);
	spi_writew(0x009F); // + fast slew rate on miso
	spiUnselect(driver);
}

static unsigned short readId() {
	spiSelect(driver);
	spi_writew(0xBAA1);
	unsigned short ID =  recv_16bit_spi();
	spiUnselect(driver);
	return ID;
}

// Read a single word in Data RAM
unsigned short mcReadDram(MC33816Mem addr) {
	unsigned short readValue;
	spiSelect(driver);
	// Select Channel command, Common Page
    spi_writew(0x7FE1);
    spi_writew(0x0004);
    // read (MSB=1) at data ram x9 (SCV_I_Hold), and 1 word
    spi_writew((0x8000 | addr << 5) + 1);
    readValue = recv_16bit_spi();

    spiUnselect(driver);
    return readValue;
}

// Update a single word in Data RAM
void mcUpdateDram(MC33816Mem addr, unsigned short data) {
	spiSelect(driver);
	// Select Channel command, Common Page
    spi_writew(0x7FE1);
    spi_writew(0x0004);
    // write (MSB=0) at data ram x9 (SCV_I_Hold), and 1 word
    spi_writew((addr << 5) + 1);
    spi_writew(data);

    spiUnselect(driver);
}

static short dacEquation(unsigned short current) {
	/*
	Current, given in mA->A
	I = (DAC_VALUE * V_DAC_LSB - V_DA_BIAS)/(G_DA_DIFF * R_SENSEx)
	DAC_VALUE = ((I*G_DA_DIFF * R_SENSEx) + V_DA_BIAS) /  V_DAC_LSB
	V_DAC_LSB is the DAC resolution = 9.77mv
	V_DA_BIAS = 250mV
	G_DA_DIFF = Gain: 5.79, 8.68, [12.53], 19.25
	R_SENSE = 10mOhm soldered on board
	*/
	return (short)(((current/1000.0f * 12.53f * 10) + 250.0f) / 9.77f);
}

static void setTimings() {

	// Convert mA to DAC values
	mcUpdateDram(MC33816Mem::Iboost, dacEquation(CONFIG(mc33_i_boost)));
	mcUpdateDram(MC33816Mem::Ipeak, dacEquation(CONFIG(mc33_i_peak)));
	mcUpdateDram(MC33816Mem::Ihold, dacEquation(CONFIG(mc33_i_hold)));

	// uint16_t mc33_t_max_boost; // not yet implemented in microcode

	// in micro seconds to clock cycles
	mcUpdateDram(MC33816Mem::Tpeak_off, (MC_CK * CONFIG(mc33_t_peak_off)));
	mcUpdateDram(MC33816Mem::Tpeak_tot, (MC_CK * CONFIG(mc33_t_peak_tot)));
	mcUpdateDram(MC33816Mem::Tbypass, (MC_CK * CONFIG(mc33_t_bypass)));
	mcUpdateDram(MC33816Mem::Thold_off, (MC_CK * CONFIG(mc33_t_hold_off)));
	mcUpdateDram(MC33816Mem::Thold_tot, (MC_CK * CONFIG(mc33_t_hold_tot)));
}

void setBoostVoltage(float volts)
{
	// Sanity checks, Datasheet says not too high, nor too low
	if (volts > 65.0f) {
		firmwareError(OBD_PCM_Processor_Fault, "DI Boost voltage setpoint too high: %.1f", volts);
		return;
	}
	if (volts < 10.0f) {
		firmwareError(OBD_PCM_Processor_Fault, "DI Boost voltage setpoint too low: %.1f", volts);
		return;
	}
	// There's a 1/32 divider on the input, then the DAC's output is 9.77mV per LSB.  (1 / 32) / 0.00977 = 3.199 counts per volt.
	unsigned short data = volts * 3.2;
	mcUpdateDram(MC33816Mem::Vboost_high, data+1);
	mcUpdateDram(MC33816Mem::Vboost_low, data-1);
	// Remember to strobe driven!!
}

static bool check_flash() {
	spiSelect(driver);

	// ch1
	// read (MSB=1) at location, and 1 word
    spi_writew((0x8000 | 0x100 << 5) + 1);
    if (!(recv_16bit_spi() & (1<<5))) {
    	spiUnselect(driver);
    	return false;
    }

    // ch2
	// read (MSB=1) at location, and 1 word
    spi_writew((0x8000 | 0x120 << 5) + 1);

    if (!(recv_16bit_spi() & (1<<5))) {
    	spiUnselect(driver);
    	return false;
    }

    spiUnselect(driver);
	return true;
}

static void mcClearDriverStatus(){
	// Note: There is a config at 0x1CE & 1 that can reset this status config register on read
	// otherwise the reload/recheck occurs with this write
	// resetting it is necessary to clear default reset behavoir, as well as if an issue has been resolved
	setup_spi(); // ensure on common page?
	spiSelect(driver);
	spi_writew((0x0000 | 0x1D2 << 5) + 1); // write, location, one word
	spi_writew(0x0000); // anything to clear
	spiUnselect(driver);
}

static unsigned short readDriverStatus(){
	unsigned short driverStatus;
	setup_spi(); // ensure on common page?
	spiSelect(driver);
    	spi_writew((0x8000 | 0x1D2 << 5) + 1);
    	driverStatus = recv_16bit_spi();
	spiUnselect(driver);
	return driverStatus;
}

static bool checkUndervoltVccP(unsigned short driverStatus){
	return (driverStatus  & (1<<0));
}

static bool checkUndervoltV5(unsigned short driverStatus){
	return (driverStatus  & (1<<1));
}

static bool checkOverTemp(unsigned short driverStatus){
	return (driverStatus  & (1<<3));
}

static bool checkDrivenEnabled(unsigned short driverStatus){
	return (driverStatus  & (1<<4));
}

static void enable_flash() {
	spiSelect(driver);
    spi_writew(0x2001); //ch1
    spi_writew(0x0018); //enable flash
    spi_writew(0x2401); //ch2
    spi_writew(0x0018); // enable flash
    spiUnselect(driver);
}

static void download_RAM(int target) {
   uint16_t memory_area = 0;         // memory area
   uint16_t start_address = 0;      // start address
   uint16_t codeWidthRegAddr = 0;   // code width register address
   uint16_t size = 0;               // size of RAM data
   uint16_t command = 0;            // command data
   const uint16_t *RAM_ptr;               // pointer to array of data to be sent to the chip


   //Why Again? For Every time, just in case?
   setup_spi();

   switch(target)            // selects target
   {
   case CODE_RAM1:
      memory_area = 0x1;
      start_address = 0;
      codeWidthRegAddr = 0x107;
      RAM_ptr = MC33816_code_RAM1;
      size = sizeof(MC33816_code_RAM1) / 2;
      break;

   case CODE_RAM2:
      memory_area = 0x2;
      start_address = 0;
      codeWidthRegAddr = 0x127;
      RAM_ptr = MC33816_code_RAM2;
      size = sizeof(MC33816_code_RAM2) / 2;
      break;

   case DATA_RAM: // ch1 only?
      memory_area = 0x4;
      start_address = 0;
      RAM_ptr = MC33816_data_RAM;
      size = sizeof(MC33816_data_RAM) / 2;
      break;
// optional, both data_rams with 0x3, writes same code to both
   default:
      break;
   }

   // Chip-Select high
   spiSelect(driver);

   if (target != DATA_RAM)
   {
	   command = codeWidthRegAddr << 5;   // control width register address
	   command |= 1;                      // number of words to follow
	   spi_writew(command);           // sends code_width command
   	   spi_writew(size);              // sends size (Code Width)
   }

   // Select Channel command
   spi_writew(0x7FE1);
   // RAM1, RAM2, or Common Page (Data RAM)
   spi_writew(memory_area);

   // "Command" of starting address
   // up to 0x03FE of code ram
   // up to 0x0080 of data ram
   command = start_address << 5;
   spi_writew(command);           // sends start address command

   spiSend(driver, size, RAM_ptr);
   spiUnselect(driver);
}

static void download_register(int r_target) {
	   uint16_t r_start_address = 0;  // start address
	   uint16_t r_size = 0;           // size of configuration data
	   uint16_t r_command = 0;        // command data
	   uint16_t remainder_size = 0;   // remainder size
	   const uint16_t *reg_ptr;            // pointer to array of data to be sent to the chip

	   switch(r_target)                     // selects target
	   {
	   case REG_CH1:                              // channel 1 configurations
	      r_start_address = 0x100;
	      reg_ptr = MC33816_ch1_config;
	      r_size = sizeof(MC33816_ch1_config) / 2;  // gets number of words to be sent
	      break;

	   case REG_CH2:                              // channel 2 configurations
	      r_start_address = 0x120;
	      reg_ptr = MC33816_ch2_config;
	      r_size = sizeof(MC33816_ch2_config) / 2;  // gets number of words to be sent
	      break;

	   case REG_DIAG:                              // diagnostic configurations
	      r_start_address = 0x140;
	      reg_ptr = MC33816_diag_config;
	      r_size = sizeof(MC33816_diag_config) / 2; // gets number of words to be sent
	      break;

	   case REG_IO:                              // IO configurations
	      r_start_address = 0x180;
	      reg_ptr = MC33816_io_config;
	      r_size = sizeof(MC33816_io_config) / 2;   // gets number of words to be sent
	      break;

	   case REG_MAIN:                              // main configurations
	      r_start_address = 0x1C0;
	      reg_ptr = MC33816_main_config;
	      r_size = sizeof(MC33816_main_config) / 2; // gets number of words to be sent
	      break;

	   default:
	      break;
	   }

	   //for location < size(remainder?)
	   // is location == 0? or past max xfer, send command + expected size
	   // if location = max xfer
	   //
	   // retrieve data, send it, increase pointer
	   // increase

	   if (r_size > MAX_SPI_MODE_A_TRANSFER_SIZE)   //if size is too large, split into two sections ... MULTIPLE sections..
	   {
	      remainder_size = r_size - MAX_SPI_MODE_A_TRANSFER_SIZE;  // creates remaining size
	      r_size = MAX_SPI_MODE_A_TRANSFER_SIZE;                   // sets first size
	   }

	   r_command = r_start_address << 5;      // start address
	   r_command += r_size;                   // number of words to follow

	   spiSelect(driver);						// Chip

	   spi_writew(r_command);             // sends address and number of words to be sent

	   spiSend(driver, r_size, reg_ptr);

	   if (remainder_size > 0)                 // if remainder size is greater than 0, download the rest
	   {
	      r_start_address += r_size;          // new start address
	      r_command = r_start_address << 5;   // start address
	      r_command += remainder_size;        // number of words to follow

	      spi_writew(r_command);          // sends address and number of words to be sent
	      spiSend(driver, remainder_size, reg_ptr + r_size);
	   }
	   spiUnselect(driver);
}

static bool haveMc33816 = false;

void initMc33816() {
	//
	// see setTest33816EngineConfiguration for default configuration
	// Pins
	if (!isBrainPinValid(CONFIG(mc33816_cs)) ||
		!isBrainPinValid(CONFIG(mc33816_rstb)) ||
		!isBrainPinValid(CONFIG(mc33816_driven))) {
		return;
	}
	if (isBrainPinValid(CONFIG(mc33816_flag0))) {
		efiSetPadMode("mc33816 flag0", CONFIG(mc33816_flag0), getInputMode(PI_DEFAULT));
	}

	chipSelect.initPin("mc33 CS", engineConfiguration->mc33816_cs /*, &engineConfiguration->csPinMode*/);

	// Initialize the chip via ResetB
	resetB.initPin("mc33 RESTB", engineConfiguration->mc33816_rstb);
	// High Voltage via DRIVEN
	driven.initPin("mc33 DRIVEN", engineConfiguration->mc33816_driven);


	spiCfg.ssport = getHwPort("hip", CONFIG(mc33816_cs));
	spiCfg.sspad = getHwPin("hip", CONFIG(mc33816_cs));

	// hard-coded for now, just resolve the conflict with SD card!
	engineConfiguration->mc33816spiDevice = SPI_DEVICE_3;

	driver = getSpiDevice(engineConfiguration->mc33816spiDevice);
	if (driver == NULL) {
		// error already reported
		return;
	}

	spiStart(driver, &spiCfg);

	addConsoleAction("mc33_stats", showStats);
	addConsoleAction("mc33_restart", mcRestart);
	//addConsoleActionI("mc33_send", sendWord);

	haveMc33816 = true;
	mcRestart();
}

static void mcShutdown() {
	driven.setValue(0); // ensure HV is off
	resetB.setValue(0); // turn off the chip
}

static void mcRestart() {
	flag0before = false;
	flag0after = false;

	efiPrintf("MC Restart");
	showStats();

	driven.setValue(0); // ensure driven is off

	if (Sensor::get(SensorType::BatteryVoltage).value_or(VBAT_FALLBACK_VALUE) < LOW_VBATT) {
		efiPrintf("GDI not Restarting until we see VBatt");
		return;
	}

	// Does starting turn this high to begin with??
	spiUnselect(driver);

	//delay/wait? .. possibly only 30us for each needed, per datasheet
	resetB.setValue(0);
	chThdSleepMilliseconds(10);
	resetB.setValue(1);
	chThdSleepMilliseconds(10);
    if (isBrainPinValid(CONFIG(mc33816_flag0))) {
   		flag0before = efiReadPin(CONFIG(mc33816_flag0));
    }


	setup_spi();

	mcClearDriverStatus(); // Initial clear necessary
    mcDriverStatus = readDriverStatus();
    if (checkUndervoltV5(mcDriverStatus)) {
    	firmwareError(OBD_PCM_Processor_Fault, "MC33 5V Under-Voltage!");
    	mcShutdown();
    	return;
    }

	mcChipId = readId();
	if (!validateChipId()) {
		firmwareError(OBD_PCM_Processor_Fault, "No comm with MC33");
		mcShutdown();
		return;
	}

    download_RAM(CODE_RAM1);        // transfers code RAM1
    download_RAM(CODE_RAM2);        // transfers code RAM2
    download_RAM(DATA_RAM);         // transfers data RAM
    /**
     * current configuration of REG_MAIN would toggle flag0 from LOW to HIGH
     */
    download_register(REG_MAIN);    // download main register configurations
    if (isBrainPinValid(CONFIG(mc33816_flag0))) {
   		flag0after = efiReadPin(CONFIG(mc33816_flag0));
   		if (flag0before || !flag0after) {
   			firmwareError(OBD_PCM_Processor_Fault, "MC33 flag0 transition no buena");
   			mcShutdown();
   			return;
   		}
    }

    download_register(REG_CH1);     // download channel 1 register configurations
    download_register(REG_CH2);     // download channel 2 register configurations
    download_register(REG_IO);      // download IO register configurations
    download_register(REG_DIAG);    // download diag register configuration

    setTimings();

    // Finished downloading, let's run the code
    enable_flash();
    if (!check_flash()) {
    	firmwareError(OBD_PCM_Processor_Fault, "MC33 no flash");
    	mcShutdown();
    	return;
    }

    mcDriverStatus = readDriverStatus();
    if (checkUndervoltVccP(mcDriverStatus)) {
    	firmwareError(OBD_PCM_Processor_Fault, "MC33 VccP (7V) Under-Voltage!");
    	mcShutdown();
    	return;
    }

    // Drive High Voltage if possible
    setBoostVoltage(CONFIG(mc33_hvolt));
    driven.setValue(1); // driven = HV
    chThdSleepMilliseconds(10); // Give it a moment
    mcDriverStatus = readDriverStatus();
    if (!checkDrivenEnabled(mcDriverStatus)) {
    	firmwareError(OBD_PCM_Processor_Fault, "MC33 Driven did not stick!");
    	mcShutdown();
    	return;
    }

    mcDriverStatus = readDriverStatus();
    if (checkUndervoltVccP(mcDriverStatus)) {
    	firmwareError(OBD_PCM_Processor_Fault, "MC33 VccP Under-Voltage After Driven"); // Likely DC-DC LS7 is dead!
    	mcShutdown();
    	return;
    }
}

void initMc33816IfNeeded() {
	if (!haveMc33816) {
		return;
	}
	if (Sensor::get(SensorType::BatteryVoltage).value_or(VBAT_FALLBACK_VALUE) < LOW_VBATT) {
		isInitializaed = false;
	} else {
		if (!isInitializaed) {
			mcRestart();
			isInitializaed = true;
		}
	}
}

#endif /* EFI_MC33816 */
