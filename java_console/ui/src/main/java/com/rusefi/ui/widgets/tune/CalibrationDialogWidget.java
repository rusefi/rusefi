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
import com.rusefi.ui.util.ScrollablePanel;
import com.rusefi.ui.util.SwingUtil;
import com.rusefi.ui.util.WrapLayout;

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
    private final JPanel contentPane = new ScrollablePanel();
    private final UIContext uiContext;

    public CalibrationDialogWidget(UIContext uiContext) {
        this.uiContext = uiContext;
        contentPane.setLayout(new BoxLayout(contentPane, BoxLayout.Y_AXIS));
        contentPane.setAlignmentX(Component.LEFT_ALIGNMENT);
    }

    public void update(DialogModel dialogModel, IniFileModel iniFileModel, ConfigurationImage ci) {
        contentPane.removeAll();
        if (dialogModel != null) {
            applyLayout(contentPane, dialogModel.getLayoutHint());
            contentPane.setAlignmentX(Component.LEFT_ALIGNMENT);
            fillPanel(contentPane, dialogModel, iniFileModel, ci);
        }
        contentPane.revalidate();
        contentPane.repaint();
        // After the initial layout gives children their actual widths,
        // WrapLayout can compute correct wrapped heights on the second pass.
        // invalidateTree is needed because revalidate() only invalidates the
        // component and its ancestors, not children — so children return
        // stale cached preferred sizes computed before wrapping.
        SwingUtilities.invokeLater(() -> {
            SwingUtil.invalidateTree(contentPane);
            contentPane.revalidate();
            contentPane.repaint();
        });
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
                row.add(Box.createHorizontalStrut(16));

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
                fixRowHeight(row);
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
                fixRowHeight(row);
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
            fixRowHeight(row);
            container.add(row);
        }

        boolean isGridLayout = container.getLayout() instanceof GridLayout;
        List<PanelModel> panels = dialogModel.getPanels();
        JPanel horizontalPanel = null;
        for (PanelModel panel : panels) {
            String placement = panel.getPlacement();
            boolean isHorizontal = "west".equalsIgnoreCase(placement) || "center".equalsIgnoreCase(placement) || "east".equalsIgnoreCase(placement);

            JPanel targetContainer;
            if (isGridLayout) {
                targetContainer = container;
            } else if (isHorizontal) {
                if (horizontalPanel == null) {
                    horizontalPanel = new JPanel(new WrapLayout(FlowLayout.LEFT, 0, 0));
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
                targetContainer.add(content);
                continue;
            }

            TableModel table = iniFileModel.getTable(panel.getPanelName());
            if (table != null) {
                TuningTableView tuningTableView = new TuningTableView(table.getTitle());
                tuningTableView.displayTable(iniFileModel, table.getTableId(), ci);
                JComponent content = tuningTableView.getContent();
                applyStyle(content);
                content.setAlignmentX(Component.LEFT_ALIGNMENT);
                targetContainer.add(content);
                continue;
            }

            JPanel panelWidget = new JPanel();
            panelWidget.setAlignmentX(Component.LEFT_ALIGNMENT);
            DialogModel subDialog = panel.resolveDialog(iniFileModel);
            String subLayoutHint = subDialog != null ? subDialog.getLayoutHint() : null;
            applyLayout(panelWidget, subLayoutHint);

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
            targetContainer.add(panelWidget);
        }
    }

    private static void fixRowHeight(JPanel row) {
        row.setMaximumSize(new Dimension(Integer.MAX_VALUE, row.getPreferredSize().height + 5));
    }

    private static void applyLayout(JPanel panel, String layoutHint) {
        if ("border".equalsIgnoreCase(layoutHint)) {
            // Equal-width columns keep the .ini West/East intent;
            // children wrap inside their allocated width via WrapLayout.
            panel.setLayout(new GridLayout(1, 0));
        } else if ("xAxis".equalsIgnoreCase(layoutHint)) {
            panel.setLayout(new BoxLayout(panel, BoxLayout.X_AXIS));
        } else {
            panel.setLayout(new BoxLayout(panel, BoxLayout.Y_AXIS));
        }
    }

    private static void applyStyle(JComponent component) {
        Font font = component.getFont();
        if (font != null) {
            component.setFont(font.deriveFont(font.getSize() * 1.2f));
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
