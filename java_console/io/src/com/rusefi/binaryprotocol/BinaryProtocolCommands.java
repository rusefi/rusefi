package com.rusefi.binaryprotocol;

import com.rusefi.config.generated.Fields;

/**
 * (c) Andrey Belomutskiy
 * 6/21/2017.
 */
public interface BinaryProtocolCommands {
    // see BLOCKING_FACTOR in firmware code
    int BLOCKING_FACTOR = Fields.BLOCKING_FACTOR;
    byte RESPONSE_OK = 0;
    byte RESPONSE_BURN_OK = 0x04;
    byte RESPONSE_COMMAND_OK = 0x07;
    char COMMAND_OUTPUTS = Fields.TS_OUTPUT_COMMAND;
    char COMMAND_HELLO = Fields.TS_HELLO_COMMAND;
    char COMMAND_PROTOCOL = 'F';
    // todo: make crc32CheckCommand shorted one day later - no need in 6 empty bytes
    char COMMAND_CRC_CHECK_COMMAND = Fields.TS_CRC_CHECK_COMMAND;
    char COMMAND_PAGE = 'P';
    char COMMAND_READ = Fields.TS_READ_COMMAND;
    char COMMAND_CHUNK_WRITE = 'C'; // pageChunkWrite
    char COMMAND_BURN = 'B'; // burnCommand
    char COMMAND_GET_STRUCT = '9'; // TS_GET_STRUCT
}
