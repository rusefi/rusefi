package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.DialogModel;

import javax.swing.*;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.net.URI;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Factory for creating individual UI widgets used in calibration dialogs.
 * Handles field rows (label and input), label-only rows, and command button rows.
 *
 * @see CalibrationDialogWidget
 */
public class CalibrationFieldFactory {

    // we need to maintain this in sync with the ones used on tunerstudio.template
    private static final String[][] CheckboxPairs = {
        {"yes", "no"},
        {"enabled", "disabled"}
    };

    public static JPanel createFieldRow(DialogModel.Field field, IniField iniField, ConfigurationImage ci, ConfigurationImage workingImage) {
        return createFieldRow(field, iniField, ci, workingImage, null);
    }

    public static JPanel createFieldRow(DialogModel.Field field, IniField iniField, ConfigurationImage ci, ConfigurationImage workingImage, Runnable onChange) {
        JPanel row = createRowPanel();
        row.add(Box.createHorizontalStrut(10));

        JLabel label = new JLabel(field.getUiName());
        applyStyle(label);
        row.add(label);
        row.add(Box.createHorizontalStrut(16));

        if (iniField instanceof EnumIniField) {
            EnumIniField enumField = (EnumIniField) iniField;
            String currentValue = ci == null ? "" : ConfigurationImageGetterSetter.getStringValue(iniField, ci);

            if (isCheckboxEnum(enumField)) {
                row.add(createCheckBox(enumField, iniField, currentValue, workingImage, onChange));
            } else {
                row.add(createComboBox(enumField, iniField, currentValue, workingImage, onChange));
            }
        } else {
            String currentValue = ci == null ? "" : ConfigurationImageGetterSetter.getStringValue(iniField, ci);
            row.add(createTextField(iniField, currentValue, workingImage, onChange));
        }

        fixRowHeight(row);
        return row;
    }

    /**
     * Creates a label-only row for a field that has no matching IniField definition.
     * Applies background color and link logic based on the field's UI name.
     */
    public static JPanel createLabelRow(DialogModel.Field field) {
        JLabel label = new JLabel(field.getUiName());
        applyStyle(label);
        label.setOpaque(true);
        label.setAlignmentX(Component.LEFT_ALIGNMENT);
        applyBackgroundColor(label, field.getUiName());
        applyLinkLogic(label, field.getUiName());

        JPanel row = createRowPanel();
        row.add(Box.createHorizontalStrut(10));
        row.add(label);
        fixRowHeight(row);
        return row;
    }

    /**
     * Creates a button row for a dialog command.
     */
    public static JPanel createCommandRow(DialogModel.Command command) {
        JButton button = new JButton(command.getUiName());
        applyStyle(button);
        button.setAlignmentX(Component.LEFT_ALIGNMENT);
        button.addActionListener(e -> {
            // TODO: implement command execution
            System.out.println("Executing command: " + command.getCommand());
        });

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

    private static JComboBox<String> createComboBox(EnumIniField enumField, IniField iniField, String currentValue, ConfigurationImage workingImage, Runnable onChange) {
        String cleanValue = currentValue.replace("\"", "");
        String[] comboValues = enumField.getEnums().values().stream().filter(v -> !v.contains("INVALID")).toArray(String[]::new);
        JComboBox<String> comboBox = new JComboBox<>(comboValues);
        applyStyle(comboBox);
        comboBox.setSelectedItem(cleanValue);
        applyBackgroundColor(comboBox, cleanValue);
        comboBox.setMaximumSize(comboBox.getPreferredSize());
        comboBox.addActionListener(e -> {
            if (workingImage == null) return;
            String selected = (String) comboBox.getSelectedItem();
            if (selected != null) {
                ConfigurationImageGetterSetter.setValue2(iniField, workingImage, iniField.getName(), selected);
                if (onChange != null) onChange.run();
            }
        });
        return comboBox;
    }

    private static JTextField createTextField(IniField iniField, String currentValue, ConfigurationImage workingImage, Runnable onChange) {
        JTextField textField = new JTextField(currentValue);
        applyStyle(textField);
        applyBackgroundColor(textField, currentValue);
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

    private static JPanel createRowPanel() {
        JPanel row = new JPanel();
        row.setLayout(new BoxLayout(row, BoxLayout.X_AXIS));
        row.setAlignmentX(Component.LEFT_ALIGNMENT);
        return row;
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

    static void applyBackgroundColor(JComponent component, String value) {
        if (value.startsWith("#")) {
            component.setBackground(Color.BLUE);
            component.setForeground(Color.WHITE);
        } else if (value.startsWith("!")) {
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
