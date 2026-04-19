package com.rusefi.ui;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniMemberNotFound;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.SensorCentral;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.ts_plugin.knock.KnockCanvas;
import com.rusefi.ts_plugin.knock.KnockMagnitudeCanvas;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionListener;
import java.util.Arrays;
import java.util.concurrent.CopyOnWriteArrayList;

/**
 * Knock spectrogram pane for the rusEFI console.
 * Mirrors the behaviour of KnockAnalyzerTab (ts_plugin) but drives data
 * via SensorCentral / UIContext instead of the TunerStudio ControllerAccess API.
 */
public class KnockPane {

    private static final int PREFERRED_WIDTH = 800;
    private static final int PREF_HEIGHT = 200;
    private static final int COMPRESSED_SPECTRUM_PROTOCOL_SIZE = 16;
    private static final int MAG_WIDTH = 760;
    private static final int MAG_HEIGHT = 200;
    // Sum of indices 0..COMPRESSED_SPECTRUM_PROTOCOL_SIZE-1; used as a "all packets received" checksum.
    private static final int SPECTRUM_INDEX_CHECKSUM = COMPRESSED_SPECTRUM_PROTOCOL_SIZE * (COMPRESSED_SPECTRUM_PROTOCOL_SIZE - 1) / 2;

    private enum CanvasType {
        COMBINED,
        CT_SENSORS,
        CT_CYLINDERS,
    }

    private static final String STATUS_NOT_CONNECTED = "Not connected";
    private static final String STATUS_NOT_SUPPORTED = "Not supported by connected ECU (requires F7 target with KNOCK_SPECTROGRAM)";

    private final UIContext uiContext;

    private final JPanel content = new JPanel(new BorderLayout());
    private final JComponent canvasesPanel = new JPanel(new VerticalFlowLayout(VerticalFlowLayout.TOP, 5, 5));
    private final JButton buttonStartStop = new JButton("Start");
    private final JLabel statusLabel = new JLabel(STATUS_NOT_CONNECTED);

    private boolean started = false;
    private boolean flushed = false;

    private final int[] line_sum_index = {0};
    private int currentChannel = 0;
    private int currentCylinder = 0;

    private final float[] values = new float[COMPRESSED_SPECTRUM_PROTOCOL_SIZE * 4];

    private int cylindersCount = 0;

    private CanvasType canvasType = CanvasType.COMBINED;

    private final java.util.List<KnockCanvas> canvases = new CopyOnWriteArrayList<>();
    private final KnockMagnitudeCanvas magnitudes = new KnockMagnitudeCanvas();

    public KnockPane(UIContext uiContext) {
        this.uiContext = uiContext;

        buttonStartStop.setEnabled(false);
        buttonStartStop.addActionListener(e -> toggleStartStop());

        JComponent buttons = new JPanel(new FlowLayout(FlowLayout.LEFT, 5, 5));
        buttons.add(statusLabel);
        buttons.add(buttonStartStop);
        content.add(buttons, BorderLayout.NORTH);

        JScrollPane canvasScroll = new JScrollPane(canvasesPanel,
                JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED,
                JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);

        // Each tab owns a wrapper; the canvas scroll is re-parented to the active tab on switch.
        JPanel allPanel      = new JPanel(new BorderLayout());
        JPanel sensorsPanel  = new JPanel(new BorderLayout());
        JPanel cylPanel      = new JPanel(new BorderLayout());
        JPanel[] tabPanels   = {allPanel, sensorsPanel, cylPanel};
        CanvasType[] tabTypes = {CanvasType.COMBINED, CanvasType.CT_SENSORS, CanvasType.CT_CYLINDERS};
        allPanel.add(canvasScroll, BorderLayout.CENTER); // default tab = All

        JTabbedPane viewTabs = new JTabbedPane();
        viewTabs.addTab("All",      allPanel);
        viewTabs.addTab("Sensors",  sensorsPanel);
        viewTabs.addTab("Cylinders", cylPanel);
        viewTabs.addChangeListener(e -> {
            int idx = viewTabs.getSelectedIndex();
            for (JPanel p : tabPanels) p.remove(canvasScroll);
            tabPanels[idx].add(canvasScroll, BorderLayout.CENTER);
            tabPanels[idx].revalidate();
            switchCanvasType(tabTypes[idx]);
        });

        JSplitPane split = new JSplitPane(JSplitPane.VERTICAL_SPLIT, viewTabs, magnitudes.getComponent());
        split.setResizeWeight(0.7);
        split.setDividerSize(4);
        content.add(split, BorderLayout.CENTER);

        JComponent mag = magnitudes.getComponent();
        mag.setFocusable(true);
        mag.setFocusTraversalKeysEnabled(false);
        mag.setDoubleBuffered(true);
        mag.setPreferredSize(new Dimension(MAG_WIDTH, MAG_HEIGHT));
        mag.setMinimumSize(new Dimension(100, 50));

        subscribe();
        createCanvas();

        ConnectionStatusLogic.INSTANCE.addAndFireListener(isConnected -> SwingUtilities.invokeLater(() -> {
            if (isConnected) {
                initFromEcu();
            } else {
                setStartState(false);
                buttonStartStop.setEnabled(false);
                statusLabel.setText(STATUS_NOT_CONNECTED);
            }
        }));
    }

    // ---- ECU init (called on connect) ----------------------------------------

    private void initFromEcu() {
        BinaryProtocol bp = uiContext.getBinaryProtocol();
        if (bp == null) return;
        IniFileModel ini = uiContext.iniFileState.getIniFileModel();
        ConfigurationImage image = bp.getControllerConfiguration();
        if (ini == null || image == null) return;

        // Gate on output channel presence: m_knockSpectrum* only exist on targets
        // that compile with KNOCK_SPECTROGRAM (F7 and a few others).
        try {
            ini.getOutputChannel("m_knockSpectrum1");
        } catch (IniMemberNotFound e) {
            statusLabel.setText(STATUS_NOT_SUPPORTED);
            return;
        }

        IniField cylField = ini.getIniField("cylindersCount");
        if (cylField != null) {
            try {
                String val = ConfigurationImageGetterSetter.getStringValue(cylField, image);
                cylindersCount = (int) Double.parseDouble(val.trim());
            } catch (NumberFormatException ignored) {
            }
        }

        statusLabel.setText("");
        setStartState(readKnockEnabled(ini, image));
        buttonStartStop.setEnabled(true);
    }

    // ---- enableKnockSpectrogram helpers ---------------------------------------

    private boolean readKnockEnabled(IniFileModel ini, ConfigurationImage image) {
        IniField field = ini.getIniField("enableKnockSpectrogram");
        if (!(field instanceof EnumIniField)) return false;
        EnumIniField ef = (EnumIniField) field;
        int raw = image.getByteBuffer(ef).getInt();
        int ordinal = ConfigurationImage.getBitRange(raw, ef.getBitPosition(), ef.getBitSize0() + 1);
        // ordinal 0 = "no" (disabled), 1 = "yes" (enabled)
        return ordinal == 1;
    }

    private void toggleStartStop() {
        BinaryProtocol bp = uiContext.getBinaryProtocol();
        if (bp == null) return;
        IniFileModel ini = uiContext.iniFileState.getIniFileModel();
        ConfigurationImage image = bp.getControllerConfiguration();
        if (ini == null || image == null) return;

        boolean newEnabled = !readKnockEnabled(ini, image);
        setStartState(newEnabled);

        IniField field = ini.getIniField("enableKnockSpectrogram");
        if (!(field instanceof EnumIniField)) return;

        ConfigurationImage modified = image.clone();
        // ordinal 0 = "no" (disabled), 1 = "yes" (enabled)
        modified.setBitValue((EnumIniField) field, newEnabled ? 1 : 0);
        uiContext.getLinkManager().submit(() -> bp.uploadChanges(modified));
    }

    // ---- SensorCentral subscriptions -----------------------------------------

    private void subscribe() {
        SensorCentral sc = SensorCentral.getInstance();

        sc.addListener("m_knockFrequencyStart", v -> {
            int freq = (int) v;
            canvases.forEach(c -> c.setFrequencyStart(freq));
            magnitudes.setFrequencyStart(freq);
        });

        sc.addListener("m_knockFrequencyStep", v -> {
            float step = (float) v;
            canvases.forEach(c -> c.setFrequencyStep(step));
            magnitudes.setFrequencyStep(step);
        });

        sc.addListener("m_knockSpectrumChannelCyl", v -> {
            long packed = (long) v;
            flush();
            currentChannel = (int) (packed >>> 8) & 0xFF;
            currentCylinder = (int) (packed & 0xFF);
        });

        for (int i = 0; i < COMPRESSED_SPECTRUM_PROTOCOL_SIZE; ++i) {
            final int idx = i;
            sc.addListener("m_knockSpectrum" + (i + 1), v -> {
                if (!started) {
                    return;
                }
                flushed = false;
                long raw = (long) v;
                values[idx * 4]     = (raw >>> 24) & 0xFF;
                values[idx * 4 + 1] = (raw >>> 16) & 0xFF;
                values[idx * 4 + 2] = (raw >>> 8)  & 0xFF;
                values[idx * 4 + 3] =  raw          & 0xFF;
                line_sum_index[0] += idx;
                if (line_sum_index[0] >= SPECTRUM_INDEX_CHECKSUM) {
                    flush();
                    line_sum_index[0] = 0;
                }
            });
        }
    }

    // ---- Canvas management ---------------------------------------------------

    private void flush() {
        if (!started || flushed) return;

        switch (canvasType) {
            case COMBINED:
                canvases.forEach(c -> c.processValues(values));
                break;
            case CT_SENSORS:
                if (currentChannel < canvases.size())
                    canvases.get(currentChannel).processValues(values);
                break;
            case CT_CYLINDERS:
                if (currentCylinder < canvases.size())
                    canvases.get(currentCylinder).processValues(values);
                break;
        }

        SwingUtilities.invokeLater(() -> {
            canvases.forEach(c -> AutoupdateUtil.trueLayoutAndRepaint(c.getComponent()));
            Arrays.fill(values, 0);
            flushed = true;
        });
    }

    public void createCanvas() {
        SwingUtilities.invokeLater(() -> {
            canvases.clear();
            canvasesPanel.removeAll();
            switch (canvasType) {
                case COMBINED:
                    addCanvas(1, 1, true);
                    break;
                case CT_SENSORS:
                    addCanvas(1, 2, false);
                    addCanvas(2, 2, false);
                    break;
                case CT_CYLINDERS:
                    for (int i = 0; i < cylindersCount; i++)
                        addCanvas(i + 1, cylindersCount, false);
                    break;
                default:
                    throw new IllegalStateException("Unknown CanvasType");
            }
            canvasesPanel.revalidate();
            canvasesPanel.repaint();
        });
    }

    private void addCanvas(int number, int divider, boolean isCombined) {
        KnockCanvas canvas = new KnockCanvas(number, divider, isCombined);
        JComponent comp = canvas.getComponent();
        comp.setFocusTraversalKeysEnabled(false);
        comp.addMouseMotionListener(new KnockMotionListener(canvas, magnitudes));
        comp.addMouseListener(new MouseAdapter() {});
        comp.setFocusable(true);
        comp.setDoubleBuffered(true);
        comp.setPreferredSize(new Dimension(PREFERRED_WIDTH, PREF_HEIGHT));
        comp.setMinimumSize(new Dimension(PREFERRED_WIDTH, PREF_HEIGHT));
        canvasesPanel.add(comp);
        canvases.add(canvas);
    }

    /** For sandbox/test use: bypass ECU init and start capturing immediately. */
    public void simulateConnected(int cylinders) {
        cylindersCount = cylinders;
        statusLabel.setText("");
        buttonStartStop.setEnabled(true);
        setStartState(true);
        createCanvas();
    }

    public void setStartState(boolean enabled) {
        started = enabled;
        buttonStartStop.setText(started ? "Stop" : "Start");
        line_sum_index[0] = 0;
        if (started)
            canvases.forEach(KnockCanvas::resetPeak);
    }

    // ---- Canvas-type switch (used by canvas-type buttons) --------------------

    private void switchCanvasType(CanvasType type) {
        canvasType = type;
        createCanvas();
        toggle();
    }

    private void toggle() {
        toggleStartStop();
        toggleStartStop();
    }

    // ---- Public API ----------------------------------------------------------

    public JPanel getContent() {
        return content;
    }

    // ---- Inner listener classes ----------------------------------------------

    private static class KnockMotionListener implements MouseMotionListener {
        private final KnockCanvas knockCanvas;
        private final KnockMagnitudeCanvas magnitudesCanvas;

        KnockMotionListener(KnockCanvas canvas, KnockMagnitudeCanvas mags) {
            knockCanvas = canvas;
            magnitudesCanvas = mags;
        }

        @Override
        public void mouseDragged(MouseEvent e) {
        }

        @Override
        public void mouseMoved(MouseEvent e) {
            knockCanvas.setMousePosition(e.getX(), e.getY());
            float[] mags = knockCanvas.getCurrentMouseMagnitudes();
            if (hasNonZeroValue(mags))
                magnitudesCanvas.processValues(mags);
        }

        private boolean hasNonZeroValue(float[] values) {
            for (float v : values)
                if (v != 0f) return true;
            return false;
        }
    }

}
