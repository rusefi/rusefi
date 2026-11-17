package com.rusefi.logview;

import java.io.BufferedWriter;
import java.io.File;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.math.BigDecimal;
import java.math.MathContext;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;

/**
 * Writes a log as MSL: tab-separated text with a quoted title line, the names row, the units row and one row per
 * sample, as MegaLogViewer and {@link CsvLogReader} read it. Time is written in seconds, whatever the source's
 * time unit was, since {@link CsvLogReader} already converted it.
 */
public final class MslWriter {
    /** Enough for 10 us steps over a day of logging. */
    private static final MathContext TIME_PRECISION = new MathContext(12);

    private MslWriter() {
    }

    public static void write(CsvLogReader log, int from, int to, String title, File file) throws IOException {
        try (Writer out = new BufferedWriter(new OutputStreamWriter(Files.newOutputStream(file.toPath()),
                StandardCharsets.ISO_8859_1))) {
            write(log, from, to, title, out);
        }
    }

    /** Writes samples {@code from} to {@code to}, both included. */
    public static void write(CsvLogReader log, int from, int to, String title, Writer out) throws IOException {
        out.write('"' + title.replace('"', '\'') + "\"\n");
        StringBuilder names = new StringBuilder();
        StringBuilder units = new StringBuilder();
        for (int field = 0; field < log.fieldCount(); field++) {
            if (field > 0) {
                names.append('\t');
                units.append('\t');
            }
            names.append(log.name(field));
            units.append(field == 0 ? "s" : log.unit(field));
        }
        out.write(names.append('\n').toString());
        out.write(units.append('\n').toString());
        StringBuilder row = new StringBuilder();
        for (int sample = from; sample <= to; sample++) {
            row.setLength(0);
            for (int field = 0; field < log.fieldCount(); field++) {
                if (field > 0) {
                    row.append('\t');
                }
                row.append(format(log.value(sample, field), field == 0));
            }
            out.write(row.append('\n').toString());
        }
    }

    /**
     * A value as plain decimal text, empty when missing. Whole numbers are written without decimals. Other values
     * are written with float precision, which is what the logs carry, so that scaling does not add digits
     * (0.30000000000000004); time keeps more digits, as a long log needs them.
     */
    static String format(double value, boolean time) {
        if (!Double.isFinite(value)) {
            return "";
        }
        if (value == Math.rint(value) && Math.abs(value) < 1e15) {
            return Long.toString((long) value);
        }
        BigDecimal decimal = time || Math.abs(value) > Float.MAX_VALUE
                ? new BigDecimal(value).round(TIME_PRECISION)
                : new BigDecimal(Float.toString((float) value));
        return decimal.stripTrailingZeros().toPlainString();
    }
}
