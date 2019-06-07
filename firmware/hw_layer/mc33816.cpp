/*
 * @file mc33816.cpp
 *
 * The NXP MC33816 is a programmable gate driver IC for precision solenoid control applications.
 *
 *
 * For MC33816 vs PT2000 differences see
 * https://www.nxp.com/docs/en/application-note/AN5203.pdf
 *
 * @date May 3, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#include "global.h"

#if EFI_MC33816

#include "mc33816.h"
#include "engine_configuration.h"
#include "efi_gpio.h"
#include "hardware.h"
#include "mc33816_data.h"
#include "mpu_util.h"

EXTERN_CONFIG;

static OutputPin chipSelect;
static OutputPin resetB;
static OutputPin driven;

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


static void showStats() {

}

// Mostly unused
unsigned short recv_16bit_spi() {
	unsigned short ret;
	//spiSelect(driver);
	spiReceive(driver, 1, &ret);
	//spiUnselect(driver);
	return ret;
}

// This could be used to detect if check byte is wrong.. or use a FLAG after init
unsigned short txrx_16bit_spi(const unsigned short param) {
	unsigned short ret;
	//spiSelect(driver);
	spiExchange(driver, 1, &param, &ret);
	//spiUnselect(driver);
	return ret;
}

// Send 16bits
static void spi_writew(unsigned short param) {
	//spiSelect(driver);
	spiSend(driver, 1, &param);
	//spiUnselect(driver);
}

static unsigned short id() {
	spiSelect(driver);
	spi_writew(0xBAA1);
	unsigned short ID =  recv_16bit_spi();
	spiUnselect(driver);
	return ID;
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

static void enable_flash()
{
	spiSelect(driver);
    spi_writew(0x2001); //ch1
    spi_writew(0x0018); //enable flash
    spi_writew(0x2401); //ch2
    spi_writew(0x0018); // enable flash
    spiUnselect(driver);
}

static void download_RAM(int target) {
   unsigned short memory_area = 0;         // memory area
   unsigned short start_address = 0;      // start address
   unsigned short codeWidthRegAddr = 0;   // code width register address
   unsigned short size = 0;               // size of RAM data
   unsigned short command = 0;            // command data
   unsigned short data = 0;               // RAM data
   unsigned int k = 0;                    // used in loop for writing RAM data to the chip
   const unsigned short *RAM_ptr;               // pointer to array of data to be sent to the chip


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

   /*
   for(k = 0; k < size; k++)          // downloads RAM
   {
      data = *RAM_ptr;                // retrieves data to be sent
      spi_writew(data);           // sends data
      RAM_ptr++;
   }
   */
   spiSend(driver, size, RAM_ptr);
   spiUnselect(driver);
}

static void download_register(int r_target) {
	   unsigned short r_start_address = 0;  // start address
	   unsigned short r_size = 0;           // size of configuration data
	   unsigned short r_command = 0;        // command data
	   unsigned short r_data = 0;           // configuration data
	   int n = 0;                           // used for loop for writing data to the chip
	   unsigned short remainder_size = 0;   // remainder size
	   const unsigned short *reg_ptr;            // pointer to array of data to be sent to the chip

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

	   if(r_size > MAX_SPI_MODE_A_TRANSFER_SIZE)   //if size is too large, split into two sections ... MULTIPLE sections..
	   {
	      remainder_size = r_size - MAX_SPI_MODE_A_TRANSFER_SIZE;  // creates remaining size
	      r_size = MAX_SPI_MODE_A_TRANSFER_SIZE;                   // sets first size
	   }

	   r_command = r_start_address << 5;      // start address
	   r_command += r_size;                   // number of words to follow

	   spiSelect(driver);						// Chip

	   spi_writew(r_command);             // sends address and number of words to be sent

	   spiSend(driver, r_size, reg_ptr);

	   if(remainder_size > 0)                 // if remainder size is greater than 0, download the rest
	   {
	      r_start_address += r_size;          // new start address
	      r_command = r_start_address << 5;   // start address
	      r_command += remainder_size;        // number of words to follow

	      spi_writew(r_command);          // sends address and number of words to be sent
	      spiSend(driver, remainder_size, reg_ptr + r_size);
	   }
	   spiUnselect(driver);
}

void initMc33816() {
	// default spi3mosiPin PB5
	// default spi3misoPin PB4
	// default spi3sckPin  PB3
	// ideally disable isSdCardEnabled since it's on SPI3

	// uncomment this to hard-code something
/* fixing continues integration - hiding these values
	CONFIG(mc33816_cs) = GPIOD_7;
	CONFIG(mc33816_rstb) = GPIOD_5;
	CONFIG(mc33816_driven) = GPIOD_6;
*/
	// and more to add...
	if (CONFIG(mc33816_cs) == GPIO_UNASSIGNED)
		return;

	// Initialize the chip via ResetB
	resetB.initPin("mc33 RESTB", engineConfiguration->mc33816_rstb);

	chipSelect.initPin("mc33 CS", engineConfiguration->mc33816_cs /*, &engineConfiguration->csPinMode*/);

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

	//addConsoleAction("mc33_stats", showStats);
	//addConsoleActionI("mc33_send", sendWord);

	// Does starting turn this high to begin with??
	spiUnselect(driver);

	//delay/wait? .. possibly only 30us for each needed, per datasheet
	resetB.setValue(0);
	chThdSleepMilliseconds(10);
	resetB.setValue(1);
	chThdSleepMilliseconds(10);

	setup_spi();
	int mc_id = id();

    download_RAM(CODE_RAM1);        // transfers code RAM1
    download_RAM(CODE_RAM2);        // transfers code RAM2
    download_RAM(DATA_RAM);         // transfers data RAM
    download_register(REG_MAIN);    // download main register configurations
    download_register(REG_CH1);     // download channel 1 register configurations
    download_register(REG_CH2);     // download channel 2 register configurations
    download_register(REG_IO);      // download IO register configurations
    download_register(REG_DIAG);    // download diag register configuration
    enable_flash();
    //driven.setValue(1); // driven = HV
}

void update_scv(unsigned short current)
{
	// Update a single word in Data RAM
	spiSelect(driver);
	// Select Channel command
    spi_writew(0x7FE1);
    // Common Page
    spi_writew(0x0004);
	// write (MSB=0) at data ram x9 (SCV_I_Hold), and 1 word
    spi_writew((9 << 5) + 1);
    spi_writew(current);
    spiUnselect(driver);

	// Strobe it to reload the value
    //GPIO_ClearPinsOutput(GPIOE, 1<<PIN21_IDX); // SCV
    //GPIO_SetPinsOutput(GPIOE, 1<<PIN21_IDX); // SCV

}

#endif /* EFI_MC33816 */
