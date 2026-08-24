package com.rusefi.maintenance.migration;

import com.opensr5.ConfigurationImageWithMeta;
import com.opensr5.ConfigurationImageMetaVersion0_0;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.StringIniField;
import com.opensr5.io.ConfigurationImageFile;
import com.rusefi.maintenance.CalibrationsHelper;
import com.rusefi.maintenance.CalibrationsInfo;
import com.rusefi.maintenance.TestCallbacks;
import com.rusefi.maintenance.TestTuneMigrationContext;
import com.rusefi.maintenance.migration.migrators.DefaultTuneMigrator;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.nio.file.Path;
import java.util.Arrays;
import java.util.Collections;
import java.util.Map;
import java.util.Optional;
import java.util.Set;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.AdditionalMatchers.not;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

/**
 * Regression test for the migration of fields that moved from the main settings page to a
 * dedicated secondary TS page (e.g. luaScript since #9693).
 *
 * A legacy field can move from the monolithic main image to a dedicated page. Its page-relative
 * offset must be applied to that destination page without changing the main page.
 */
public class SecondaryPageFieldMigrationTest {
    private static final String LUA_SCRIPT = "luaScript";
    private static final String OLD_SCRIPT_VALUE = "\"function onTick() end\"";

    @TempDir
    Path tempDir;

    @Test
    public void secondaryPageFieldIsMigratedToItsDestinationPage() throws Exception {
        // luaScript was on the main page in the old .ini...
        final IniField prevField = new StringIniField(LUA_SCRIPT, 5716, 8000);
        // ...and moved to its own dedicated page in the new .ini.
        final IniField updatedField = new StringIniField(LUA_SCRIPT, 0, 8000);
        updatedField.setPageIndex(0x0400);
        final Constant prevConst = new Constant(LUA_SCRIPT, null, OLD_SCRIPT_VALUE, null);

        final IniFileModel prevIni = mock(IniFileModel.class);
        when(prevIni.getAllIniFields()).thenReturn(Collections.singletonMap(LUA_SCRIPT, prevField));
        when(prevIni.findIniField(eq(LUA_SCRIPT))).thenReturn(Optional.of(prevField));
        when(prevIni.findIniField(not(eq(LUA_SCRIPT)))).thenReturn(Optional.empty());

        final IniFileModel updatedIni = mock(IniFileModel.class);
        when(updatedIni.getAllIniFields()).thenReturn(Collections.emptyMap());
        when(updatedIni.getSecondaryIniFields()).thenReturn(Collections.singletonMap(LUA_SCRIPT, updatedField));
        when(updatedIni.findIniField(eq(LUA_SCRIPT))).thenReturn(Optional.of(updatedField));
        when(updatedIni.findIniField(not(eq(LUA_SCRIPT)))).thenReturn(Optional.empty());
        when(updatedIni.getSignature()).thenReturn("updated");
        final IniFileMetaInfo meta = mock(IniFileMetaInfo.class);
        when(meta.getnPages()).thenReturn(2);
        when(meta.getPageIdentifier(0)).thenReturn(0);
        when(meta.getPageIdentifier(1)).thenReturn(0x0400);
        when(meta.getPageSize(0)).thenReturn(8000);
        when(meta.getPageSize(1)).thenReturn(8000);
        when(meta.getBurnCommand(0)).thenReturn("B");
        when(meta.getBurnCommand(1)).thenReturn("B");
        when(updatedIni.getMetaInfo()).thenReturn(meta);

        final Msq prevMsq = mock(Msq.class);
        when(prevMsq.getConstantsAsMap()).thenReturn(Collections.singletonMap(LUA_SCRIPT, prevConst));

        final byte[] mainContent = new byte[8000];
        Arrays.fill(mainContent, (byte) 0x5a);
        final CalibrationsInfo updatedCalibrations = new CalibrationsInfo(
            updatedIni,
            Map.of(
                0, new ConfigurationImageWithMeta(
                    new ConfigurationImageMetaVersion0_0(8000, "updated"), mainContent),
                0x0400, new ConfigurationImageWithMeta(
                    new ConfigurationImageMetaVersion0_0(8000, "updated"), new byte[8000])
            ),
            Collections.emptySet()
        );

        final Optional<CalibrationsInfo> result = CalibrationsHelper.mergeCalibrations(
            prevIni,
            prevMsq,
            updatedCalibrations,
            new TestCallbacks(),
            Collections.emptySet()
        );

        assertTrue(result.isPresent());
        assertArrayEquals(mainContent, result.get().getImage().getConfigurationImage().getContent());
        assertEquals(
            OLD_SCRIPT_VALUE,
            com.opensr5.ConfigurationImageGetterSetter.getStringValue(
                updatedField,
                result.get().getPage(0x0400).getConfigurationImage()
            )
        );
        assertEquals(Set.of(0x0400), result.get().getPagesToWrite());
        assertEquals(
            OLD_SCRIPT_VALUE,
            result.get().generateMsq().getConstantsAsMap().get(LUA_SCRIPT).getValue()
        );
        assertEquals("2", result.get().generateMsq().versionInfo.getNPages());

        Path backup = tempDir.resolve("multipage.zip");
        ConfigurationImageFile.saveToFile(result.get().getPages(), backup.toString());
        Map<Integer, ConfigurationImageWithMeta> restoredPages =
            ConfigurationImageFile.readPagesFromFile(backup.toString());
        assertArrayEquals(
            result.get().getImage().getConfigurationImage().getContent(),
            restoredPages.get(0).getConfigurationImage().getContent()
        );
        assertArrayEquals(
            result.get().getPage(0x0400).getConfigurationImage().getContent(),
            restoredPages.get(0x0400).getConfigurationImage().getContent()
        );
    }

    @Test
    public void secondaryPageFieldIsPreservedOnLaterMigrations() {
        final IniField prevField = new StringIniField(LUA_SCRIPT, 0, 8000);
        prevField.setPageIndex(0x0400);
        final IniField updatedField = new StringIniField(LUA_SCRIPT, 0, 8000);
        updatedField.setPageIndex(0x0400);
        final Constant prevValue = new Constant(LUA_SCRIPT, null, OLD_SCRIPT_VALUE, null);
        final Constant updatedValue = new Constant(LUA_SCRIPT, null, "default", null);

        final IniFileModel prevIni = mock(IniFileModel.class);
        when(prevIni.getAllIniFields()).thenReturn(Collections.emptyMap());
        when(prevIni.getSecondaryIniFields()).thenReturn(Collections.singletonMap(LUA_SCRIPT, prevField));
        final IniFileModel updatedIni = mock(IniFileModel.class);
        when(updatedIni.findIniField(LUA_SCRIPT)).thenReturn(Optional.of(updatedField));
        final IniFileMetaInfo meta = mock(IniFileMetaInfo.class);
        when(meta.getnPages()).thenReturn(2);
        when(meta.getPageIdentifier(0)).thenReturn(0);
        when(meta.getPageIdentifier(1)).thenReturn(0x0400);
        when(meta.getBurnCommand(0)).thenReturn("B");
        when(meta.getBurnCommand(1)).thenReturn("B");
        when(prevIni.getMetaInfo()).thenReturn(meta);
        when(updatedIni.getMetaInfo()).thenReturn(meta);
        final Msq prevMsq = mock(Msq.class);
        when(prevMsq.getConstantsAsMap()).thenReturn(Collections.singletonMap(LUA_SCRIPT, prevValue));
        final Msq updatedMsq = mock(Msq.class);
        when(updatedMsq.getConstantsAsMap()).thenReturn(Collections.singletonMap(LUA_SCRIPT, updatedValue));
        final TestTuneMigrationContext context =
            new TestTuneMigrationContext(prevMsq, prevIni, updatedMsq, updatedIni, new TestCallbacks());

        DefaultTuneMigrator.INSTANCE.migrateTune(context);

        assertEquals(OLD_SCRIPT_VALUE, context.getMigratedValue(LUA_SCRIPT).getValue());
    }
}
