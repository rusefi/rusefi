/*
 * @file fl_protocol.h
 *
 * @date Mar 14, 2017
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef CONSOLE_FL_BINARY_FT_PROTOCOL_H_
#define CONSOLE_FL_BINARY_FT_PROTOCOL_H_

#include "rusefi_types.h"

#define START_TAG 0xAA
#define MAGIC_TAG 0xBB
#define END_TAG 0xCC

#define FL_Firmware_version 0x0002
#define FL_Interface_Version 0x0000
#define FL_Compiler_Version 0xEEF2
#define FL_Decoder_Name 0xEEEE
#define FL_Firmware_Build_Date 0xEEF0B
#define FL_Max_Packet_Size 0x0004
#define FL_Operating_System 0xEEF4

bool isStartOfFLProtocol(uint8_t firstByte);

#endif /* CONSOLE_FL_BINARY_FT_PROTOCOL_H_ */
