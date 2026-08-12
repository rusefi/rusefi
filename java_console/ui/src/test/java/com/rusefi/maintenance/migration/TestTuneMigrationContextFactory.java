package com.rusefi.maintenance.migration;

import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.IniField;
import com.rusefi.maintenance.TestCallbacks;
import com.rusefi.maintenance.TestTuneMigrationContext;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;

import java.util.Map;
import java.util.Optional;

import static org.mockito.AdditionalMatchers.not;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

public class TestTuneMigrationContextFactory {
    public static TestTuneMigrationContext createTestMigrationContext(
        final Constant prevConst,
        final IniField prevIniField,
        final Constant updatedConst,
        final IniField updatedIniField
    ) {
        final IniFileModel prevIniFile = mock(IniFileModel.class);
        when(prevIniFile.findIniField(not(eq(prevIniField.getName())))).thenReturn(Optional.empty());
        when(prevIniFile.findIniField(prevIniField.getName())).thenReturn(Optional.of(prevIniField));
        when(prevIniFile.getAllIniFields()).thenReturn(Map.of(prevIniField.getName(), prevIniField));

        final Msq prevMsq = mock(Msq.class);
        when(prevMsq.getConstantsAsMap()).thenReturn(Map.of(prevConst.getName(), prevConst));

        final IniFileModel updatedIniFile = mock(IniFileModel.class);
        when(updatedIniFile.findIniField(not(eq(updatedIniField.getName())))).thenReturn(Optional.empty());
        when(updatedIniFile.findIniField(updatedIniField.getName())).thenReturn(Optional.of(updatedIniField));
        when(updatedIniFile.getAllIniFields()).thenReturn(Map.of(updatedIniField.getName(), updatedIniField));

        final Msq updatedMsq = mock(Msq.class);
        when(updatedMsq.getConstantsAsMap()).thenReturn(Map.of(updatedConst.getName(), updatedConst));

        return new TestTuneMigrationContext(prevMsq, prevIniFile, updatedMsq, updatedIniFile, new TestCallbacks());
    }

    public static TestTuneMigrationContext createTestMigrationContextWithDisappearedConst(
        final Constant prevConst,
        final IniField prevIniField
    ) {
        final IniFileModel prevIniFile = mock(IniFileModel.class);
        when(prevIniFile.findIniField(not(eq(prevIniField.getName())))).thenReturn(Optional.empty());
        when(prevIniFile.findIniField(prevIniField.getName())).thenReturn(Optional.of(prevIniField));
        when(prevIniFile.getAllIniFields()).thenReturn(Map.of(prevIniField.getName(), prevIniField));

        final Msq prevMsq = mock(Msq.class);
        when(prevMsq.getConstantsAsMap()).thenReturn(Map.of(prevConst.getName(), prevConst));

        final IniFileModel updatedIniFile = mock(IniFileModel.class);
        when(updatedIniFile.findIniField(any())).thenReturn(Optional.empty());
        when(updatedIniFile.getAllIniFields()).thenReturn(Map.of());

        final Msq updatedMsq = mock(Msq.class);
        when(updatedMsq.getConstantsAsMap()).thenReturn(Map.of());

        return new TestTuneMigrationContext(prevMsq, prevIniFile, updatedMsq, updatedIniFile, new TestCallbacks());
    }
}
