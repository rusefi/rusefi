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
import java.awt.*;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.function.IntPredicate;
import java.util.function.Predicate;

import static com.devexperts.logging.Logging.getLogging;

public class WizardContainer extends JPanel {
    private static final Logging log = getLogging(WizardContainer.class);
    private static final List<WizardStepDescriptor> FLAGGED = WizardCatalog.flaggedSteps();
    private static final int FO_1 = 0;
    private static final int FO_1_2 = 8;

    private final UIContext uiContext;
    private final WizardProgressPanel progressPanel = new WizardProgressPanel();
    private final JPanel stepContentPanel = new JPanel(new CardLayout());
    private final List<WizardStep> steps = new ArrayList<>();
    private final List<Integer> visibleCatalogIndices = new ArrayList<>();

    // Debug panel labels, keyed by flag name
    private final Map<String, JLabel> debugLabels = new LinkedHashMap<>();
    private final JPanel debugPanel = new JPanel();

    private Runnable onWizardExit;
    private Runnable onAllStepsComplete;
    private int currentStepIndex = 0;
    private int activeStepIndex = -1;
    private boolean singleStepMode;
    private int selectedCylinders = 4; // default, updated by step 0

    public WizardContainer(UIContext uiContext) {
        this(uiContext, false);
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
        headerPanel.setBorder(BorderFactory.createEmptyBorder(
            compact ? WizardStyle.GAP : WizardStyle.LARGE_GAP,
            WizardStyle.LARGE_GAP,
            WizardStyle.GAP,
            WizardStyle.LARGE_GAP));
        headerPanel.add(progressPanel, BorderLayout.CENTER);

        JButton cancelButton = new JButton("Exit Wizard");
        AbstractWizardStep.styleButton(cancelButton);
        cancelButton.addActionListener(e -> exitWizard());
        headerPanel.add(cancelButton, BorderLayout.EAST);
        progressPanel.setOnCompletedSelected(this::showStep);

        add(headerPanel, BorderLayout.NORTH);
        stepContentPanel.setBorder(BorderFactory.createEmptyBorder(
            WizardStyle.GAP, WizardStyle.LARGE_GAP, WizardStyle.LARGE_GAP, WizardStyle.LARGE_GAP));
        add(stepContentPanel, BorderLayout.CENTER);

        // Debug panel at the bottom showing wizard flag states
        buildDebugPanel();
        debugPanel.setVisible(false);
        add(debugPanel, BorderLayout.SOUTH);
    }

    private void buildDebugPanel() {
        debugPanel.setLayout(new FlowLayout(FlowLayout.LEFT, 15, 5));
        debugPanel.setBorder(BorderFactory.createCompoundBorder(
            BorderFactory.createMatteBorder(1, 0, 0, 0, Color.GRAY),
            BorderFactory.createEmptyBorder(5, 10, 5, 10)
        ));

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
                label.setForeground(Color.GRAY);
                continue;
            }

            if (!d.applicable.test(uiContext)) {
                label.setText(shortName + ": N/A");
                label.setForeground(Color.GRAY);
                continue;
            }

            IniField field = ini.findIniField(d.flagName).orElse(null);
            if (field == null) {
                // Board's INI doesn't declare this wizard flag — render as unsupported.
                label.setText(shortName + ": --");
                label.setForeground(Color.GRAY);
                continue;
            }
            if (field instanceof EnumIniField) {
                EnumIniField ef = (EnumIniField) field;
                int raw = image.getByteBuffer(ef).getInt();
                int ordinal = ConfigurationImage.getBitRange(raw, ef.getBitPosition(), ef.getBitSize0() + 1);
                boolean isSet = ordinal == 1;
                label.setText(shortName + ": " + (isSet ? "yes" : "no"));
                label.setForeground(isSet ? new Color(0, 160, 0) : Color.RED);
            } else {
                label.setText(shortName + ": ?");
                label.setForeground(Color.GRAY);
            }
        }
    }

    public void setOnWizardExit(Runnable onWizardExit) {
        this.onWizardExit = onWizardExit;
    }

    public void startWizard() {
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

        // Step 2: Base VE Table (wizardDisplacement — silently skipped on older firmware)
        VeTableWizardStep veStep = new VeTableWizardStep(uiContext);
        wireStep(veStep, 2);
        steps.add(veStep);
        stepContentPanel.add(veStep.getPanel(), "step2");

        // Step 3: MAP Sensor Type
        MapSensorTypePanel mapPanel = new MapSensorTypePanel(uiContext);
        wireStep(mapPanel, 3);
        steps.add(mapPanel);
        stepContentPanel.add(mapPanel.getPanel(), "step3");

        // Step 4: TPS
        TpsPanel tpsPanel = new TpsPanel(uiContext);
        wireStep(tpsPanel, 4);
        steps.add(tpsPanel);
        stepContentPanel.add(tpsPanel.getPanel(), "step4");

        // Step 5: CLT Sensor
        CltSensorPanel cltPanel = new CltSensorPanel(uiContext);
        wireStep(cltPanel, 5);
        steps.add(cltPanel);
        stepContentPanel.add(cltPanel.getPanel(), "step5");

        // Step 6: Crank Trigger
        CrankTriggerPanel crankPanel = new CrankTriggerPanel(uiContext);
        wireStep(crankPanel, 6);
        steps.add(crankPanel);
        stepContentPanel.add(crankPanel.getPanel(), "step6");

        // Step 7: Cam Trigger
        CamTriggerPanel camPanel = new CamTriggerPanel(uiContext);
        wireStep(camPanel, 7);
        steps.add(camPanel);
        stepContentPanel.add(camPanel.getPanel(), "step7");

        // Step 8: Ignition Outputs
        OutputAssignmentPanel ignitionOutputs = new OutputAssignmentPanel(
            uiContext, OutputAssignmentPanel.OutputType.IGNITION);
        wireStep(ignitionOutputs, 8);
        steps.add(ignitionOutputs);
        stepContentPanel.add(ignitionOutputs.getPanel(), "step8");

        // Step 9: Injector Outputs
        OutputAssignmentPanel injectorOutputs = new OutputAssignmentPanel(
            uiContext, OutputAssignmentPanel.OutputType.INJECTOR);
        wireStep(injectorOutputs, 9);
        steps.add(injectorOutputs);
        stepContentPanel.add(injectorOutputs.getPanel(), "step9");

        // Step 10: Injector Flow
        InjectorFlowPanel injPanel = new InjectorFlowPanel(uiContext);
        wireStep(injPanel, 10);
        steps.add(injPanel);
        stepContentPanel.add(injPanel.getPanel(), "step10");

        // Completion card
        JPanel completionPanel = new JPanel(new GridBagLayout());
        JLabel doneLabel = new JLabel("Wizard Complete!");
        doneLabel.setHorizontalAlignment(SwingConstants.CENTER);
        AbstractWizardStep.styleTitle(doneLabel);
        completionPanel.add(doneLabel);
        JButton doneButton = new JButton("Return to Console");
        AbstractWizardStep.stylePrimaryAction(doneButton);
        doneButton.addActionListener(e -> exitWizard());
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.gridy = 1;
        gbc.insets = new Insets(20, 0, 0, 0);
        completionPanel.add(doneButton, gbc);
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
            JOptionPane.showMessageDialog(this,
                "ECU is not connected. Please reconnect and try again.",
                "Not Connected", JOptionPane.WARNING_MESSAGE);
            return;
        }
        IniFileModel ini = uiContext.iniFileState.getIniFileModel();
        ConfigurationImage image = bp.getControllerConfiguration();
        if (ini == null || image == null) {
            JOptionPane.showMessageDialog(this,
                "Configuration not loaded. Please reconnect and try again.",
                "Error", JOptionPane.WARNING_MESSAGE);
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
}
