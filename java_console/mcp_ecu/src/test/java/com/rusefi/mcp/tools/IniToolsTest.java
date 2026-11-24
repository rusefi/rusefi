package com.rusefi.mcp.tools;

import org.json.simple.JSONObject;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.File;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;

import static org.junit.jupiter.api.Assertions.*;

/**
 * Covers error paths of {@link IniTools#importIniToCache(JSONObject)}. The happy path
 * targets {@code ~/.rusEFI/ini_database/}, which is a user-wide shared folder keyed
 * off {@code user.home} at class-load time — see {@code SignatureHelperImportTest}
 * for coverage of {@link com.rusefi.core.SignatureHelper#importIntoCache} against an
 * injected cache directory.
 */
class IniToolsTest {
    @Test
    @SuppressWarnings("unchecked")
    void rejectsMissingOrInvalidPath() throws IOException {
        for (Object badPath : new Object[]{null, "", "   ", 42L}) {
            JSONObject args = new JSONObject();
            if (badPath != null) args.put("path", badPath);
            JSONObject body = IniTools.importIniToCache(args);
            assertEquals(Boolean.FALSE, body.get("success"));
            assertTrue(body.get("error").toString().contains("path"), body.toJSONString());
        }
    }

    @Test
    @SuppressWarnings("unchecked")
    void rejectsPathPointingToNowhere(@TempDir Path directory) throws IOException {
        JSONObject args = new JSONObject();
        args.put("path", directory.resolve("missing.ini").toString());
        JSONObject body = IniTools.importIniToCache(args);
        assertEquals(Boolean.FALSE, body.get("success"));
        assertTrue(body.get("error").toString().contains("not an existing regular file"));
    }

    @Test
    @SuppressWarnings("unchecked")
    void rejectsIniWithoutSignature(@TempDir Path directory) throws IOException {
        Path ini = directory.resolve("no-signature.ini");
        Files.write(ini, "pageSize = 1234\n".getBytes(StandardCharsets.UTF_8));
        JSONObject args = new JSONObject();
        args.put("path", ini.toString());
        JSONObject body = IniTools.importIniToCache(args);
        assertEquals(Boolean.FALSE, body.get("success"));
        assertTrue(body.get("error").toString().contains("signature"));
    }

    @Test
    @SuppressWarnings("unchecked")
    void rejectsUnparseableOverrideSignature(@TempDir Path directory) throws IOException {
        Path ini = directory.resolve("any.ini");
        Files.write(ini, "pageSize = 1234\n".getBytes(StandardCharsets.UTF_8));
        JSONObject args = new JSONObject();
        args.put("path", ini.toString());
        args.put("signature", "not a signature");
        JSONObject body = IniTools.importIniToCache(args);
        assertEquals(Boolean.FALSE, body.get("success"));
        assertTrue(body.get("error").toString().contains("Unparseable"));
    }

    @Test
    @SuppressWarnings("unchecked")
    void rejectsNonStringSignature(@TempDir Path directory) throws IOException {
        Path ini = directory.resolve("any.ini");
        Files.write(ini, "pageSize = 1234\n".getBytes(StandardCharsets.UTF_8));
        JSONObject args = new JSONObject();
        args.put("path", ini.toString());
        args.put("signature", 42L);
        JSONObject body = IniTools.importIniToCache(args);
        assertEquals(Boolean.FALSE, body.get("success"));
        assertTrue(body.get("error").toString().contains("signature"));
    }

    @Test
    void readSignatureExtractsFirstTokenAfterKey(@TempDir Path directory) throws Exception {
        Path ini = directory.resolve("ok.ini");
        Files.write(ini,
                ("; a comment line\n" +
                        "signature= \"rusEFI master.2026.07.02.uaefi.2367417284\" ; comment tail\n" +
                        "pageSize = 1234\n").getBytes(StandardCharsets.UTF_8));
        assertEquals("rusEFI master.2026.07.02.uaefi.2367417284",
                IniTools.readSignature(new File(ini.toString())));
    }
}
