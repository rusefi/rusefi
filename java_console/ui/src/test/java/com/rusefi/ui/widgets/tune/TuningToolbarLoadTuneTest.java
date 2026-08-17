package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ConfigurationImageMetaVersion0_0;
import com.opensr5.ConfigurationImageWithMeta;
import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.StringIniField;
import com.rusefi.maintenance.CalibrationsHelper;
import com.rusefi.maintenance.CalibrationsInfo;
import com.rusefi.maintenance.TestCallbacks;
import com.rusefi.maintenance.TestTuneMigrationContext;
import com.rusefi.maintenance.migration.afr_msq_import_migration.AfrMsqImportMigrationContext;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;
import org.junit.jupiter.api.Test;

import java.util.Arrays;
import java.util.Collections;
import java.util.Map;
import java.util.Optional;
import java.util.Set;
import java.util.TreeMap;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.AdditionalMatchers.not;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

class TuningToolbarLoadTuneTest {
    @Test
    void connectedLoadAfrLambdaTableEncoding() throws Exception {
        TestTuneMigrationContext context = AfrMsqImportMigrationContext.loadAfrToLambda();
        IniFileModel targetIni = context.getUpdatedIniFile();
        ConfigurationImage targetImage = context.getUpdatedTune().asImage(targetIni);

        Map<Integer, ConfigurationImageWithMeta> targetPages = Map.of(
            0, ConfigurationImageWithMeta.valueOf(targetIni, targetImage));

        CalibrationsHelper.MergeResult result = TuningToolbarWidget.mergeLoadedTune(
            context.getPrevTune(), context.getPrevIniFile(), targetIni, targetPages, context.getCallbacks());

        ConfigurationImage loaded = result.mergedCalibrations.orElseThrow().getImage().getConfigurationImage();
        ArrayIniField field = (ArrayIniField) targetIni.findIniField("lambdaTable").orElseThrow();
        assertEquals(147, rawValue(loaded, field, 0, 0));
        assertEquals(140, rawValue(loaded, field, 8, 4));
        assertEquals(126, rawValue(loaded, field, 11, 13));
    }

    @Test
    void secondaryPageFieldsAreMergedOnConnectedLoad() {
        final String luaScript = "luaScript";
        final String scriptValue = "\"function onTick() end\"";

        // luaScript lives on its own TS page (identifier 0x0400) in both inis
        final IniField prevField = new StringIniField(luaScript, 0, 8000);
        prevField.setPageIndex(0x0400);
        final Constant prevConst = new Constant(luaScript, null, scriptValue, null);

        final IniFileMetaInfo meta = meta();
        final IniFileModel prevIni = mock(IniFileModel.class);
        when(prevIni.getAllIniFields()).thenReturn(Collections.emptyMap());
        when(prevIni.getSecondaryIniFields()).thenReturn(Collections.singletonMap(luaScript, prevField));
        when(prevIni.getMetaInfo()).thenReturn(meta);

        final IniFileModel targetIni = mock(IniFileModel.class);
        when(targetIni.getAllIniFields()).thenReturn(Collections.emptyMap());
        when(targetIni.getSecondaryIniFields()).thenReturn(Collections.singletonMap(luaScript, prevField));
        when(targetIni.findIniField(eq(luaScript))).thenReturn(Optional.of(prevField));
        when(targetIni.findIniField(not(eq(luaScript)))).thenReturn(Optional.empty());
        when(targetIni.getSignature()).thenReturn("test");
        when(targetIni.getMetaInfo()).thenReturn(meta);

        final Msq prevMsq = mock(Msq.class);
        when(prevMsq.getConstantsAsMap()).thenReturn(Collections.singletonMap(luaScript, prevConst));

        byte[] main = new byte[8000];
        Arrays.fill(main, (byte) 0x5a);
        Map<Integer, ConfigurationImageWithMeta> targetPages = new TreeMap<>();
        targetPages.put(0, new ConfigurationImageWithMeta(
            new ConfigurationImageMetaVersion0_0(8000, "test"), main));
        targetPages.put(0x0400, new ConfigurationImageWithMeta(
            new ConfigurationImageMetaVersion0_0(8000, "test"), new byte[8000]));

        CalibrationsHelper.MergeResult result = TuningToolbarWidget.mergeLoadedTune(
            prevMsq, prevIni, targetIni, targetPages, new TestCallbacks());

        assertTrue(result.mergedCalibrations.isPresent());
        CalibrationsInfo migrated = result.mergedCalibrations.get();
        assertEquals(Set.of(0x0400), migrated.getPagesToWrite());
        assertTrue(result.failedFields.isEmpty());
        assertEquals(
            scriptValue,
            ConfigurationImageGetterSetter.getStringValue(
                prevField, migrated.getPage(0x0400).getConfigurationImage())
        );
    }

    private static IniFileMetaInfo meta() {
        final IniFileMetaInfo meta = mock(IniFileMetaInfo.class);
        when(meta.getnPages()).thenReturn(2);
        when(meta.getPageIdentifier(0)).thenReturn(0);
        when(meta.getPageIdentifier(1)).thenReturn(0x0400);
        when(meta.getPageSize(0)).thenReturn(8000);
        when(meta.getPageSize(1)).thenReturn(8000);
        when(meta.getBurnCommand(0)).thenReturn("B");
        when(meta.getBurnCommand(1)).thenReturn("B");
        return meta;
    }

    private static int rawValue(ConfigurationImage image, ArrayIniField field, int row, int column) {
        return Byte.toUnsignedInt(image.getContent()[field.getOffset(row, column)]);
    }
}
