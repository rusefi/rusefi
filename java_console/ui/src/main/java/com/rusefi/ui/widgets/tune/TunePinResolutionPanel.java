package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.DialogModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.EnumIniField;
import com.rusefi.tune.xml.Msq;
import com.rusefi.ui.util.ScrollablePanel;

import javax.swing.*;
import java.awt.*;
import java.util.Collections;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.concurrent.CompletableFuture;

public class TunePinResolutionPanel {
    static final int CARD_WIDTH = 960;
    private static final int REPLACEMENT_WIDTH = 360;
    private static final String DROP_OPTION = "Drop obsolete setting";

    public static final class Resolution {
        private final Map<String, String> selections;

        private Resolution(Map<String, String> selections) {
            this.selections = selections;
        }

        public Map<String, String> getSelections() {
            return Collections.unmodifiableMap(selections);
        }
    }

    private final JPanel content = new JPanel(new BorderLayout());
    private final JPanel card = new JPanel(new BorderLayout(0, 16)) {
        @Override
        public Dimension getPreferredSize() {
            Dimension preferred = super.getPreferredSize();
            preferred.width = CARD_WIDTH;
            return preferred;
        }

        @Override
        public Dimension getMaximumSize() {
            Dimension preferred = super.getPreferredSize();
            return new Dimension(CARD_WIDTH, preferred.height);
        }
    };
    private final JPanel rows = new JPanel(new GridBagLayout());
    private final JButton cancelButton = new JButton("Cancel");
    private final JButton applyButton = new JButton();
    private final Map<Msq.ApplyIssue, JComboBox<String>> editors = new LinkedHashMap<>();
    private final Runnable onCancel;
    private final Runnable onApplyStarted;
    private CompletableFuture<Resolution> currentRequest;

    public TunePinResolutionPanel(Runnable onCancel, Runnable onApplyStarted) {
        this.onCancel = onCancel;
        this.onApplyStarted = onApplyStarted;

        JPanel header = new JPanel();
        header.setLayout(new BoxLayout(header, BoxLayout.Y_AXIS));
        JLabel title = new JLabel("Tune needs pin assignments");
        title.setFont(title.getFont().deriveFont(Font.BOLD, 24f));
        title.setAlignmentX(Component.LEFT_ALIGNMENT);
        JLabel guidance = new JLabel(
            "<html>Some saved pins are not available by their old names. Choose a replacement or select NONE to disable the assignment.</html>");
        guidance.setAlignmentX(Component.LEFT_ALIGNMENT);
        header.add(title);
        header.add(Box.createVerticalStrut(8));
        header.add(guidance);

        card.setBorder(BorderFactory.createCompoundBorder(
            BorderFactory.createLineBorder(new Color(0xC8, 0xC8, 0xC8)),
            BorderFactory.createEmptyBorder(22, 26, 22, 26)));
        card.add(header, BorderLayout.NORTH);
        card.add(rows, BorderLayout.CENTER);

        cancelButton.addActionListener(e -> cancel());
        applyButton.addActionListener(e -> apply());
        applyButton.setFont(applyButton.getFont().deriveFont(Font.BOLD));
        JPanel actions = new JPanel(new FlowLayout(FlowLayout.RIGHT, 12, 0));
        actions.add(cancelButton);
        actions.add(applyButton);
        card.add(actions, BorderLayout.SOUTH);

        JPanel wrapper = new ScrollablePanel();
        wrapper.setLayout(new BoxLayout(wrapper, BoxLayout.Y_AXIS));
        wrapper.setBorder(BorderFactory.createEmptyBorder(16, 16, 16, 16));
        card.setAlignmentX(Component.CENTER_ALIGNMENT);
        wrapper.add(card);
        content.add(new JScrollPane(wrapper), BorderLayout.CENTER);
    }

    public JPanel getContent() {
        return content;
    }

    public void showRequest(Msq.ApplyResult result, IniFileModel ini, boolean connected,
                            CompletableFuture<Resolution> request) {
        currentRequest = request;
        editors.clear();
        rows.removeAll();
        applyButton.setText(connected ? "Apply & Burn" : "Open Tune");

        GridBagConstraints gbc = new GridBagConstraints();
        gbc.gridy = 0;
        gbc.insets = new Insets(4, 6, 8, 12);
        gbc.anchor = GridBagConstraints.WEST;
        String[] headers = {"TYPE", "SETTING", "SAVED ASSIGNMENT", "CURRENT ASSIGNMENT", "REPLACEMENT"};
        for (int column = 0; column < headers.length; column++) {
            gbc.gridx = column;
            boolean flexible = column == 1;
            gbc.weightx = flexible ? 1 : 0;
            gbc.fill = flexible ? GridBagConstraints.HORIZONTAL : GridBagConstraints.NONE;
            JLabel label = new JLabel(headers[column]);
            label.setForeground(Color.GRAY);
            rows.add(label, gbc);
        }

        int row = 1;
        for (Msq.ApplyIssue issue : result.getUnresolvedPins()) {
            EnumIniField targetField = issue.getTargetField();
            JComboBox<String> selector = new JComboBox<>();
            if (targetField == null) {
                selector.addItem(DROP_OPTION);
            } else {
                for (String value : targetField.getEnums().values()) {
                    if (value != null && !value.isEmpty() && !"INVALID".equalsIgnoreCase(value)) {
                        selector.addItem(value);
                    }
                }
            }
            selector.setSelectedIndex(-1);
            Dimension selectorSize = selector.getPreferredSize();
            selectorSize.width = REPLACEMENT_WIDTH;
            selector.setPreferredSize(selectorSize);
            selector.addActionListener(e -> updateApplyEnabled());
            editors.put(issue, selector);

            String fieldName = issue.getConstant().getName();
            DialogModel.Field uiField = ini.getFieldsInUiOrder().get(fieldName);
            String friendlyName = uiField == null ? fieldName : uiField.getUiName();
            String currentValue = targetField == null ? "Unavailable" : readCurrentValue(targetField, result);
            String savedValue = issue.getConstant().getValue().replace("\"", "");

            addCell(issue.getPinType().getDisplayName(), row, 0, false);
            addCell("<html>" + friendlyName + "<br><small>" + fieldName + "</small></html>", row, 1, true);
            addCell(savedValue, row, 2, false);
            addCell(currentValue, row, 3, false);
            gbc.gridy = row;
            gbc.gridx = 4;
            gbc.weightx = 0;
            gbc.fill = GridBagConstraints.HORIZONTAL;
            rows.add(selector, gbc);
            row++;
        }

        updateApplyEnabled();
        rows.revalidate();
        rows.repaint();
    }

    private String readCurrentValue(EnumIniField field, Msq.ApplyResult result) {
        try {
            return ConfigurationImageGetterSetter.getStringValue(field, result.getImage()).replace("\"", "");
        } catch (RuntimeException e) {
            return "Unavailable";
        }
    }

    private void addCell(String text, int row, int column, boolean fill) {
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.gridy = row;
        gbc.gridx = column;
        gbc.insets = new Insets(4, 6, 4, 12);
        gbc.anchor = GridBagConstraints.WEST;
        gbc.weightx = fill ? 1 : 0;
        gbc.fill = fill ? GridBagConstraints.HORIZONTAL : GridBagConstraints.NONE;
        rows.add(new JLabel(text), gbc);
    }

    private void updateApplyEnabled() {
        applyButton.setEnabled(!editors.isEmpty()
            && editors.values().stream().allMatch(selector -> selector.getSelectedItem() != null));
    }

    private void cancel() {
        CompletableFuture<Resolution> request = currentRequest;
        currentRequest = null;
        if (request != null) {
            request.complete(null);
            onCancel.run();
        }
    }

    public void cancelPending() {
        cancel();
    }

    private void apply() {
        if (!applyButton.isEnabled()) {
            return;
        }
        Map<String, String> selections = new LinkedHashMap<>();
        for (Map.Entry<Msq.ApplyIssue, JComboBox<String>> entry : editors.entrySet()) {
            Msq.ApplyIssue issue = entry.getKey();
            if (issue.getTargetField() != null) {
                selections.put(issue.getConstant().getName(), (String) entry.getValue().getSelectedItem());
            }
        }
        CompletableFuture<Resolution> request = currentRequest;
        currentRequest = null;
        if (request != null) {
            onApplyStarted.run();
            request.complete(new Resolution(selections));
        }
    }

    int getIssueCountForUnitTest() {
        return editors.size();
    }

    boolean isApplyEnabledForUnitTest() {
        return applyButton.isEnabled();
    }

    void selectForUnitTest(int index, String value) {
        editors.values().toArray(new JComboBox[0])[index].setSelectedItem(value);
    }

    void applyForUnitTest() {
        applyButton.doClick();
    }

    void cancelForUnitTest() {
        cancelButton.doClick();
    }

    JComponent getCardForTests() {
        return card;
    }
}
