package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.StringIniField;
import com.opensr5.ini.DialogModel;
import com.rusefi.core.ui.AutoupdateUtil;

import javax.swing.*;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.event.HyperlinkEvent;
import javax.swing.event.PopupMenuEvent;
import javax.swing.event.PopupMenuListener;
import java.awt.*;
import java.awt.datatransfer.StringSelection;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.net.URI;
import java.util.ArrayList;
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
    private static final int HELP_ICON_SIZE = 18;
    private static final int HELP_COLUMN_WIDTH = 24;
    private static final int HELP_MAX_TEXT_WIDTH = 500;
    private static final int HELP_PADDING = 8;
    private static final int HELP_MAX_HEIGHT = 300;
    private static final int HELP_LAYOUT_ALLOWANCE = 8;
    private static final Color HELP_BACKGROUND = new Color(255, 255, 190);
    private static final Icon SETTING_HELP_ICON = loadHelpIcon();
    private static JPopupMenu openHelpPopup;
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

    public static JPanel createFieldRow(DialogModel.Field field, IniField iniField, ConfigurationImage ci, ConfigurationImage workingImage, Runnable onChange, Consumer<String> onShowInPinout) {
        return createFieldRow(field, iniField, ci, workingImage, onChange, onShowInPinout, 0);
    }

    static JPanel createFieldRow(DialogModel.Field field, IniField iniField, ConfigurationImage ci,
                                 ConfigurationImage workingImage, Runnable onChange,
                                 Consumer<String> onShowInPinout, int labelWidth) {
        return createFieldRow(field, iniField, ci, workingImage, onChange, onShowInPinout, labelWidth, 0);
    }

    static JPanel createFieldRow(DialogModel.Field field, IniField iniField, ConfigurationImage ci,
                                 ConfigurationImage workingImage, Runnable onChange,
                                 Consumer<String> onShowInPinout, int labelWidth, int fieldEditorWidth) {
        return createFieldRow(field, iniField, ci, workingImage, onChange, onShowInPinout,
            labelWidth, fieldEditorWidth, null);
    }

    static JPanel createFieldRow(DialogModel.Field field, IniField iniField, ConfigurationImage ci,
                                 ConfigurationImage workingImage, Runnable onChange,
                                 Consumer<String> onShowInPinout, int labelWidth, int fieldEditorWidth,
                                 String helpText) {
        JPanel row = createRowPanel();
        row.add(Box.createHorizontalStrut(10));
        row.add(createHelpSlot(helpText));

        String labelText = field.getUiName();
        JLabel label = new JLabel(labelText);
        applyStyle(label);
        if (labelWidth > 0) {
            if (labelText != null && label.getPreferredSize().width > labelWidth && !labelText.startsWith("<html>")) {
                label.setText("<html><div style='width: " + (labelWidth - 4) + "px'>" +
                    escapeHtml(labelText) + "</div></html>");
                label.setToolTipText(labelText);
            }
            Dimension size = label.getPreferredSize();
            size.width = labelWidth;
            size.height = Math.min(size.height, label.getFontMetrics(label.getFont()).getHeight() * 2);
            label.setMinimumSize(size);
            label.setPreferredSize(size);
            label.setMaximumSize(size);
        }
        row.add(label);
        row.add(Box.createHorizontalStrut(16));

        Supplier<String> value;
        if (iniField instanceof EnumIniField) {
            EnumIniField enumField = (EnumIniField) iniField;
            String currentValue = ci == null ? "" : ConfigurationImageGetterSetter.getStringValue(iniField, ci);

            if (isCheckboxEnum(enumField)) {
                JCheckBox checkBox = createCheckBox(enumField, iniField, currentValue, workingImage, onChange);
                row.add(checkBox);
                value = () -> checkBox.isSelected() ? "enabled" : "disabled";
            } else {
                JComboBox<String> comboBox = createComboBox(
                    enumField, iniField, currentValue, workingImage, onChange, fieldEditorWidth);
                row.add(comboBox);
                value = () -> String.valueOf(comboBox.getSelectedItem());
                JButton pinoutButton = createPinoutButton(comboBox, field.getKey(), onShowInPinout);
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
        }
        installCopyListener(label, labelText, value);

        fixRowHeight(row);
        return row;
    }

    private static JComponent createHelpSlot(String helpText) {
        JPanel slot = new JPanel(new FlowLayout(FlowLayout.LEFT, 0, 0));
        slot.setName("settingHelpSlot");
        Dimension slotSize = new Dimension(HELP_COLUMN_WIDTH, HELP_ICON_SIZE);
        slot.setMinimumSize(slotSize);
        slot.setPreferredSize(slotSize);
        slot.setMaximumSize(slotSize);

        if (helpText == null || helpText.trim().isEmpty()) {
            return slot;
        }

        JButton button = new JButton(SETTING_HELP_ICON);
        button.setName("settingHelpButton");
        button.setToolTipText(formatHelpHtml(helpText));
        button.setBorderPainted(false);
        button.setContentAreaFilled(false);
        button.setFocusPainted(false);
        button.setMargin(new Insets(0, 0, 0, 0));
        button.setPreferredSize(new Dimension(HELP_ICON_SIZE, HELP_ICON_SIZE));
        button.setMaximumSize(button.getPreferredSize());
        button.getAccessibleContext().setAccessibleName("Setting help");
        button.addActionListener(event -> showHelpPopup(button, helpText));
        slot.add(button);
        return slot;
    }

    private static Icon loadHelpIcon() {
        ImageIcon source = AutoupdateUtil.loadIcon("icons/tuning/help48.png");
        return source == null ? null : new ImageIcon(
            source.getImage().getScaledInstance(HELP_ICON_SIZE, HELP_ICON_SIZE, Image.SCALE_SMOOTH));
    }

    private static void showHelpPopup(JButton owner, String helpText) {
        closeHelpPopup();
        if (!owner.isShowing()) {
            return;
        }

        JScrollPane scrollPane = createHelpScrollPane(helpText);

        JPanel content = new JPanel(new BorderLayout(4, 4));
        content.setBackground(HELP_BACKGROUND);
        content.setBorder(BorderFactory.createCompoundBorder(
            BorderFactory.createLineBorder(new Color(60, 90, 160), 2),
            BorderFactory.createEmptyBorder(6, 8, 8, 8)));
        JButton close = new JButton("×");
        close.setToolTipText("Close help");
        close.setMargin(new Insets(0, 5, 0, 5));
        JPanel closeRow = new JPanel(new FlowLayout(FlowLayout.RIGHT, 0, 0));
        closeRow.setOpaque(false);
        closeRow.add(close);
        content.add(closeRow, BorderLayout.NORTH);
        content.add(scrollPane, BorderLayout.CENTER);

        JPopupMenu popup = new JPopupMenu();
        popup.setBorder(BorderFactory.createEmptyBorder());
        popup.add(content);
        popup.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent event) {
            }

            @Override
            public void popupMenuWillBecomeInvisible(PopupMenuEvent event) {
                if (openHelpPopup == popup) {
                    openHelpPopup = null;
                }
            }

            @Override
            public void popupMenuCanceled(PopupMenuEvent event) {
                if (openHelpPopup == popup) {
                    openHelpPopup = null;
                }
            }
        });
        close.addActionListener(event -> popup.setVisible(false));
        openHelpPopup = popup;
        popup.show(owner, 0, owner.getHeight());
    }

    static JScrollPane createHelpScrollPane(String helpText) {
        JEditorPane helpPane = new JEditorPane("text/html", formatHelpHtml(helpText));
        helpPane.setEditable(false);
        helpPane.setOpaque(true);
        helpPane.setBackground(HELP_BACKGROUND);
        helpPane.putClientProperty(JEditorPane.HONOR_DISPLAY_PROPERTIES, Boolean.TRUE);
        helpPane.addHyperlinkListener(event -> {
            if (event.getEventType() == HyperlinkEvent.EventType.ACTIVATED && event.getURL() != null) {
                try {
                    Desktop.getDesktop().browse(event.getURL().toURI());
                } catch (Exception ignored) {
                    // Help remains useful even when the desktop cannot open links.
                }
            }
        });
        helpPane.setBorder(BorderFactory.createEmptyBorder(
            HELP_PADDING, HELP_PADDING, HELP_PADDING, HELP_PADDING));
        helpPane.setCaretPosition(0);
        Dimension naturalSize = helpPane.getPreferredSize();
        int maxPaddedWidth = HELP_MAX_TEXT_WIDTH + HELP_PADDING * 2;
        int viewportWidth = Math.min(naturalSize.width, maxPaddedWidth);
        // Re-measure after constraining the width: the HTML renderer may wrap a long line into
        // several rows, making its laid-out height larger than its unconstrained natural height.
        helpPane.setSize(new Dimension(viewportWidth, Short.MAX_VALUE));
        Dimension laidOutSize = helpPane.getPreferredSize();
        int viewportHeight = Math.min(laidOutSize.height, HELP_MAX_HEIGHT);
        boolean needsHorizontalScroll = naturalSize.width > viewportWidth;
        boolean needsVerticalScroll = laidOutSize.height > viewportHeight;

        JScrollPane scrollPane = new JScrollPane(helpPane);
        scrollPane.setBorder(BorderFactory.createEmptyBorder());
        scrollPane.setHorizontalScrollBarPolicy(needsHorizontalScroll
            ? ScrollPaneConstants.HORIZONTAL_SCROLLBAR_AS_NEEDED
            : ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);
        scrollPane.setVerticalScrollBarPolicy(needsVerticalScroll
            ? ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED
            : ScrollPaneConstants.VERTICAL_SCROLLBAR_NEVER);
        scrollPane.getViewport().setBackground(HELP_BACKGROUND);
        int preferredWidth = viewportWidth + (needsVerticalScroll
            ? scrollPane.getVerticalScrollBar().getPreferredSize().width : 0);
        int preferredHeight = viewportHeight + (needsHorizontalScroll
            ? scrollPane.getHorizontalScrollBar().getPreferredSize().height : 0)
            + HELP_LAYOUT_ALLOWANCE;
        scrollPane.setPreferredSize(new Dimension(preferredWidth, preferredHeight));
        return scrollPane;
    }

    static void closeHelpPopup() {
        if (openHelpPopup != null) {
            JPopupMenu popup = openHelpPopup;
            openHelpPopup = null;
            popup.setVisible(false);
        }
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
        checkBox.setSelected(currentValue.equalsIgnoreCase("\"Enabled\"") || currentValue.equalsIgnoreCase("\"Yes\""));
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

    private static JComboBox<String> createComboBox(EnumIniField enumField, IniField iniField, String currentValue,
                                                     ConfigurationImage workingImage, Runnable onChange,
                                                     int fieldEditorWidth) {
        String cleanValue = currentValue.replace("\"", "");
        String[] comboValues = getComboValues(enumField);
        JComboBox<String> comboBox = new JComboBox<>(comboValues);
        applyStyle(comboBox);
        comboBox.setSelectedItem(cleanValue);
        comboBox.setToolTipText(cleanValue);
        applyBackgroundColor(comboBox, cleanValue);
        Dimension size = comboBox.getPreferredSize();
        size.width = fieldEditorWidth > 0
            ? Math.min(fieldEditorWidth, MAX_FIELD_EDITOR_WIDTH)
            : Math.min(size.width, MAX_FIELD_EDITOR_WIDTH);
        comboBox.setMinimumSize(new Dimension(0, size.height));
        comboBox.setPreferredSize(size);
        comboBox.setMaximumSize(size);
        comboBox.addActionListener(e -> {
            if (workingImage == null) return;
            String selected = (String) comboBox.getSelectedItem();
            if (selected != null) {
                comboBox.setToolTipText(selected);
                ConfigurationImageGetterSetter.setValue2(iniField, workingImage, iniField.getName(), selected);
                if (onChange != null) onChange.run();
            }
        });
        return comboBox;
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
        return enumField.getEnums().values().stream()
            .filter(value -> !value.contains("INVALID"))
            .toArray(String[]::new);
    }

    /**
     * For pin-enum fields (name matches .*pins?\d*), adds a button that fires the
     * current combo value up to the caller for cross-tab navigation.
     */
    private static JButton createPinoutButton(JComboBox<String> comboBox, String fieldKey, Consumer<String> onShowInPinout) {
        if (onShowInPinout == null || fieldKey == null) {
            return null;
        }
        if (!fieldKey.toLowerCase().matches(".*pins?\\d*")) {
            return null;
        }

        JButton button = new JButton("W") {
            @Override
            public void setEnabled(boolean enabled) {
                super.setEnabled(enabled && getSelectedPin(comboBox) != null);
            }
        };
        button.setToolTipText("Wiring/Pinout");
        button.setMargin(new Insets(0, 6, 0, 6));
        button.setMaximumSize(button.getPreferredSize());
        button.setEnabled(comboBox.isEnabled());
        comboBox.addActionListener(e -> button.setEnabled(comboBox.isEnabled()));
        button.addActionListener(e -> {
            String value = getSelectedPin(comboBox);
            if (value == null) {
                return;
            }
            onShowInPinout.accept(value);
        });
        return button;
    }

    private static String getSelectedPin(JComboBox<String> comboBox) {
        Object selected = comboBox.getSelectedItem();
        if (selected == null) {
            return null;
        }
        String value = selected.toString().replace("\"", "").trim();
        return value.isEmpty() || "NONE".equalsIgnoreCase(value) || "INVALID".equalsIgnoreCase(value)
            ? null : value;
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
        applyStyle(textField);
        applyBackgroundColor(textField, currentValue);
        return textField;
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

    private static void installCopyListener(JLabel label, String labelText, Supplier<String> value) {
        if (label.getToolTipText() == null) {
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
            BorderFactory.createLineBorder(Color.DARK_GRAY),
            BorderFactory.createEmptyBorder(4, 8, 4, 8)));
        message.setBackground(Color.DARK_GRAY);
        message.setForeground(Color.WHITE);
        Point location = label.getLocationOnScreen();
        Popup popup = PopupFactory.getSharedInstance().getPopup(label, message, location.x, location.y + label.getHeight());
        popup.show();
        Timer timer = new Timer(1500, event -> popup.hide());
        timer.setRepeats(false);
        timer.start();
    }

    static void fixRowHeight(JPanel row) {
        row.setMaximumSize(new Dimension(Integer.MAX_VALUE, row.getPreferredSize().height + 5));
    }

    static void applyStyle(JComponent component) {
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
            component.setBackground(Color.BLUE);
            component.setForeground(Color.WHITE);
        } else if (value.startsWith(RED_PREFIX)) {
            component.setBackground(Color.RED);
            component.setForeground(Color.WHITE);
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
