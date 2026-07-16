package com.rusefi.ui.widgets.tune;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.rusefi.config.generated.VariableRegistryValues;
import com.rusefi.trigger.TriggerImage;
import com.rusefi.trigger.TriggerWheelInfo;
import com.opensr5.ini.field.IniField;

import javax.swing.*;
import javax.swing.border.LineBorder;
import java.awt.*;
import java.io.File;
import java.util.Optional;

import static com.devexperts.logging.Logging.getLogging;

/**
 * Extraction of trigger-related UI hacks from CalibrationDialogWidget
 */
public class TriggerImageHelper {
    private static final Logging log = getLogging(TriggerImageHelper.class);

    private static final String TRIGGER_PRIMARY = "trigger_primary";
    private static final String TRIGGER_CAMS = "trigger_cams";
    private static final String PRIMARY_TRIGGER_UI_NAME = "Primary Trigger";
    private static final String CAM_INPUTS_UI_NAME = "Cam Inputs";

    private final JPanel triggerImagePanel = new JPanel(new BorderLayout());

    public enum Channel {
        PRIMARY,
        SECONDARY
    }

    public void updateTriggerImage(IniFileModel currentIniFileModel, ConfigurationImage workingImage) {
        updateTriggerImage(currentIniFileModel, workingImage, Channel.PRIMARY);
    }

    public void updateTriggerImage(IniFileModel currentIniFileModel, ConfigurationImage workingImage, Channel channel) {
        triggerImagePanel.removeAll();
        if (currentIniFileModel == null || workingImage == null) {
            showFallback("Trigger configuration unavailable");
            return;
        }
        Optional<IniField> triggerTypeField = currentIniFileModel.findIniField("trigger_type");
        if (!triggerTypeField.isPresent()) {
            showFallback("Trigger type unavailable");
            return;
        }
        Double typeId = workingImage.readNumericValue(triggerTypeField.get());
        if (typeId == null) {
            showFallback("Trigger type unavailable");
            return;
        }

        renderTriggerWheel(typeId.intValue(), channel, channel == Channel.PRIMARY);
    }

    public void updateVvtTriggerImage(IniFileModel currentIniFileModel, ConfigurationImage workingImage) {
        triggerImagePanel.removeAll();
        if (currentIniFileModel == null || workingImage == null) {
            showFallback("Cam configuration unavailable");
            return;
        }
        if (!currentIniFileModel.findIniField("vvtMode1").isPresent()) {
            showFallback("Cam mode unavailable");
            return;
        }

        JPanel previews = new JPanel(new GridLayout(0, 1, 0, 8));
        addVvtPreview(previews, "Intake cam", "vvtMode1", currentIniFileModel, workingImage);
        addVvtPreview(previews, "Exhaust cam", "vvtMode2", currentIniFileModel, workingImage);
        if (previews.getComponentCount() == 0) {
            showFallback("No cam mode selected");
        } else {
            triggerImagePanel.add(previews);
            triggerImagePanel.revalidate();
            triggerImagePanel.repaint();
        }
    }

    private void addVvtPreview(JPanel previews, String title, String fieldName,
                               IniFileModel ini, ConfigurationImage image) {
        Optional<IniField> modeField = ini.findIniField(fieldName);
        if (!modeField.isPresent()) {
            return;
        }
        Double modeValue = image.readNumericValue(modeField.get());
        if (modeValue == null || modeValue.intValue() == VariableRegistryValues.vvt_mode_e_VVT_INACTIVE) {
            return;
        }
        Integer triggerType = vvtModeToTriggerType(modeValue.intValue());
        JComponent preview = triggerType == null
            ? fallbackLabel("No wheel preview for this cam mode")
            : createWheelComponent(triggerType, Channel.PRIMARY, false);
        JPanel labeledPreview = new JPanel(new BorderLayout(0, 4));
        JLabel label = new JLabel(title, SwingConstants.CENTER);
        labeledPreview.add(label, BorderLayout.NORTH);
        labeledPreview.add(preview, BorderLayout.CENTER);
        previews.add(labeledPreview);
    }

    private void renderTriggerWheel(int typeId, Channel channel, boolean showTdc) {
        triggerImagePanel.removeAll();
        triggerImagePanel.add(createWheelComponent(typeId, channel, showTdc));
        triggerImagePanel.revalidate();
        triggerImagePanel.repaint();
    }

    private JComponent createWheelComponent(int typeId, Channel channel, boolean showTdc) {
        JComponent[] result = {null};
        try {
            // Check current directory then parent directory for unit_tests
            String unitTestsPath = "unit_tests";
            if (!new File(unitTestsPath).exists()) {
                unitTestsPath = ".." + File.separator + "unit_tests";
            }
            if (!new File(unitTestsPath).exists()) {
                unitTestsPath = ".." + File.separator + ".." + File.separator + "unit_tests";
            }

            TriggerWheelInfo.readWheels(unitTestsPath, wheelInfo -> {
                if (typeId == wheelInfo.getId()) {
                    if (channel == Channel.SECONDARY && !wheelInfo.isHasSecondChannel()) {
                        result[0] = fallbackLabel("No cam wheel for this trigger");
                        return;
                    }
                    result[0] = TriggerImage.createWheelPanel(
                        channel == Channel.PRIMARY
                            ? wheelInfo.getFirstWheeTriggerSignals()
                            : wheelInfo.getSecondWheeTriggerSignals(),
                        showTdc,
                        wheelInfo);
                }
            });
        } catch (Exception e) {
            log.error("Error reading trigger wheels: " + e.getMessage());
            return fallbackLabel("Trigger wheel metadata unavailable");
        }
        return result[0] == null ? fallbackLabel("Trigger wheel metadata unavailable") : result[0];
    }

    static Integer vvtModeToTriggerType(int vvtMode) {
        switch (vvtMode) {
            case VariableRegistryValues.vvt_mode_e_VVT_TOYOTA_3_TOOTH:
                return VariableRegistryValues.trigger_type_e_TT_VVT_TOYOTA_3_TOOTH;
            case VariableRegistryValues.vvt_mode_e_VVT_MIATA_NB:
                return VariableRegistryValues.trigger_type_e_TT_VVT_MIATA_NB;
            case VariableRegistryValues.vvt_mode_e_VVT_BOSCH_QUICK_START:
                return VariableRegistryValues.trigger_type_e_TT_VVT_BOSCH_QUICK_START;
            case VariableRegistryValues.vvt_mode_e_VVT_HONDA_K_EXHAUST:
                return VariableRegistryValues.trigger_type_e_TT_HONDA_K_CAM_4_1;
            case VariableRegistryValues.vvt_mode_e_VVT_HONDA_K_INTAKE:
            case VariableRegistryValues.vvt_mode_e_VVT_SINGLE_TOOTH:
            case VariableRegistryValues.vvt_mode_e_VVT_MAP_V_TWIN:
                return VariableRegistryValues.trigger_type_e_TT_HALF_MOON;
            case VariableRegistryValues.vvt_mode_e_VVT_FORD_ST170:
                return VariableRegistryValues.trigger_type_e_TT_FORD_ST170;
            case VariableRegistryValues.vvt_mode_e_VVT_BARRA_3_PLUS_1:
                return VariableRegistryValues.trigger_type_e_TT_VVT_BARRA_3_PLUS_1;
            case VariableRegistryValues.vvt_mode_e_VVT_FORD_COYOTE:
                return VariableRegistryValues.trigger_type_e_TT_VVT_FORD_COYOTE;
            case VariableRegistryValues.vvt_mode_e_VVT_DEV:
                return VariableRegistryValues.trigger_type_e_TT_DEV;
            case VariableRegistryValues.vvt_mode_e_VVT_MAZDA_SKYACTIV:
                return VariableRegistryValues.trigger_type_e_TT_VVT_MAZDA_SKYACTIV;
            case VariableRegistryValues.vvt_mode_e_VVT_MAZDA_L:
                return VariableRegistryValues.trigger_type_e_TT_VVT_MAZDA_L;
            case VariableRegistryValues.vvt_mode_e_VVT_MITSUBISHI_6G75:
                return VariableRegistryValues.trigger_type_e_TT_VVT_MITSUBISHI_6G75;
            case VariableRegistryValues.vvt_mode_e_VVT_NISSAN_VQ:
                return VariableRegistryValues.trigger_type_e_TT_VVT_NISSAN_VQ35;
            case VariableRegistryValues.vvt_mode_e_VVT_TOYOTA_4_1:
                return VariableRegistryValues.trigger_type_e_TT_VVT_TOYOTA_4_1;
            case VariableRegistryValues.vvt_mode_e_VVT_MITSUBISHI_4G69:
                return VariableRegistryValues.trigger_type_e_TT_VVT_MITSUBISHI_4G69;
            case VariableRegistryValues.vvt_mode_e_VVT_MITSUBISHI_3A92:
                return VariableRegistryValues.trigger_type_e_TT_VVT_MITSUBISHI_3A92;
            case VariableRegistryValues.vvt_mode_e_VVT_MITSUBISHI_6G72:
                return VariableRegistryValues.trigger_type_e_TT_VVT_MITSU_6G72;
            case VariableRegistryValues.vvt_mode_e_VVT_HONDA_CBR_600:
                return VariableRegistryValues.trigger_type_e_TT_HONDA_CBR_600;
            case VariableRegistryValues.vvt_mode_e_VVT_CHRYSLER_PHASER:
                return VariableRegistryValues.trigger_type_e_TT_CHRYSLER_PHASER;
            case VariableRegistryValues.vvt_mode_e_VVT_TOYOTA_3TOOTH_UZ:
                return VariableRegistryValues.trigger_type_e_TT_TOYOTA_3_TOOTH_UZ;
            case VariableRegistryValues.vvt_mode_e_VVT_NISSAN_MR:
                return VariableRegistryValues.trigger_type_e_TT_NISSAN_MR18_CAM_VVT;
            case VariableRegistryValues.vvt_mode_e_VVT_BMW_N63TU:
            case VariableRegistryValues.vvt_mode_e_VVT_MITSUBISHI_4G63:
                return VariableRegistryValues.trigger_type_e_TT_MITSU_4G63_CAM;
            case VariableRegistryValues.vvt_mode_e_VVT_HR12DDR_IN:
                return VariableRegistryValues.trigger_type_e_TT_NISSAN_HR_CAM_IN;
            case VariableRegistryValues.vvt_mode_e_VVT_SUBARU_7TOOTH:
                return VariableRegistryValues.trigger_type_e_TT_VVT_SUBARU_7_WITHOUT_6;
            default:
                return null;
        }
    }

    private void showFallback(String text) {
        triggerImagePanel.removeAll();
        triggerImagePanel.add(fallbackLabel(text));
        triggerImagePanel.revalidate();
        triggerImagePanel.repaint();
    }

    private static JLabel fallbackLabel(String text) {
        return new JLabel(text, SwingConstants.CENTER);
    }

    public JComponent getPanel() {
        return triggerImagePanel;
    }

    public Channel getChannel(String key, String uiName) {
        return TRIGGER_CAMS.equalsIgnoreCase(key) || CAM_INPUTS_UI_NAME.equalsIgnoreCase(uiName)
            ? Channel.SECONDARY
            : Channel.PRIMARY;
    }

    public boolean isTriggerPanel(String key, String uiName) {
        return TRIGGER_PRIMARY.equalsIgnoreCase(key) ||
                TRIGGER_CAMS.equalsIgnoreCase(key) ||
                PRIMARY_TRIGGER_UI_NAME.equalsIgnoreCase(uiName) ||
                CAM_INPUTS_UI_NAME.equalsIgnoreCase(uiName) ||
                "Sub Panel".equalsIgnoreCase(uiName);
    }

    public void addTriggerPanelExtras(JPanel panel) {
        String name = panel.getName();
        if (!isTriggerPanel(name, name)) {
            return;
        }

        if (!(panel.getLayout() instanceof BorderLayout)) {
            wrapWithBorderLayout(panel);
        }

        JPanel eastPanel = new JPanel(new BorderLayout()) {
            @Override
            public Dimension getPreferredSize() {
                Dimension d = super.getPreferredSize();
                Container parent = getParent();
                if (parent != null && parent.getWidth() > 0) {
                    return new Dimension((int) (parent.getWidth() * 0.2), d.height);
                }
                return new Dimension(100, d.height);
            }
        };
        Color border = UIManager.getColor("Separator.foreground");
        eastPanel.setBorder(BorderFactory.createCompoundBorder(
            BorderFactory.createEmptyBorder(0, 16, 0, 0),
            new LineBorder(border == null ? Color.GRAY : border)));

        eastPanel.add(triggerImagePanel);
        panel.add(eastPanel, BorderLayout.EAST);
    }

    private static void wrapWithBorderLayout(JPanel panel) {
        JPanel inner = new JPanel();
        LayoutManager oldLayout = panel.getLayout();
        if (oldLayout instanceof BoxLayout) {
            int axis = ((BoxLayout) oldLayout).getAxis();
            inner.setLayout(new BoxLayout(inner, axis));
        } else {
            inner.setLayout(oldLayout);
        }
        Component[] components = panel.getComponents();
        panel.removeAll();
        for (Component c : components) {
            inner.add(c);
        }
        panel.setLayout(new BorderLayout());
        panel.add(inner, BorderLayout.CENTER);
    }
}
