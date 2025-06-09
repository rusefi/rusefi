package com.rusefi.io.commands;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Integration;

import static com.rusefi.binaryprotocol.IoHelper.checkResponseCode;
import static com.rusefi.config.generated.Integration.TS_RESPONSE_BURN_OK;

public class BurnCommand {

    private static final byte[] PAGE_0 = new byte[2];

    public static boolean execute(BinaryProtocol bp) {
        byte[] response = bp.executeCommand(Integration.TS_BURN_COMMAND, PAGE_0, "burn");
        boolean isExpectedBurnResponseCode = checkResponseCode(response, (byte) TS_RESPONSE_BURN_OK);
        boolean isExpectedBurnResponseLength = response.length == 1;
        return isExpectedBurnResponseCode && isExpectedBurnResponseLength;
    }
}
