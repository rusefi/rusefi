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
    char COMMAND_OUTPUTS = 'O'; // ochGetCommand
    char COMMAND_HELLO = 'S'; // queryCommand
    char COMMAND_PROTOCOL = 'F';
    char COMMAND_CRC_CHECK_COMMAND = 'k';
    char COMMAND_PAGE = 'P';
    char COMMAND_READ = 'R'; // 082 decimal
    char COMMAND_CHUNK_WRITE = 'C'; // pageChunkWrite
    char COMMAND_BURN = 'B'; // burnCommand
}
