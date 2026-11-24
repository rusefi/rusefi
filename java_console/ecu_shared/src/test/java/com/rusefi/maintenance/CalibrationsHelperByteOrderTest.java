package com.rusefi.maintenance;

import com.opensr5.ConfigurationImageMeta;
import com.opensr5.ConfigurationImageWithMeta;
import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileModel;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.binaryprotocol.IniFileProvider;
import com.rusefi.config.generated.Integration;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.io.IoStream;
import com.rusefi.io.UpdateOperationCallbacks;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.nio.ByteOrder;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Collections;
import java.util.Optional;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.ArgumentMatchers.*;
import static org.mockito.Mockito.*;

/**
 * [tag:image_byte_order] Calibration images hold the controller's raw bytes, so every copy of an
 * image must keep the byte order declared by the ini ('endianness = big' on PowerPC targets).
 * A copy that falls back to little-endian byte-swaps each multi-byte value read from or written
 * into it.
 */
class CalibrationsHelperByteOrderTest {
    private static final String OLD_SIGNATURE = "rusEFI master.2026.11.02.test.123";
    private static final String NEW_SIGNATURE = "rusEFI master.2026.11.06.test.456";

    @TempDir
    Path tempDir;

    @Test
    void mergeWritesMigratedValuesInTheControllerByteOrder() throws Exception {
        IniFileModel before = bigEndianIni(OLD_SIGNATURE);
        IniFileModel after = bigEndianIni(NEW_SIGNATURE);
        assertEquals(ByteOrder.BIG_ENDIAN, after.getByteOrder());
        // pwm = 42 in the previous tune, 10 in the new defaults
        CalibrationsInfo source = new CalibrationsInfo(before,
            new ConfigurationImageWithMeta(null, new byte[]{0, 42, 0, 7}, ByteOrder.BIG_ENDIAN));
        CalibrationsInfo destination = new CalibrationsInfo(after,
            new ConfigurationImageWithMeta(null, new byte[]{0, 10, 0, 7}, ByteOrder.BIG_ENDIAN));

        CalibrationsHelper.MergeResult merged = CalibrationsHelper.mergeCalibrationsWithPartialFailure(
            before, source.generateMsq(), destination, UpdateOperationCallbacks.DUMMY, Collections.emptySet());

        assertTrue(merged.failedFields.isEmpty(), merged.failedFields.toString());
        ConfigurationImageWithMeta image = merged.mergedCalibrations.orElseThrow().getImage();
        assertEquals(ByteOrder.BIG_ENDIAN, image.getConfigurationImage().getByteOrder());
        assertArrayEquals(new byte[]{0, 42, 0, 7}, image.getConfigurationImage().getContent());
    }

    @Test
    void secondaryPagesReadFromTheControllerKeepItsByteOrder() throws Exception {
        IniFileModel ini = mock(IniFileModel.class);
        IniFileMetaInfo meta = mock(IniFileMetaInfo.class);
        when(ini.getSignature()).thenReturn(NEW_SIGNATURE);
        when(ini.getByteOrder()).thenReturn(ByteOrder.BIG_ENDIAN);
        when(ini.getMetaInfo()).thenReturn(meta);
        when(meta.getByteOrder()).thenReturn(ByteOrder.BIG_ENDIAN);
        when(meta.getnPages()).thenReturn(2);
        when(meta.getPageIdentifier(1)).thenReturn(0x0400);
        when(meta.getPageSize(0)).thenReturn(4);
        when(meta.getPageSize(1)).thenReturn(2);

        BinaryProtocol bp = protocol(NEW_SIGNATURE);
        when(bp.readFullImageFromController(any(ConfigurationImageMeta.class))).thenAnswer(call ->
            new ConfigurationImageWithMeta(call.getArgument(0), new byte[]{0, 10, 0, 7}, ByteOrder.BIG_ENDIAN));
        when(bp.readFromPage(0x0400, 0, 2)).thenReturn(new byte[]{0x12, 0x34});

        IniFileProvider previousProvider = BinaryProtocol.iniFileProvider;
        try {
            BinaryProtocol.iniFileProvider = signature -> ini;
            Optional<CalibrationsInfo> calibrations =
                CalibrationsHelper.readCurrentCalibrations(bp, UpdateOperationCallbacks.DUMMY);

            assertTrue(calibrations.isPresent());
            assertEquals(ByteOrder.BIG_ENDIAN,
                calibrations.get().getPage(0).getConfigurationImage().getByteOrder());
            assertEquals(ByteOrder.BIG_ENDIAN,
                calibrations.get().getPage(0x0400).getConfigurationImage().getByteOrder());
        } finally {
            BinaryProtocol.iniFileProvider = previousProvider;
        }
    }

    private static BinaryProtocol protocol(String signature) throws Exception {
        byte[] hello = new byte[signature.length() + 2];
        hello[0] = (byte) Integration.TS_RESPONSE_OK;
        System.arraycopy(signature.getBytes(StandardCharsets.US_ASCII), 0, hello, 1, signature.length());
        IoStream stream = mock(IoStream.class);
        IncomingDataBuffer buffer = mock(IncomingDataBuffer.class);
        when(stream.getDataBuffer()).thenReturn(buffer);
        doReturn(hello).when(buffer).getPacket(anyInt(), anyString());
        BinaryProtocol bp = mock(BinaryProtocol.class);
        when(bp.getStream()).thenReturn(stream);
        return bp;
    }

    private IniFileModel bigEndianIni(String signature) throws Exception {
        Path path = Files.createTempFile(tempDir, "bigEndian", ".ini");
        Files.write(path, ("[MegaTune]\nsignature = \"" + signature + "\"\n"
            + "[Constants]\nendianness = big\nnPages = 1\npageSize = 4\npageReadCommand = \"R\"\nburnCommand = \"B\"\npage = 1\n"
            + "pwm = scalar, U16, 0, \"\", 1, 0, 0, 65535, 0\n"
            + "other = scalar, U16, 2, \"\", 1, 0, 0, 65535, 0\n"
            + "[OutputChannels]\nochBlockSize = 4\n").getBytes(StandardCharsets.UTF_8));
        return IniFileReaderUtil.readIniFile(path.toString());
    }
}
