package com.rusefi.ui.wizard;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.FieldType;
import com.rusefi.ui.UIContext;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.util.Optional;
import java.util.concurrent.atomic.AtomicReference;
import java.util.function.Predicate;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

class DisplacementPanelTest {
    @Test
    void requiresFinitePositiveLitresWithinConfigurationRange() {
        for (String value : new String[]{"", " ", "abc", "0", "-2", "NaN", "Infinity", "65.1"}) {
            assertNotNull(DisplacementPanel.validationError(value), value);
        }
        for (String value : new String[]{"0.05", "1.998", " 2.0 ", "65"}) {
            assertNull(DisplacementPanel.validationError(value), value);
        }
    }

    @Test
    void savesOnlyDisplacementToAClone() {
        Fixture f = new Fixture();
        byte[] original = f.image.getContent().clone();
        WizardStepResult result = DisplacementPanel.createResult(f.ini, f.image, "1.998");
        assertEquals(1.998, Double.parseDouble(ConfigurationImageGetterSetter.getStringValue(f.field, result.modifiedImage)), 0.0001);
        assertArrayEquals(original, f.image.getContent());
        for (int i = 0; i < original.length; i++) {
            if (i < 4 || i >= 8) {
                assertEquals(original[i], result.modifiedImage.getContent()[i]);
            }
        }
        assertThrows(IllegalArgumentException.class, () -> DisplacementPanel.createResult(f.ini, f.image, "NaN"));
        when(f.ini.findIniField("displacement")).thenReturn(Optional.empty());
        assertThrows(IllegalArgumentException.class, () -> DisplacementPanel.createResult(f.ini, f.image, "2"));
    }

    @Test
    void requiresExplicitSaveAndOffersNoSkip() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            Fixture f = new Fixture();
            DisplacementPanel panel = new DisplacementPanel(f.context);
            AtomicReference<WizardStepResult> saved = new AtomicReference<>();
            panel.setOnStepCompleted(saved::set);
            panel.onShow();
            assertEquals("wizardDisplacement", panel.getWizardFlagFieldName());
            JTextField input = (JTextField) find(panel.getPanel(), c -> "displacement".equals(c.getName()));
            JButton save = (JButton) find(panel.getPanel(), c -> c instanceof JButton);
            assertEquals("Save and Continue", save.getText());
            assertNull(find(panel.getPanel(), c -> c instanceof JButton && ((JButton) c).getText().contains("Skip")));
            assertEquals(2, Double.parseDouble(input.getText()), 0.001);
            assertNull(saved.get());
            input.setText("");
            assertFalse(save.isEnabled());
            save.doClick();
            assertNull(saved.get());
            input.setText("3.2");
            assertTrue(save.isEnabled());
            // Save must use the latest image, including changes since the screen was shown.
            f.image.getContent()[12] = 42;
            save.doClick();
            assertNotNull(saved.get());
            assertEquals(42, saved.get().modifiedImage.getContent()[12]);
        });
    }

    @Test
    void disconnectedSaveDoesNotComplete() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            Fixture f = new Fixture();
            DisplacementPanel panel = new DisplacementPanel(f.context);
            AtomicReference<WizardStepResult> saved = new AtomicReference<>();
            panel.setOnStepCompleted(saved::set);
            panel.onShow();
            when(f.bp.getControllerConfiguration()).thenReturn(null);
            JButton save = (JButton) find(panel.getPanel(), c -> c instanceof JButton);
            save.doClick();
            assertFalse(save.isEnabled());
            assertNull(saved.get());
        });
    }

    private static Component find(Container root, Predicate<Component> predicate) {
        for (Component child : root.getComponents()) {
            if (predicate.test(child)) {
                return child;
            }
            if (child instanceof Container) {
                Component found = find((Container) child, predicate);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }

    private static class Fixture {
        final ScalarIniField field = new ScalarIniField("displacement", 4, "L", FieldType.FLOAT, 1, "3", 0);
        final IniFileModel ini = mock(IniFileModel.class);
        final ConfigurationImage image = new ConfigurationImage(new byte[16]);
        final BinaryProtocol bp = mock(BinaryProtocol.class);
        final UIContext context = new UIContext();

        Fixture() {
            when(ini.findIniField("displacement")).thenReturn(Optional.of(field));
            ConfigurationImageGetterSetter.setValue2(field, image, "displacement", "2");
            when(bp.getControllerConfiguration()).thenReturn(image);
            context.iniFileState.setIniFileModelForTest(ini);
            context.getLinkManager().setBinaryProtocolForTests(bp);
        }
    }
}
