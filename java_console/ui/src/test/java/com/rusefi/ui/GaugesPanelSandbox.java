package com.rusefi.ui;

import com.devexperts.logging.Logging;
import com.opensr5.ini.*;
import com.rusefi.NamedThreadFactory;
import com.rusefi.core.SensorCentral;
import com.rusefi.core.SensorNames;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ini.reader.IniParsingException;

import javax.swing.*;
import java.awt.*;
import java.io.InputStream;
import java.util.concurrent.*;

import static com.devexperts.logging.Logging.getLogging;

public class GaugesPanelSandbox {
    private static final Logging log = getLogging(GaugesPanelSandbox.class);
    private static final String INI_PATH = "/january.ini";

    // Configuration array: {Channel Name, Fallback Gauge Name}
    private static final String[][] GAUGE_CONF = {
        {"RPMValue", "RPMGauge"},
        {SensorNames.VEHICLESPEEDKPH, "vssGauge"}
    };

    private final ScheduledExecutorService scheduler = Executors.newSingleThreadScheduledExecutor(new NamedThreadFactory("Simulation", true));
    private final String[] resolvedGauges = new String[GAUGE_CONF.length];
    private IniFileModel ini;

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> new GaugesPanelSandbox().init());
    }

    private void init() {
        try (InputStream is = getClass().getResourceAsStream(INI_PATH)) {
            if (is == null) {
                log.error("Resource file not found at: " + INI_PATH);
                return;
            }

            RawIniFile content = IniFileReaderUtil.read(is, INI_PATH);
            this.ini = IniFileReaderUtil.readIniFile(content, INI_PATH, new IniFileMetaInfoImpl(content));

            UIContext uiContext = new UIContext();
            uiContext.iniFileState.setIniFileModelForTest(ini);

            // Create 1x2 grid for the two requested gauges
            GaugesGrid grid = new GaugesGrid(1, GAUGE_CONF.length);
            Node layoutNode = new Node();

            for (int i = 0; i < GAUGE_CONF.length; i++) {
                String channel = GAUGE_CONF[i][0];
                String fallback = GAUGE_CONF[i][1];

                // Resolve gauge name from INI or use fallback
                GaugeModel m = ini.findGaugeByChannel(channel);
                resolvedGauges[i] = (m != null) ? m.getName() : fallback;

                grid.panel.add(GaugesGridElement.create(uiContext, layoutNode.getChild("g" + i), resolvedGauges[i]).getContent());
            }

            JFrame frame = new JFrame("RusEFI Sandbox - Dual View");
            frame.add(grid.panel);
            frame.setSize(1000, 500);
            frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
            frame.addWindowListener(new java.awt.event.WindowAdapter() {
                @Override public void windowClosing(java.awt.event.WindowEvent e) { scheduler.shutdown(); }
            });
            frame.setLocationRelativeTo(null);
            frame.setVisible(true);

            // Start fixed-rate simulation
            scheduler.scheduleAtFixedRate(this::tick, 0, 100, TimeUnit.MILLISECONDS);
            log.info("Sandbox initialized. Gauges: " + resolvedGauges[0] + ", " + resolvedGauges[1]);
        } catch (Exception | IniParsingException e) {
            log.error("Initialization failed: " + e.getMessage());
        }
    }

    private void tick() {
        if (GaugesPanel.IS_PAUSED) return;
        SensorCentral sc = SensorCentral.getInstance();
        java.util.concurrent.ThreadLocalRandom rnd = java.util.concurrent.ThreadLocalRandom.current();

        for (String gaugeName : resolvedGauges) {
            GaugeModel m = ini.getGauge(gaugeName);
            if (m != null) {
                // Generate random value within INI limits
                double val = rnd.nextDouble(m.getLowValue(), Math.max(m.getLowValue() + 1, m.getHighValue()));
                sc.setValue(val, m.getChannel());
            }
        }
    }
}
