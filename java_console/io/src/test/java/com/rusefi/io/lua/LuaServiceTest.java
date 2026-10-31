package com.rusefi.io.lua;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniFileModelMocks;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.StringIniField;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.LinkManager;
import org.junit.jupiter.api.Test;

import java.nio.charset.StandardCharsets;
import java.util.Optional;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertSame;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.anyInt;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

/**
 * Covers the page-aware LUASCRIPT lookup and download paths: old firmware keeps the
 * script on the main settings page (served from the cached controller image), new
 * firmware keeps it on a dedicated TS page (fetched directly from the ECU).
 */
public class LuaServiceTest {
    private static final int TS_PAGE_LUA = 0x0400;

    private static BinaryProtocol mockBpWithField(IniField field) {
        BinaryProtocol bp = mock(BinaryProtocol.class);
        IniFileModel ini = IniFileModelMocks.empty();
        when(ini.findIniField(LuaService.LUASCRIPT_FIELD)).thenReturn(Optional.ofNullable(field));
        when(bp.getIniFileNullable()).thenReturn(ini);
        return bp;
    }

    private static LinkManager linkManagerWith(BinaryProtocol bp) {
        LinkManager linkManager = mock(LinkManager.class);
        when(linkManager.getBinaryProtocol()).thenReturn(bp);
        // run link-thread work inline so ECU-touching paths execute synchronously under test
        when(linkManager.submit(any(Runnable.class))).thenAnswer(invocation -> {
            ((Runnable) invocation.getArgument(0)).run();
            return null;
        });
        return linkManager;
    }

    private static byte[] paddedScript(String script, int fieldSize) {
        byte[] padded = new byte[fieldSize];
        byte[] scriptBytes = script.getBytes(StandardCharsets.US_ASCII);
        System.arraycopy(scriptBytes, 0, padded, 0, scriptBytes.length);
        return padded;
    }

    @Test
    public void fieldLookupWithoutIniReturnsNull() {
        BinaryProtocol bp = mock(BinaryProtocol.class);
        when(bp.getIniFileNullable()).thenReturn(null);
        assertNull(LuaService.getLuaScriptField(bp));
    }

    @Test
    public void fieldLookupWithoutFieldReturnsNull() {
        assertNull(LuaService.getLuaScriptField(mockBpWithField(null)));
    }

    @Test
    public void fieldLookupIgnoresNonStringField() {
        // wrong field type must degrade to null, not ClassCastException
        assertNull(LuaService.getLuaScriptField(mockBpWithField(mock(IniField.class))));
    }

    @Test
    public void fieldLookupFindsFieldOnSecondaryPage() {
        StringIniField field = new StringIniField(LuaService.LUASCRIPT_FIELD, 0, 12000);
        field.setPageIndex(TS_PAGE_LUA);
        assertSame(field, LuaService.getLuaScriptField(mockBpWithField(field)));
    }

    @Test
    public void mainPageScriptComesFromCachedImage() throws Exception {
        StringIniField field = new StringIniField(LuaService.LUASCRIPT_FIELD, 8, 16);
        BinaryProtocol bp = mockBpWithField(field);

        byte[] image = new byte[64];
        System.arraycopy(paddedScript("hello", 16), 0, image, 8, 16);
        when(bp.getControllerConfiguration()).thenReturn(new ConfigurationImage(image));

        assertEquals("hello", LuaService.readLuaScript(linkManagerWith(bp)));
        verify(bp, never()).readFromPage(anyInt(), anyInt(), anyInt());
    }

    @Test
    public void secondaryPageScriptIsFetchedFromEcu() throws Exception {
        StringIniField field = new StringIniField(LuaService.LUASCRIPT_FIELD, 0, 16);
        field.setPageIndex(TS_PAGE_LUA);
        BinaryProtocol bp = mockBpWithField(field);
        when(bp.readFromPage(TS_PAGE_LUA, 0, 16)).thenReturn(paddedScript("lua!", 16));

        assertEquals("lua!", LuaService.readLuaScript(linkManagerWith(bp)));
        // cached image only holds page 0 and must not be consulted
        verify(bp, never()).getControllerConfiguration();
    }

    @Test
    public void scriptFillingWholeFieldIsNotTruncated() throws Exception {
        StringIniField field = new StringIniField(LuaService.LUASCRIPT_FIELD, 0, 4);
        field.setPageIndex(TS_PAGE_LUA);
        BinaryProtocol bp = mockBpWithField(field);
        when(bp.readFromPage(TS_PAGE_LUA, 0, 4)).thenReturn("full".getBytes(StandardCharsets.US_ASCII));

        assertEquals("full", LuaService.readLuaScript(linkManagerWith(bp)));
    }

    @Test
    public void secondaryPageReadFailureThrows() {
        StringIniField field = new StringIniField(LuaService.LUASCRIPT_FIELD, 0, 16);
        field.setPageIndex(TS_PAGE_LUA);
        BinaryProtocol bp = mockBpWithField(field);
        when(bp.readFromPage(TS_PAGE_LUA, 0, 16)).thenReturn(null);

        assertThrows(IllegalStateException.class, () -> LuaService.readLuaScript(linkManagerWith(bp)));
    }

    @Test
    public void mainPageWithoutCachedImageThrows() {
        StringIniField field = new StringIniField(LuaService.LUASCRIPT_FIELD, 0, 16);
        BinaryProtocol bp = mockBpWithField(field);
        when(bp.getControllerConfiguration()).thenReturn(null);

        assertThrows(IllegalStateException.class, () -> LuaService.readLuaScript(linkManagerWith(bp)));
    }

    @Test
    public void missingBinaryProtocolThrows() {
        assertThrows(IllegalStateException.class, () -> LuaService.readLuaScript(linkManagerWith(null)));
    }

    @Test
    public void missingFieldThrows() {
        assertThrows(IllegalStateException.class,
                () -> LuaService.readLuaScript(linkManagerWith(mockBpWithField(null))));
    }
}
