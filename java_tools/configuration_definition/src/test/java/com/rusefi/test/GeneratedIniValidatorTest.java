package com.rusefi.test;

import com.rusefi.output.GeneratedIniValidator;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;

import static org.junit.jupiter.api.Assertions.*;

public class GeneratedIniValidatorTest {
    @Test
    public void generatedMarkersAreRejectedWithFileAndLine(@TempDir Path directory) throws IOException {
        Path ini = directory.resolve("generated.ini");
        for (String marker : new String[]{"@@if_FLAG", "@if_FLAG", "@@endif_block", "@endif_block",
                "@@UNEXPANDED@@", "@@UNTERMINATED", "@#UNEXPANDED#@", "@OFFSET@"}) {
            Files.write(ini, ("[Menu]\nsubMenu = offsets, \"Offsets\", 0" + marker + "\n").getBytes(StandardCharsets.UTF_8));
            IllegalStateException e = assertThrows(IllegalStateException.class, () -> GeneratedIniValidator.validate(ini));
            assertTrue(e.getMessage().contains(ini + ":2:"), e.getMessage());
        }
    }

    @Test
    public void commentsAndZeroPlaceholdersAreAllowed() {
        GeneratedIniValidator.validateLine("subMenu = offsets, \"Offsets\", 0");
        GeneratedIniValidator.validateLine("subMenu = offsets, \"Offsets\", 0, { enabled }");
        GeneratedIniValidator.validateLine("; Template documentation: @@if_FLAG");
        GeneratedIniValidator.validateLine("field = \"Literal ; semicolon\" ; @@comment");
        GeneratedIniValidator.validateLine("field = \"Escaped \\\" quote\" ; @@comment");
        assertThrows(IllegalStateException.class,
                () -> GeneratedIniValidator.validateLine("field = \"Literal ; @@UNEXPANDED\""));
    }

    @Test
    public void commandRequiresExistingFiles(@TempDir Path directory) {
        assertThrows(IllegalArgumentException.class, () -> GeneratedIniValidator.main(new String[0]));
        assertThrows(IOException.class, () -> GeneratedIniValidator.main(new String[]{directory.resolve("missing.ini").toString()}));
    }
}
