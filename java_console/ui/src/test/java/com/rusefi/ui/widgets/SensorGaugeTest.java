package com.rusefi.ui.widgets;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileMetaInfoImpl;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniFileModelMocks;
import com.opensr5.ini.RawIniFile;
import com.opensr5.ini.TsStringFunction;
import com.rusefi.core.SensorCentral;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.util.UiUtils;
import com.rusefi.ui.widgets.tune.CalibrationDialogWidget;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.BorderLayout;
import java.awt.Component;
import java.io.ByteArrayInputStream;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

class SensorGaugeTest {
    @Test
    void gaugeMenuResolvesGppwmNoteAndSearchesItsDisplayName() throws Throwable {
        String text = "[Constants]\npage = 1\n"
            + "gpPwmNote1 = string, ASCII, 0, 16\n"
            + "[GaugeConfigurations]\ngaugeCategory = GPPWM Outputs\n"
            + "gppwmGauge1 = gppwmOutput1, { stringValue(gpPwmNote1) }, \"%\", 0, 100, 0, 0, 100, 100, 1, 1\n";
        IniFileModel ini = IniFileReaderUtil.readIniFile(IniFileReaderUtil.read(
            new ByteArrayInputStream(text.getBytes(StandardCharsets.US_ASCII))),
            "gppwm.ini", mock(IniFileMetaInfo.class));
        ConfigurationImage image = new ConfigurationImage(new byte[16]);
        byte[] note = "Cooling fan".getBytes(StandardCharsets.US_ASCII);
        System.arraycopy(note, 0, image.getContent(), 0, note.length);
        assertEquals("Cooling fan", TsStringFunction.resolve(ini.getGauge("gppwmGauge1").getTitle(), ini, image, null));

        SwingUtilities.invokeAndWait(() -> {
            UIContext context = new UIContext();
            context.iniFileState.setIniFileModelForTest(ini);
            BinaryProtocol protocol = mock(BinaryProtocol.class);
            when(protocol.getControllerConfiguration()).thenReturn(image);
            context.getLinkManager().setBinaryProtocolForTests(protocol);
            JPopupMenu menu = new JPopupMenu();
            SensorGauge.fillGaugeMenuItems(context, menu, new JPanelWithListener(new BorderLayout()), name -> { }, null);
            List<String> expected = java.util.Collections.singletonList("GPPWM Outputs / Cooling fan");
            assertEquals(expected, visibleGaugePaths(menu));
            GaugeMenuSearch.addTo(menu);
            JTextField search = (JTextField) UiUtils.getAllComponents(menu).stream()
                .filter(component -> component instanceof JTextField)
                .findFirst().orElseThrow(AssertionError::new);
            search.setText("cooling fan");
            assertEquals(expected, visibleGaugePaths(menu));
            search.setText("GPPWM OUTPUTS / COOLING FAN");
            assertEquals(expected, visibleGaugePaths(menu));
            search.setText("stringValue");
            assertTrue(visibleGaugePaths(menu).isEmpty());
            search.setText("");
            assertEquals(expected, visibleGaugePaths(menu));
        });
    }

    @Test
    void gaugeMenuUsesLoadedIniWithoutAnEcuConnection() throws Throwable {
        IniFileModel ini;
        try (InputStream input = getClass().getResourceAsStream("/january.ini")) {
            assertNotNull(input);
            RawIniFile raw = IniFileReaderUtil.read(input, "/january.ini");
            ini = IniFileReaderUtil.readIniFile(raw, "/january.ini", new IniFileMetaInfoImpl(raw));
        }
        List<String> expectedPaths = new ArrayList<>();
        ini.getGaugeCategories().forEach((category, model) -> model.getGauges().forEach(
            gauge -> expectedPaths.add(category + " / "
                + (gauge.getTitleValue().isExpression() ? gauge.getName() : gauge.getTitle()))));
        assertTrue(expectedPaths.size() > 100, "Exercise the sandbox's full INI catalog");

        SwingUtilities.invokeAndWait(() -> {
            UIContext context = new UIContext();
            context.iniFileState.setIniFileModelForTest(ini);
            assertNull(context.getBinaryProtocol());
            JPanelWithListener wrapper = new JPanelWithListener(new BorderLayout());
            JPopupMenu menu = new JPopupMenu();
            SensorGauge.fillGaugeMenuItems(context, menu, wrapper, name -> { }, null);
            assertEquals(expectedPaths, visibleGaugePaths(menu));
            GaugeMenuSearch.addTo(menu);
            JTextField search = (JTextField) UiUtils.getAllComponents(menu).stream()
                .filter(component -> component instanceof JTextField)
                .findFirst().orElseThrow(AssertionError::new);
            assertEquals(expectedPaths, visibleGaugePaths(menu), "Empty search shows the full catalog");
            search.setText("not-a-gauge-in-this-ini");
            assertTrue(visibleGaugePaths(menu).isEmpty());
            search.setText("");
            assertEquals(expectedPaths, visibleGaugePaths(menu), "Clearing search restores the full catalog");

            BinaryProtocol protocol = mock(BinaryProtocol.class);
            when(protocol.getIniFileNullable()).thenReturn(ini);
            context.getLinkManager().setBinaryProtocolForTests(protocol);
            JPopupMenu connectedMenu = new JPopupMenu();
            SensorGauge.fillGaugeMenuItems(context, connectedMenu, wrapper, name -> { }, null);
            assertEquals(expectedPaths, visibleGaugePaths(connectedMenu));
        });
    }

    private static List<String> visibleGaugePaths(JPopupMenu menu) {
        List<String> paths = new ArrayList<>();
        for (Component component : menu.getComponents()) {
            if (component instanceof JMenu && component.isVisible()) {
                JMenu category = (JMenu) component;
                for (Component child : category.getMenuComponents()) {
                    if (child instanceof JMenuItem && child.isVisible()) {
                        paths.add(category.getText() + " / " + ((JMenuItem) child).getText());
                    }
                }
            }
        }
        return paths;
    }

    @Test
    void tuneExpressionsDefineTheScaleAndDangerZone() throws Throwable {
        String text = "[Constants]\npage = 1\n"
            + "circularRpmLimit = scalar, U16, 0, \"rpm\", 1, 0, 0, 20000, 0\n"
            + "[GaugeConfigurations]\n"
            + "circularRpmGauge = circularRpmChannel, \"RPM\", \"rpm\", 0, {circularRpmLimit + 2000}, "
            + "0, 0, {circularRpmLimit - 500}, {circularRpmLimit}, 0, 0\n";
        IniFileModel ini = IniFileReaderUtil.readIniFile(IniFileReaderUtil.read(
            new ByteArrayInputStream(text.getBytes(StandardCharsets.US_ASCII))),
            "circular-rpm.ini", mock(IniFileMetaInfo.class));
        ConfigurationImage image = new ConfigurationImage(new byte[4]);
        image.getByteBuffer(0, 2).putShort((short) 6000);
        UIContext context = new UIContext();
        context.iniFileState.setIniFileModelForTest(ini);
        BinaryProtocol protocol = mock(BinaryProtocol.class);
        when(protocol.getControllerConfiguration()).thenReturn(image);
        context.getLinkManager().setBinaryProtocolForTests(protocol);
        JPanelWithListener wrapper = new JPanelWithListener(new BorderLayout());
        SwingUtilities.invokeAndWait(() -> {
            try {
                SensorGauge.createGaugeBody(context, "circularRpmGauge", wrapper, name -> { }, null);
                CircularGauge gauge = (CircularGauge) wrapper.getComponent(0);
                gauge.setValue(4000);
                // Resolve the tune's 6000 RPM limit: range 0..8000, danger at 6000.
                assertEquals(135, gauge.getProgressDegrees());
                assertEquals(CircularGauge.PROGRESS.getRGB() & 0xFFFFFF, CircularGaugeTest.render(gauge, 400, 400).getRGB(70, 150) & 0xFFFFFF);
                gauge.setValue(6000);
                assertEquals(203, gauge.getProgressDegrees());
                assertEquals(CircularGauge.DANGER_FILL.getRGB() & 0xFFFFFF, CircularGaugeTest.render(gauge, 400, 400).getRGB(70, 150) & 0xFFFFFF);
            } finally {
                wrapper.removeAllChildrenAndListeners();
            }
        });
    }

    @Test
    void dataFramesUpdateValuesAndExpressionLabelsInDashboardAndCalibrationGauges() throws Throwable {
        String text = "[PcVariables]\n"
            + "circularTitles = bits, U08, [0:1], \"Cold\", \"Warm\"\n"
            + "circularUnits = bits, U08, [0:1], \"F\", \"C\"\n"
            + "[OutputChannels]\n"
            + "circularFrameChannel = scalar, F32, 0, \"\", 1, 0\n"
            + "circularLabelIndex = scalar, U08, 4, \"\", 1, 0\n"
            + "[GaugeConfigurations]\n"
            + "circularFrameGauge = circularFrameChannel, {bitStringValue(circularTitles, circularLabelIndex)}, "
            + "{bitStringValue(circularUnits, circularLabelIndex)}, 0, 100, 0, 0, 80, 80, 1, 0\n"
            + "[UserDefined]\n"
            + "dialog = circularDialog, \"Circular gauge\"\n"
            + "gauge = circularFrameGauge\n";
        IniFileModel ini = IniFileReaderUtil.readIniFile(IniFileReaderUtil.read(
            new ByteArrayInputStream(text.getBytes(StandardCharsets.US_ASCII))),
            "circular-gauge.ini", mock(IniFileMetaInfo.class));
        UIContext context = new UIContext();
        context.iniFileState.setIniFileModelForTest(ini);
        JPanelWithListener wrapper = new JPanelWithListener(new BorderLayout());
        AtomicReference<CalibrationDialogWidget> dialog = new AtomicReference<>();
        AtomicReference<CircularGauge> dashboardGauge = new AtomicReference<>();
        AtomicReference<CircularGauge> dialogGauge = new AtomicReference<>();
        SensorCentral sensors = SensorCentral.getInstance();
        try {
            SwingUtilities.invokeAndWait(() -> {
                SensorGauge.createGaugeBody(context, "circularFrameGauge", wrapper, name -> { }, null);
                dashboardGauge.set((CircularGauge) wrapper.getComponent(0));
                assertEquals("", dashboardGauge.get().getTitle());
                assertEquals("", dashboardGauge.get().getUnitString());
                dialog.set(new CalibrationDialogWidget(context));
                dialog.get().update(ini.getDialogs().get("circularDialog"), ini, null);
                dialog.get().setActive(true);
                dialogGauge.set((CircularGauge) UiUtils.getAllComponents(dialog.get().getContentPane()).stream()
                    .filter(c -> c instanceof CircularGauge).findFirst().orElseThrow(AssertionError::new));
            });
            for (int i = 0; i < 2; i++) {
                // Output-channel offsets follow the protocol response-code byte.
                byte[] frame = ByteBuffer.allocate(6).order(ByteOrder.LITTLE_ENDIAN)
                    .put((byte) 0).putFloat(43 + i * 2).put((byte) i).array();
                sensors.grabSensorValues(frame, ini, null);
                final int index = i;
                SwingUtilities.invokeAndWait(() -> {
                    for (CircularGauge gauge : new CircularGauge[] {dashboardGauge.get(), dialogGauge.get()}) {
                        assertEquals(index == 0 ? "Cold" : "Warm", gauge.getTitle());
                        assertEquals(index == 0 ? "F" : "C", gauge.getUnitString());
                        assertEquals(index == 0 ? "44.0" : "46.0", gauge.getDisplayText());
                    }
                });
            }
        } finally {
            SwingUtilities.invokeAndWait(() -> {
                wrapper.removeAllChildrenAndListeners();
                if (dialog.get() != null) {
                    dialog.get().destroy();
                }
            });
        }
        assertFalse(sensors.getOutputChannelDemand().getChannels().contains("circularframechannel"));
        assertFalse(sensors.getOutputChannelDemand().getChannels().contains("circularlabelindex"));
    }

    @Test
    void headlessGaugeReceivesDataAndReplacementReleasesSubscriptions() throws Exception {
        GaugeModel model = CircularGaugeTest.model(0, 100, 80, 0);
        IniFileModelMocks.GaugeRegistry registry = IniFileModelMocks.mutableWithGauges();
        registry.register(model);
        UIContext context = new UIContext();
        context.iniFileState.setIniFileModelForTest(registry.model);
        SensorCentral sensors = SensorCentral.getInstance();
        sensors.setValue(20, model.getChannel());
        JPanelWithListener wrapper = new JPanelWithListener(new BorderLayout());
        AtomicReference<CircularGauge> original = new AtomicReference<>();
        try {
            SwingUtilities.invokeAndWait(() -> {
                SensorGauge.createGaugeBody(context, model.getName(), wrapper, name -> { }, null);
                assertEquals(1, wrapper.getComponentCount());
                original.set((CircularGauge) wrapper.getComponent(0));
                assertEquals("20", original.get().getDisplayText());
                assertTrue(original.get().getToolTipText().contains("Double-click to detach"));
                assertTrue(original.get().getMouseListeners().length > 0);
            });
            // Simulate the ECU thread. The rendered reading must be published atomically.
            sensors.setValue(43, model.getChannel());
            assertEquals("44", original.get().getDisplayText());
            SwingUtilities.invokeAndWait(() -> wrapper.setActive(false));
            sensors.setValue(50, model.getChannel());
            assertEquals("44", original.get().getDisplayText());
            SwingUtilities.invokeAndWait(() -> {
                wrapper.setActive(true);
                SensorGauge.createGaugeBody(context, model.getName(), wrapper, name -> { }, null);
                assertNotSame(original.get(), wrapper.getComponent(0));
                assertEquals(1, wrapper.getMouseListeners().length);
            });
            sensors.setValue(60, model.getChannel());
            assertEquals("44", original.get().getDisplayText(), "Removed gauge no longer receives data");
            SwingUtilities.invokeAndWait(() ->
                assertEquals("60", ((CircularGauge) wrapper.getComponent(0)).getDisplayText()));
        } finally {
            SwingUtilities.invokeAndWait(wrapper::removeAllChildrenAndListeners);
        }
        assertFalse(sensors.getOutputChannelDemand().getChannels().contains(model.getChannel().toLowerCase()));
        assertEquals(0, wrapper.getMouseListeners().length);
    }
}
