package com.rusefi.mcp;

import com.rusefi.tune.xml.Msq;
import com.rusefi.tune.xml.Page;
import jakarta.xml.bind.JAXBException;

import java.io.IOException;
import java.nio.file.FileAlreadyExistsException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.time.LocalDate;
import java.util.Objects;

/** Daily tune backups: reuse matching calibration content, never overwrite an existing file. */
final class TuneSnapshot {
    private TuneSnapshot() {
    }

    static Path save(Path directory, Msq tune, LocalDate date) throws IOException {
        Path temporary = Files.createTempFile(directory, ".rusefi_tune_", ".tmp");
        try {
            tune.writeXmlFile(temporary.toString());
            for (int suffix = 0; suffix < Integer.MAX_VALUE; suffix++) {
                Path candidate = directory.resolve(date + (suffix == 0 ? "" : "_" + suffix) + ".msq");
                try {
                    // No REPLACE_EXISTING: also protects against another recorder creating this name.
                    Files.copy(temporary, candidate);
                    return candidate;
                } catch (FileAlreadyExistsException exists) {
                    if (matches(candidate, tune)) {
                        return candidate;
                    }
                }
            }
            throw new IOException("No available tune snapshot name for " + date);
        } catch (JAXBException failure) {
            throw new IOException("Failed to serialize tune snapshot", failure);
        } finally {
            Files.deleteIfExists(temporary);
        }
    }

    private static boolean matches(Path path, Msq tune) {
        try {
            Msq existing = Msq.readTune(path.toString());
            if (!Objects.equals(existing.versionInfo.getSignature(), tune.versionInfo.getSignature())
                    || existing.page.size() != tune.page.size()) {
                return false;
            }
            for (Page page : tune.page) {
                Page saved = existing.page.stream()
                        .filter(candidate -> Objects.equals(candidate.getNumber(), page.getNumber()))
                        .findFirst().orElse(null);
                if (saved == null || !Objects.equals(saved.getSize(), page.getSize())
                        || !saved.getConstantsAsMap().equals(page.getConstantsAsMap())) {
                    return false;
                }
            }
            return true;
        } catch (JAXBException | IllegalStateException failure) {
            // An unreadable/invalid existing file is not evidence of an identical tune. Preserve it.
            return false;
        }
    }
}
