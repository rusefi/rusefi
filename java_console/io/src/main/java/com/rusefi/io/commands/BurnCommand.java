package com.rusefi.io.commands;

import com.devexperts.logging.Logging;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Integration;

import static com.rusefi.binaryprotocol.IoHelper.checkResponseCode;
import static com.rusefi.binaryprotocol.IoHelper.putShort;
import static com.rusefi.binaryprotocol.IoHelper.swap16;
import static com.rusefi.config.generated.Integration.TS_RESPONSE_BURN_OK;

public class BurnCommand {
    private static final Logging log = Logging.getLogging(BurnCommand.class);

    public static boolean execute(BinaryProtocol bp) {
        return execute(bp, 0);
    }

    /**
     * Page-aware burn: firmware routes e.g. TS_PAGE_LUA to burnExtraFlashPage, so burning
     * the page the data was written to is required for secondary pages to persist.
     */
    public static boolean execute(BinaryProtocol bp, int pageIndex) {
        byte[] page = null;
        if (!bp.isSinglePageController()) {
            // same 16-bit wire encoding as ByteRange.packPageOffsetAndSize
            page = new byte[2];
            putShort(page, 0, swap16(pageIndex));
        }
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
