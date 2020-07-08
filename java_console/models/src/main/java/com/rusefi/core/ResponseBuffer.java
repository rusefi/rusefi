package com.rusefi.core;

import com.rusefi.io.LinkDecoder;
import org.jetbrains.annotations.NotNull;

/**
 * this class takes buffers input characters and separates them into full response lines
 * <p/>
 * Date: 12/25/12
 * Andrey Belomutskiy, (c) 2013-2020
 *
 * @see ResponseBufferTest
 */
public class ResponseBuffer {
    private final ResponseListener listener;
    private final StringBuffer pending = new StringBuffer();

    public ResponseBuffer(@NotNull ResponseListener listener) {
        this.listener = listener;
    }

    public void append(String append, LinkDecoder decoder) {
        pending.append(append);

        /**
         * we might have anything between one unterminated line and numerous terminated lines
         */
        while (hasCRLF(pending)) {
            int cr = pending.indexOf("\r");
            int lf = pending.indexOf("\n");

            if (cr != -1 && lf == -1)
                lf = cr;

            if (cr == -1 && lf != -1)
                cr = lf;

            if (cr != -1) {
                int endOfLine = Math.min(cr, lf);
                if (endOfLine > 0) {
                    String fullLine = pending.substring(0, endOfLine);
                    listener.onResponse(decoder.unpack(fullLine));
                }
                while (pending.length() > endOfLine && (pending.charAt(endOfLine) == '\r' || pending.charAt(endOfLine) == '\n'))
                    endOfLine++;

                pending.delete(0, endOfLine);
            }
        }
    }

    private static boolean hasCRLF(StringBuffer str) {
        int cr = str.indexOf("\r");
        int lf = str.indexOf("\n");
        return cr != -1 || lf != -1;
    }

    public interface ResponseListener {
        void onResponse(String unpack);
    }
}
