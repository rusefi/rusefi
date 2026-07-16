package com.rusefi.ui.wizard;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.EnumIniField;
import com.rusefi.ui.UIContext;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Optional;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.ArgumentMatchers.anyString;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

public class WizardContainerTest {
    @Test
    public void progressFiltersMissingOrUnsupportedFlags() {
        UIContext context = new UIContext();
        IniFileModel ini = mock(IniFileModel.class);
        Map<String, Object> fields = new HashMap<>();
        for (WizardStepDescriptor descriptor : WizardCatalog.flaggedSteps()) {
            fields.put(descriptor.flagName, mock(EnumIniField.class));
        }
        fields.remove("wizardCrankTrigger");
        fields.put("wizardCamTrigger", new Object());
        when(ini.findIniField(anyString())).thenAnswer(invocation -> {
            Object field = fields.get(invocation.getArgument(0));
            return field instanceof EnumIniField ? Optional.of(field) : Optional.empty();
        });

        assertEquals(Arrays.asList(0, 1, 2, 3, 4, 7), WizardContainer.findVisibleCatalogIndices(context, ini));
    }

    @Test
    public void persistentFlagsDetermineStateEvenAfterAnIncompleteStep() {
        List<WizardProgressPanel.Item> items = WizardContainer.buildProgressItems(
            Arrays.asList(0, 1, 2), 0, flag -> flag.equals("wizardMapSensorType"));

        assertEquals(WizardProgressPanel.State.CURRENT, items.get(0).state);
        assertEquals(WizardProgressPanel.State.UPCOMING, items.get(1).state);
        assertEquals(WizardProgressPanel.State.COMPLETED, items.get(2).state);
    }

    @Test
    public void completionAfterRevisitReturnsToEarliestIncomplete() {
        assertEquals(0, WizardContainer.findFirstIncomplete(
            Arrays.asList(0, 1, 2), index -> index == 1 || index == 2));
        assertEquals(-1, WizardContainer.findFirstIncomplete(
            Arrays.asList(0, 1, 2), index -> true));
    }

    @Test
    public void savingCylinderCountInvalidatesFiringOrder() {
        IniFileModel ini = mock(IniFileModel.class);
        EnumIniField firingOrderFlag = mock(EnumIniField.class);
        ConfigurationImage image = mock(ConfigurationImage.class);
        when(ini.findIniField("wizardFiringOrder")).thenReturn(Optional.of(firingOrderFlag));

        WizardContainer.clearDependentWizardFlags("wizardNumberOfCylinders", ini, image);

        verify(image).setBitValue(firingOrderFlag, 0);
    }

    @Test
    public void singleStepHidesProgressInBothContainerSizes() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            for (boolean compact : Arrays.asList(false, true)) {
                WizardContainer container = new WizardContainer(new UIContext(), compact);
                container.startSingleStep(new StubStep());
                assertFalse(container.isProgressVisibleForTests());
            }
        });
    }

    private static final class StubStep implements WizardStep {
        private final JPanel panel = new JPanel();

        @Override public String getTitle() { return "Stub"; }
        @Override public JComponent getPanel() { return panel; }
        @Override public String getWizardFlagFieldName() { return null; }
        @Override public void setOnStepCompleted(java.util.function.Consumer<WizardStepResult> callback) { }
    }
}
