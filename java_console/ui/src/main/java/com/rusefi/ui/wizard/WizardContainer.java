package com.rusefi.ui.wizard;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.ui.UIContext;

import javax.swing.*;
import javax.swing.text.JTextComponent;
import java.awt.*;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.function.IntPredicate;
import java.util.function.Predicate;
import java.util.function.Consumer;

import static com.devexperts.logging.Logging.getLogging;

public class WizardContainer extends JPanel {
    private static final Logging log = getLogging(WizardContainer.class);
    private static final List<WizardStepDescriptor> FLAGGED = WizardCatalog.flaggedSteps();
    private static final int FO_1 = 0;
    private static final int FO_1_2 = 8;

    private final UIContext uiContext;
    private final WizardProgressPanel progressPanel = new WizardProgressPanel();
    private final JLabel wizardTitle = new JLabel("rusEFI Setup Wizard");
    private final JPanel stepContentPanel = new JPanel(new CardLayout());
    private final JButton cancelButton = new JButton("Exit Wizard");
    private final JButton dontShowAgainButton = new JButton("Don't Show Again");
    private final JPanel exitPanel = new JPanel(new FlowLayout(FlowLayout.RIGHT, WizardStyle.GAP / 2, 0));
    /** Right side of the footer: the shown step's Skip and Continue buttons, the same place on every step. */
    private final JPanel navigationPanel = new JPanel(new FlowLayout(FlowLayout.RIGHT, WizardStyle.GAP / 2, 0));
    private final JLabel footerHint = new JLabel();
    private final JButton returnToConsoleButton = new JButton("Return to Console");
    private final JPanel footerPanel = new JPanel(new BorderLayout(WizardStyle.GAP, 0));
    private final List<WizardStep> steps = new ArrayList<>();
    private final List<Integer> visibleCatalogIndices = new ArrayList<>();

    // Debug panel labels, keyed by flag name
    private final Map<String, JLabel> debugLabels = new LinkedHashMap<>();
    private final JPanel debugPanel = new JPanel();

    private Runnable onWizardExit;
    private Runnable onDontShowAgain;
    private Runnable onAllStepsComplete;
    private int currentStepIndex = 0;
    private int activeStepIndex = -1;
    private boolean singleStepMode;
    private int selectedCylinders = 4; // default, updated by step 0
    private Consumer<String> messageHandler = message -> JOptionPane.showMessageDialog(this, message, "Error", JOptionPane.WARNING_MESSAGE);

    public WizardContainer(UIContext uiContext) {
        this(uiContext, false);
    }

    public void setMessageHandler(Consumer<String> messageHandler) {
        this.messageHandler = messageHandler;
    }

    /**
     * @param compact when {@code true}, skips the oversized header fonts/buttons so the wizard
     *                fits inside a modest host frame (e.g. the splash screen). Use {@code false}
     *                for the fullscreen-console experience.
     */
    public WizardContainer(UIContext uiContext, boolean compact) {
        super(new BorderLayout());
        this.uiContext = uiContext;

        JPanel headerPanel = new JPanel(new BorderLayout());
        headerPanel.setBackground(WizardStyle.barBackground());
        headerPanel.setBorder(BorderFactory.createCompoundBorder(
            BorderFactory.createMatteBorder(0, 0, 1, 0, WizardStyle.separator()),
            BorderFactory.createEmptyBorder(
                compact ? WizardStyle.GAP : WizardStyle.LARGE_GAP,
                WizardStyle.LARGE_GAP,
                WizardStyle.GAP / 2,
                WizardStyle.LARGE_GAP)));
        wizardTitle.setFont(wizardTitle.getFont().deriveFont(Font.BOLD, compact ? 16f : 20f));
        wizardTitle.setForeground(WizardStyle.text());
        // title on the left and the way out at the top right, where people look to leave a full-window flow
        JPanel titleRow = new JPanel(new BorderLayout());
        titleRow.setOpaque(false);
        titleRow.setBorder(BorderFactory.createEmptyBorder(0, 0, WizardStyle.GAP / 2, 0));
        titleRow.add(wizardTitle, BorderLayout.WEST);
        titleRow.add(exitPanel, BorderLayout.EAST);
        headerPanel.add(titleRow, BorderLayout.NORTH);
        headerPanel.add(progressPanel, BorderLayout.CENTER);

        AbstractWizardStep.styleFooterAction(cancelButton);
        cancelButton.setIcon(WizardStyle.ActionIcon.CANCEL);
        cancelButton.addActionListener(e -> exitWizard());
        AbstractWizardStep.styleFooterAction(dontShowAgainButton);
        dontShowAgainButton.setVisible(false);
        dontShowAgainButton.addActionListener(e -> {
            if (onDontShowAgain != null) {
                onDontShowAgain.run();
            }
            exitWizard();
        });
        exitPanel.setOpaque(false);
        exitPanel.add(dontShowAgainButton);
        exitPanel.add(cancelButton);
        navigationPanel.setOpaque(false);
        footerHint.setForeground(WizardStyle.muted());
        footerHint.setHorizontalAlignment(SwingConstants.RIGHT);
        AbstractWizardStep.styleContinueAction(returnToConsoleButton);
        returnToConsoleButton.setText("Return to Console");
        returnToConsoleButton.addActionListener(e -> exitWizard());
        progressPanel.setOnCompletedSelected(this::showStep);

        add(headerPanel, BorderLayout.NORTH);
        stepContentPanel.setBorder(BorderFactory.createEmptyBorder(
            WizardStyle.GAP, WizardStyle.LARGE_GAP, WizardStyle.LARGE_GAP, WizardStyle.LARGE_GAP));
        add(stepContentPanel, BorderLayout.CENTER);

        // Debug panel at the bottom showing wizard flag states
        buildDebugPanel();
        debugPanel.setVisible(false);
        footerPanel.setBackground(WizardStyle.barBackground());
        footerPanel.setBorder(BorderFactory.createCompoundBorder(
            BorderFactory.createMatteBorder(1, 0, 0, 0, WizardStyle.separator()),
            BorderFactory.createEmptyBorder(WizardStyle.GAP, WizardStyle.LARGE_GAP, WizardStyle.GAP, WizardStyle.LARGE_GAP)));
        footerPanel.add(debugPanel, BorderLayout.NORTH);
        footerPanel.add(footerHint, BorderLayout.CENTER);
        footerPanel.add(navigationPanel, BorderLayout.EAST);
        add(footerPanel, BorderLayout.SOUTH);
    }

    private void buildDebugPanel() {
        debugPanel.setLayout(new FlowLayout(FlowLayout.LEFT, 15, 5));
        debugPanel.setOpaque(false);
        debugPanel.setBorder(BorderFactory.createEmptyBorder(5, 10, 5, 10));

        JLabel header = new JLabel("Debug flags:");
        header.setFont(header.getFont().deriveFont(Font.BOLD));
        debugPanel.add(header);

        for (WizardStepDescriptor d : FLAGGED) {
            String flag = d.flagName;
            // Strip "wizard" prefix for a shorter display name
            String shortName = flag.startsWith("wizard") ? flag.substring(6) : flag;
            JLabel label = new JLabel(shortName + ": ?");
            label.setFont(label.getFont().deriveFont(Font.PLAIN));
            debugLabels.put(flag, label);
            debugPanel.add(label);
        }

        JButton refreshButton = new JButton("Refresh");
        refreshButton.addActionListener(e -> refreshDebugFlags());
        debugPanel.add(refreshButton);

        JButton resetButton = new JButton("Reset All");
        resetButton.addActionListener(e -> resetAllFlags());
        debugPanel.add(resetButton);
    }

    private void resetAllFlags() {
        WizardConfig cfg = WizardConfig.snapshot(uiContext);
        if (cfg == null) return;

        ConfigurationImage modified = cfg.image.clone();
        for (WizardStepDescriptor d : FLAGGED) {
            IniField field = cfg.ini.findIniField(d.flagName).orElse(null);
            if (field instanceof EnumIniField) {
                modified.setBitValue((EnumIniField) field, 0);
            }
        }
        uiContext.getLinkManager().submit(() -> {
            cfg.bp.uploadChanges(modified);
            SwingUtilities.invokeLater(this::refreshDebugFlags);
        });
    }

    private void refreshDebugFlags() {
        BinaryProtocol bp = uiContext.getBinaryProtocol();
        IniFileModel ini = (bp != null) ? uiContext.iniFileState.getIniFileModel() : null;
        ConfigurationImage image = (bp != null) ? bp.getControllerConfiguration() : null;

        for (WizardStepDescriptor d : FLAGGED) {
            JLabel label = debugLabels.get(d.flagName);
            if (label == null) continue;
            String shortName = d.flagName.startsWith("wizard") ? d.flagName.substring(6) : d.flagName;

            if (ini == null || image == null) {
                label.setText(shortName + ": --");
                label.setForeground(WizardStyle.muted());
                continue;
            }

            if (!d.applicable.test(uiContext)) {
                label.setText(shortName + ": N/A");
                label.setForeground(WizardStyle.muted());
                continue;
            }

            IniField field = ini.findIniField(d.flagName).orElse(null);
            if (field == null) {
                // Board's INI doesn't declare this wizard flag — render as unsupported.
                label.setText(shortName + ": --");
                label.setForeground(WizardStyle.muted());
                continue;
            }
            if (field instanceof EnumIniField) {
                EnumIniField ef = (EnumIniField) field;
                int raw = image.getByteBuffer(ef).getInt();
                int ordinal = ConfigurationImage.getBitRange(raw, ef.getBitPosition(), ef.getBitSize0() + 1);
                boolean isSet = ordinal == 1;
                label.setText(shortName + ": " + (isSet ? "yes" : "no"));
                label.setForeground(isSet ? WizardStyle.success() : WizardStyle.error());
            } else {
                label.setText(shortName + ": ?");
                label.setForeground(WizardStyle.muted());
            }
        }
    }

    public void setOnWizardExit(Runnable onWizardExit) {
        this.onWizardExit = onWizardExit;
    }

    public void setOnDontShowAgain(Runnable onDontShowAgain) {
        this.onDontShowAgain = onDontShowAgain;
    }

    public void startWizard() {
        startWizard(false);
    }

    public void startWizard(boolean autoLaunch) {
        configureExitButtons(autoLaunch);
        hideCurrentStep();
        currentStepIndex = 0;
        singleStepMode = false;
        progressPanel.setVisible(true);
        onAllStepsComplete = this::showCompletionCard;
        // debugPanel.setVisible(true);
        refreshDebugFlags();
        steps.clear();
        stepContentPanel.removeAll();

        // Step 0: Number of Cylinders
        NumberOfCylindersPanel cylPanel = new NumberOfCylindersPanel(uiContext);
        cylPanel.setOnStepCompleted(result -> {
            if (result.value != null) {
                selectedCylinders = Integer.parseInt(result.value);
            }
            if (implicitFiringOrderOrdinal(selectedCylinders) == null) {
                createFiringOrderStep();
            }
            rebuildVisibleCatalogIndices();
            onStepCompleted(result, steps.get(0));
        });
        steps.add(cylPanel);
        stepContentPanel.add(cylPanel.getPanel(), "step0");

        // Step 1: FiringOrder — placeholder card, replaced lazily in createFiringOrderStep()
        JPanel firingOrderPlaceholder = new JPanel();
        stepContentPanel.add(firingOrderPlaceholder, "step1");
        steps.add(null); // placeholder slot

        // Step 2: Optional Base VE Table (wizardVeTable — silently skipped on older firmware)
        VeTableWizardStep veStep = new VeTableWizardStep(uiContext);
        wireStep(veStep, 2);
        steps.add(veStep);
        stepContentPanel.add(veStep.getPanel(), "step2");

        // Step 3: Required engine displacement, independent of VE Apply/Skip
        DisplacementPanel displacementPanel = new DisplacementPanel(uiContext);
        wireStep(displacementPanel, 3);
        steps.add(displacementPanel);
        stepContentPanel.add(displacementPanel.getPanel(), "step3");

        // Step 4: MAP Sensor Type
        MapSensorTypePanel mapPanel = new MapSensorTypePanel(uiContext);
        wireStep(mapPanel, 4);
        steps.add(mapPanel);
        stepContentPanel.add(mapPanel.getPanel(), "step4");

        // Step 5: TPS
        TpsPanel tpsPanel = new TpsPanel(uiContext);
        wireStep(tpsPanel, 5);
        steps.add(tpsPanel);
        stepContentPanel.add(tpsPanel.getPanel(), "step5");

        // Step 6: CLT Sensor
        CltSensorPanel cltPanel = new CltSensorPanel(uiContext);
        wireStep(cltPanel, 6);
        steps.add(cltPanel);
        stepContentPanel.add(cltPanel.getPanel(), "step6");

        // Step 7: Crank Trigger
        CrankTriggerPanel crankPanel = new CrankTriggerPanel(uiContext);
        wireStep(crankPanel, 7);
        steps.add(crankPanel);
        stepContentPanel.add(crankPanel.getPanel(), "step7");

        // Step 8: Cam Trigger
        CamTriggerPanel camPanel = new CamTriggerPanel(uiContext);
        wireStep(camPanel, 8);
        steps.add(camPanel);
        stepContentPanel.add(camPanel.getPanel(), "step8");

        // Step 9: Ignition Outputs
        OutputAssignmentPanel ignitionOutputs = new OutputAssignmentPanel(
            uiContext, OutputAssignmentPanel.OutputType.IGNITION);
        wireStep(ignitionOutputs, 9);
        steps.add(ignitionOutputs);
        stepContentPanel.add(ignitionOutputs.getPanel(), "step9");

        // Step 10: Injector Outputs
        OutputAssignmentPanel injectorOutputs = new OutputAssignmentPanel(
            uiContext, OutputAssignmentPanel.OutputType.INJECTOR);
        wireStep(injectorOutputs, 10);
        steps.add(injectorOutputs);
        stepContentPanel.add(injectorOutputs.getPanel(), "step10");

        // Step 11: Injector Flow
        InjectorFlowPanel injPanel = new InjectorFlowPanel(uiContext);
        wireStep(injPanel, 11);
        steps.add(injPanel);
        stepContentPanel.add(injPanel.getPanel(), "step11");

        // Completion card
        JPanel completionPanel = new JPanel(new GridBagLayout());
        JPanel completionCard = new JPanel(new GridBagLayout());
        WizardStyle.styleCard(completionCard, 32, 48);
        JLabel doneLabel = WizardStyle.cardTitle("Wizard Complete!");
        doneLabel.setIcon(WizardStyle.ActionIcon.CONFIRM);
        doneLabel.setIconTextGap(WizardStyle.GAP);
        doneLabel.setForeground(WizardStyle.success());
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.gridy = 0;
        completionCard.add(doneLabel, gbc);
        gbc.gridy = 1;
        gbc.insets = new Insets(WizardStyle.GAP, 0, 0, 0);
        completionCard.add(WizardStyle.description("All setup steps are saved to the ECU."), gbc);
        completionPanel.add(completionCard);
        stepContentPanel.add(completionPanel, "complete");

        // If step 0 is already done, read cylindersCount before building the remaining flow.
        if (isStepSatisfied(0)) {
            selectedCylinders = readCylindersCountFromEcu();
            if (implicitFiringOrderOrdinal(selectedCylinders) == null) {
                createFiringOrderStep();
            }
        }

        refreshDebugFlags();
        rebuildVisibleCatalogIndices();

        // Skip to the first incomplete step
        int firstIncomplete = findFirstIncompleteStep();
        if (firstIncomplete < 0) {
            showCompletionCard();
        } else {
            showStep(firstIncomplete);
        }
    }

    /**
     * Start the wizard with a single custom step. When the step completes (or is already
     * satisfied), the wizard auto-exits. Used for targeted prompts like an empty-VIN auto-launch.
     */
    public void startSingleStep(WizardStep step) {
        configureExitButtons(false);
        hideCurrentStep();
        currentStepIndex = 0;
        singleStepMode = true;
        progressPanel.setVisible(false);
        onAllStepsComplete = this::exitWizard;
        debugPanel.setVisible(false);
        steps.clear();
        stepContentPanel.removeAll();

        wireStep(step, 0);
        steps.add(step);
        stepContentPanel.add(step.getPanel(), "step0");

        refreshDebugFlags();
        showStep(0);
    }

    private void showCompletionCard() {
        hideCurrentStep();
        currentStepIndex = -1;
        refreshProgress();
        CardLayout cl = (CardLayout) stepContentPanel.getLayout();
        cl.show(stepContentPanel, "complete");
        setNavigation(null, returnToConsoleButton, null);
    }

    /**
     * Exit sits first in focus order (top right), so the window's initial focus would land on it and a stray
     * Space would leave the wizard; start in the step's first control instead.
     */
    private void moveFocusOffExit(Container stepPanel) {
        if (cancelButton.isFocusOwner() || dontShowAgainButton.isFocusOwner()) {
            Component first = firstFocusable(stepPanel);
            if (first != null) {
                first.requestFocusInWindow();
            }
        }
    }

    private static Component firstFocusable(Container root) {
        for (Component child : root.getComponents()) {
            if (!child.isShowing() || !child.isEnabled()) {
                continue;
            }
            if ((child instanceof AbstractButton || child instanceof JComboBox || child instanceof JTextComponent
                || child instanceof JList) && child.isFocusable()) {
                return child;
            }
            if (child instanceof Container) {
                Component found = firstFocusable((Container) child);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }

    /** Puts the shown page's actions at the right of the footer, or its hint when it has no Continue. */
    private void setNavigation(JButton skip, JButton next, String hint) {
        navigationPanel.removeAll();
        if (skip != null) {
            navigationPanel.add(skip);
        }
        if (next != null) {
            navigationPanel.add(next);
        }
        footerHint.setText(next == null && hint != null ? hint : "");
        navigationPanel.revalidate();
        navigationPanel.repaint();
    }

    private int readCylindersCountFromEcu() {
        WizardConfig cfg = WizardConfig.snapshot(uiContext);
        if (cfg == null) return 4;

        IniField field = cfg.ini.findIniField("cylindersCount").orElse(null);
        if (field == null) return 4;

        String value = ConfigurationImageGetterSetter.getStringValue(field, cfg.image);
        try {
            int count = (int) Double.parseDouble(value);
            return count > 0 ? count : 4;
        } catch (NumberFormatException e) {
            return 4;
        }
    }

    private int findFirstIncompleteStep() {
        return findFirstIncomplete(visibleCatalogIndices, this::isStepSatisfied);
    }

    static int findFirstIncomplete(List<Integer> visibleIndices, IntPredicate satisfied) {
        for (int index : visibleIndices) {
            if (!satisfied.test(index)) return index;
        }
        return -1;
    }

    public boolean hasIncompleteSteps() {
        List<Integer> visibleIndices = findVisibleCatalogIndices(
            uiContext, uiContext.iniFileState.getIniFileModel());
        return findFirstIncomplete(visibleIndices, this::isStepSatisfied) >= 0;
    }

    private void configureExitButtons(boolean autoLaunch) {
        cancelButton.setText(autoLaunch ? "Cancel" : "Exit Wizard");
        dontShowAgainButton.setVisible(autoLaunch);
    }

    private void rebuildVisibleCatalogIndices() {
        visibleCatalogIndices.clear();
        visibleCatalogIndices.addAll(findVisibleCatalogIndices(uiContext, uiContext.iniFileState.getIniFileModel()));
        hideImplicitFiringOrderStep(visibleCatalogIndices, selectedCylinders);
        refreshProgress();
    }

    static void hideImplicitFiringOrderStep(List<Integer> visibleIndices, int cylindersCount) {
        if (implicitFiringOrderOrdinal(cylindersCount) != null) {
            visibleIndices.remove(Integer.valueOf(1));
        }
    }

    static Integer implicitFiringOrderOrdinal(int cylindersCount) {
        if (cylindersCount == 1) {
            return FO_1;
        }
        if (cylindersCount == 2) {
            return FO_1_2;
        }
        return null;
    }

    static List<Integer> findVisibleCatalogIndices(UIContext context, IniFileModel ini) {
        List<Integer> result = new ArrayList<>();
        if (ini != null) {
            for (int i = 0; i < FLAGGED.size(); i++) {
                WizardStepDescriptor descriptor = FLAGGED.get(i);
                IniField flag = ini.findIniField(descriptor.flagName).orElse(null);
                if (descriptor.applicable.test(context) && flag instanceof EnumIniField) {
                    result.add(i);
                }
            }
        }
        return result;
    }

    private void refreshProgress() {
        progressPanel.setItems(buildProgressItems(visibleCatalogIndices, currentStepIndex, this::isFlagSet));
    }

    static List<WizardProgressPanel.Item> buildProgressItems(List<Integer> visibleIndices, int currentIndex,
                                                              Predicate<String> completed) {
        List<WizardProgressPanel.Item> items = new ArrayList<>();
        for (int i = 0; i < visibleIndices.size(); i++) {
            int catalogIndex = visibleIndices.get(i);
            WizardStepDescriptor descriptor = FLAGGED.get(catalogIndex);
            WizardProgressPanel.State state;
            if (catalogIndex == currentIndex) {
                state = WizardProgressPanel.State.CURRENT;
            } else if (completed.test(descriptor.flagName)) {
                state = WizardProgressPanel.State.COMPLETED;
            } else {
                state = WizardProgressPanel.State.UPCOMING;
            }
            items.add(new WizardProgressPanel.Item(catalogIndex, i + 1, descriptor.displayTitle, state));
        }
        return items;
    }

    /**
     * A flagged step is "satisfied" (skipped by the wizard flow) when:
     * - it's not applicable to this board, OR
     * - the board's INI does not declare the wizard flag (older firmware without the wizard bits), OR
     * - the flag is already set to "yes".
     */
    private boolean isStepSatisfied(int stepIndex) {
        if (stepIndex >= FLAGGED.size()) return true;
        WizardStepDescriptor d = FLAGGED.get(stepIndex);
        if (!d.applicable.test(uiContext)) return true;
        IniFileModel ini = uiContext.iniFileState.getIniFileModel();
        if (ini != null && !(ini.findIniField(d.flagName).orElse(null) instanceof EnumIniField)) return true;
        return isFlagSet(d.flagName);
    }

    private boolean isFlagSet(String flagName) {
        WizardConfig cfg = WizardConfig.snapshot(uiContext);
        if (cfg == null) return false;

        IniField field = cfg.ini.findIniField(flagName).orElse(null);
        if (field instanceof EnumIniField) {
            EnumIniField ef = (EnumIniField) field;
            int raw = cfg.image.getByteBuffer(ef).getInt();
            int ordinal = ConfigurationImage.getBitRange(raw, ef.getBitPosition(), ef.getBitSize0() + 1);
            return ordinal == 1;
        }
        return false;
    }

    private void createFiringOrderStep() {
        FiringOrderPanel firingPanel = new FiringOrderPanel(uiContext, selectedCylinders);
        wireStep(firingPanel, 1);
        steps.set(1, firingPanel);
        // Replace the placeholder card
        // CardLayout allows adding a new component with the same name — the old one is replaced
        stepContentPanel.add(firingPanel.getPanel(), "step1");
    }

    private void wireStep(WizardStep step, int stepIndex) {
        step.setOnStepCompleted(result -> onStepCompleted(result, steps.get(stepIndex)));
    }

    private void onStepCompleted(WizardStepResult result, WizardStep step) {
        BinaryProtocol bp = uiContext.getBinaryProtocol();
        if (bp == null) {
            messageHandler.accept("ECU is not connected. Please reconnect and try again.");
            return;
        }
        IniFileModel ini = uiContext.iniFileState.getIniFileModel();
        ConfigurationImage image = bp.getControllerConfiguration();
        if (ini == null || image == null) {
            messageHandler.accept("Configuration not loaded. Please reconnect and try again.");
            return;
        }

        // Use the step's modified image if provided (multi-field dialogs), else clone the current ECU image
        ConfigurationImage modified = result.modifiedImage != null ? result.modifiedImage.clone() : image.clone();

        // Set the actual config value if this step provides a single-field edit
        if (result.configFieldName != null && result.value != null) {
            IniField configField = ini.findIniField(result.configFieldName).orElse(null);
            if (configField != null) {
                ConfigurationImageGetterSetter.setValue2(configField, modified, result.configFieldName, result.value);
                log.info("Wizard: set " + result.configFieldName + " = " + result.value);
            } else {
                log.warn("Wizard: INI field not found: " + result.configFieldName);
            }
        }

        // Set the wizard progress flag to "yes" (ordinal 1), if this step has one
        String flagName = step.getWizardFlagFieldName();
        clearDependentWizardFlags(flagName, ini, modified);
        if (flagName != null) {
            IniField flagField = ini.findIniField(flagName).orElse(null);
            if (flagField instanceof EnumIniField) {
                modified.setBitValue((EnumIniField) flagField, 1);
                log.info("Wizard: set flag " + flagName + " = yes");
            } else {
                log.warn("Wizard: flag field not found or not enum: " + flagName);
            }
        }
        completeImplicitFiringOrder(flagName, result.value, ini, modified);

        // Upload + burn on IO thread, advance step on EDT
        uiContext.getLinkManager().submit(() -> {
            bp.uploadChanges(modified);
            SwingUtilities.invokeLater(() -> {
                refreshDebugFlags();
                advanceToNextStep();
            });
        });
    }

    static void clearDependentWizardFlags(String completedFlag, IniFileModel ini, ConfigurationImage image) {
        if ("wizardVeTable".equals(completedFlag)) {
            // Older consoles used wizardDisplacement for VE completion. Always require
            // a separate displacement confirmation after applying or skipping VE.
            IniField displacementFlag = ini.findIniField("wizardDisplacement").orElse(null);
            if (displacementFlag instanceof EnumIniField) {
                image.setBitValue((EnumIniField) displacementFlag, 0);
            }
            return;
        }
        if (!"wizardNumberOfCylinders".equals(completedFlag)) return;

        for (String flagName : new String[]{
            "wizardFiringOrder", "wizardIgnitionOutputs", "wizardInjectorOutputs"
        }) {
            IniField flag = ini.findIniField(flagName).orElse(null);
            if (flag instanceof EnumIniField) {
                image.setBitValue((EnumIniField) flag, 0);
            }
        }
    }

    static void completeImplicitFiringOrder(String completedFlag, String cylindersValue,
                                             IniFileModel ini, ConfigurationImage image) {
        if (!"wizardNumberOfCylinders".equals(completedFlag) || cylindersValue == null) {
            return;
        }

        final int cylindersCount;
        try {
            cylindersCount = Integer.parseInt(cylindersValue);
        } catch (NumberFormatException e) {
            return;
        }

        Integer firingOrderOrdinal = implicitFiringOrderOrdinal(cylindersCount);
        if (firingOrderOrdinal == null) {
            return;
        }

        IniField firingOrderField = ini.findIniField("firingOrder").orElse(null);
        IniField firingOrderFlag = ini.findIniField("wizardFiringOrder").orElse(null);
        if (!(firingOrderField instanceof EnumIniField) || !(firingOrderFlag instanceof EnumIniField)) {
            log.warn("Wizard: cannot complete implicit firing order for " + cylindersCount + " cylinders");
            return;
        }

        image.setBitValue((EnumIniField) firingOrderField, firingOrderOrdinal);
        image.setBitValue((EnumIniField) firingOrderFlag, 1);
        log.info("Wizard: set firingOrder ordinal = " + firingOrderOrdinal + " and wizardFiringOrder = yes");
    }

    private void advanceToNextStep() {
        if (singleStepMode) {
            if (onAllStepsComplete != null) {
                onAllStepsComplete.run();
            }
            return;
        }

        int next = findFirstIncompleteStep();
        if (next >= 0) {
            showStep(next);
        } else if (onAllStepsComplete != null) {
            onAllStepsComplete.run();
        } else {
            showCompletionCard();
        }
    }

    private void showStep(int index) {
        hideCurrentStep();
        currentStepIndex = index;
        if (index == 1 && steps.get(index) == null) {
            selectedCylinders = readCylindersCountFromEcu();
            createFiringOrderStep();
        }
        WizardStep step = steps.get(index);
        activeStepIndex = index;
        refreshProgress();
        CardLayout cl = (CardLayout) stepContentPanel.getLayout();
        cl.show(stepContentPanel, "step" + index);
        if (step != null) {
            step.onShow();
            setNavigation(step.getSkipButton(), step.getContinueButton(), step.getFooterHint());
            SwingUtilities.invokeLater(() -> moveFocusOffExit(step.getPanel()));
        } else {
            setNavigation(null, null, null);
        }
    }

    private void exitWizard() {
        hideCurrentStep();
        if (onWizardExit != null) {
            onWizardExit.run();
        }
    }

    private void hideCurrentStep() {
        if (activeStepIndex >= 0 && activeStepIndex < steps.size()) {
            WizardStep active = steps.get(activeStepIndex);
            if (active != null) {
                active.onHide();
            }
        }
        activeStepIndex = -1;
    }

    List<String> assembledStepFlagsForTests() {
        List<String> result = new ArrayList<>();
        for (WizardStep step : steps) {
            result.add(step == null ? "wizardFiringOrder" : step.getWizardFlagFieldName());
        }
        return result;
    }

    boolean isProgressVisibleForTests() {
        return progressPanel.isVisible();
    }

    String getWizardTitleForTests() {
        return wizardTitle.getText();
    }

    String getCancelButtonTextForTests() {
        return cancelButton.getText();
    }

    boolean isDontShowAgainVisibleForTests() {
        return dontShowAgainButton.isVisible();
    }

    Object getExitPanelConstraintForTests() {
        return ((BorderLayout) exitPanel.getParent().getLayout()).getConstraints(exitPanel);
    }

    Object getNavigationConstraintForTests() {
        return ((BorderLayout) footerPanel.getLayout()).getConstraints(navigationPanel);
    }

    List<Component> getNavigationForTests() {
        return java.util.Arrays.asList(navigationPanel.getComponents());
    }

    String getFooterHintForTests() {
        return footerHint.getText();
    }

    void clickDontShowAgainForTests() {
        dontShowAgainButton.doClick();
    }

    void clickCancelForTests() {
        cancelButton.doClick();
    }
}
