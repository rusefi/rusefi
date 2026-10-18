package com.rusefi.ui.widgets;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileModel;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.ISensorHolder;
import com.rusefi.core.SensorCentral;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ui.UIContext;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import javax.swing.SwingUtilities;
import java.io.ByteArrayInputStream;
import java.nio.charset.StandardCharsets;
import java.util.Collections;
import java.util.Map;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

class GaugeMenuLabelTest {
    private IniFileModel ini;
    private UIContext context;
    private Map<String, ISensorHolder.ResolvedGaugeLabels> previousLabels;

    @BeforeEach
    void setUp() throws Throwable {
        String text = "[Constants]\npage = 1\n"
            + "gpPwmNote1 = string, ASCII, 0, 16\n"
            + "[GaugeConfigurations]\ngaugeCategory = GPPWM Outputs\n"
            + "gppwmGauge1 = gppwmOutput1, { stringValue(gpPwmNote1) }, \"%\", 0, 100, 0, 0, 100, 100, 1, 1\n"
            + "rpmGauge = rpm, \"Engine speed\", \"rpm\", 0, 8000, 0, 0, 7000, 7000, 0, 0\n";
        ini = IniFileReaderUtil.readIniFile(IniFileReaderUtil.read(
            new ByteArrayInputStream(text.getBytes(StandardCharsets.US_ASCII))),
            "menu-labels.ini", mock(IniFileMetaInfo.class));
        context = new UIContext();
        context.iniFileState.setIniFileModelForTest(ini);
        previousLabels = SensorCentral.getInstance().getResolvedGaugeLabels();
        SensorCentral.getInstance().onGaugeLabelsResolved(Collections.emptyMap());
    }

    @AfterEach
    void restoreLabels() {
        SensorCentral.getInstance().onGaugeLabelsResolved(previousLabels);
    }

    @Test
    void currentTuneOverridesEcuAndCachedLabels() throws Exception {
        BinaryProtocol protocol = mock(BinaryProtocol.class);
        when(protocol.getControllerConfiguration()).thenReturn(image("Cooling fan"));
        context.getLinkManager().setBinaryProtocolForTests(protocol);
        SensorCentral.getInstance().onGaugeLabelsResolved(Collections.singletonMap("gppwmGauge1",
            new ISensorHolder.ResolvedGaugeLabels("Old note", "%")));
        SwingUtilities.invokeAndWait(() -> {
            assertEquals("Cooling fan", title());
            context.fireConfigImageChanged(image("Water pump"));
            assertEquals("Water pump", title());
            context.fireConfigImageChanged(image(""));
            assertEquals("gppwmGauge1", title(), "An empty note remains selectable and does not reuse a stale label");
        });
    }

    @Test
    void offlineTuneResolvesWithoutSensorUpdates() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            context.setOfflineMode(true);
            context.fireConfigImageChanged(image("Cooling fan"));
            assertEquals("Cooling fan", title());
        });
    }

    @Test
    void missingDataUsesIdentifierAndKeepsStaticTitles() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            assertEquals("gppwmGauge1", title());
            assertEquals("Engine speed", GaugeMenuLabel.getTitle(context, ini.getGauge("rpmGauge")));
            // A resolved units expression alone must not expose an unresolved title.
            SensorCentral.getInstance().onGaugeLabelsResolved(Collections.singletonMap("gppwmGauge1",
                new ISensorHolder.ResolvedGaugeLabels(ini.getGauge("gppwmGauge1").getTitle(), "%")));
            assertEquals("gppwmGauge1", title());
        });
    }

    @Test
    void usesResolvedLiveLabelsWithoutConfigurationImage() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            SensorCentral.getInstance().onGaugeLabelsResolved(Collections.singletonMap("gppwmGauge1",
                new ISensorHolder.ResolvedGaugeLabels("Cooling fan", "%")));
            assertEquals("Cooling fan", title());
            context.setOfflineMode(true);
            assertEquals("gppwmGauge1", title(), "Offline menus must not use another session's live labels");
        });
    }

    private String title() {
        return GaugeMenuLabel.getTitle(context, ini.getGauge("gppwmGauge1"));
    }

    private static ConfigurationImage image(String note) {
        byte[] content = new byte[16];
        byte[] bytes = note.getBytes(StandardCharsets.US_ASCII);
        System.arraycopy(bytes, 0, content, 0, bytes.length);
        return new ConfigurationImage(content);
    }
}
