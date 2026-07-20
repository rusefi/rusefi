package com.rusefi.ui.wizard;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.EnumIniField.EnumKeyValueMap;
import com.rusefi.config.FieldType;
import com.rusefi.ui.UIContext;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.TreeMap;

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

        assertEquals(Arrays.asList(0, 1, 2, 3, 4, 5, 8, 9, 10),
            WizardContainer.findVisibleCatalogIndices(context, ini));
    }

    @Test
    public void persistentFlagsDetermineStateEvenAfterAnIncompleteStep() {
        List<WizardProgressPanel.Item> items = WizardContainer.buildProgressItems(
            Arrays.asList(0, 1, 3), 0, flag -> flag.equals("wizardMapSensorType"));

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
    public void savingCylinderCountInvalidatesDependentSteps() {
        IniFileModel ini = mock(IniFileModel.class);
        EnumIniField firingOrderFlag = mock(EnumIniField.class);
        EnumIniField ignitionOutputsFlag = mock(EnumIniField.class);
        EnumIniField injectorOutputsFlag = mock(EnumIniField.class);
        ConfigurationImage image = mock(ConfigurationImage.class);
        when(ini.findIniField("wizardFiringOrder")).thenReturn(Optional.of(firingOrderFlag));
        when(ini.findIniField("wizardIgnitionOutputs")).thenReturn(Optional.of(ignitionOutputsFlag));
        when(ini.findIniField("wizardInjectorOutputs")).thenReturn(Optional.of(injectorOutputsFlag));

        WizardContainer.clearDependentWizardFlags("wizardNumberOfCylinders", ini, image);

        verify(image).setBitValue(firingOrderFlag, 0);
        verify(image).setBitValue(ignitionOutputsFlag, 0);
        verify(image).setBitValue(injectorOutputsFlag, 0);
    }

    @Test
    public void oneAndTwoCylindersUseImplicitFiringOrders() {
        assertEquals(0, WizardContainer.implicitFiringOrderOrdinal(1));
        assertEquals(8, WizardContainer.implicitFiringOrderOrdinal(2));
        assertNull(WizardContainer.implicitFiringOrderOrdinal(3));
        assertNull(WizardContainer.implicitFiringOrderOrdinal(4));
    }

    @Test
    public void implicitFiringOrderStepIsNotVisible() {
        List<Integer> oneCylinderSteps = new ArrayList<>(Arrays.asList(0, 1, 2));
        List<Integer> twoCylinderSteps = new ArrayList<>(Arrays.asList(0, 1, 2));
        List<Integer> fourCylinderSteps = new ArrayList<>(Arrays.asList(0, 1, 2));

        WizardContainer.hideImplicitFiringOrderStep(oneCylinderSteps, 1);
        WizardContainer.hideImplicitFiringOrderStep(twoCylinderSteps, 2);
        WizardContainer.hideImplicitFiringOrderStep(fourCylinderSteps, 4);

        assertEquals(Arrays.asList(0, 2), oneCylinderSteps);
        assertEquals(Arrays.asList(0, 2), twoCylinderSteps);
        assertEquals(Arrays.asList(0, 1, 2), fourCylinderSteps);
    }

    @Test
    public void completingCylinderStepWritesImplicitFiringOrderAndFlag() {
        Map<Integer, String> orderValues = new TreeMap<>();
        orderValues.put(0, "One Cylinder");
        orderValues.put(8, "1-2");
        EnumIniField firingOrder = new EnumIniField(
            "firingOrder", 0, FieldType.UINT8, new EnumKeyValueMap(orderValues), 0, 3);

        Map<Integer, String> flagValues = new TreeMap<>();
        flagValues.put(0, "no");
        flagValues.put(1, "yes");
        EnumIniField firingOrderFlag = new EnumIniField(
            "wizardFiringOrder", 4, FieldType.INT, new EnumKeyValueMap(flagValues), 0, 0);

        IniFileModel ini = mock(IniFileModel.class);
        when(ini.findIniField("firingOrder")).thenReturn(Optional.of(firingOrder));
        when(ini.findIniField("wizardFiringOrder")).thenReturn(Optional.of(firingOrderFlag));

        for (String cylinders : Arrays.asList("1", "2")) {
            ConfigurationImage image = new ConfigurationImage(new byte[8]);
            WizardContainer.completeImplicitFiringOrder(
                "wizardNumberOfCylinders", cylinders, ini, image);

            String expectedOrder = orderValues.get(
                WizardContainer.implicitFiringOrderOrdinal(Integer.parseInt(cylinders)));
            assertEquals(expectedOrder,
                AbstractWizardStep.stripQuotes(ConfigurationImageGetterSetter.getStringValue(firingOrder, image)));
            assertEquals("yes",
                AbstractWizardStep.stripQuotes(ConfigurationImageGetterSetter.getStringValue(firingOrderFlag, image)));
        }
    }

    @Test
    public void largerCylinderCountsDoNotCompleteFiringOrder() {
        IniFileModel ini = mock(IniFileModel.class);

        WizardContainer.completeImplicitFiringOrder(
            "wizardNumberOfCylinders", "4", ini, new ConfigurationImage(new byte[1]));

        verify(ini, org.mockito.Mockito.never()).findIniField(anyString());
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
