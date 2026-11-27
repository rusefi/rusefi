package com.rusefi.binaryprotocol;

import org.junit.jupiter.api.Test;

import java.io.ByteArrayOutputStream;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;

import static org.junit.jupiter.api.Assertions.*;

/**
 * Pure-Java extraction of the ECU-served .ini filesystem image (#10349).
 * The fixture test_uaefi_fs.bin is a real FAT12 image built by
 * firmware/hw_layer/mass_storage/create_ini_image.sh from the uaefi .ini
 * (label RUSEFI, holding rusefi.ini.7z plus the README/url files).
 */
public class CanIniDownloaderTest {
    @Test
    public void extractsIniFromUaefiFsImage() throws Exception {
        byte[] image = readResource("test_uaefi_fs.bin");
        assertTrue(image.length > 0, "fixture test_uaefi_fs.bin is empty");

        byte[] ini = CanIniDownloader.extractIni(image);

        // the uaefi .ini is ~946 KB; the LZMA2-compressed copy in the FAT is ~140 KB
        assertTrue(ini.length > 500_000, "extracted .ini unexpectedly small: " + ini.length);
        String text = new String(ini, StandardCharsets.ISO_8859_1);
        assertTrue(text.contains("uaefi"), "extracted file does not look like the uaefi .ini");
        assertTrue(text.contains("signature"), "extracted file has no TunerStudio signature line");
    }

    private static byte[] readResource(String name) throws Exception {
        try (InputStream in = CanIniDownloaderTest.class.getResourceAsStream(name)) {
            assertNotNull(in, "test resource not found on classpath: " + name);
            ByteArrayOutputStream out = new ByteArrayOutputStream();
            byte[] buffer = new byte[8192];
            int read;
            while ((read = in.read(buffer)) != -1) {
                out.write(buffer, 0, read);
            }
            return out.toByteArray();
        }
    }
}
