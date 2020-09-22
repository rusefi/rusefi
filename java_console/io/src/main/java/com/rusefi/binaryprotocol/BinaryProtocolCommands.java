package com.rusefi.binaryprotocol;

import com.rusefi.config.generated.Fields;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 6/21/2017.
 */
public interface BinaryProtocolCommands {
    byte RESPONSE_OK = Fields.TS_RESPONSE_OK;
    byte RESPONSE_BURN_OK = Fields.TS_RESPONSE_BURN_OK;
    byte RESPONSE_COMMAND_OK = Fields.TS_RESPONSE_COMMAND_OK;
    char COMMAND_PROTOCOL = Fields.TS_COMMAND_F;
    char COMMAND_CRC_CHECK_COMMAND = Fields.TS_CRC_CHECK_COMMAND;
    char COMMAND_CHUNK_WRITE = Fields.TS_CHUNK_WRITE_COMMAND;
    char COMMAND_BURN = Fields.TS_BURN_COMMAND;
    char COMMAND_GET_STRUCT = '9'; // TS_GET_STRUCT
}
