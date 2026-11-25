package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileModel;
import com.rusefi.binaryprotocol.BinaryProtocol;
import org.junit.jupiter.api.Test;

import java.nio.ByteOrder;
import java.util.Collections;
import java.util.Map;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

class TunePageIoTest {
    private static final int PAGE_ID = 0x0100;

    @Test
    void readsByWireIdAndWarnsInsteadOfReturningFallbackData() {
        BinaryProtocol bp = mock(BinaryProtocol.class);
        IniFileModel ini = ini(2, 4, "B");
        when(bp.getWireByteOrder()).thenReturn(ByteOrder.LITTLE_ENDIAN);
        when(bp.readFromPage(PAGE_ID, 0, 4)).thenReturn(null);
        java.util.List<String> warnings = new java.util.ArrayList<>();

        Map<Integer, ConfigurationImage> result = TunePageIo.readSecondary(bp, ini, warnings);

        assertTrue(result.isEmpty());
        assertEquals(1, warnings.size());
        verify(bp).readFromPage(PAGE_ID, 0, 4);
    }

    @Test
    void writesOnlyChangedRangesAndVerifiesTheWholePage() {
        BinaryProtocol bp = mock(BinaryProtocol.class);
        IniFileModel model = ini(2, 12, "B");
        when(bp.getIniFile()).thenReturn(model);
        when(bp.readFromPage(PAGE_ID, 0, 12)).thenReturn(new byte[] {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0});
        ConfigurationImage original = image(new byte[12]);
        ConfigurationImage target = image(new byte[] {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0});

        TunePageIo.uploadSecondary(bp, Collections.singletonMap(PAGE_ID, original),
            Collections.singletonMap(PAGE_ID, target));

        verify(bp).writeInBlocks(target.getContent(), 0, 0, 4, PAGE_ID);
        verify(bp).writeInBlocks(target.getContent(), 10, 10, 2, PAGE_ID);
        verify(bp).readFromPage(PAGE_ID, 0, 12);
        verify(bp, never()).burnPage(anyInt());
    }

    @Test
    void requiresBaseAndRejectsMainPage() {
        BinaryProtocol bp = mock(BinaryProtocol.class);
        IniFileModel model = ini(2, 2, "B");
        when(bp.getIniFile()).thenReturn(model);
        Map<Integer, ConfigurationImage> selected = Collections.singletonMap(PAGE_ID, image(new byte[] {1, 2}));
        assertThrows(IllegalStateException.class, () -> TunePageIo.uploadSecondary(bp, Collections.emptyMap(), selected));
        assertThrows(IllegalArgumentException.class,
            () -> TunePageIo.uploadSecondary(bp, Collections.emptyMap(), Collections.singletonMap(0, image(new byte[2]))));
    }

    @Test
    void secondaryBurnFailsClosedWhenProtocolRejectsIt() {
        BinaryProtocol bp = mock(BinaryProtocol.class);
        IniFileModel model = ini(2, 2, "B");
        when(bp.getIniFile()).thenReturn(model);
        when(bp.burnPage(PAGE_ID)).thenReturn(false);

        assertThrows(IllegalStateException.class,
            () -> TunePageIo.burnSecondary(bp, Collections.singleton(PAGE_ID)));
        verify(bp).burnPage(PAGE_ID);
    }

    private static ConfigurationImage image(byte[] bytes) {
        return new ConfigurationImage(bytes, ByteOrder.LITTLE_ENDIAN);
    }

    private static IniFileModel ini(int pageCount, int pageSize, String burnCommand) {
        IniFileMetaInfo meta = mock(IniFileMetaInfo.class);
        when(meta.getnPages()).thenReturn(pageCount);
        when(meta.getPageIdentifier(1)).thenReturn(PAGE_ID);
        when(meta.getPageSize(1)).thenReturn(pageSize);
        when(meta.getBurnCommand(1)).thenReturn(burnCommand);
        IniFileModel ini = mock(IniFileModel.class);
        when(ini.getMetaInfo()).thenReturn(meta);
        return ini;
    }
}
