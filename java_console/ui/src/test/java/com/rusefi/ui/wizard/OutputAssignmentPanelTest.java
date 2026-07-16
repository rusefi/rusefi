package com.rusefi.ui.wizard;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.IniFileMetaInfoImpl;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Integration;
import com.rusefi.config.generated.VariableRegistryValues;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.ConnectionStatusValue;
import com.rusefi.ui.UIContext;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.io.InputStream;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.inOrder;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

public class OutputAssignmentPanelTest {
    @Test
    void configuredCylinderCountControlsRowsAndLoadsAssignments() throws Throwable {
        UIContext context = configuredContext(4, "ignitionPins", "NONE");

        OutputAssignmentPanel panel = new OutputAssignmentPanel(
            context, OutputAssignmentPanel.OutputType.IGNITION);

        assertEquals(4, panel.getRowCountForTests());
        for (int index = 1; index <= 4; index++) {
            assertEquals("NONE", panel.getEditorForTests(index).getSelectedItem());
        }
    }

    @Test
    void invalidAndOutOfRangeAssignmentsRequireSelection() throws Throwable {
        UIContext context = configuredContext(2, "injectionPins", "NONE");
        WizardConfig cfg = WizardConfig.snapshot(context);
        assertNotNull(cfg);
        ConfigurationImage image = cfg.image.clone();

        setValue(cfg, image, "injectionPins1", "INVALID");
        EnumIniField second = (EnumIniField) cfg.ini.findIniField("injectionPins2").orElseThrow();
        image.setBitValue(second, (1 << (second.getBitSize0() + 1)) - 1);
        cfg.bp.setConfigurationImage(image);

        OutputAssignmentPanel panel = new OutputAssignmentPanel(
            context, OutputAssignmentPanel.OutputType.INJECTOR);

        assertNull(panel.getEditorForTests(1).getSelectedItem());
        assertNull(panel.getEditorForTests(2).getSelectedItem());
        assertNull(panel.buildModifiedImageForTests());
    }

    @Test
    void modifiedImageContainsVisibleSelections() throws Throwable {
        UIContext context = configuredContext(1, "ignitionPins", "NONE");
        OutputAssignmentPanel panel = new OutputAssignmentPanel(
            context, OutputAssignmentPanel.OutputType.IGNITION);
        JComboBox<String> editor = panel.getEditorForTests(1);
        String replacement = firstNonNone(editor);
        editor.setSelectedItem(replacement);

        ConfigurationImage modified = panel.buildModifiedImageForTests();
        assertNotNull(modified);
        WizardConfig cfg = WizardConfig.snapshot(context);
        IniField field = cfg.ini.findIniField("ignitionPins1").orElseThrow();
        assertEquals(replacement, AbstractWizardStep.readValue(field, modified));
    }

    @Test
    void payloadUsesBigEndianCategoryAndOneBasedIndex() {
        assertArrayEquals(new byte[]{0, 18, 0, 4}, OutputAssignmentPanel.buildTestPayload(
            VariableRegistryValues.ts_command_e_TS_IGNITION_CATEGORY, 4));
        assertArrayEquals(new byte[]{0, 19, 0, 12}, OutputAssignmentPanel.buildTestPayload(
            VariableRegistryValues.ts_command_e_TS_INJECTOR_CATEGORY, 12));
        assertThrows(IllegalArgumentException.class,
            () -> OutputAssignmentPanel.buildTestPayload(
                VariableRegistryValues.ts_command_e_TS_INJECTOR_CATEGORY, 0));
    }

    @Test
    void appliesAssignmentsBeforeSendingBenchCommand() {
        BinaryProtocol bp = mock(BinaryProtocol.class);
        ConfigurationImage image = new ConfigurationImage(4);
        String description = "test ignition output 2";
        byte[] response = new byte[]{(byte) Integration.TS_RESPONSE_OK};
        when(bp.executeCommand(eq(Integration.TS_IO_TEST_COMMAND), any(), eq(description)))
            .thenReturn(response);

        assertTrue(OutputAssignmentPanel.applyAndTest(bp, image,
            VariableRegistryValues.ts_command_e_TS_IGNITION_CATEGORY, 2, description));

        org.mockito.InOrder order = inOrder(bp);
        order.verify(bp).uploadChanges(image);
        order.verify(bp).executeCommand(Integration.TS_IO_TEST_COMMAND,
            new byte[]{0, 18, 0, 2}, description);
    }

    @Test
    void protocolErrorResponseFailsBenchTest() {
        BinaryProtocol bp = mock(BinaryProtocol.class);
        when(bp.executeCommand(eq(Integration.TS_IO_TEST_COMMAND), any(), any()))
            .thenReturn(new byte[]{(byte) Integration.TS_RESPONSE_UNRECOGNIZED_COMMAND});

        assertFalse(OutputAssignmentPanel.applyAndTest(bp, new ConfigurationImage(4),
            VariableRegistryValues.ts_command_e_TS_INJECTOR_CATEGORY, 1, "test injector"));
    }

    @Test
    void disconnectDisablesActions() throws Throwable {
        UIContext context = configuredContext(1, "injectionPins", "NONE");
        OutputAssignmentPanel[] holder = new OutputAssignmentPanel[1];
        try {
            ConnectionStatusLogic.INSTANCE.setValue(ConnectionStatusValue.CONNECTED);
            SwingUtilities.invokeAndWait(() -> {
                holder[0] = new OutputAssignmentPanel(context, OutputAssignmentPanel.OutputType.INJECTOR);
                holder[0].onShow();
            });
            SwingUtilities.invokeAndWait(() -> { });
            assertTrue(holder[0].isSaveEnabledForTests());
            assertFalse(holder[0].areTestButtonsEnabledForTests());

            SwingUtilities.invokeAndWait(() -> holder[0].setBenchTestsEnabledForTests(true));
            assertTrue(holder[0].areTestButtonsEnabledForTests());

            ConnectionStatusLogic.INSTANCE.setValue(ConnectionStatusValue.NOT_CONNECTED);
            SwingUtilities.invokeAndWait(() -> { });
            assertFalse(holder[0].isSaveEnabledForTests());
            assertFalse(holder[0].areTestButtonsEnabledForTests());
        } finally {
            if (holder[0] != null) {
                SwingUtilities.invokeAndWait(holder[0]::onHide);
            }
            ConnectionStatusLogic.INSTANCE.setValue(ConnectionStatusValue.NOT_CONNECTED);
        }
    }

    private static UIContext configuredContext(int cylinders, String prefix, String value) throws Throwable {
        UIContext context = WizardSandbox.createOfflineContext(loadTestIni());
        WizardConfig cfg = WizardConfig.snapshot(context);
        assertNotNull(cfg);
        ConfigurationImage image = cfg.image.clone();
        setValue(cfg, image, "cylindersCount", Integer.toString(cylinders));
        for (int index = 1; index <= cylinders; index++) {
            setValue(cfg, image, prefix + index, value);
        }
        cfg.bp.setConfigurationImage(image);
        return context;
    }

    private static IniFileModel loadTestIni() throws Throwable {
        try (InputStream stream = OutputAssignmentPanelTest.class.getResourceAsStream("/january.ini")) {
            assertNotNull(stream);
            RawIniFile content = IniFileReaderUtil.read(stream, "/january.ini");
            return IniFileReaderUtil.readIniFile(content, "/january.ini", new IniFileMetaInfoImpl(content));
        }
    }

    private static void setValue(WizardConfig cfg, ConfigurationImage image, String name, String value) {
        IniField field = cfg.ini.findIniField(name).orElseThrow();
        ConfigurationImageGetterSetter.setValue2(field, image, name, value);
    }

    private static String firstNonNone(JComboBox<String> editor) {
        for (int index = 0; index < editor.getItemCount(); index++) {
            String value = editor.getItemAt(index);
            if (!"NONE".equals(value)) {
                return value;
            }
        }
        fail("Expected an assignable output pin");
        return null;
    }
}
