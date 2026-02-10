package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.CurveModel;
import com.opensr5.ini.DialogModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.PanelModel;
import com.opensr5.ini.TableModel;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.laf.GradientTitleBorder;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.net.URI;
import java.util.List;
import java.util.Optional;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * @see TuningTableView
 */
public class CalibrationDialogWidget {
    private final JPanel contentPane = new JPanel();
    private final UIContext uiContext;

    public CalibrationDialogWidget(UIContext uiContext) {
        this.uiContext = uiContext;
        contentPane.setLayout(new BoxLayout(contentPane, BoxLayout.Y_AXIS));
        contentPane.setAlignmentX(Component.LEFT_ALIGNMENT);
    }

    public void update(DialogModel dialogModel, IniFileModel iniFileModel, ConfigurationImage ci) {
        contentPane.removeAll();
        if (dialogModel != null) {
            String layoutHint = dialogModel.getLayoutHint();
            if ("border".equalsIgnoreCase(layoutHint)) {
                contentPane.setLayout(new BorderLayout());
            } else if ("xAxis".equalsIgnoreCase(layoutHint)) {
                contentPane.setLayout(new BoxLayout(contentPane, BoxLayout.X_AXIS));
            } else {
                contentPane.setLayout(new BoxLayout(contentPane, BoxLayout.Y_AXIS));
            }
            contentPane.setAlignmentX(Component.LEFT_ALIGNMENT);
            fillPanel(contentPane, dialogModel, iniFileModel, ci);
        }
        contentPane.revalidate();
        contentPane.repaint();
    }

    public void update(String key) {
        IniFileModel iniFileModel = uiContext.iniFileState.getIniFileModel();
        ConfigurationImage ci = uiContext.getBinaryProtocol().getControllerConfiguration();
        update(key, iniFileModel, ci);
    }

    public void update(String key, IniFileModel iniFileModel, ConfigurationImage ci) {
        contentPane.removeAll();
        if (key != null) {
            DialogModel dialog = iniFileModel.getDialogs().get(key);
            if (dialog != null) {
                update(dialog, iniFileModel, ci);
                return;
            }

            TableModel table = iniFileModel.getTable(key);
            if (table != null) {
                contentPane.setLayout(new BoxLayout(contentPane, BoxLayout.Y_AXIS));
                TuningTableView tuningTableView = new TuningTableView(table.getTitle());
                tuningTableView.displayTable(iniFileModel, table.getTableId(), ci);
                contentPane.add(tuningTableView.getContent());
            } else {
                CurveModel curve = iniFileModel.getCurves().get(key);
                if (curve != null) {
                    contentPane.setLayout(new BoxLayout(contentPane, BoxLayout.Y_AXIS));
                    CurveWidget curveWidget = new CurveWidget(curve, iniFileModel, ci);
                    contentPane.add(curveWidget.getContentPane());
                }
            }
        }
        contentPane.revalidate();
        contentPane.repaint();
    }

    private void fillPanel(JPanel container, DialogModel dialogModel, IniFileModel iniFileModel, ConfigurationImage ci) {
        for (DialogModel.Field field : dialogModel.getFields()) {
            Optional<IniField> iniField = iniFileModel.findIniField(field.getKey());
            if (iniField.isPresent()) {
                IniField f = iniField.get();
                JPanel row = new JPanel();
                row.setLayout(new BoxLayout(row, BoxLayout.X_AXIS));
                row.setAlignmentX(Component.LEFT_ALIGNMENT);
                row.add(Box.createHorizontalStrut(10));
                JLabel label = new JLabel(field.getUiName());
                applyStyle(label);
                row.add(label);

                if (f instanceof EnumIniField) {
                    EnumIniField enumField = (EnumIniField) f;

                    boolean isCheckBox = isCheckboxEnum(enumField);

                    String currentValue = ci == null ? "" : ConfigurationImageGetterSetter.getStringValue(f, ci);

                    if (isCheckBox) {
                        JCheckBox checkBox = new JCheckBox();
                        applyStyle(checkBox);
                        checkBox.setSelected(currentValue.equalsIgnoreCase("\"Enabled\"") || currentValue.equalsIgnoreCase("\"Yes\""));
                        row.add(checkBox);
                    } else {
                        String cleanValue = currentValue.replace("\"", "");
                        JComboBox<String> comboBox = new JComboBox<>(enumField.getEnums().values().toArray(new String[0]));
                        applyStyle(comboBox);
                        comboBox.setSelectedItem(cleanValue);
                        applyBackgroundColor(comboBox, cleanValue);
                        comboBox.setMaximumSize(comboBox.getPreferredSize());
                        row.add(comboBox);
                    }
                } else {
                    String currentValue = ci == null ? "" : ConfigurationImageGetterSetter.getStringValue(f, ci);
                    JTextField textField = new JTextField(currentValue);
                    applyStyle(textField);
                    applyBackgroundColor(textField, currentValue);
                    textField.setMaximumSize(textField.getPreferredSize());
                    row.add(textField);
                }
                container.add(row);
            } else {
                JLabel label = new JLabel(field.getUiName());
                applyStyle(label);
                label.setOpaque(true);
                label.setAlignmentX(Component.LEFT_ALIGNMENT);
                applyBackgroundColor(label, field.getUiName());
                applyLinkLogic(label, field.getUiName());

                JPanel row = new JPanel();
                row.setLayout(new BoxLayout(row, BoxLayout.X_AXIS));
                row.setAlignmentX(Component.LEFT_ALIGNMENT);
                row.add(Box.createHorizontalStrut(10));
                row.add(label);
                container.add(row);
            }
        }

        for (DialogModel.Command command : dialogModel.getCommandsOfCurrentDialog()) {
            JButton button = new JButton(command.getUiName());
            applyStyle(button);
            button.setAlignmentX(Component.LEFT_ALIGNMENT);
            button.addActionListener(e -> {
                // TODO: implement command execution
                System.out.println("Executing command: " + command.getCommand());
            });
            JPanel row = new JPanel();
            row.setLayout(new BoxLayout(row, BoxLayout.X_AXIS));
            row.setAlignmentX(Component.LEFT_ALIGNMENT);
            row.add(Box.createHorizontalStrut(10));
            row.add(button);
            container.add(row);
        }

        boolean isBorderLayout = container.getLayout() instanceof BorderLayout;
        List<PanelModel> panels = dialogModel.getPanels();
        JPanel horizontalPanel = null;
        for (PanelModel panel : panels) {
            String placement = panel.getPlacement();
            boolean isHorizontal = "west".equalsIgnoreCase(placement) || "center".equalsIgnoreCase(placement) || "east".equalsIgnoreCase(placement);

            JPanel targetContainer;
            if (isBorderLayout) {
                targetContainer = container;
            } else if (isHorizontal) {
                if (horizontalPanel == null) {
                    horizontalPanel = new JPanel();
                    horizontalPanel.setLayout(new BoxLayout(horizontalPanel, BoxLayout.X_AXIS));
                    horizontalPanel.setAlignmentX(Component.LEFT_ALIGNMENT);
                    container.add(horizontalPanel);
                }
                targetContainer = horizontalPanel;
            } else {
                horizontalPanel = null;
                targetContainer = container;
            }

            CurveModel curve = iniFileModel.getCurves().get(panel.getPanelName());
            if (curve != null) {
                CurveWidget curveWidget = new CurveWidget(curve, iniFileModel, ci);
                JComponent content = curveWidget.getContentPane();
                applyStyle(content);
                content.setAlignmentX(Component.LEFT_ALIGNMENT);
                addToContainer(targetContainer, content, placement, isBorderLayout);
                continue;
            }

            TableModel table = iniFileModel.getTable(panel.getPanelName());
            if (table != null) {
                TuningTableView tuningTableView = new TuningTableView(table.getTitle());
                tuningTableView.displayTable(iniFileModel, table.getTableId(), ci);
                JComponent content = tuningTableView.getContent();
                applyStyle(content);
                content.setAlignmentX(Component.LEFT_ALIGNMENT);
                addToContainer(targetContainer, content, placement, isBorderLayout);
                continue;
            }

            JPanel panelWidget = new JPanel();
            panelWidget.setAlignmentX(Component.LEFT_ALIGNMENT);
            DialogModel subDialog = panel.resolveDialog(iniFileModel);
            String subLayoutHint = subDialog != null ? subDialog.getLayoutHint() : null;
            if ("border".equalsIgnoreCase(subLayoutHint)) {
                panelWidget.setLayout(new BorderLayout());
            } else if ("xAxis".equalsIgnoreCase(subLayoutHint)) {
                panelWidget.setLayout(new BoxLayout(panelWidget, BoxLayout.X_AXIS));
            } else {
                panelWidget.setLayout(new BoxLayout(panelWidget, BoxLayout.Y_AXIS));
            }

            if (subDialog != null) {
                String uiName = subDialog.getUiName();
                if (uiName == null || uiName.isEmpty()) {
                    uiName = subDialog.getKey();
                }
                panelWidget.setName(uiName);
                GradientTitleBorder.installBorder(uiName, panelWidget);
                fillPanel(panelWidget, subDialog, iniFileModel, ci);
            } else {
                panelWidget.setName(panel.getPanelName());
                GradientTitleBorder.installBorder(panel.getPanelName(), panelWidget);
            }
            addToContainer(targetContainer, panelWidget, placement, isBorderLayout);
        }
    }

    private static void addToContainer(JPanel container, JComponent component, String placement, boolean isBorderLayout) {
        if (isBorderLayout && placement != null) {
            container.add(component, toBorderLayoutConstraint(placement));
        } else {
            container.add(component);
        }
    }

    private static String toBorderLayoutConstraint(String placement) {
        if ("west".equalsIgnoreCase(placement)) return BorderLayout.WEST;
        if ("east".equalsIgnoreCase(placement)) return BorderLayout.EAST;
        if ("north".equalsIgnoreCase(placement)) return BorderLayout.NORTH;
        if ("south".equalsIgnoreCase(placement)) return BorderLayout.SOUTH;
        return BorderLayout.CENTER;
    }

    private static void applyStyle(JComponent component) {
        Font font = component.getFont();
        if (font != null) {
            component.setFont(font.deriveFont(font.getSize() * 2.0f));
        }
    }

    private static void applyBackgroundColor(JComponent component, String value) {
        if (value.startsWith("#")) {
            component.setBackground(java.awt.Color.BLUE);
            component.setForeground(java.awt.Color.WHITE);
        } else if (value.startsWith("!")) {
            component.setBackground(java.awt.Color.RED);
            component.setForeground(java.awt.Color.WHITE);
        }
    }

    private static void applyLinkLogic(JLabel label, String text) {
        if (text == null) {
            return;
        }
        // Basic pattern to match <a href=URL>text</a>
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
        boolean isCheckBox = false;
        if (enumField.getEnums().size() == 2) {
            List<String> values = new java.util.ArrayList<>(enumField.getEnums().values());
            String v1 = values.get(0).toLowerCase();
            String v2 = values.get(1).toLowerCase();

            if ((v1.equals("yes") && v2.equals("no")) || (v1.equals("no") && v2.equals("yes")) ||
                    (v1.equals("enabled") && v2.equals("disabled")) || (v1.equals("disabled") && v2.equals("enabled"))) {
                isCheckBox = true;
            }
        }
        return isCheckBox;
    }

    public JPanel getContentPane() {
        return contentPane;
    }
}
