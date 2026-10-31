package com.rusefi.util;

import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;
import com.rusefi.tune.xml.Page;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.nio.file.Files;
import java.nio.file.Path;
import java.time.LocalDate;

import static org.junit.jupiter.api.Assertions.*;

class TuneSnapshotTest {
    private static final LocalDate DATE = LocalDate.of(2026, 9, 15);
    @TempDir
    Path directory;

    @Test
    void namesDailySnapshotsAndReusesBothOriginalAndNumberedTunes() throws Exception {
        Path original = TuneSnapshot.save(directory, tune("10"), DATE);
        assertEquals(directory.resolve("2026-09-15.msq"), original);
        byte[] originalBytes = Files.readAllBytes(original);
        Msq same = tune("10");
        same.bibliography.setTuneComment("metadata is not a calibration change");
        same.versionInfo.setFirmwareInfo("different console version");
        assertEquals(original, TuneSnapshot.save(directory, same, DATE));

        Path changed = TuneSnapshot.save(directory, tune("11"), DATE);
        assertEquals(directory.resolve("2026-09-15_1.msq"), changed);
        assertEquals(changed, TuneSnapshot.save(directory, tune("11"), DATE));
        assertEquals(directory.resolve("2026-09-15_2.msq"), TuneSnapshot.save(directory, tune("12"), DATE));
        assertEquals(original, TuneSnapshot.save(directory, tune("10"), DATE));
        assertArrayEquals(originalBytes, Files.readAllBytes(original));
        assertEquals("11", Msq.readTune(changed.toString()).getConstantsAsMap().get("fuel").getValue());
        assertEquals(directory.resolve("2026-09-16.msq"),
                TuneSnapshot.save(directory, tune("10"), DATE.plusDays(1)));
        try (java.util.stream.Stream<Path> files = Files.list(directory)) {
            assertEquals(4, files.count(), "No temporary files or redundant snapshots");
        }
    }

    @Test
    void secondaryPageAndSignatureChangesAreDetected() throws Exception {
        TuneSnapshot.save(directory, tune("10"), DATE);
        Msq changed = tune("10");
        changed.page.get(2).constant.set(0, new Constant("luaScript", "", "print(2)", "0"));
        assertEquals(directory.resolve("2026-09-15_1.msq"), TuneSnapshot.save(directory, changed, DATE));
        Msq firmwareChanged = tune("10");
        firmwareChanged.versionInfo.setSignature("other firmware");
        assertEquals(directory.resolve("2026-09-15_2.msq"), TuneSnapshot.save(directory, firmwareChanged, DATE));
    }

    @Test
    void preservesInvalidExistingFiles() throws Exception {
        Path existing = directory.resolve("2026-09-15.msq");
        Files.writeString(existing, "not a tune");
        assertEquals(directory.resolve("2026-09-15_1.msq"), TuneSnapshot.save(directory, tune("10"), DATE));
        assertEquals("not a tune", Files.readString(existing));
    }

    static Msq tune(String value) {
        Msq tune = Msq.create(128, "test firmware");
        tune.findPage().constant.add(new Constant("fuel", "ms", value, "0"));
        Page secondary = new Page(1, 64);
        secondary.constant.add(new Constant("luaScript", "", "print(1)", "0"));
        tune.page.add(secondary);
        tune.versionInfo.setPageCount(2);
        return tune;
    }
}
