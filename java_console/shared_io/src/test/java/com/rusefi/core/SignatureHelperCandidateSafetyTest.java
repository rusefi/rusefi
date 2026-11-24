package com.rusefi.core;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;

import static org.junit.jupiter.api.Assertions.*;

class SignatureHelperCandidateSafetyTest {
    private static final String SIGNATURE = "rusEFI master.2026.11.19.uaefi_pro.10667003";

    @Test
    void malformedCandidateDoesNotEscapeSignatureLookup() {
        byte[] malformed = ("}\nsignature = \"" + SIGNATURE + "\"\n").getBytes(StandardCharsets.UTF_8);
        // #10667: candidate lookup should skip malformed files, without changing the tokenizer API.
        assertNull(SignatureHelper.readIniSignature(malformed));
        assertThrows(IllegalArgumentException.class, () -> IniTokenUtil.splitTokens("}"));
    }

    @Test
    void nestedCompressedPayloadPreservesExistingCache(@TempDir Path directory) throws Exception {
        byte[] ini = ("[MegaTune]\nsignature = \"" + SIGNATURE + "\"\n").getBytes(StandardCharsets.UTF_8);
        Path innerArchive = CompressedTestArchive.create(directory.resolve("inner.7z"),
            directory.resolve("inner-input"), new CompressedTestArchive.Entry("board.ini", ini));
        Path archive = CompressedTestArchive.create(directory.resolve("outer.7z"),
            directory.resolve("outer-input"),
            new CompressedTestArchive.Entry("nested.ini", Files.readAllBytes(innerArchive)));
        Path cache = directory.resolve("cache");
        Files.createDirectories(cache);
        Path cached = cache.resolve("10667003.ini");
        byte[] original = "preserve existing cache".getBytes(StandardCharsets.UTF_8);
        Files.write(cached, original);

        assertThrows(java.io.IOException.class,
            () -> SignatureHelper.importIntoCache(SIGNATURE, archive.toFile(), cache.toString()));

        // #10667: a matching plaintext line inside nested 7z bytes must not make them a valid INI.
        assertArrayEquals(original, Files.readAllBytes(cached));
    }

    @Test
    void mismatchedSignatureCacheIsIgnoredWithoutDeletingOrRelabelingIt(@TempDir Path directory) throws Exception {
        String wrongSignature = "rusEFI master.2026.11.19.other_board.10667004";
        Path cache = Files.createDirectory(directory.resolve("cache"));
        Path cached = cache.resolve("10667003.ini");
        byte[] original = ("[MegaTune]\nsignature = \"" + wrongSignature + "\"\n"
            + ";".repeat(12_000) + "\n").getBytes(StandardCharsets.UTF_8);
        Files.write(cached, original);

        String result = SignatureHelper.downloadMatchingIniIfNotAvailableForTests(
            SignatureHelper.getUrl(SIGNATURE), false, cache.toString(), SIGNATURE);

        assertNull(result, "a cache is a miss unless its embedded signature matches exactly");
        assertArrayEquals(original, Files.readAllBytes(cached),
            "rejecting a cache candidate never deletes or relabels the existing INI");
    }
}
