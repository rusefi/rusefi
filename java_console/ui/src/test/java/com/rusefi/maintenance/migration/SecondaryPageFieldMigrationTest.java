package com.rusefi.maintenance.migration;

import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.StringIniField;
import com.rusefi.maintenance.TestCallbacks;
import com.rusefi.maintenance.TestTuneMigrationContext;
import com.rusefi.maintenance.migration.migrators.DefaultTuneMigrator;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;
import org.junit.jupiter.api.Test;

import java.util.Collections;
import java.util.Optional;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.mockito.AdditionalMatchers.not;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

/**
 * Regression test for the migration of fields that moved from the main settings page to a
 * dedicated secondary TS page (e.g. luaScript since #9693).
 *
 * Such a field is present in {@link IniFileModel#findIniField(String)} but NOT in
 * {@link IniFileModel#getAllIniFields()} (which only holds main-page fields). DefaultTuneMigrator
 * used to resolve the new field via getAllIniFields().get(name), so the moved field looked
 * "missed in new .ini file" and its value was silently dropped from the migrated tune.
 */
public class SecondaryPageFieldMigrationTest {
    private static final String LUA_SCRIPT = "luaScript";
    private static final String OLD_SCRIPT_VALUE = "\"function onTick() end\"";

    @Test
    public void secondaryPageFieldIsMigrated() {
        // luaScript was on the main page in the old .ini...
        final IniField prevField = new StringIniField(LUA_SCRIPT, 5716, 8000);
        // ...and moved to its own dedicated page in the new .ini.
        final IniField updatedField = new StringIniField(LUA_SCRIPT, 0, 8000);
        final Constant prevConst = new Constant(LUA_SCRIPT, null, OLD_SCRIPT_VALUE, null);

        final IniFileModel prevIni = mock(IniFileModel.class);
        when(prevIni.getAllIniFields()).thenReturn(Collections.singletonMap(LUA_SCRIPT, prevField));
        when(prevIni.findIniField(eq(LUA_SCRIPT))).thenReturn(Optional.of(prevField));
        when(prevIni.findIniField(not(eq(LUA_SCRIPT)))).thenReturn(Optional.empty());

        final IniFileModel updatedIni = mock(IniFileModel.class);
        // the moved field is NOT in getAllIniFields() (it lives on a secondary page),
        // but it IS resolvable via findIniField().
        when(updatedIni.getAllIniFields()).thenReturn(Collections.emptyMap());
        when(updatedIni.findIniField(eq(LUA_SCRIPT))).thenReturn(Optional.of(updatedField));
        when(updatedIni.findIniField(not(eq(LUA_SCRIPT)))).thenReturn(Optional.empty());

        final Msq prevMsq = mock(Msq.class);
        when(prevMsq.getConstantsAsMap()).thenReturn(Collections.singletonMap(LUA_SCRIPT, prevConst));
        final Msq updatedMsq = mock(Msq.class);
        when(updatedMsq.getConstantsAsMap()).thenReturn(Collections.emptyMap());

        final TestTuneMigrationContext context =
            new TestTuneMigrationContext(prevMsq, prevIni, updatedMsq, updatedIni, new TestCallbacks());

        DefaultTuneMigrator.INSTANCE.migrateTune(context);

        final Constant migrated = context.getMigratedConstants().get(LUA_SCRIPT);
        assertNotNull(migrated, "Secondary-page field must be migrated, not dropped as 'missed in new .ini'");
        assertEquals(OLD_SCRIPT_VALUE, migrated.getValue());
    }
}
