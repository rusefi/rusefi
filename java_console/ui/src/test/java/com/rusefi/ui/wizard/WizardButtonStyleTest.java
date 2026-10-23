package com.rusefi.ui.wizard;

import com.formdev.flatlaf.FlatClientProperties;
import com.formdev.flatlaf.ui.FlatStylingSupport;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.EnumIniField;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.basic.UiHelper;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.util.Collections;
import java.util.Optional;
import java.util.concurrent.atomic.AtomicInteger;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

class WizardButtonStyleTest {
    private LookAndFeel previous;
    private boolean previousFrameDecorated;
    private boolean previousDialogDecorated;

    @BeforeEach
    void installConsoleTheme() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            previous = UIManager.getLookAndFeel();
            previousFrameDecorated = JFrame.isDefaultLookAndFeelDecorated();
            previousDialogDecorated = JDialog.isDefaultLookAndFeelDecorated();
            UiHelper.configureLaf();
        });
    }

    @AfterEach
    void restoreTheme() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            UiHelper.unregisterThemeForUnitTest();
            JFrame.setDefaultLookAndFeelDecorated(previousFrameDecorated);
            JDialog.setDefaultLookAndFeelDecorated(previousDialogDecorated);
            try {
                UIManager.setLookAndFeel(previous);
            } catch (UnsupportedLookAndFeelException e) {
                throw new AssertionError(e);
            }
        });
    }

    @Test
    void everySetupStepAdvancesWithOneFooterContinueIncludingEmbeddedVeAndStandaloneFields() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            UIContext context = context();
            assertFooterContinue(new DisplacementPanel(context));
            assertFooterContinue(new MapSensorTypePanel(context));
            assertFooterContinue(new TpsPanel(context));
            assertFooterContinue(new CltSensorPanel(context));
            assertFooterContinue(new CrankTriggerPanel(context));
            assertFooterContinue(new CamTriggerPanel(context));
            for (OutputAssignmentPanel.OutputType type : OutputAssignmentPanel.OutputType.values()) {
                assertFooterContinue(new OutputAssignmentPanel(context, type));
            }
            assertFooterContinue(new InjectorFlowPanel(context));
            assertFooterContinue(new GenericFieldsPanel(context, "VIN", "", Collections.singletonList("vin"), null));
            VeTableWizardStep ve = new VeTableWizardStep(context);
            ve.onShow();
            assertFooterContinue(ve);
            assertFalse(ve.getContinueButton().isEnabled(), "no generated preview to apply");
            assertNeutral(button(ve.getPanel(), "Generate Preview"));
        });
    }

    @Test
    void skippableStepsOfferOneNeutralFooterSkip() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            UIContext context = context();
            VeTableWizardStep ve = new VeTableWizardStep(context);
            ve.onShow();
            for (WizardStep step : new WizardStep[]{new CrankTriggerPanel(context), new CamTriggerPanel(context), ve}) {
                JButton skip = step.getSkipButton();
                assertNotNull(skip, step.getTitle());
                assertEquals(AbstractWizardStep.SKIP_TEXT, skip.getText());
                assertNeutral(skip);
                assertNull(findButton(step.getPanel(), AbstractWizardStep.SKIP_TEXT), "skip belongs in the footer");
            }
            assertNull(new DisplacementPanel(context).getSkipButton());
        });
    }

    @Test
    void oneClickChoicesStandOutAndExplainHowToContinue() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            UIContext context = context();
            NumberOfCylindersPanel cylinders = new NumberOfCylindersPanel(context);
            FiringOrderPanel firingOrder = new FiringOrderPanel(context, 4);
            for (WizardStep step : new WizardStep[]{cylinders, firingOrder}) {
                assertNull(step.getContinueButton(), step.getTitle());
                assertNotNull(step.getFooterHint(), step.getTitle());
            }
            // #10481: a click on these saves immediately, so they read as actions; 7, 9 and 16 only explain
            assertEquals("wizardChoice", button(cylinders.getPanel(), "4").getClientProperty(FlatClientProperties.STYLE_CLASS));
            assertEquals("wizardChoiceMuted", button(cylinders.getPanel(), "7").getClientProperty(FlatClientProperties.STYLE_CLASS));
            assertEquals("wizardChoice", button(firingOrder.getPanel(), "1-3-4-2 (typical inline 4)")
                .getClientProperty(FlatClientProperties.STYLE_CLASS));
            assertNeutral(button(new TpsPanel(context).getPanel(), "Grab"));
        });
    }

    @Test
    void footerActionsKeepTheirPlaceAcrossLaunchModes() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            WizardContainer container = new WizardContainer(context());
            container.startWizard(true);
            assertNeutral(button(container, "Don't Show Again"));
            assertNull(button(container, "Cancel").getClientProperty(FlatClientProperties.STYLE_CLASS));
            assertEquals("primary", button(container, "Return to Console").getClientProperty(FlatClientProperties.STYLE_CLASS));
            assertEquals(BorderLayout.EAST, container.getExitPanelConstraintForTests());
            assertEquals(BorderLayout.EAST, container.getNavigationConstraintForTests());

            GenericFieldsPanel vin = new GenericFieldsPanel(context(), "VIN", "", Collections.singletonList("vin"), null);
            container.startSingleStep(vin);
            assertNull(button(container, "Exit Wizard").getClientProperty(FlatClientProperties.STYLE_CLASS));
            assertFalse(button(container, "Don't Show Again").isVisible());
            assertEquals(Collections.singletonList(vin.getContinueButton()), container.getNavigationForTests());
            assertEquals(BorderLayout.EAST, container.getExitPanelConstraintForTests());

            container.startSingleStep(new NumberOfCylindersPanel(context()));
            assertTrue(container.getNavigationForTests().isEmpty());
            assertEquals(new NumberOfCylindersPanel(context()).getFooterHint(), container.getFooterHintForTests());
        });
    }

    @Test
    void firmwareConfirmationAndDestructiveActionsHaveAppropriateRoles() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            DfuBoardSelectionPanel dfu = new DfuBoardSelectionPanel(board -> { });
            assertConfirm(button(dfu, "Download and flash"));
            assertNegative(button(dfu, "Cancel"));
            EmergencyWipePanel wipe = new EmergencyWipePanel("Erase flash", () -> { }, () -> { });
            assertNegative(button(wipe, "Erase Internal Flash"));
            assertNegative(button(wipe, "Cancel"));
            FirmwareRollbackPanel rollback = new FirmwareRollbackPanel(true, new String[]{"LTS"}, index -> { }, () -> { });
            assertNegative(button(rollback, "Cancel"));
        });
    }

    @Test
    void themePreservesRolesOnHoverPressFocusAndDefaultButtonsAndDisablesActions() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            JButton confirm = new JButton("Confirm");
            AbstractWizardStep.styleConfirmAction(confirm);
            JButton cancel = new JButton("Cancel");
            AbstractWizardStep.styleNegativeAction(cancel);
            for (JButton button : new JButton[]{confirm, cancel}) {
                Color base = button.getBackground();
                FlatStylingSupport.StyleableUI ui = (FlatStylingSupport.StyleableUI) button.getUI();
                for (String prefix : new String[]{"", "default."}) {
                    assertEquals(base, ui.getStyleableValue(button, prefix + "focusedBackground"));
                    for (String state : new String[]{"hover", "pressed", "focused"}) {
                        Color background = (Color) ui.getStyleableValue(button, prefix + state + "Background");
                        assertHue(background, button == confirm);
                        assertEquals(Color.WHITE, ui.getStyleableValue(button, prefix + state + "Foreground"));
                    }
                }
                JRootPane root = new JRootPane();
                root.getContentPane().add(button);
                root.setDefaultButton(button);
                assertHue(paintedBackground(button), button == confirm);
                button.getModel().setRollover(true);
                assertHue(paintedBackground(button), button == confirm);
                button.getModel().setArmed(true);
                button.getModel().setPressed(true);
                assertHue(paintedBackground(button), button == confirm);
                button.getModel().setArmed(false);
                button.getModel().setPressed(false);
                button.getModel().setRollover(false);

                AtomicInteger clicks = new AtomicInteger();
                button.addActionListener(e -> clicks.incrementAndGet());
                button.setEnabled(false);
                assertEquals(UIManager.getColor("Button.disabledBackground"), paintedBackground(button));
                button.doClick(0);
                assertEquals(0, clicks.get());
                button.setEnabled(true);
                button.doClick(0);
                assertEquals(1, clicks.get());
                assertEquals(base, paintedBackground(button));
            }
        });
    }

    private static UIContext context() {
        UIContext context = new UIContext();
        IniFileModel ini = mock(IniFileModel.class);
        EnumIniField firingOrder = mock(EnumIniField.class);
        EnumIniField.EnumKeyValueMap orders = new EnumIniField.EnumKeyValueMap(Collections.singletonMap(0, "1-3-4-2"));
        when(firingOrder.getEnums()).thenReturn(orders);
        when(ini.findIniField("firingOrder")).thenReturn(Optional.of(firingOrder));
        BinaryProtocol bp = mock(BinaryProtocol.class);
        when(bp.getControllerConfiguration()).thenReturn(new ConfigurationImage(64));
        context.iniFileState.setIniFileModelForTest(ini);
        context.getLinkManager().setBinaryProtocolForTests(bp);
        return context;
    }

    private static void assertConfirm(JButton button) {
        assertEquals("wizardConfirm", button.getClientProperty(FlatClientProperties.STYLE_CLASS), button.getText());
        assertEquals(new Color(0x15803d), button.getBackground(), button.getText());
        assertEquals(Color.WHITE, button.getForeground());
        assertSame(WizardStyle.ActionIcon.CONFIRM, button.getIcon());
    }

    private static void assertNegative(JButton button) {
        assertEquals("wizardNegative", button.getClientProperty(FlatClientProperties.STYLE_CLASS), button.getText());
        assertEquals(new Color(0xb91c1c), button.getBackground(), button.getText());
        assertEquals(Color.WHITE, button.getForeground());
        assertSame(WizardStyle.ActionIcon.CANCEL, button.getIcon());
    }

    private static void assertFooterContinue(WizardStep step) {
        JButton next = step.getContinueButton();
        assertNotNull(next, step.getTitle());
        assertEquals(AbstractWizardStep.CONTINUE_TEXT, next.getText(), step.getTitle());
        assertEquals("primary", next.getClientProperty(FlatClientProperties.STYLE_CLASS), step.getTitle());
        assertSame(WizardStyle.ActionIcon.NEXT, next.getIcon());
        assertNull(findButton(step.getPanel(), AbstractWizardStep.CONTINUE_TEXT),
            step.getTitle() + ": Continue belongs in the footer, not the card");
    }

    private static void assertNeutral(JButton button) {
        assertNull(button.getClientProperty(FlatClientProperties.STYLE_CLASS), button.getText());
        assertNull(button.getIcon(), button.getText());
    }

    private static void assertHue(Color color, boolean green) {
        assertNotNull(color);
        assertTrue(green ? color.getGreen() > color.getRed() && color.getGreen() > color.getBlue()
            : color.getRed() > color.getGreen() && color.getRed() > color.getBlue(), color.toString());
    }

    private static Color paintedBackground(JButton button) {
        button.setSize(button.getPreferredSize());
        BufferedImage image = new BufferedImage(button.getWidth(), button.getHeight(), BufferedImage.TYPE_INT_RGB);
        Graphics2D graphics = image.createGraphics();
        try {
            button.paint(graphics);
        } finally {
            graphics.dispose();
        }
        return new Color(image.getRGB(8, button.getHeight() / 2));
    }

    private static JButton button(Container root, String text) {
        JButton found = findButton(root, text);
        assertNotNull(found, text);
        return found;
    }

    private static JButton findButton(Container root, String text) {
        for (Component child : root.getComponents()) {
            if (child instanceof JButton && text.equals(((JButton) child).getText())) {
                return (JButton) child;
            }
            if (child instanceof Container) {
                JButton found = findButton((Container) child, text);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }
}
