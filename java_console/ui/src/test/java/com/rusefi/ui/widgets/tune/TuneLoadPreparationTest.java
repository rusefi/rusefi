package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.StringIniField;
import com.rusefi.config.FieldType;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;
import com.rusefi.tune.xml.Page;
import org.junit.jupiter.api.Test;

import java.nio.ByteOrder;
import java.util.Arrays;
import java.util.Collections;
import java.util.Map;
import java.util.Optional;
import java.util.Set;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.ArgumentMatchers.anyString;
import static org.mockito.Mockito.*;

class TuneLoadPreparationTest {
    @Test
    void pcMetadataNeverWritesToTheEcuPageImage() {
        EnumIniField labels = new EnumIniField("unitsLabels", 0, FieldType.UINT8,
            new EnumIniField.EnumKeyValueMap(Map.of(0, "F", 1, "C")), 0, 0);
        IniFileModel ini = mock(IniFileModel.class);
        when(ini.getSignature()).thenReturn("same");
        when(ini.getPcVariableNames()).thenReturn(Set.of("unitsLabels"));
        when(ini.findIniField("unitsLabels")).thenReturn(Optional.of(labels));
        IniFileMetaInfo meta = metaForMainPage(1);
        when(ini.getMetaInfo()).thenReturn(meta);
        when(ini.getByteOrder()).thenReturn(ByteOrder.LITTLE_ENDIAN);
        Msq tune = new Msq();
        tune.getVersionInfo().setSignature("same");
        Page page = new Page(0, 1);
        page.constant.add(new Constant("unitsLabels", null, "\"C\"", null));
        tune.page.add(page);
        assertArrayEquals(new byte[]{0}, TuneLoadPreparation.openOffline(tune, ini).get(0).getContent());
    }

    @Test
    void offlineOpenRejectsInvalidKnownValuesButRetainsLegacyBlankEnumBehavior() {
        EnumIniField mode = new EnumIniField("mode", 0, FieldType.UINT8,
            new EnumIniField.EnumKeyValueMap(Map.of(0, "\"Off\"", 1, "\"On\"")), 0, 0);
        IniFileModel ini = mock(IniFileModel.class);
        when(ini.getSignature()).thenReturn("same");
        when(ini.getAllIniFields()).thenReturn(Collections.singletonMap("mode", mode));
        when(ini.findIniField("mode")).thenReturn(Optional.of(mode));
        IniFileMetaInfo meta = metaForMainPage(1);
        when(ini.getMetaInfo()).thenReturn(meta);
        when(ini.getByteOrder()).thenReturn(ByteOrder.LITTLE_ENDIAN);

        Msq blankEnum = new Msq();
        blankEnum.getVersionInfo().setSignature("same");
        Page blankPage = new Page(0, 1);
        blankPage.constant.add(new Constant("mode", null, "", null));
        blankEnum.page.add(blankPage);
        Map<Integer, ConfigurationImage> opened = TuneLoadPreparation.openOffline(blankEnum, ini);
        assertEquals(0, opened.get(0).getContent()[0]);

        blankPage.constant.get(0).setValue("\"Missing\"");
        assertThrows(IllegalArgumentException.class, () -> TuneLoadPreparation.openOffline(blankEnum, ini));
    }

    @Test
    void migratesSecondaryPageLuaToItsNewSecondaryPage() {
        String script = "function onTick() end";
        IniField oldLua = new StringIniField("luaScript", 4, 32);
        oldLua.setPageIndex(0x0200);
        IniFileModel sourceIni = mock(IniFileModel.class);
        when(sourceIni.getSignature()).thenReturn("old-signature");
        when(sourceIni.getAllIniFields()).thenReturn(Collections.emptyMap());
        when(sourceIni.getSecondaryIniFields()).thenReturn(Collections.singletonMap("luaScript", oldLua));
        IniFileMetaInfo sourceMeta = mock(IniFileMetaInfo.class);
        when(sourceMeta.getnPages()).thenReturn(2);
        when(sourceMeta.getPageIdentifier(0)).thenReturn(0);
        when(sourceMeta.getPageIdentifier(1)).thenReturn(0x0200);
        when(sourceMeta.getPageSize(0)).thenReturn(64);
        when(sourceMeta.getPageSize(1)).thenReturn(32);
        when(sourceMeta.getBurnCommand(1)).thenReturn("B");
        when(sourceIni.getMetaInfo()).thenReturn(sourceMeta);

        StringIniField targetLua = new StringIniField("luaScript", 0, 32);
        targetLua.setPageIndex(0x0400);
        IniFileModel targetIni = mock(IniFileModel.class);
        when(targetIni.getSignature()).thenReturn("new-signature");
        when(targetIni.getAllIniFields()).thenReturn(Collections.emptyMap());
        when(targetIni.getSecondaryIniFields()).thenReturn(Collections.singletonMap("luaScript", targetLua));
        when(targetIni.findIniField(anyString())).thenAnswer(invocation ->
            "luaScript".equalsIgnoreCase(invocation.getArgument(0))
                ? Optional.of(targetLua) : Optional.empty());
        when(targetIni.getByteOrder()).thenReturn(ByteOrder.LITTLE_ENDIAN);
        IniFileMetaInfo meta = metaForTwoPages(64, 32, 0x0400);
        when(meta.getBurnCommand(1)).thenReturn("B");
        when(targetIni.getMetaInfo()).thenReturn(meta);

        Msq oldTune = new Msq();
        oldTune.getVersionInfo().setSignature("old-signature");
        Page oldSecondary = new Page(1, 32);
        oldSecondary.constant.add(new Constant("luaScript", null, script, null));
        oldTune.page.add(oldSecondary);
        ConfigurationImage main = new ConfigurationImage(64, ByteOrder.LITTLE_ENDIAN);
        Arrays.fill(main.getContent(), (byte) 0x5a);
        ConfigurationImage secondary = new ConfigurationImage(32, ByteOrder.LITTLE_ENDIAN);

        TuneLoadPreparation prepared = TuneLoadPreparation.prepare(
            oldTune, sourceIni, targetIni,
            Map.of(0, main, 0x0400, secondary),
            UpdateOperationCallbacks.DUMMY);

        assertArrayEquals(main.getContent(), prepared.incoming.get(0).getContent());
        assertEquals(script, ConfigurationImageGetterSetter.getStringValue(targetLua, prepared.incoming.get(0x0400)));
        assertTrue(prepared.warnings.stream().noneMatch(warning -> warning.contains("luaScript")));

        for (String invalid : new String[]{"non-ASCII \u00e9", "embedded\u0000NUL"}) {
            oldSecondary.constant.get(0).setValue(invalid);
            TuneLoadPreparation rejected = TuneLoadPreparation.prepare(
                oldTune, sourceIni, targetIni,
                Map.of(0, main, 0x0400, secondary),
                UpdateOperationCallbacks.DUMMY);
            assertArrayEquals(secondary.getContent(), rejected.incoming.get(0x0400).getContent());
            assertTrue(rejected.warnings.stream().anyMatch(warning -> warning.contains("NUL-free ASCII")));
        }
        oldSecondary.constant.get(0).setValue(script);
        oldTune.getVersionInfo().setSignature("wrong-signature");
        TuneLoadPreparation wrongSource = TuneLoadPreparation.prepare(oldTune, sourceIni, targetIni,
            Map.of(0, main, 0x0400, secondary), UpdateOperationCallbacks.DUMMY);
        assertArrayEquals(secondary.getContent(), wrongSource.incoming.get(0x0400).getContent());
        assertTrue(wrongSource.warnings.stream().anyMatch(warning -> warning.contains("does not match")));
    }

    private static IniFileMetaInfo metaForMainPage(int pageSize) {
        IniFileMetaInfo meta = mock(IniFileMetaInfo.class);
        when(meta.getnPages()).thenReturn(1);
        when(meta.getPageIdentifier(0)).thenReturn(0);
        when(meta.getPageSize(0)).thenReturn(pageSize);
        return meta;
    }

    private static IniFileMetaInfo metaForTwoPages(int mainSize, int secondarySize, int secondaryId) {
        IniFileMetaInfo meta = mock(IniFileMetaInfo.class);
        when(meta.getnPages()).thenReturn(2);
        when(meta.getPageIdentifier(0)).thenReturn(0);
        when(meta.getPageIdentifier(1)).thenReturn(secondaryId);
        when(meta.getPageSize(0)).thenReturn(mainSize);
        when(meta.getPageSize(1)).thenReturn(secondarySize);
        return meta;
    }
}
