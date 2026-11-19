package com.opensr5.io;

import com.opensr5.ConfigurationImageMetaVersion0_0;
import com.opensr5.ConfigurationImageWithMeta;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.nio.ByteOrder;
import java.nio.file.Path;
import java.util.Map;
import java.util.TreeMap;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;

/**
 * [tag:image_byte_order] A calibration archive stores the controller's raw bytes and no byte
 * order, see CalibrationsHelperByteOrderTest for the rule.
 */
class ConfigurationImageFileByteOrderTest {
    @TempDir
    Path tempDir;

    @Test
    void pagesReadFromAnArchiveOfABigEndianController() throws Exception {
        Map<Integer, ConfigurationImageWithMeta> pages = new TreeMap<>();
        pages.put(0, new ConfigurationImageWithMeta(
            new ConfigurationImageMetaVersion0_0(2, "big"), new byte[]{0x12, 0x34}, ByteOrder.BIG_ENDIAN));
        pages.put(0x0400, new ConfigurationImageWithMeta(
            new ConfigurationImageMetaVersion0_0(2, "big"), new byte[]{0x56, 0x78}, ByteOrder.BIG_ENDIAN));
        String archive = tempDir.resolve("bigEndian.zip").toString();
        ConfigurationImageFile.saveToFile(pages, archive);

        Map<Integer, ConfigurationImageWithMeta> restored = ConfigurationImageFile.readPagesFromFile(archive, ByteOrder.BIG_ENDIAN);

        assertArrayEquals(new byte[]{0x12, 0x34}, restored.get(0).getConfigurationImage().getContent());
        assertEquals(ByteOrder.BIG_ENDIAN, restored.get(0).getConfigurationImage().getByteOrder());
        assertEquals(ByteOrder.BIG_ENDIAN, restored.get(0x0400).getConfigurationImage().getByteOrder());
        assertEquals(0x1234, restored.get(0).getConfigurationImage().getByteBuffer(0, 2).getShort());
    }
}
