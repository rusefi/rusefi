package com.rusefi.maintenance;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.UpdateOperationCallbacks;

import java.util.function.Supplier;

public class CallbacksWaitingUtil {
    public static final int TOTAL_WAIT_SECONDS = 60;
    private static final int ONE_DOT_DURATION_MS = 200;

    static boolean waitForPredicate(
        final String waitingMessage,
        final Supplier<Boolean> shouldFinish,
        final UpdateOperationCallbacks callbacks
    ) {
        callbacks.log(waitingMessage, false, true);
        try {
            for (int attemptsCount = 0; attemptsCount < TOTAL_WAIT_SECONDS * 1000 / ONE_DOT_DURATION_MS; attemptsCount++) {
                // Give the bootloader sec to enumerate
                BinaryProtocol.sleep(ONE_DOT_DURATION_MS);
                if (shouldFinish.get()) {
                    return true;
                } else {
                    callbacks.log(".", false, false);
                }
            }
            return false;
        } finally {
            callbacks.log("", true, false);
        }
    }
}
