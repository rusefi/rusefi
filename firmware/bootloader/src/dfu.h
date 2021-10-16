#pragma once

#include "tunerstudio_io.h"

// This is where the bootloader starts
#define BOOTLOADER_ADDR    0x08000000
// Bootloader code max. size, in bytes
#define BOOTLOADER_SIZE    0x8000
// Number of sectors for the bootloader
#define BOOTLOADER_NUM_SECTORS (BOOTLOADER_SIZE/0x4000)

// This is where the application starts
#define APPLICATION_ADDR   0x08008000

#define DFU_BUFFER_SIZE    258   // Max. 256 bytes at a time plus 2 bytes (numBytes+checksum)


// DFU/USART Protocol is described in AN3155 document "Application note. USART protocol used in the STM32 bootloader"
// http://www.st.com/resource/en/application_note/cd00264342.pdf

#define DFU_UART_CHECK     0x7F  // "UART handshake" escape byte

#define DFU_GET_LIST_CMD   0x00  // "Get supported commands list" command
#define DFU_DEVICE_ID_CMD  0x02  // "Get device ID" command
#define DFU_READ_CMD       0x11  // "Read memory" command
#define DFU_GO_CMD         0x21  // "Go" command
#define DFU_WRITE_CMD      0x31  // "Write memory" command
#define DFU_ERASE_CMD      0x44  // "Erase memory" command

#define DFU_VERSION_NUMBER 0x31  // The DFU protocol version number
#define DFU_ACK_BYTE       0x79  // Acknowledge byte ID
#define DFU_NACK_BYTE      0x1F  // Not-Acknowledge byte ID

#define DFU_SR5_TIMEOUT_FIRST  TIME_MS2I(200)
#define DFU_SR5_TIMEOUT_NORMAL TIME_MS2I(1000)

#define MCU_REVISION_MASK  0xfff // MCU Revision ID is needed by DFU protocol

// The address in MCU system memory where the bootloader version number is stored (2 bytes)
#define DFU_BOOTLOADER_VERSION_ADDRESS 0x1FFF76DE

/**
  * @brief  This function waits for the command to apply (write, read etc...)
  */
bool dfuStartLoop(void);
/**
  * @brief  Jump to the application
  */
void dfuJumpToApp(uint32_t addr);

SerialTsChannelBase* getTsChannel();
