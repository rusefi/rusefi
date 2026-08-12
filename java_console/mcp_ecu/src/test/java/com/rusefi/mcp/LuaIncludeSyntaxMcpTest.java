package com.rusefi.mcp;

import com.rusefi.ui.lua.LuaIncludeSyntax;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class LuaIncludeSyntaxMcpTest {

    @TempDir
    Path tempDir;

    @Test
    public void testIncludeLogic() throws IOException {
        Path mainFile = tempDir.resolve("main.lua");
        Path libFile = tempDir.resolve("lib.lua");

        Files.write(libFile, "print('hello from lib')".getBytes(StandardCharsets.US_ASCII));

        String mainContent = "print('start')\n" +
                "-- include lib.lua\n" +
                "-- endinclude\n" +
                "print('end')";

        Files.write(mainFile, mainContent.getBytes(StandardCharsets.US_ASCII));

        // This is what I want to implement in EcuMcpServer
        String processed = LuaIncludeSyntax.reloadScript(mainContent, name -> {
            try {
                return new String(Files.readAllBytes(mainFile.getParent().resolve(name)), StandardCharsets.US_ASCII);
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        });

        assertEquals("print('start')\n" +
                "-- include lib.lua\n" +
                "print('hello from lib')\n" +
                "print('end')", processed);
    }

    @Test
    public void testIncludeWithoutEndIncludeFails() throws IOException {
        String mainContent = "print('start')\n" +
                "-- include lib.lua\n" +
                "print('end')";

        try {
            LuaIncludeSyntax.reloadScript(mainContent, name -> "something");
            org.junit.jupiter.api.Assertions.fail("Should have thrown IllegalStateException");
        } catch (IllegalStateException e) {
            assertEquals("No end for lib.lua", e.getMessage());
        }
    }
}
