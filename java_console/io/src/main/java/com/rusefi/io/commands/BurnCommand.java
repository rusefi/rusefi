package com.rusefi.io.commands;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Fields;

import static com.rusefi.binaryprotocol.IoHelper.checkResponseCode;
import static com.rusefi.config.generated.Fields.TS_RESPONSE_BURN_OK;

public class BurnCommand {
    public static boolean execute(BinaryProtocol bp) {
        byte[] response = bp.executeCommand(Fields.TS_BURN_COMMAND, "burn");
        boolean isExpectedBurnResponseCode = checkResponseCode(response, (byte) TS_RESPONSE_BURN_OK);
        boolean isExpectedBurnResponseLength = response.length == 1;
        return isExpectedBurnResponseCode && isExpectedBurnResponseLength;
    }
}
