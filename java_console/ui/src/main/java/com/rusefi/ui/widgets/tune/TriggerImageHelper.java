package com.rusefi.ui.widgets.tune;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.rusefi.trigger.TriggerImage;
import com.rusefi.trigger.TriggerWheelInfo;
import com.opensr5.ini.field.IniField;
import com.rusefi.ui.util.SwingUtil;

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

    public void updateTriggerImage(IniFileModel currentIniFileModel, ConfigurationImage workingImage) {
        if (currentIniFileModel == null || workingImage == null) {
            return;
        }
        Optional<IniField> triggerTypeField = currentIniFileModel.findIniField("trigger_type");
        if (!triggerTypeField.isPresent()) {
            return;
        }
        Double typeId = workingImage.readNumericValue(triggerTypeField.get());
        if (typeId == null) {
            return;
        }

        triggerImagePanel.removeAll();
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
                if (typeId.intValue() == wheelInfo.getId()) {
                    JPanel wheelPanel = TriggerImage.createWheelPanel(wheelInfo.getFirstWheeTriggerSignals(), true, wheelInfo);
                    triggerImagePanel.add(wheelPanel);
                }
            });
        } catch (Exception e) {
            log.error("Error reading trigger wheels: " + e.getMessage());
        }
        triggerImagePanel.revalidate();
        triggerImagePanel.repaint();
    }

    public boolean isTriggerPanel(String key, String uiName) {
        return TRIGGER_PRIMARY.equalsIgnoreCase(key) ||
                TRIGGER_CAMS.equalsIgnoreCase(key) ||
                PRIMARY_TRIGGER_UI_NAME.equalsIgnoreCase(uiName) ||
                CAM_INPUTS_UI_NAME.equalsIgnoreCase(uiName);
    }

    public void addTriggerPanelExtras(JPanel panel) {
        if (!isTriggerPanel(panel.getName(), panel.getName())) {
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
        eastPanel.setBorder(new LineBorder(Color.ORANGE));

        if (TRIGGER_PRIMARY.equalsIgnoreCase(panel.getName()) || PRIMARY_TRIGGER_UI_NAME.equalsIgnoreCase(panel.getName())) {
            eastPanel.add(triggerImagePanel);
        }
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
