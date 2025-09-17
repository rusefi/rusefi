package com.rusefi.io.commands;

import com.devexperts.logging.Logging;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Integration;

import static com.rusefi.binaryprotocol.IoHelper.checkResponseCode;
import static com.rusefi.config.generated.Integration.TS_RESPONSE_BURN_OK;

public class BurnCommand {
    private static final Logging log = Logging.getLogging(BurnCommand.class);
    private static final byte[] PAGE_0 = new byte[2];

    public static boolean execute(BinaryProtocol bp) {
        byte[] page = bp.isSinglePageController() ? null : PAGE_0;
        byte[] response = bp.executeCommand(Integration.TS_BURN_COMMAND, page, "burn");
        boolean isExpectedBurnResponseCode = checkResponseCode(response, (byte) TS_RESPONSE_BURN_OK);
        if (response == null) {
            log.warn("empty BURN response on " + bp.getStream());
            return false;
        }
        boolean isExpectedBurnResponseLength = response.length == 1;
        return isExpectedBurnResponseCode && isExpectedBurnResponseLength;
    }
}
