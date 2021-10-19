#include "pch.h"

#include "hardware.h"

#include "flash_int.h"

#include "dfu.h"

// Communication vars
static UartTsChannel blTsChannel(TS_PRIMARY_UART);
static uint8_t buffer[DFU_BUFFER_SIZE];
// Use short timeout for the first data packet, and normal timeout for the rest
static int sr5Timeout = DFU_SR5_TIMEOUT_FIRST;

// This big buffer is used for temporary storing of the bootloader flash page
static uint8_t bootloaderVirtualPageBuffer[BOOTLOADER_SIZE];


// needed by DFU protocol (DFU_DEVICE_ID_CMD)
static uint32_t getMcuRevision() {
	// =0x413 for F407
	// =0x419 for F42xxx and F43xxx
	// =0x434 for F469
	return DBGMCU->IDCODE & MCU_REVISION_MASK;
}

static bool getByte(uint8_t *b) {
	return blTsChannel.readTimeout(b, 1, sr5Timeout) == 1;
}

static void sendByte(uint8_t b) {
	blTsChannel.write(&b, 1, true);
}

static uint8_t dfuCalcChecksum(const uint8_t *buf, uint8_t size) {  
	uint8_t checksum = buf[0];
	
	for (uint8_t i = 1; i < size; i++) {
		checksum ^= buf[i];
	}	 
	return checksum;
}

// Used to detect writing of the current flash sector
static bool isBootloaderAddress(uint32_t addr) {
	return addr >= BOOTLOADER_ADDR && addr < (BOOTLOADER_ADDR + BOOTLOADER_SIZE);
}

static bool isInVirtualPageBuffer(uint32_t addr) {
	return addr >= (uint32_t)bootloaderVirtualPageBuffer && addr < (uint32_t)bootloaderVirtualPageBuffer + sizeof(bootloaderVirtualPageBuffer);
}

// Read 32-bit address and 8-bit checksum.
// Returns true if all 5 bytes are received and checksum is correct, and false otherwise.
static bool readAddress(uint32_t *addr) {
	uint8_t buf[5];	// 4 bytes+checksum
	if (blTsChannel.readTimeout(buf, 5, sr5Timeout) != 5)
		return false;
	if (dfuCalcChecksum(buf, 4) != buf[4])
		return false;
	*addr = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];

	// for bootloader flash, return a virtual buffer instead
	if (isBootloaderAddress(*addr)) {
		*addr = (uint32_t)bootloaderVirtualPageBuffer + (*addr - BOOTLOADER_ADDR);
	}

	return true;
}

// needed by DFU protocol to validate received bytes
static uint8_t complementByte(uint8_t c) {
	return c ^ 0xff;
}

static uint16_t bufToInt16(uint8_t *buf) {
	return (buf[0] << 8) | buf[1];
}

static void prepareInterruptsForJump(void) {
#ifdef STM32F4
	// interrupt control
	SCB->ICSR &= ~SCB_ICSR_PENDSVSET_Msk;
	// set interrupt vectors
	for(int i = 0; i < 8; i++)
		NVIC->ICER[i] = NVIC->IABR[i];
	__set_CONTROL(0);
#else
// todo: add support for other MCUs
#error "Unsupported MCU configuration!"
#endif
}

// some weird STM32 magic...
void dfuJumpToApp(uint32_t addr) {
	typedef void (*pFunction)(void);

	// goodbye ChibiOS, we're leaving...
	chSysDisable();

	// get jump addr
	uint32_t jumpAddress = *((uint32_t *)(addr + 4));
	pFunction jump = (pFunction) jumpAddress;
	prepareInterruptsForJump();
	// set stack pointer
    __set_MSP(*(uint32_t *)addr);
	
	// call
    jump();

    // we shouldn't get here
    chSysHalt("dfuJumpToApp FAIL");
}

static void dfuHandleGetList(void) {
	static const uint8_t cmdsInfo[] = { DFU_VERSION_NUMBER, DFU_GET_LIST_CMD, DFU_DEVICE_ID_CMD, DFU_READ_CMD, DFU_GO_CMD, 
		DFU_WRITE_CMD, DFU_ERASE_CMD };
	size_t numBytes = sizeof(cmdsInfo);
	sendByte(numBytes - 1);  // number of commands
	for (size_t i = 0; i < numBytes; i++)
		sendByte(cmdsInfo[i]);
	sendByte(DFU_ACK_BYTE);						
}

static void dfuHandleDeviceId(void) {
	uint32_t mcuRev = getMcuRevision();
	sendByte(0x01); // the number of bytes to be send - 1
    // send 12 bit MCU revision
    sendByte((uint8_t)((mcuRev >> 8) & 0xf));
	sendByte((uint8_t)(mcuRev & 0xff));
	sendByte(DFU_ACK_BYTE);
}

static void dfuHandleGo(void) {
	uint32_t addr;
	
	if (!readAddress(&addr)) {
		sendByte(DFU_NACK_BYTE);
		return;
	}
	// todo: check if the address is valid
	sendByte(DFU_ACK_BYTE);
	dfuJumpToApp(addr);
}

static void dfuHandleRead(void) {
	uint32_t addr;

	if (!readAddress(&addr)) {
		sendByte(DFU_NACK_BYTE);
		return;
	}
	sendByte(DFU_ACK_BYTE);
	uint8_t byte, complement;
    if (!getByte(&byte))
    	return;
    if (!getByte(&complement))
		return;
	// check if we have a correct byte received
	if (complement != complementByte(byte)) {
		sendByte(DFU_NACK_BYTE);
		return;
	}
    int numBytes = (int)byte + 1;
    sendByte(DFU_ACK_BYTE);

    // read flash or virtual RAM buffer (don't transmit directly from flash)
    if (isInVirtualPageBuffer(addr))
    	memcpy(buffer, (uint8_t *)addr, numBytes);
    else
		intFlashRead(addr, (char *)buffer, numBytes);

	// transmit data
	blTsChannel.write(buffer, numBytes, true);
}

static void dfuHandleWrite(void) {
	uint32_t addr;

	if (!readAddress(&addr)) {
		sendByte(DFU_NACK_BYTE);
		return;
	}
	sendByte(DFU_ACK_BYTE);
	if (!getByte(buffer))
		return;

	int numBytes = buffer[0] + 1;
	int numBytesAndChecksum = numBytes + 1;	// +1 byte of checkSum
	// receive data
	if (blTsChannel.readTimeout(buffer + 1, numBytesAndChecksum, sr5Timeout) != numBytesAndChecksum)
		return;
	// don't write corrupted data!
	if (dfuCalcChecksum(buffer, numBytesAndChecksum) != buffer[numBytesAndChecksum]) {
		sendByte(DFU_NACK_BYTE);
		return;
	}
	
	// now write to flash (or to the virtual RAM buffer)
	if (isInVirtualPageBuffer(addr))
    	memcpy((uint8_t *)addr, (buffer + 1), numBytes);
    else
		intFlashWrite(addr, (const char *)(buffer + 1), numBytes);
	
	// we're done!
	sendByte(DFU_ACK_BYTE);
}

static void dfuHandleErase(void) {
	int numSectors;
	if (!getByte(buffer))
    	return;
    if (!getByte(buffer + 1))
    	return;
    numSectors = bufToInt16(buffer);
    int numSectorData;
	if (numSectors == 0xffff)	// erase all chip
		numSectorData = 1;
	else
		numSectorData = (numSectors + 1) * 2 + 1;
	uint8_t *sectorList = buffer + 2;
	// read sector data & checksum
	if (blTsChannel.readTimeout(sectorList, numSectorData, sr5Timeout) != numSectorData)
		return;
	// verify checksum
	if (dfuCalcChecksum(buffer, 2 + numSectorData - 1) != buffer[2 + numSectorData - 1]) {
		sendByte(DFU_NACK_BYTE);
		return;
	}
	// Erase the chosen sectors, sector by sector
	for (int i = 0; i < numSectorData - 1; i += 2) {
		int sectorIdx = bufToInt16(sectorList + i);
		if (sectorIdx < BOOTLOADER_NUM_SECTORS) {	// skip first sectors where our bootloader is
			// imitate flash erase by writing '0xff'
			memset(bootloaderVirtualPageBuffer, 0xff, BOOTLOADER_SIZE);
			continue;
		}
		// erase sector
		intFlashSectorErase(sectorIdx);
	}

    sendByte(DFU_ACK_BYTE);
}

bool dfuStartLoop(void) {
	bool wasCommand = false;
	uint8_t command, complement;	

	sr5Timeout = DFU_SR5_TIMEOUT_FIRST;

	// We cannot afford waiting for the first handshake byte, so we have to send an answer in advance!
	sendByte(DFU_ACK_BYTE);

	// Fill the temporary buffer from the real flash memory
	memcpy(bootloaderVirtualPageBuffer, (void *)BOOTLOADER_ADDR, BOOTLOADER_SIZE);
	 
	while (true) {
		// read command & complement bytes
	    if (!getByte(&command)) {
	    	// timeout, but wait more if we're in bootloader mode
	    	if (wasCommand)
	    		continue;
	    	// exit if no data was received
	    	break;
	    }
		if (!getByte(&complement)) {
			if (wasCommand) {
				// something is wrong, but keep the connection
	    		sendByte(DFU_NACK_BYTE);
	    		continue;
	    	}
			break;
		}
		
		// check if we have a correct command received
		if (complement != complementByte(command)) {
			sendByte(DFU_NACK_BYTE);
			continue;
		}
 
		// confirm that we've got the command
		sendByte(DFU_ACK_BYTE);
		wasCommand = true;
		// set normal (longer) timeout, we're not in a hurry anymore
		sr5Timeout = DFU_SR5_TIMEOUT_NORMAL;

		// now execute it (see ST appnote "AN3155")
    	switch (command) {
        case DFU_UART_CHECK:
			break;
        case DFU_GET_LIST_CMD:
        	dfuHandleGetList();
        	break;
        case DFU_DEVICE_ID_CMD:
        	dfuHandleDeviceId();
			break;				
        case DFU_GO_CMD:
        	dfuHandleGo();
			break;
        case DFU_READ_CMD:
        	dfuHandleRead();
			break;
        case DFU_WRITE_CMD:
        	dfuHandleWrite();
			break;
        case DFU_ERASE_CMD:
        	dfuHandleErase();
			break;
        default: 
           break;	
    	} /* End switch */
    }

    return wasCommand;
}		

SerialTsChannelBase *getTsChannel() {
	return &blTsChannel;
}
