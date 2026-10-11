package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileModel;
import com.rusefi.core.SensorCentral;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ui.UIContext;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.io.ByteArrayInputStream;
import java.nio.charset.StandardCharsets;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;

class RuntimeValueTest {
    private static final String VOLTAGE = "issue10351Voltage";
    private static final String GEAR = "issue10351Gear";
    private static final String CHANNELS = "[OutputChannels]\n"
        + VOLTAGE + " = scalar, U16, 0, \"V\", 0.001, 0\n"
        + GEAR + " = scalar, U08, 2, \"\", 1, 0\n[UserDefined]\n";

    @Test
    void nestedRuntimeValuesReceiveLiveDataAndReleaseDemand() throws Throwable {
        IniFileModel ini = parse(CHANNELS + "dialog = gearSensorReadout\n"
            + "runtimeValue = \"Gear sensor voltage\", " + VOLTAGE + "\n"
            + "runtimeValue = \"Gear detected\", " + GEAR + "\n"
            + "dialog = calibration, \"Gear sensor calibration\"\npanel = gearSensorReadout\n");
        AtomicReference<CalibrationDialogWidget> ref = new AtomicReference<>();
        try {
            SwingUtilities.invokeAndWait(() -> {
                CalibrationDialogWidget widget = new CalibrationDialogWidget(new UIContext());
                ref.set(widget);
                widget.update("calibration", ini, new ConfigurationImage(4));
                assertEquals(Arrays.asList("Gear sensor voltage", "---", "Gear detected", "---"),
                    labels(widget.getContentPane()));
                assertFalse(hasDemand(VOLTAGE));
                widget.setActive(true);
                assertTrue(hasDemand(VOLTAGE));
                assertTrue(hasDemand(GEAR));
            });
            publish(ini, 332, 2);
            SwingUtilities.invokeAndWait(() -> {
                CalibrationDialogWidget widget = ref.get();
                assertEquals(Arrays.asList("Gear sensor voltage", new DecimalFormat("0.###").format(0.332),
                    "Gear detected", "2"), labels(widget.getContentPane()));
                assertArrayEquals(new byte[4], widget.getWorkingImage().getContent());
                widget.setActive(false);
                assertFalse(hasDemand(VOLTAGE));
                assertFalse(hasDemand(GEAR));
            });
            publish(ini, 1234, 3);
            SwingUtilities.invokeAndWait(() -> {
                assertEquals("2", labels(ref.get().getContentPane()).get(3), "Inactive readouts do not refresh");
                ref.get().setActive(true);
            });
            publish(ini, 1234, 3);
            SwingUtilities.invokeAndWait(() -> {
                assertEquals(new DecimalFormat("0.###").format(1.234), labels(ref.get().getContentPane()).get(1));
                assertEquals("3", labels(ref.get().getContentPane()).get(3));
                ref.get().reset();
                assertFalse(hasDemand(VOLTAGE));
                ref.get().update("calibration", ini, null);
                assertTrue(hasDemand(VOLTAGE));
            });
        } finally {
            SwingUtilities.invokeAndWait(() -> {
                if (ref.get() != null) {
                    ref.get().destroy();
                }
            });
        }
        assertFalse(hasDemand(VOLTAGE));
        assertFalse(hasDemand(GEAR));
    }

    @Test
    void runtimeValuesKeepOrderAndHonorExpressionsAmongEditableFields() throws Throwable {
        IniFileModel ini = parse("[Constants]\npage = 1\n"
            + "enabled = scalar, U08, 0, \"\", 1, 0, 0, 1, 0\n" + CHANNELS
            + "dialog = mixed\nfield = \"Enabled\", enabled\n"
            + "runtimeValue = \"Voltage\", " + VOLTAGE + ", { enabled }, { 1 }\n"
            + "runtimeValue = \"Gear\", " + GEAR + ", { 1 }, { enabled }\n"
            + "commandButton = \"Grab\", grab\n");
        SwingUtilities.invokeAndWait(() -> {
            CalibrationDialogWidget widget = new CalibrationDialogWidget(new UIContext());
            try {
                widget.update("mixed", ini, new ConfigurationImage(4));
                JPanel panel = widget.getContentPane();
                assertEquals(4, panel.getComponentCount());
                JPanel voltage = (JPanel) panel.getComponent(1);
                JPanel gear = (JPanel) panel.getComponent(2);
                assertEquals(Arrays.asList("Voltage", "---"), labels(voltage));
                assertFalse(voltage.getComponent(1).isEnabled());
                assertFalse(gear.isVisible());
                JTextField editor = null;
                for (Component child : ((JPanel) panel.getComponent(0)).getComponents()) {
                    if (child instanceof JTextField) {
                        editor = (JTextField) child;
                    }
                }
                assertNotNull(editor);
                editor.setText("1");
                assertTrue(voltage.getComponent(1).isEnabled());
                assertTrue(gear.isVisible());
                assertEquals(1, widget.getWorkingImage().getContent()[0]);
            } finally {
                widget.destroy();
            }
        });
    }

    private static IniFileModel parse(String text) throws Throwable {
        return IniFileReaderUtil.readIniFile(IniFileReaderUtil.read(new ByteArrayInputStream(
            text.getBytes(StandardCharsets.US_ASCII))), "issue10351.ini", mock(IniFileMetaInfo.class));
    }

    private static void publish(IniFileModel ini, int voltage, int gear) {
        SensorCentral.getInstance().grabSensorValues(
            new byte[]{0, (byte) voltage, (byte) (voltage >> 8), (byte) gear}, ini, null);
    }

    private static boolean hasDemand(String channel) {
        return SensorCentral.getInstance().getOutputChannelDemand().getChannels()
            .contains(channel.toLowerCase(java.util.Locale.ROOT));
    }

    private static List<String> labels(Container container) {
        List<String> result = new ArrayList<>();
        for (Component component : container.getComponents()) {
            if (component instanceof JLabel) {
                result.add(((JLabel) component).getText());
            } else if (component instanceof Container) {
                result.addAll(labels((Container) component));
            }
        }
        return result;
    }
}
