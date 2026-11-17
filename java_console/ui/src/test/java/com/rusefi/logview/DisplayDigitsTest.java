package com.rusefi.logview;

import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.io.InputStream;
import java.io.StringReader;

import static org.junit.jupiter.api.Assertions.assertEquals;

/** Display precision comes from each field's values; the logs' own digit fields are not usable. */
class DisplayDigitsTest {
    @Test
    void ruleGivesAboutFourSignificantDigits() {
        assertEquals(0, CsvLogReader.displayDigits(false, 6194), "whole values");
        assertEquals(0, CsvLogReader.displayDigits(true, 6194.5));
        assertEquals(1, CsvLogReader.displayDigits(true, 100.2));
        assertEquals(2, CsvLogReader.displayDigits(true, 83.18));
        assertEquals(3, CsvLogReader.displayDigits(true, 0.97));
        assertEquals(3, CsvLogReader.displayDigits(true, 0.0012), "capped at three");
        assertEquals(0, CsvLogReader.displayDigits(true, 0));
        assertEquals(0, CsvLogReader.displayDigits(true, Double.NaN));
    }

    private static int field(CsvLogReader log, String name) {
        for (int field = 0; field < log.fieldCount(); field++) {
            if (log.name(field).equals(name)) {
                return field;
            }
        }
        throw new AssertionError("no field " + name);
    }

    @Test
    void ecuLogFields() throws IOException {
        CsvLogReader log;
        try (InputStream stream = getClass().getResourceAsStream("/logview/test-log-binary-1.mlg")) {
            log = BinaryLogReader.read(stream);
        }
        // the file itself declares 0 or 3 digits; the values decide instead
        assertEquals(0, log.digits(field(log, "RPM")));
    }

    @Test
    void textLogFields() throws IOException {
        CsvLogReader log;
        try (InputStream stream = getClass().getResourceAsStream("/logview/test-log-1.msl")) {
            log = LogFiles.read(stream, "x.msl");
        }
        assertEquals(0, log.digits(field(log, "RPM")));
        assertEquals(1, log.digits(field(log, "TPS")), "0..100 %");
        assertEquals(2, log.digits(field(log, "CLT")), "about 82 C");
    }

    @Test
    void wholeNumberColumnHasNoDecimals() throws IOException {
        CsvLogReader log = CsvLogReader.read(new StringReader("Time\tGear\tAFR\ns\t\t\n0\t1\t14.7\n1\t2\t12.95\n"));
        assertEquals(0, log.digits(1));
        assertEquals(2, log.digits(2));
    }
}
