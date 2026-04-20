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

import static com.devexperts.logging.Logging.getLogging;

public class WizardContainer extends JPanel {
    private static final Logging log = getLogging(WizardContainer.class);
    private static final List<WizardStepDescriptor> FLAGGED = WizardCatalog.flaggedSteps();
    private static final int TOTAL_STEPS = FLAGGED.size();

    private final UIContext uiContext;
    private final JLabel stepLabel = new JLabel();
    private final JPanel stepContentPanel = new JPanel(new CardLayout());
    private final List<WizardStep> steps = new ArrayList<>();

    // Debug panel labels, keyed by flag name
    private final Map<String, JLabel> debugLabels = new LinkedHashMap<>();
    private final JPanel debugPanel = new JPanel();

    private Runnable onWizardExit;
    private Runnable onAllStepsComplete;
    private int currentStepIndex = 0;
    private int totalSteps = TOTAL_STEPS;
    private int selectedCylinders = 4; // default, updated by step 0

    public WizardContainer(UIContext uiContext) {
        super(new BorderLayout());
        this.uiContext = uiContext;

        // Header panel
        JPanel headerPanel = new JPanel(new BorderLayout());
        headerPanel.setBorder(BorderFactory.createEmptyBorder(10, 20, 10, 20));

        scaleComponent(stepLabel, 1.5f);
        headerPanel.add(stepLabel, BorderLayout.CENTER);

        JButton cancelButton = new JButton("Exit Wizard");
        scaleComponent(cancelButton, 1.5f);
        cancelButton.addActionListener(e -> exitWizard());
        headerPanel.add(cancelButton, BorderLayout.EAST);

        add(headerPanel, BorderLayout.NORTH);
        stepContentPanel.setBorder(BorderFactory.createEmptyBorder(10, 20, 20, 20));
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
        currentStepIndex = 0;
        totalSteps = TOTAL_STEPS;
        onAllStepsComplete = this::showCompletionCard;
        // uncomment to show the debug panel; we need it to reset the flags on local env
        // debugPanel.setVisible(true);
        steps.clear();
        stepContentPanel.removeAll();

        // Step 0: Number of Cylinders
        NumberOfCylindersPanel cylPanel = new NumberOfCylindersPanel(uiContext);
        cylPanel.setOnStepCompleted(result -> {
            if (result.value != null) {
                selectedCylinders = Integer.parseInt(result.value);
            }
            // Create FiringOrderPanel lazily now that we know the cylinder count
            createFiringOrderStep();
            onStepCompleted(result, steps.get(0));
        });
        steps.add(cylPanel);
        stepContentPanel.add(cylPanel.getPanel(), "step0");

        // Step 1: FiringOrder — placeholder card, replaced lazily in createFiringOrderStep()
        JPanel firingOrderPlaceholder = new JPanel();
        stepContentPanel.add(firingOrderPlaceholder, "step1");
        steps.add(null); // placeholder slot

        // Step 2: MAP Sensor Type
        MapSensorTypePanel mapPanel = new MapSensorTypePanel(uiContext);
        wireStep(mapPanel, 2);
        steps.add(mapPanel);
        stepContentPanel.add(mapPanel.getPanel(), "step2");

        // Step 3: Crank Trigger
        CrankTriggerPanel crankPanel = new CrankTriggerPanel(uiContext);
        wireStep(crankPanel, 3);
        steps.add(crankPanel);
        stepContentPanel.add(crankPanel.getPanel(), "step3");

        // Step 4: Cam Trigger
        CamTriggerPanel camPanel = new CamTriggerPanel(uiContext);
        wireStep(camPanel, 4);
        steps.add(camPanel);
        stepContentPanel.add(camPanel.getPanel(), "step4");

        // Step 5: Injector Flow
        InjectorFlowPanel injPanel = new InjectorFlowPanel(uiContext);
        wireStep(injPanel, 5);
        steps.add(injPanel);
        stepContentPanel.add(injPanel.getPanel(), "step5");

        // Completion card
        JPanel completionPanel = new JPanel(new GridBagLayout());
        JLabel doneLabel = new JLabel("Wizard Complete!");
        scaleComponent(doneLabel, 3);
        completionPanel.add(doneLabel);
        JButton doneButton = new JButton("Return to Console");
        scaleComponent(doneButton, 2);
        doneButton.addActionListener(e -> exitWizard());
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.gridy = 1;
        gbc.insets = new Insets(20, 0, 0, 0);
        completionPanel.add(doneButton, gbc);
        stepContentPanel.add(completionPanel, "complete");

        // If step 0 is already done, read cylindersCount from the ECU and create FiringOrderPanel now
        if (isStepSatisfied(0)) {
            selectedCylinders = readCylindersCountFromEcu();
            createFiringOrderStep();
        }

        refreshDebugFlags();

        // Skip to the first incomplete step
        int firstIncomplete = findFirstIncompleteStep();
        if (firstIncomplete >= TOTAL_STEPS) {
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
        currentStepIndex = 0;
        totalSteps = 1;
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
        stepLabel.setText("Configuration Complete");
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
        for (int i = 0; i < TOTAL_STEPS; i++) {
            if (!isStepSatisfied(i)) {
                return i;
            }
        }
        return TOTAL_STEPS;
    }

    /** Total flagged steps applicable to this board — drives the "of N" denominator shown to the user. */
    private int applicableFlaggedCount() {
        int count = 0;
        for (WizardStepDescriptor d : FLAGGED) {
            if (d.applicable.test(uiContext)) count++;
        }
        return count;
    }

    /** 1-based position of flagged step {@code index} among applicable steps — drives the "Step N" numerator. */
    private int applicableCountUpTo(int index) {
        int count = 0;
        for (int i = 0; i <= index && i < FLAGGED.size(); i++) {
            if (FLAGGED.get(i).applicable.test(uiContext)) count++;
        }
        return count;
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
        if (ini != null && !ini.findIniField(d.flagName).isPresent()) return true;
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
        if (flagName != null) {
            IniField flagField = ini.findIniField(flagName).orElse(null);
            if (flagField instanceof EnumIniField) {
                modified.setBitValue((EnumIniField) flagField, 1);
                log.info("Wizard: set flag " + flagName + " = yes");
            } else {
                log.warn("Wizard: flag field not found or not enum: " + flagName);
            }
        }

        // Upload + burn on IO thread, advance step on EDT
        uiContext.getLinkManager().submit(() -> {
            bp.uploadChanges(modified);
            SwingUtilities.invokeLater(() -> {
                refreshDebugFlags();
                advanceToNextStep();
            });
        });
    }

    private void advanceToNextStep() {
        // Skip ahead past satisfied steps (already-set flag or non-applicable to this board)
        int next = currentStepIndex + 1;
        while (next < totalSteps && next < FLAGGED.size() && isStepSatisfied(next)) {
            log.info("Wizard: skipping satisfied step " + next + " (" + FLAGGED.get(next).flagName + ")");
            next++;
        }

        if (next < totalSteps) {
            showStep(next);
        } else if (onAllStepsComplete != null) {
            onAllStepsComplete.run();
        } else {
            showCompletionCard();
        }
    }

    private void showStep(int index) {
        currentStepIndex = index;
        WizardStep step = steps.get(index);
        String title = step != null ? step.getTitle() : "...";
        if (totalSteps == 1) {
            stepLabel.setText(title);
        } else {
            stepLabel.setText("Step " + applicableCountUpTo(index) + " of " + applicableFlaggedCount() + ": " + title);
        }
        CardLayout cl = (CardLayout) stepContentPanel.getLayout();
        cl.show(stepContentPanel, "step" + index);
        if (step != null) {
            step.onShow();
        }
    }

    private void exitWizard() {
        if (onWizardExit != null) {
            onWizardExit.run();
        }
    }

    private void scaleComponent(JComponent component, float factor) {
        Font font = component.getFont();
        component.setFont(font.deriveFont(font.getSize() * factor));
    }
}
