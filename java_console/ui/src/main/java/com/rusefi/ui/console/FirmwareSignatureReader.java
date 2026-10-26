package com.rusefi.ui.console;

import com.rusefi.core.RusEfiSignature;
import com.rusefi.core.SignatureHelper;
import com.rusefi.libopenblt.file.SrecParser;

import java.io.File;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.time.DateTimeException;
import java.time.LocalDate;

/**
 * Reads the firmware's compiled protocol signature from an S-record image.
 * [tag:embedded_firmware_signature] Compare the compiled TS_SIGNATURE, not the
 * independently dated filename. Its date distinguishes builds that share a
 * configuration hash, but cannot distinguish code-only changes on the same day.
 * Missing/ambiguous data must remain unchecked.
 */
final class FirmwareSignatureReader {
    private static final byte[] PREFIX = "rusEFI ".getBytes(StandardCharsets.US_ASCII);
    private static final int MAX_SIGNATURE_LENGTH = 256;

    private FirmwareSignatureReader() {
    }

    /** Returns null if the image has no single unambiguous, valid signature. */
    static RusEfiSignature read(File file) throws IOException {
        if (file == null) {
            return null;
        }

        SrecParser parser = new SrecParser();
        parser.parse(file);
        String foundText = null;
        RusEfiSignature foundSignature = null;
        for (SrecParser.SRecord segment : parser.getSegments()) {
            byte[] data = segment.data;
            for (int start = 0; start <= data.length - PREFIX.length; start++) {
                if (!matches(data, start, PREFIX)) {
                    continue;
                }
                int end = start + PREFIX.length;
                while (end < data.length && data[end] != 0 && end - start <= MAX_SIGNATURE_LENGTH) {
                    end++;
                }
                if (end >= data.length || data[end] != 0 || end - start > MAX_SIGNATURE_LENGTH) {
                    continue;
                }
                String text = asciiString(data, start, end);
                if (text == null) {
                    continue;
                }
                RusEfiSignature signature = validSignature(text);
                if (signature != null) {
                    if (foundText != null && !foundText.equals(text)) {
                        return null;
                    }
                    foundText = text;
                    foundSignature = signature;
                }
            }
        }
        return foundSignature;
    }

    private static RusEfiSignature validSignature(String text) {
        // SignatureHelper's split drops trailing empty fields; do not accept a truncated/extended literal.
        if (text.substring(PREFIX.length).split("\\.", -1).length != 6) {
            return null;
        }
        RusEfiSignature signature = SignatureHelper.parse(text);
        if (signature == null || empty(signature.getBranch()) || empty(signature.getBundleTarget())
            || text.indexOf(' ', PREFIX.length) >= 0) {
            return null;
        }
        if (!digits(signature.getYear()) || !digits(signature.getMonth()) || !digits(signature.getDay())
            || !digits(signature.getHash()) || signature.getYear().length() != 4
            || signature.getMonth().length() != 2 || signature.getDay().length() != 2) {
            return null;
        }
        try {
            int year = Integer.parseInt(signature.getYear());
            int month = Integer.parseInt(signature.getMonth());
            int day = Integer.parseInt(signature.getDay());
            LocalDate.of(year, month, day);
        } catch (NumberFormatException | DateTimeException e) {
            return null;
        }
        return signature;
    }

    private static boolean empty(String value) {
        return value == null || value.isEmpty();
    }

    private static boolean digits(String value) {
        if (empty(value)) {
            return false;
        }
        for (int i = 0; i < value.length(); i++) {
            if (value.charAt(i) < '0' || value.charAt(i) > '9') {
                return false;
            }
        }
        return true;
    }

    private static boolean matches(byte[] data, int offset, byte[] expected) {
        for (int i = 0; i < expected.length; i++) {
            if (data[offset + i] != expected[i]) {
                return false;
            }
        }
        return true;
    }

    private static String asciiString(byte[] data, int start, int end) {
        for (int i = start; i < end; i++) {
            if (data[i] < 0x20 || data[i] > 0x7E) {
                return null;
            }
        }
        return new String(data, start, end - start, StandardCharsets.US_ASCII);
    }
}
