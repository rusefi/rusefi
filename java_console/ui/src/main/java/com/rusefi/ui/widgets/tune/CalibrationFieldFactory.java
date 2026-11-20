package com.rusefi.ui.widgets.tune;

import com.rusefi.ui.PinoutLookup;
import com.rusefi.ui.widgets.ConnectorIcon;
import com.rusefi.ui.widgets.TuningIcons;
import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.StringIniField;
import com.opensr5.ini.DialogModel;
import com.opensr5.ini.ContextHelpModel;
import com.opensr5.ini.IniFileModel;

import javax.swing.*;
import javax.swing.text.AbstractDocument;
import javax.swing.text.AttributeSet;
import javax.swing.text.BadLocationException;
import javax.swing.text.DocumentFilter;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.event.HyperlinkEvent;
import java.awt.*;
import java.awt.datatransfer.StringSelection;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.net.URI;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.function.Consumer;
import java.util.function.Supplier;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Factory for creating individual UI widgets used in calibration dialogs.
 * Handles field rows (label and input), label-only rows, and command button rows.
 *
 * @see CalibrationDialogWidget
 */
public class CalibrationFieldFactory {
    static final int MAX_FIELD_EDITOR_WIDTH = 360;
    static final int MAX_LABEL_WIDTH = 360;
    /**
     * Room added to the measured width of the label column. Labels are measured before they are
     * shown; on screen (display scaling, text antialiasing) the text can be a pixel or two wider,
     * and a label even 1 px short is cut to "..." several letters early.
     */
    static final int LABEL_TEXT_SLACK = 6;
    private static final int HELP_ICON_SIZE = 18;
    private static final int HELP_COLUMN_WIDTH = 24;
    private static final Icon SETTING_HELP_ICON = loadHelpIcon();
    private static final Pattern URL_PATTERN = Pattern.compile("https?://[^\\s<>\"']+");

    private static class CalibrationTextField extends JTextField {
        private int fieldEditorWidth;

        CalibrationTextField(String text, int columns) {
            super(text, columns);
        }

        void setFieldEditorWidth(int fieldEditorWidth) {
            this.fieldEditorWidth = fieldEditorWidth;
        }

        @Override
        public Dimension getPreferredSize() {
            Dimension size = super.getPreferredSize();
            if (fieldEditorWidth > 0) {
                size.width = fieldEditorWidth;
            }
            return size;
        }
    }

    // we need to maintain this in sync with the ones used on tunerstudio.template
    private static final String[][] CheckboxPairs = {
        {"yes", "no"},
        {"enabled", "disabled"}
    };
    private static final String BLUE_PREFIX = "#";
    public static final String RED_PREFIX = "!";

    public static JPanel createFieldRow(DialogModel.Field field, IniField iniField, ConfigurationImage ci, ConfigurationImage workingImage) {
        return createFieldRow(field, iniField, ci, workingImage, null, null);
    }

    public static JPanel createFieldRow(DialogModel.Field field, IniField iniField, ConfigurationImage ci, ConfigurationImage workingImage, Runnable onChange) {
        return createFieldRow(field, iniField, ci, workingImage, onChange, null);
    }

    public static JPanel createFieldRow(DialogModel.Field field, IniField iniField, ConfigurationImage ci, ConfigurationImage workingImage, Runnable onChange, PinoutLink onShowInPinout) {
        return createFieldRow(field, iniField, ci, workingImage, onChange, onShowInPinout, 0);
    }

    static JPanel createFieldRow(DialogModel.Field field, IniField iniField, ConfigurationImage ci,
                                 ConfigurationImage workingImage, Runnable onChange,
                                 PinoutLink onShowInPinout, int labelWidth) {
        return createFieldRow(field, iniField, ci, workingImage, onChange, onShowInPinout, labelWidth, 0);
    }

    static JPanel createFieldRow(DialogModel.Field field, IniField iniField, ConfigurationImage ci,
                                 ConfigurationImage workingImage, Runnable onChange,
                                 PinoutLink onShowInPinout, int labelWidth, int fieldEditorWidth) {
        return createFieldRow(field, iniField, ci, workingImage, onChange, onShowInPinout,
            labelWidth, fieldEditorWidth, null);
    }

    static JPanel createFieldRow(DialogModel.Field field, IniField iniField, ConfigurationImage ci,
                                 ConfigurationImage workingImage, Runnable onChange,
                                 PinoutLink onShowInPinout, int labelWidth, int fieldEditorWidth,
                                 String helpText) {
        JPanel row = createRowPanel();
        row.setLayout(new SettingRowLayout(row));
        // a row with help shows its help card; tooltips that only repeat the row would compete with it
        helpText = SettingHelpCard.usefulHelp(helpText);
        boolean hasHelp = helpText != null;
        row.add(Box.createHorizontalStrut(10));
        row.add(createHelpSlot(helpText));

        String labelText = field.getUiName();
        JLabel label = new JLabel(labelText);
        applyStyle(label);
        if (labelWidth > 0) {
            if (labelText != null && label.getPreferredSize().width > labelWidth && !labelText.startsWith("<html>")) {
                label.setText("<html><div style='width: " + (labelWidth - 4) + "px'>" +
                    escapeHtml(labelText) + "</div></html>");
                if (!hasHelp) {
                    label.setToolTipText(labelText);
                }
            }
            Dimension size = label.getPreferredSize();
            size.width = labelWidth;
            size.height = Math.min(size.height, label.getFontMetrics(label.getFont()).getHeight() * 2);
            label.setMinimumSize(size);
            label.setPreferredSize(size);
            label.setMaximumSize(size);
        }
        row.add(label);
        SettingRowLayout.markWrapAfter(label);
        row.add(Box.createHorizontalStrut(16));

        Supplier<String> value;
        // [tag:unburned_marks] shows a stored value (as getStringValue gives it) in the editor
        Consumer<String> showStoredValue;
        if (iniField instanceof EnumIniField) {
            EnumIniField enumField = (EnumIniField) iniField;
            String currentValue = ci == null ? "" : ConfigurationImageGetterSetter.getStringValue(iniField, ci);

            if (isCheckboxEnum(enumField)) {
                JCheckBox checkBox = createCheckBox(enumField, iniField, currentValue, workingImage, onChange);
                row.add(checkBox);
                value = () -> checkBox.isSelected() ? "enabled" : "disabled";
                showStoredValue = stored -> checkBox.setSelected(isCheckboxOn(stored));
            } else {
                JComboBox<Object> comboBox = createComboBox(
                    enumField, iniField, currentValue, workingImage, onChange, fieldEditorWidth, !hasHelp);
                row.add(comboBox);
                value = () -> String.valueOf(comboBox.getSelectedItem());
                showStoredValue = stored -> comboBox.setSelectedItem(stored.replace("\"", ""));
                JButton pinoutButton = createPinoutButton(comboBox, field.getKey(), enumField, onShowInPinout);
                if (pinoutButton != null) {
                    row.add(Box.createHorizontalStrut(4));
                    row.add(pinoutButton);
                }
            }
        } else {
            String currentValue = ci == null ? "" : ConfigurationImageGetterSetter.getStringValue(iniField, ci);
            JTextField textField = createTextField(
                iniField, currentValue, workingImage, onChange, fieldEditorWidth);
            row.add(textField);
            value = textField::getText;
            showStoredValue = textField::setText;
        }
        JPanel unburnedSlot = UnburnedRowMark.createSlot();
        row.add(unburnedSlot);
        installCopyListener(label, labelText, value, !hasHelp);
        fixRowHeight(row);
        String description = SettingHelpCard.isShowDescriptions() ? SettingHelpCard.descriptionWithHint(helpText) : null;
        if (description != null) {
            row = withDescription(row, description);
        }
        row.putClientProperty(UnburnedRowMark.SLOT, unburnedSlot);
        row.putClientProperty(UnburnedRowMark.EDITOR, showStoredValue);
        SettingHelpCard.attach(row, labelText, iniField.getUnits(), helpText);
        return row;
    }

    /**
     * The row with its description on a second line, under the name ("Descriptions" on the Tuning
     * toolbar). The text wraps within the width of the setting line above it, so descriptions never
     * make a dialog wider. It stays one panel, so a hidden or disabled setting hides or greys its
     * description too.
     */
    private static JPanel withDescription(JPanel line, String text) {
        int indent = 10 + HELP_COLUMN_WIDTH;
        int width = Math.max(120, line.getPreferredSize().width - indent);
        JTextArea description = new JTextArea(text);
        description.setName("settingDescription");
        description.setLineWrap(true);
        description.setWrapStyleWord(true);
        description.setEditable(false);
        description.setFocusable(false);
        description.setHighlighter(null);
        description.setOpaque(false);
        description.setFont(UIManager.getFont("Label.font"));
        Color muted = TuningStyle.color("Console.Tuning.mutedForeground", Color.GRAY);
        description.setForeground(muted);
        description.setDisabledTextColor(UIManager.getColor("Label.disabledForeground"));
        description.setBorder(BorderFactory.createEmptyBorder(0, indent, 4, 0));
        // measure the wrapped height at the line's width
        description.setSize(new Dimension(width + indent, Short.MAX_VALUE));
        Dimension size = new Dimension(width + indent, description.getPreferredSize().height);
        description.setPreferredSize(size);
        description.setMaximumSize(size);
        description.setAlignmentX(Component.LEFT_ALIGNMENT);

        JPanel row = new JPanel();
        row.setLayout(new BoxLayout(row, BoxLayout.Y_AXIS));
        row.setAlignmentX(Component.LEFT_ALIGNMENT);
        line.setAlignmentX(Component.LEFT_ALIGNMENT);
        row.add(line);
        row.add(description);
        fixRowHeight(row);
        return row;
    }

    private static JComponent createHelpSlot(String helpText) {
        JPanel slot = new JPanel(new FlowLayout(FlowLayout.LEFT, 0, 0));
        slot.setName("settingHelpSlot");
        // shows the row's background, including a FieldHighlight tint
        slot.setOpaque(false);
        Dimension slotSize = new Dimension(HELP_COLUMN_WIDTH, HELP_ICON_SIZE);
        slot.setMinimumSize(slotSize);
        slot.setPreferredSize(slotSize);
        slot.setMaximumSize(slotSize);

        if (helpText == null || helpText.trim().isEmpty()) {
            return slot;
        }

        JButton button = new JButton(SETTING_HELP_ICON);
        button.setName("settingHelpButton");
        button.setBorderPainted(false);
        button.setContentAreaFilled(false);
        button.setFocusPainted(false);
        button.setMargin(new Insets(0, 0, 0, 0));
        button.setPreferredSize(new Dimension(HELP_ICON_SIZE, HELP_ICON_SIZE));
        button.setMaximumSize(button.getPreferredSize());
        button.getAccessibleContext().setAccessibleName("Setting help");
        button.addActionListener(event -> SettingHelpCard.showExpanded(button));
        slot.add(button);
        return slot;
    }

    private static Icon loadHelpIcon() {
        ImageIcon source = TuningIcons.load("help");
        return source == null ? null : new ImageIcon(
            source.getImage().getScaledInstance(HELP_ICON_SIZE, HELP_ICON_SIZE, Image.SCALE_SMOOTH));
    }

    static JButton createTopicHelpButton(IniFileModel ini, String reference) {
        if (reference == null || reference.trim().isEmpty()) {
            return null;
        }
        reference = reference.trim();
        ContextHelpModel help = ini.getContextHelp(reference);
        SettingHelpCard.Help card;
        if (help != null) {
            card = new SettingHelpCard.Help(help.getTitle(), "", topicText(help.getTextLines()), help.getWebHelp());
        } else if (reference.startsWith("https://") || reference.startsWith("http://")) {
            card = new SettingHelpCard.Help("Help", "", "", reference);
        } else {
            return null;
        }
        JButton button = new JButton("Help", SETTING_HELP_ICON);
        button.setName("topicHelpButton");
        button.setToolTipText("Help for this page");
        button.getAccessibleContext().setAccessibleName(help == null ? "Topic help" : help.getTitle());
        button.putClientProperty(SettingHelpCard.TOPIC_KEY, card);
        button.addActionListener(event -> SettingHelpCard.showTopic(button, card));
        return button;
    }

    /** INI topic help lines may use simple HTML such as {@code <br>}; the card shows plain text. */
    static String topicText(List<String> lines) {
        return String.join("\n", lines)
            .replaceAll("(?i)<br\\s*/?>|</p>", "\n")
            .replaceAll("<[^>]*>", "")
            .replace("&nbsp;", " ").replace("&lt;", "<").replace("&gt;", ">")
            .replace("&quot;", "\"").replace("&#39;", "'").replace("&amp;", "&")
            .trim();
    }

    static void openHelpLink(HyperlinkEvent event) {
        if (event.getEventType() == HyperlinkEvent.EventType.ACTIVATED && event.getURL() != null) {
            openUrl(event.getURL().toString());
        }
    }

    static void openUrl(String url) {
        try {
            Desktop.getDesktop().browse(new URI(url));
        } catch (Exception ignored) {
            // Help remains useful even when the desktop cannot open links.
        }
    }

    static void closeHelpPopup() {
        SettingHelpCard.hideCard();
    }

    static String formatHelpHtml(String helpText) {
        String normalized = helpText == null ? "" : helpText.replace("\\n", "\n");
        Matcher matcher = URL_PATTERN.matcher(normalized);
        StringBuilder html = new StringBuilder("<html><div>");
        int previous = 0;
        while (matcher.find()) {
            appendEscapedHelpText(html, normalized.substring(previous, matcher.start()));
            String url = matcher.group();
            html.append("<a href='").append(escapeHtml(url)).append("'>")
                .append(escapeHtml(url)).append("</a>");
            previous = matcher.end();
        }
        appendEscapedHelpText(html, normalized.substring(previous));
        return html.append("</div></html>").toString();
    }

    private static void appendEscapedHelpText(StringBuilder html, String text) {
        html.append(escapeHtml(text).replace("\n", "<br>"));
    }

    private static String escapeHtml(String text) {
        return text.replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;")
            .replace("\"", "&quot;").replace("'", "&#39;");
    }

    /**
     * Creates a label-only row for a field that has no matching IniField definition.
     * Applies background color and link logic based on the field's UI name.
     */
    public static JPanel createLabelRow(DialogModel.Field field) {
        String uiName = field.getUiName();
        JLabel label = new JLabel(stripStylePrefix(uiName));
        applyStyle(label);
        label.setOpaque(true);
        label.setAlignmentX(Component.LEFT_ALIGNMENT);
        applyBackgroundColor(label, uiName);
        applyLinkLogic(label, uiName);

        JPanel row = createRowPanel();
        row.add(Box.createHorizontalStrut(10));
        row.add(label);
        fixRowHeight(row);
        return row;
    }

    /**
     * Creates a button row for a dialog command. {@code onExecute} is called on the EDT when
     * the button is clicked; it is responsible for dispatching the command to the ECU on the
     * IO thread. May be null, in which case the button is rendered but does nothing.
     */
    public static JPanel createCommandRow(DialogModel.Command command, Runnable onExecute) {
        JButton button = new JButton(command.getUiName());
        applyStyle(button);
        button.setAlignmentX(Component.LEFT_ALIGNMENT);
        if (onExecute != null) {
            button.addActionListener(e -> onExecute.run());
        } else {
            // disable button since we can get the action
            button.setEnabled(false);
        }

        JPanel row = createRowPanel();
        row.add(Box.createHorizontalStrut(10));
        row.add(button);
        fixRowHeight(row);
        return row;
    }

    private static JCheckBox createCheckBox(EnumIniField enumField, IniField iniField, String currentValue, ConfigurationImage workingImage, Runnable onChange) {
        JCheckBox checkBox = new JCheckBox();
        applyStyle(checkBox);
        checkBox.setSelected(isCheckboxOn(currentValue));
        checkBox.addItemListener(e -> {
            if (workingImage == null) return;
            List<String> values = new ArrayList<>(enumField.getEnums().values());
            String v1 = values.get(0).toLowerCase();
            boolean firstIsOn = v1.equals("yes") || v1.equals("enabled");
            String selected = checkBox.isSelected()
                ? (firstIsOn ? values.get(0) : values.get(1))
                : (firstIsOn ? values.get(1) : values.get(0));
            ConfigurationImageGetterSetter.setValue2(iniField, workingImage, iniField.getName(), selected);
            if (onChange != null) onChange.run();
        });
        return checkBox;
    }

    /** @param storedValue a check-box setting's value as {@code getStringValue} gives it, quoted */
    static boolean isCheckboxOn(String storedValue) {
        return storedValue.equalsIgnoreCase("\"Enabled\"") || storedValue.equalsIgnoreCase("\"Yes\"");
    }

    private static JComboBox<Object> createComboBox(EnumIniField enumField, IniField iniField, String currentValue,
                                                     ConfigurationImage workingImage, Runnable onChange,
                                                     int fieldEditorWidth, boolean valueTooltip) {
        boolean unknownCurrentValue = !EnumIniField.isQuoted(currentValue)
            && EnumIniField.isUnknownOrdinalValue(currentValue);
        String cleanValue = currentValue.replace("\"", "");
        List<Object> comboValues = new ArrayList<>(Arrays.asList(getComboValues(enumField)));
        UnknownPinChoice unknownChoice = unknownCurrentValue
            ? new UnknownPinChoice(EnumIniField.parseUnknownOrdinal(cleanValue))
            : null;
        if (unknownChoice != null) {
            comboValues.add(unknownChoice);
        }
        JComboBox<Object> comboBox = new JComboBox<>(comboValues.toArray());
        applyStyle(comboBox);
        comboBox.setSelectedItem(unknownChoice == null ? cleanValue : unknownChoice);
        UnknownPinChoice[] currentUnknownChoice = {unknownChoice};
        String[] lastKnownValue = {unknownChoice == null ? cleanValue : null};
        boolean[] restoringSelection = {false};
        comboBox.setRenderer((list, value, index, isSelected, cellHasFocus) -> {
            JLabel label = (JLabel) new DefaultListCellRenderer()
                .getListCellRendererComponent(list, value, index, isSelected, cellHasFocus);
            if (value instanceof UnknownPinChoice) {
                label.setText(value.toString());
            }
            return label;
        });
        if (valueTooltip) {
            comboBox.setToolTipText(unknownChoice == null ? cleanValue : unknownChoice.toString());
        }
        applyBackgroundColor(comboBox, unknownChoice == null ? cleanValue : unknownChoice.toString());
        Dimension size = comboBox.getPreferredSize();
        size.width = fieldEditorWidth > 0
            ? Math.min(fieldEditorWidth, MAX_FIELD_EDITOR_WIDTH)
            : Math.min(size.width, MAX_FIELD_EDITOR_WIDTH);
        comboBox.setMinimumSize(new Dimension(0, size.height));
        comboBox.setPreferredSize(size);
        comboBox.setMaximumSize(size);
        comboBox.addActionListener(e -> {
            if (restoringSelection[0]) return;
            if (workingImage == null) return;
            Object selectedItem = comboBox.getSelectedItem();
            if (selectedItem instanceof UnknownPinChoice) {
                if (selectedItem == currentUnknownChoice[0]) return;
                if (lastKnownValue[0] != null) {
                    restoringSelection[0] = true;
                    comboBox.setSelectedItem(lastKnownValue[0]);
                    restoringSelection[0] = false;
                }
                return;
            }
            if (selectedItem instanceof String) {
                String selected = (String) selectedItem;
                if (valueTooltip) {
                    comboBox.setToolTipText(selected);
                }
                ConfigurationImageGetterSetter.setValue2(iniField, workingImage, iniField.getName(),
                    enumField.isPinEnum() ? javax.management.ObjectName.quote(selected) : selected);
                lastKnownValue[0] = selected;
                if (currentUnknownChoice[0] != null) {
                    comboBox.removeItem(currentUnknownChoice[0]);
                    currentUnknownChoice[0] = null;
                }
                if (onChange != null) onChange.run();
            }
        });
        return comboBox;
    }

    private static final class UnknownPinChoice {
        private final int ordinal;

        private UnknownPinChoice(int ordinal) {
            this.ordinal = ordinal;
        }

        @Override
        public String toString() {
            return "Unknown pin (ID " + ordinal + ")";
        }
    }

    static int getFieldEditorPreferredWidth(IniField iniField, String currentValue) {
        JComponent editor;
        if (iniField instanceof EnumIniField) {
            EnumIniField enumField = (EnumIniField) iniField;
            if (isCheckboxEnum(enumField)) {
                return 0;
            }
            JComboBox<String> comboBox = new JComboBox<>(getComboValues(enumField));
            applyStyle(comboBox);
            editor = comboBox;
        } else {
            editor = createTextFieldComponent(iniField, currentValue);
        }
        return Math.min(editor.getPreferredSize().width, MAX_FIELD_EDITOR_WIDTH);
    }

    private static String[] getComboValues(EnumIniField enumField) {
        return enumField.getDisplayValues().stream()
            .filter(value -> !value.contains("INVALID"))
            .toArray(String[]::new);
    }

    static final String SHOW_ON_CONNECTOR = "Show on connector";
    /** Written beside the connector icon: an icon alone did not read as a connector. */
    static final String PINOUT_BUTTON_TEXT = "Pinout";

    /** Tooltip of the connector button: names the chosen pin, or says that none is chosen. */
    static String pinoutButtonToolTip(String pin) {
        return pin == null ? "Choose a pin to show it on its connector"
            : "Show " + pin + " on its connector (opens Pinout)";
    }

    /**
     * For pin selectors ({@link PinoutLookup#isPinSetting}, analog inputs included), adds a connector button
     * that fires the current combo value up to the caller for cross-tab navigation.
     */
    private static JButton createPinoutButton(JComboBox<?> comboBox, String fieldKey, EnumIniField field,
                                              PinoutLink onShowInPinout) {
        if (onShowInPinout == null || fieldKey == null) {
            return null;
        }
        if (!PinoutLookup.isPinSetting(fieldKey, field)) {
            return null;
        }

        JButton button = new JButton(PINOUT_BUTTON_TEXT, new ConnectorIcon()) {
            @Override
            public void setEnabled(boolean enabled) {
                String pin = getSelectedPin(comboBox);
                super.setEnabled(enabled && pin != null);
                setToolTipText(pinoutButtonToolTip(pin));
            }

            /** As tall as the drop-down beside it, so the row reads as one control. */
            @Override
            public Dimension getPreferredSize() {
                Dimension size = super.getPreferredSize();
                return new Dimension(size.width, Math.max(size.height, comboBox.getPreferredSize().height));
            }

            @Override
            public Dimension getMaximumSize() {
                return getPreferredSize();
            }
        };
        button.getAccessibleContext().setAccessibleName(SHOW_ON_CONNECTOR);
        button.setIconTextGap(6);
        button.setMargin(new Insets(0, 6, 0, 8));
        button.setEnabled(comboBox.isEnabled());
        comboBox.addActionListener(e -> button.setEnabled(comboBox.isEnabled()));
        button.addActionListener(e -> {
            String value = getSelectedPin(comboBox);
            if (value == null) {
                return;
            }
            onShowInPinout.show(value);
        });
        HoverPreview.install(button, () -> {
            String pin = getSelectedPin(comboBox);
            return pin == null ? null : onShowInPinout.preview(pin);
        });
        return button;
    }

    private static String getSelectedPin(JComboBox<?> comboBox) {
        if (comboBox.getSelectedItem() instanceof UnknownPinChoice) {
            return null;
        }
        return PinoutLookup.pinChoice(comboBox.getSelectedItem());
    }

    private static JTextField createTextField(IniField iniField, String currentValue,
                                              ConfigurationImage workingImage, Runnable onChange,
                                              int fieldEditorWidth) {
        CalibrationTextField textField = createTextFieldComponent(iniField, currentValue);
        if (fieldEditorWidth > 0) {
            textField.setFieldEditorWidth(Math.min(fieldEditorWidth, MAX_FIELD_EDITOR_WIDTH));
            Dimension size = textField.getPreferredSize();
            textField.setMinimumSize(new Dimension(0, size.height));
        }
        textField.setMaximumSize(textField.getPreferredSize());
        textField.getDocument().addDocumentListener(new DocumentListener() {
            private void sync() {
                if (workingImage == null) return;
                try {
                    ConfigurationImageGetterSetter.setValue2(iniField, workingImage, iniField.getName(), textField.getText());
                    if (onChange != null) onChange.run();
                } catch (Exception ignored) {
                    // invalid input while typing
                }
            }

            @Override
            public void insertUpdate(DocumentEvent e) {
                sync();
            }

            @Override
            public void removeUpdate(DocumentEvent e) {
                sync();
            }

            @Override
            public void changedUpdate(DocumentEvent e) {
                sync();
            }
        });
        return textField;
    }

    private static CalibrationTextField createTextFieldComponent(IniField iniField, String currentValue) {
        int columns = iniField instanceof StringIniField
            ? Math.min(((StringIniField) iniField).getSize(), 32) : 0;
        CalibrationTextField textField = new CalibrationTextField(currentValue, columns);
        if (iniField instanceof StringIniField) {
            ((AbstractDocument) textField.getDocument()).setDocumentFilter(
                new StringLengthFilter(((StringIniField) iniField).getSize()));
        }
        applyStyle(textField);
        applyBackgroundColor(textField, currentValue);
        return textField;
    }

    private static final class StringLengthFilter extends DocumentFilter {
        private final int maximumLength;

        private StringLengthFilter(int maximumLength) {
            this.maximumLength = maximumLength;
        }

        @Override
        public void insertString(FilterBypass bypass, int offset, String text, AttributeSet attributes)
                throws BadLocationException {
            replace(bypass, offset, 0, text, attributes);
        }

        @Override
        public void replace(FilterBypass bypass, int offset, int length, String text, AttributeSet attributes)
                throws BadLocationException {
            String inserted = text == null ? "" : text;
            int available = maximumLength - (bypass.getDocument().getLength() - length);
            if (available <= 0) {
                inserted = "";
            } else if (inserted.length() > available) {
                inserted = inserted.substring(0, available);
            }
            bypass.replace(offset, length, inserted, attributes);
        }
    }

    private static JPanel createRowPanel() {
        JPanel row = new JPanel();
        row.setLayout(new BoxLayout(row, BoxLayout.X_AXIS));
        row.setAlignmentX(Component.LEFT_ALIGNMENT);
        return row;
    }

    static String copiedFieldText(String label, String value) {
        return label + ": " + value;
    }

    private static void installCopyListener(JLabel label, String labelText, Supplier<String> value,
                                            boolean copyTooltip) {
        if (copyTooltip && label.getToolTipText() == null) {
            label.setToolTipText("Double-click to copy");
        }
        label.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent event) {
                if (event.getClickCount() != 2) {
                    return;
                }

                String copied = copiedFieldText(labelText, value.get());
                Toolkit.getDefaultToolkit().getSystemClipboard().setContents(new StringSelection(copied), null);
                showCopiedMessage(label, copied);
            }
        });
    }

    private static void showCopiedMessage(JLabel label, String copied) {
        if (!label.isShowing()) {
            return;
        }

        JLabel message = new JLabel("Copied: " + copied);
        message.setOpaque(true);
        message.setBorder(BorderFactory.createCompoundBorder(
            // [tag:fixed_color] a dark toast with white text reads the same in every theme
            BorderFactory.createLineBorder(Color.DARK_GRAY),
            BorderFactory.createEmptyBorder(4, 8, 4, 8)));
        message.setBackground(Color.DARK_GRAY); // [tag:fixed_color] the dark toast
        message.setForeground(Color.WHITE); // [tag:fixed_color] text on the dark toast
        Point location = label.getLocationOnScreen();
        Popup popup = PopupFactory.getSharedInstance().getPopup(label, message, location.x, location.y + label.getHeight());
        popup.show();
        Timer timer = new Timer(1500, event -> popup.hide());
        timer.setRepeats(false);
        timer.start();
    }

    /** Lets a row grow in width only; its height follows its current preferred height, which changes when it wraps. */
    static void fixRowHeight(JPanel row) {
        if (!(row.getLayout() instanceof RowHeightCap)) {
            row.setLayout(new RowHeightCap(row.getLayout()));
        }
    }

    private static final class RowHeightCap implements LayoutManager2 {
        private final LayoutManager delegate;

        RowHeightCap(LayoutManager delegate) {
            this.delegate = delegate;
        }

        @Override
        public Dimension maximumLayoutSize(Container target) {
            return new Dimension(Integer.MAX_VALUE, delegate.preferredLayoutSize(target).height + 5);
        }

        @Override
        public Dimension preferredLayoutSize(Container target) {
            return delegate.preferredLayoutSize(target);
        }

        @Override
        public Dimension minimumLayoutSize(Container target) {
            return delegate.minimumLayoutSize(target);
        }

        @Override
        public void layoutContainer(Container target) {
            delegate.layoutContainer(target);
        }

        @Override
        public void addLayoutComponent(Component comp, Object constraints) {
            if (delegate instanceof LayoutManager2) {
                ((LayoutManager2) delegate).addLayoutComponent(comp, constraints);
            }
        }

        @Override
        public void addLayoutComponent(String name, Component comp) {
            delegate.addLayoutComponent(name, comp);
        }

        @Override
        public void removeLayoutComponent(Component comp) {
            delegate.removeLayoutComponent(comp);
        }

        @Override
        public float getLayoutAlignmentX(Container target) {
            return delegate instanceof LayoutManager2 ? ((LayoutManager2) delegate).getLayoutAlignmentX(target) : 0.5f;
        }

        @Override
        public float getLayoutAlignmentY(Container target) {
            return delegate instanceof LayoutManager2 ? ((LayoutManager2) delegate).getLayoutAlignmentY(target) : 0.5f;
        }

        @Override
        public void invalidateLayout(Container target) {
            if (delegate instanceof LayoutManager2) {
                ((LayoutManager2) delegate).invalidateLayout(target);
            }
        }
    }

    /** Background of the setting help popup [tag:console_theme]. */
    static Color helpBackground() {
        return TuningStyle.color("Console.Tuning.help.background", Color.WHITE);
    }

    public static void applyStyle(JComponent component) {
        Font font = component.getFont();
        if (font != null) {
            component.setFont(font.deriveFont(font.getSize() * 1.2f));
        }
    }

    /**
     * A leading '!' (red) or '#' (blue) on a TS ini label is styling markup consumed by
     * {@link #applyBackgroundColor}, not visible text.
     */
    static String stripStylePrefix(String uiName) {
        if (uiName != null && (uiName.startsWith(RED_PREFIX) || uiName.startsWith(BLUE_PREFIX))) {
            return uiName.substring(1).trim();
        }
        return uiName;
    }

    static void applyBackgroundColor(JComponent component, String value) {
        if (value.startsWith(BLUE_PREFIX)) {
            component.setBackground(Color.BLUE); // [tag:fixed_color] INI '#' label markup asks for blue
            component.setForeground(Color.WHITE); // [tag:fixed_color] text on the INI colour
        } else if (value.startsWith(RED_PREFIX)) {
            component.setBackground(Color.RED); // [tag:fixed_color] INI '!' label markup asks for red
            component.setForeground(Color.WHITE); // [tag:fixed_color] text on the INI colour
        }
    }

    static void applyLinkLogic(JLabel label, String text) {
        if (text == null) {
            return;
        }
        Pattern pattern = Pattern.compile("href=([^> ]+)>([^<]+)</a>");
        Matcher matcher = pattern.matcher(text);
        if (matcher.find()) {
            String url = matcher.group(1);
            String visibleText = matcher.group(2);
            label.setText(visibleText);
            label.setCursor(Cursor.getPredefinedCursor(Cursor.HAND_CURSOR));
            label.addMouseListener(new MouseAdapter() {
                @Override
                public void mouseClicked(MouseEvent e) {
                    try {
                        Desktop.getDesktop().browse(new URI(url));
                    } catch (Exception ex) {
                        System.err.println("Failed to open URL: " + url);
                    }
                }
            });
        }
    }

    public static boolean isCheckboxEnum(EnumIniField enumField) {
        if (enumField.getEnums().size() == 2) {
            List<String> values = new ArrayList<>(enumField.getEnums().values());
            String v1 = values.get(0).toLowerCase();
            String v2 = values.get(1).toLowerCase();

            for (String[] pair : CheckboxPairs) {
                if ((v1.equals(pair[0]) && v2.equals(pair[1])) || (v1.equals(pair[1]) && v2.equals(pair[0]))) {
                    return true;
                }
            }
        }
        return false;
    }
}
