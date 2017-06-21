package com.rusefi.binaryprotocol;

import com.rusefi.config.Fields;

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
    /**
     * that's hex for "~\n", see BINARY_SWITCH_TAG in firmware source code
     */
    int SWITCH_TO_BINARY_RESPONSE = 0x7e0a;
    /**
     * See SWITCH_TO_BINARY_COMMAND in firmware source code
     */
    String SWITCH_TO_BINARY_COMMAND = "~";
    char COMMAND_OUTPUTS = 'O'; // ochGetCommand
    char COMMAND_HELLO = 'S'; // queryCommand
    char COMMAND_PROTOCOL = 'F';
    char COMMAND_CRC_CHECK_COMMAND = 'k';
    char COMMAND_PAGE = 'P';
    char COMMAND_READ = 'R'; // 082 decimal
    char COMMAND_CHUNK_WRITE = 'C'; // pageChunkWrite
    char COMMAND_BURN = 'B'; // burnCommand
}
