package com.rusefi.ui;

import com.formdev.flatlaf.FlatClientProperties;
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
import com.rusefi.core.preferences.storage.PersistentConfiguration;
import com.rusefi.ts_plugin.knock.KnockPlotStyle;
import com.rusefi.ui.widgets.CommandIcon;
import com.rusefi.ui.widgets.ConsoleFonts;
import com.rusefi.ui.widgets.JoinedButtons;
import com.rusefi.ui.widgets.SegmentedControl;
import com.rusefi.ui.widgets.StatusChip;
import com.rusefi.ui.widgets.ToolbarStrip;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.Arrays;
import java.util.concurrent.CopyOnWriteArrayList;

/**
 * Knock spectrogram pane for the rusEFI console.
 * Mirrors the behaviour of KnockAnalyzerTab (ts_plugin) but drives data
 * via SensorCentral / UIContext instead of the TunerStudio ControllerAccess API.
 */
public class KnockPane {

    /** Minimal readable height of a single spectrogram, the scroll pane kicks in below that. */
    private static final int MIN_CANVAS_HEIGHT = 140;
    private static final int PREFERRED_WIDTH = 600;
    private static final int COMPRESSED_SPECTRUM_PROTOCOL_SIZE = 16;
    private static final int MAG_HEIGHT = 170;
    // Sum of indices 0..COMPRESSED_SPECTRUM_PROTOCOL_SIZE-1; used as a "all packets received" checksum.
    private static final int SPECTRUM_INDEX_CHECKSUM = COMPRESSED_SPECTRUM_PROTOCOL_SIZE * (COMPRESSED_SPECTRUM_PROTOCOL_SIZE - 1) / 2;

    private enum CanvasType {
        COMBINED,
        CT_SENSORS,
        CT_CYLINDERS,
    }

    static final String START_CAPTURE = "Start capture";
    static final String STOP_CAPTURE = "Stop capture";
    static final String RESET_PEAK = "Reset peak";
    static final String MAGNITUDES = "Magnitudes";
    static final String SOURCE_LATEST = "Latest";
    static final String EMPTY_NOT_SUPPORTED = "Not available on this firmware";
    static final String EMPTY_NOT_CONNECTED = "Connect an ECU";
    static final String EMPTY_CHECKING = "Checking the ECU" + "\u2026";
    static final String EMPTY_STOPPED = "No knock spectrum yet";
    static final String EMPTY_WAITING = "Waiting for the first spectrum" + "\u2026";
    private static final String PLOTS_CARD = "plots";
    private static final String EMPTY_CARD = "empty";
    static final String SOURCE_AT_CURSOR = "At cursor";
    static final String VIEW_ALL = "All";
    static final String VIEW_SENSORS = "Sensors";
    static final String VIEW_CYLINDERS = "Cylinders";
    static final String PLOT_LIGHT = "Light";
    static final String PLOT_DARK = "Dark";
    /** Remembers the Light / Dark plot choice between sessions. */
    static final String DARK_PLOT_KEY = "knock_dark_plot";
    static final String STATUS_NOT_CONNECTED = "Not connected";
    static final String STATUS_CHECKING = "Checking ECU" + "…";
    static final String STATUS_NOT_SUPPORTED = "Not supported";
    static final String STATUS_CAPTURING = "Capturing";
    static final String STATUS_STOPPED = "Stopped";
    private static final String NOT_SUPPORTED_DETAIL =
        "The connected firmware has no knock spectrogram (needs a build with KNOCK_SPECTROGRAM, e.g. F7 boards)";
    private static final String CAPTURE_TOOLTIP =
        "Turns the ECU's knock spectrogram setting on or off; the change is not burned";

    private final UIContext uiContext;

    private final JPanel content = new JPanel(new BorderLayout());
    private final CanvasesPanel canvasesPanel = new CanvasesPanel();
    private final JButton buttonStartStop = new JButton(START_CAPTURE);
    private final JButton resetPeakButton = new JButton(RESET_PEAK);
    private final StatusChip statusChip = new StatusChip();

    private boolean started = false;
    private boolean connected = false;
    /** Whether the connected firmware streams the spectrum; null while not yet known. */
    private Boolean supported = null;
    private boolean flushed = false;

    private final int[] line_sum_index = {0};
    private int currentChannel = 0;
    private int currentCylinder = 0;

    private final float[] values = new float[COMPRESSED_SPECTRUM_PROTOCOL_SIZE * 4];

    private int cylindersCount = 0;

    private CanvasType canvasType = CanvasType.COMBINED;

    private final java.util.List<KnockCanvas> canvases = new CopyOnWriteArrayList<>();
    private final KnockMagnitudeCanvas magnitudes = new KnockMagnitudeCanvas();
    private boolean darkPlot = PersistentConfiguration.getConfig().getRoot().getBoolProperty(DARK_PLOT_KEY, false);
    /**
     * Whether the pointer is over a spectrogram: the magnitudes then show the slice under it,
     * otherwise the newest line as it arrives.
     */
    private boolean hovering;
    private final StatusChip magnitudeSource = new StatusChip();
    /**
     * Whether the current view's plots have received a line. Until then a message card replaces
     * them; afterwards they stay, so the last capture remains visible after Stop or a disconnect.
     */
    private boolean hasData;
    private final JPanel plotsOrMessage = new JPanel(new CardLayout());
    private final JLabel messageIcon = new JLabel();
    private final JLabel messageTitle = new JLabel();
    private final JLabel messageText = new JLabel();
    private final JButton messageStart = new JButton(START_CAPTURE);
    private final JLabel magnitudeHint = new JLabel();

    public KnockPane(UIContext uiContext) {
        this.uiContext = uiContext;

        buttonStartStop.setEnabled(false);
        buttonStartStop.setToolTipText(CAPTURE_TOOLTIP);
        buttonStartStop.setIconTextGap(8);
        buttonStartStop.addActionListener(e -> toggleStartStop());
        resetPeakButton.setEnabled(false);
        resetPeakButton.setToolTipText("Forget the highest amplitude seen so far and track it again");
        resetPeakButton.addActionListener(e -> resetPeak());
        content.add(createToolbar(), BorderLayout.NORTH);

        // horizontal scrolling is never needed: the panel always matches the viewport width
        JScrollPane canvasScroll = new JScrollPane(canvasesPanel,
                JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED,
                JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
        canvasScroll.setBorder(BorderFactory.createEmptyBorder());
        canvasScroll.getVerticalScrollBar().setUnitIncrement(16);

        JComponent mag = magnitudes.getComponent();
        magnitudes.setStyle(plotStyle(darkPlot));
        mag.setFont(UIManager.getFont("Label.font"));
        mag.setFocusable(true);
        mag.setFocusTraversalKeysEnabled(false);
        mag.setDoubleBuffered(true);
        mag.setPreferredSize(new Dimension(PREFERRED_WIDTH, MAG_HEIGHT));
        mag.setMinimumSize(new Dimension(100, 60));

        // [tag:console_theme] the plots sit on cards on the grey page, like the Log Viewer's charts
        JSplitPane split = new JSplitPane(JSplitPane.VERTICAL_SPLIT,
            card(null, canvasScroll), card(ToolbarStrip.header(MAGNITUDES, createMagnitudeSource(), null), mag));
        split.setResizeWeight(1.0);
        split.setDividerSize(12);
        split.setBorder(BorderFactory.createEmptyBorder());
        split.setOpaque(false);
        plotsOrMessage.setOpaque(false);
        plotsOrMessage.add(split, PLOTS_CARD);
        plotsOrMessage.add(createMessageCard(), EMPTY_CARD);
        JPanel page = new JPanel(new BorderLayout());
        page.setBorder(BorderFactory.createEmptyBorder(12, 12, 12, 12));
        page.add(plotsOrMessage, BorderLayout.CENTER);
        content.add(page, BorderLayout.CENTER);

        subscribe();
        createCanvas();
        updateStatus();

        ConnectionStatusLogic.INSTANCE.addAndFireListener(isConnected -> SwingUtilities.invokeLater(() -> {
            connected = isConnected;
            if (isConnected) {
                initFromEcu();
            } else {
                supported = null;
                setStartState(false);
                buttonStartStop.setEnabled(false);
                resetPeakButton.setEnabled(false);
                updateStatus();
            }
        }));
    }

    /** Capture and peak actions and the view on the left, the capture status on the right. */
    private JComponent createToolbar() {
        JToggleButton all = SegmentedControl.segment(VIEW_ALL);
        JToggleButton sensors = SegmentedControl.segment(VIEW_SENSORS);
        JToggleButton cylinders = SegmentedControl.segment(VIEW_CYLINDERS);
        all.setToolTipText("One spectrogram of every line the ECU sends");
        sensors.setToolTipText("One spectrogram per knock sensor input");
        cylinders.setToolTipText("One spectrogram per cylinder");
        ButtonGroup views = new ButtonGroup();
        views.add(all);
        views.add(sensors);
        views.add(cylinders);
        all.setSelected(true);
        all.addActionListener(e -> switchCanvasType(CanvasType.COMBINED));
        sensors.addActionListener(e -> switchCanvasType(CanvasType.CT_SENSORS));
        cylinders.addActionListener(e -> switchCanvasType(CanvasType.CT_CYLINDERS));

        JPanel left = new JPanel(new FlowLayout(FlowLayout.LEFT, JoinedButtons.ROW_GAP, 0));
        left.setOpaque(false);
        left.add(new JoinedButtons(buttonStartStop));
        left.add(new JoinedButtons(resetPeakButton));
        left.add(Box.createHorizontalStrut(JoinedButtons.ROW_GAP));
        JLabel viewLabel = new JLabel("View");
        viewLabel.setForeground(UIManager.getColor("Label.disabledForeground"));
        left.add(viewLabel);
        left.add(new SegmentedControl(all, sensors, cylinders));

        JToggleButton light = SegmentedControl.segment(PLOT_LIGHT);
        JToggleButton dark = SegmentedControl.segment(PLOT_DARK);
        light.setToolTipText("Plot area with a light background, like the Log Viewer's light plot");
        dark.setToolTipText("Plot area with a dark background, like the Log Viewer's dark plot");
        ButtonGroup plots = new ButtonGroup();
        plots.add(light);
        plots.add(dark);
        (darkPlot ? dark : light).setSelected(true);
        light.addActionListener(e -> setDarkPlot(false));
        dark.addActionListener(e -> setDarkPlot(true));
        left.add(Box.createHorizontalStrut(JoinedButtons.ROW_GAP));
        JLabel plotLabel = new JLabel("Plot");
        plotLabel.setForeground(UIManager.getColor("Label.disabledForeground"));
        left.add(plotLabel);
        left.add(new SegmentedControl(light, dark));

        // GridBagLayout centres the chip vertically in the toolbar
        JPanel right = new JPanel(new GridBagLayout());
        right.setOpaque(false);
        right.setBorder(BorderFactory.createEmptyBorder(0, 0, 0, JoinedButtons.ROW_GAP));
        right.add(statusChip);

        JPanel toolbar = new ToolbarStrip(new BorderLayout());
        toolbar.setBorder(BorderFactory.createEmptyBorder(8, 0, 9, JoinedButtons.ROW_END_INSET - JoinedButtons.ROW_GAP));
        toolbar.add(left, BorderLayout.CENTER);
        toolbar.add(right, BorderLayout.EAST);
        return toolbar;
    }

    /** "Latest" or "At cursor" and a one-line hint, next to the magnitudes title. */
    private JComponent createMagnitudeSource() {
        magnitudeHint.setForeground(UIManager.getColor("Label.disabledForeground"));
        JPanel source = new JPanel(new FlowLayout(FlowLayout.LEFT, 10, 0));
        source.setOpaque(false);
        source.add(magnitudeSource);
        source.add(magnitudeHint);
        showMagnitudeSource();
        return source;
    }

    private void showMagnitudeSource() {
        if (hovering) {
            magnitudeSource.show(SOURCE_AT_CURSOR, StatusChip.Tone.ACCENT);
            magnitudeHint.setText("The slice under the dashed line");
        } else {
            magnitudeSource.show(SOURCE_LATEST, StatusChip.Tone.GOOD);
            magnitudeHint.setText("Newest slice, updating live; hover a spectrogram to inspect an earlier one");
        }
    }

    private void setHovering(boolean hovering) {
        if (this.hovering != hovering) {
            this.hovering = hovering;
            showMagnitudeSource();
        }
    }

    /** A plot on a white card with a thin border, optionally under a titled header. */
    private static JPanel card(JComponent header, JComponent body) {
        JPanel card = new JPanel(new BorderLayout());
        Color background = UIManager.getColor("Console.Card.background");
        card.setBackground(background != null ? background : Color.WHITE);
        Color border = UIManager.getColor("Console.Card.borderColor");
        card.setBorder(BorderFactory.createLineBorder(border != null ? border : Color.LIGHT_GRAY));
        if (header != null) {
            card.add(header, BorderLayout.NORTH);
        }
        card.add(body, BorderLayout.CENTER);
        return card;
    }

    /** Icon, title, explanation and, when stopped, a Start capture button, centred on a card. */
    private JComponent createMessageCard() {
        Font base = UIManager.getFont("Label.font");
        if (base != null) {
            messageTitle.setFont(base.deriveFont(Font.BOLD, base.getSize2D() + 2));
        }
        messageText.setForeground(UIManager.getColor("Label.disabledForeground"));
        messageStart.putClientProperty(FlatClientProperties.STYLE_CLASS, "primary");
        messageStart.setPreferredSize(new Dimension(messageStart.getPreferredSize().width + 32, JoinedButtons.HEIGHT));
        messageStart.setToolTipText(CAPTURE_TOOLTIP);
        messageStart.addActionListener(e -> toggleStartStop());

        JPanel column = new JPanel(new GridBagLayout());
        column.setOpaque(false);
        GridBagConstraints c = new GridBagConstraints();
        c.gridx = 0;
        c.insets = new Insets(0, 0, 12, 0);
        column.add(messageIcon, c);
        c.insets = new Insets(0, 0, 6, 0);
        column.add(messageTitle, c);
        c.insets = new Insets(0, 0, 16, 0);
        column.add(messageText, c);
        c.insets = new Insets(0, 0, 0, 0);
        column.add(messageStart, c);
        return card(null, column);
    }

    /** Shows the plots once they have data, otherwise a message for the current state. */
    private void updateMessage() {
        boolean notSupported = Boolean.FALSE.equals(supported);
        if (hasData && !notSupported) {
            ((CardLayout) plotsOrMessage.getLayout()).show(plotsOrMessage, PLOTS_CARD);
            return;
        }
        boolean offerStart = false;
        Icon icon;
        String title;
        String text;
        if (notSupported) {
            icon = CommandIcon.INFO;
            title = EMPTY_NOT_SUPPORTED;
            text = NOT_SUPPORTED_DETAIL + ".";
        } else if (!connected) {
            icon = CommandIcon.PLUG;
            title = EMPTY_NOT_CONNECTED;
            text = "The knock spectrum streams live from a connected ECU. A capture stays visible after a disconnect.";
        } else if (supported == null) {
            icon = CommandIcon.TRIGGER;
            title = EMPTY_CHECKING;
            text = "Looking for the knock spectrogram in the connected firmware.";
        } else if (!started) {
            icon = CommandIcon.TRIGGER;
            title = EMPTY_STOPPED;
            text = "Start capture to stream the spectrum. It switches on the ECU's knock spectrogram setting; the change is not burned.";
            offerStart = true;
        } else {
            icon = CommandIcon.TRIGGER;
            title = EMPTY_WAITING;
            text = "Capture is on: the spectrogram appears with the first complete line from the ECU.";
        }
        messageIcon.setIcon(new ScaledIcon(icon, 2.5));
        messageTitle.setText(title);
        messageText.setText("<html><div style='text-align:center;width:380px'>" + text + "</div></html>");
        messageStart.setVisible(offerStart);
        // from the state, not the toolbar button: on connect the state changes before it is enabled
        messageStart.setEnabled(connected && Boolean.TRUE.equals(supported));
        ((CardLayout) plotsOrMessage.getLayout()).show(plotsOrMessage, EMPTY_CARD);
    }

    /** The title of the message shown instead of the plots, or null while the plots are shown. */
    String getMessageForTests() {
        for (Component card : plotsOrMessage.getComponents()) {
            if (card.isVisible()) {
                return card == plotsOrMessage.getComponent(0) ? null : messageTitle.getText();
            }
        }
        return null;
    }

    boolean isMessageStartOfferedForTests() {
        return messageStart.isVisible();
    }

    private void updateStatus() {
        if (!connected) {
            statusChip.show(STATUS_NOT_CONNECTED, StatusChip.Tone.BAD);
            statusChip.setToolTipText("Connect an ECU to stream the knock spectrum");
        } else if (supported == null) {
            statusChip.show(STATUS_CHECKING, StatusChip.Tone.NEUTRAL);
            statusChip.setToolTipText(null);
        } else if (!supported) {
            statusChip.show(STATUS_NOT_SUPPORTED, StatusChip.Tone.WARNING);
            statusChip.setToolTipText(NOT_SUPPORTED_DETAIL);
        } else if (started) {
            statusChip.show(STATUS_CAPTURING, StatusChip.Tone.GOOD);
            statusChip.setToolTipText("The ECU is streaming the knock spectrum");
        } else {
            statusChip.show(STATUS_STOPPED, StatusChip.Tone.NEUTRAL);
            statusChip.setToolTipText("Start capture to stream the knock spectrum");
        }
        updateMessage();
    }

    private void setDarkPlot(boolean dark) {
        darkPlot = dark;
        PersistentConfiguration.getConfig().getRoot().setBoolProperty(DARK_PLOT_KEY, dark);
        KnockPlotStyle style = plotStyle(dark);
        canvases.forEach(c -> c.setStyle(style));
        magnitudes.setStyle(style);
    }

    /**
     * [tag:console_theme] The Log Viewer's light or dark plot colours (Console.LogChart.*, the
     * same keys as logview.PlotPalette, with its values as fallbacks) in the Log Viewer layout.
     * Current, peak and cursor use the console's amber, red and accent so they read as states.
     */
    static KnockPlotStyle plotStyle(boolean dark) {
        String key = "Console.LogChart." + (dark ? "dark" : "light") + ".";
        Color accent = themed(key + "accent", dark ? 0x5bcdfa : 0x2563eb);
        Color peak = new Color(dark ? 0xf6839d : 0xdc2626);
        return KnockPlotStyle.DEFAULT.toBuilder()
            .background(themed(key + "background", dark ? 0x151b26 : 0xffffff))
            .grid(themed(key + "grid", dark ? 0x374352 : 0xe3e7ed))
            .axisText(themed(key + "label", dark ? 0xc6d1de : 0x586273))
            .title(themed(key + "hint", dark ? 0x788698 : 0x6b7485))
            .cursor(dark ? new Color(0xdc, 0xe6, 0xf5, 0x9b) : new Color(0x33, 0x41, 0x55, 0x99))
            .current(new Color(dark ? 0xffbf47 : 0xd97706))
            .peak(peak)
            .peakMarker(peak)
            .mouse(accent)
            .magnitudeFill(new Color(accent.getRed(), accent.getGreen(), accent.getBlue(), dark ? 0x30 : 0x29))
            .magnitudeLine(accent)
            .magnitudePeak(peak)
            .text(new Color(dark ? 0xe6edf5 : 0x111827))
            .chipBackground(new Color(dark ? 0x232c3b : 0xeef0f3))
            .logViewerLayout(true)
            .valueFontFamily(ConsoleFonts.mono(Font.PLAIN, ConsoleFonts.baseSize()).getFamily())
            .build();
    }

    private static Color themed(String key, int fallback) {
        Color c = UIManager.getColor(key);
        return c != null ? c : new Color(fallback);
    }

    private void resetPeak() {
        canvases.forEach(c -> {
            c.resetPeak();
            c.getComponent().repaint();
        });
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
            supported = false;
            updateStatus();
            return;
        }
        supported = true;

        IniField cylField = ini.getIniField("cylindersCount");
        if (cylField != null) {
            try {
                String val = ConfigurationImageGetterSetter.getStringValue(cylField, image);
                cylindersCount = (int) Double.parseDouble(val.trim());
            } catch (NumberFormatException ignored) {
            }
        }

        setStartState(readKnockEnabled(ini, image));
        buttonStartStop.setEnabled(true);
        resetPeakButton.setEnabled(true);
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

        // the line is cleared on the EDT below, so the magnitudes get their own copy
        final float[] latest = values.clone();
        SwingUtilities.invokeLater(() -> {
            canvases.forEach(c -> AutoupdateUtil.trueLayoutAndRepaint(c.getComponent()));
            if (!hovering) {
                magnitudes.processValues(latest);
            }
            if (!hasData) {
                hasData = true;
                updateMessage();
            }
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
                    addCanvas(1, 1, true, "Combined");
                    break;
                case CT_SENSORS:
                    addCanvas(1, 2, false, "Sensor 1");
                    addCanvas(2, 2, false, "Sensor 2");
                    break;
                case CT_CYLINDERS:
                    if (cylindersCount < 1) {
                        addCanvas(1, 1, false, "Cylinder 1");
                        break;
                    }
                    for (int i = 0; i < cylindersCount; i++)
                        addCanvas(i + 1, cylindersCount, false, "Cylinder " + (i + 1));
                    break;
                default:
                    throw new IllegalStateException("Unknown CanvasType");
            }
            canvasesPanel.revalidate();
            canvasesPanel.repaint();
        });
    }

    private void addCanvas(int number, int divider, boolean isCombined, String title) {
        KnockCanvas canvas = new KnockCanvas(number, divider, isCombined);
        canvas.setStyle(plotStyle(darkPlot));
        canvas.setTitle(title);
        JComponent comp = canvas.getComponent();
        comp.setFont(UIManager.getFont("Label.font"));
        comp.setFocusTraversalKeysEnabled(false);
        SpectrogramPointer pointer = new SpectrogramPointer(canvas);
        comp.addMouseMotionListener(pointer);
        comp.addMouseListener(pointer);
        comp.setFocusable(true);
        comp.setDoubleBuffered(true);
        // width is driven by the layout, only the height matters for the scroll pane
        comp.setPreferredSize(new Dimension(PREFERRED_WIDTH, MIN_CANVAS_HEIGHT));
        comp.setMinimumSize(new Dimension(0, MIN_CANVAS_HEIGHT));
        canvasesPanel.add(comp);
        canvases.add(canvas);
    }

    /** For sandbox/test use: bypass ECU init and start capturing immediately. */
    /** The spectrogram components, so tests can move the pointer over them. */
    java.util.List<JComponent> getSpectrogramComponentsForTests() {
        java.util.List<JComponent> components = new java.util.ArrayList<>();
        canvases.forEach(c -> components.add(c.getComponent()));
        return components;
    }

    /** The slice the magnitudes plot shows. */
    float[] getShownMagnitudesForTests() {
        return magnitudes.getMagnitudes();
    }

    public void simulateConnected(int cylinders) {
        cylindersCount = cylinders;
        connected = true;
        supported = true;
        buttonStartStop.setEnabled(true);
        resetPeakButton.setEnabled(true);
        setStartState(true);
        createCanvas();
    }

    public void setStartState(boolean enabled) {
        started = enabled;
        buttonStartStop.setText(started ? STOP_CAPTURE : START_CAPTURE);
        buttonStartStop.setIcon(started ? CommandIcon.STOP : CommandIcon.PLAY);
        line_sum_index[0] = 0;
        if (started)
            canvases.forEach(KnockCanvas::resetPeak);
        updateStatus();
    }

    // ---- Canvas-type switch (used by canvas-type buttons) --------------------

    /**
     * [tag:knock_view_switch_writes] A view only changes how the spectrum is drawn: rebuild the
     * canvases and drop the partly received line, but never touch the ECU. This used to toggle
     * capture off and on, which uploaded "enableKnockSpectrogram" twice per switch (from the same
     * image, so both uploads carried the same flipped value). See KnockPaneViewSwitchTest.
     */
    private void switchCanvasType(CanvasType type) {
        canvasType = type;
        line_sum_index[0] = 0;
        Arrays.fill(values, 0);
        // the new view's plots start empty: show "waiting" until their first line
        hasData = false;
        updateMessage();
        createCanvas();
    }

    // ---- Public API ----------------------------------------------------------

    public JPanel getContent() {
        return content;
    }

    // ---- Inner classes -------------------------------------------------------

    /**
     * Stretches the spectrogram stack to the full viewport width and, as long as the canvases fit,
     * to the full viewport height as well, so nothing is squeezed into a corner.
     */
    private static class CanvasesPanel extends JPanel implements Scrollable {
        CanvasesPanel() {
            super(new GridLayout(0, 1, 0, 4));
        }

        @Override
        public Dimension getPreferredScrollableViewportSize() {
            return getPreferredSize();
        }

        @Override
        public int getScrollableUnitIncrement(Rectangle visibleRect, int orientation, int direction) {
            return 16;
        }

        @Override
        public int getScrollableBlockIncrement(Rectangle visibleRect, int orientation, int direction) {
            return orientation == SwingConstants.VERTICAL ? visibleRect.height : visibleRect.width;
        }

        @Override
        public boolean getScrollableTracksViewportWidth() {
            return true;
        }

        @Override
        public boolean getScrollableTracksViewportHeight() {
            Container parent = getParent();
            return parent == null || getPreferredSize().height <= parent.getHeight();
        }
    }

    /** Draws a small line icon larger, e.g. a 16 px icon at 40 px on a message card. */
    private static final class ScaledIcon implements Icon {
        private final Icon icon;
        private final double scale;

        ScaledIcon(Icon icon, double scale) {
            this.icon = icon;
            this.scale = scale;
        }

        @Override
        public void paintIcon(Component c, Graphics g, int x, int y) {
            Graphics2D g2 = (Graphics2D) g.create();
            try {
                g2.translate(x, y);
                g2.scale(scale, scale);
                icon.paintIcon(c, g2, 0, 0);
            } finally {
                g2.dispose();
            }
        }

        @Override
        public int getIconWidth() {
            return (int) Math.round(icon.getIconWidth() * scale);
        }

        @Override
        public int getIconHeight() {
            return (int) Math.round(icon.getIconHeight() * scale);
        }
    }

    // ---- Inner listener classes ----------------------------------------------

    /**
     * Hovering a spectrogram shows the slice under the pointer in the magnitudes; leaving it
     * clears the cursor and returns the magnitudes to the newest line.
     */
    private class SpectrogramPointer extends MouseAdapter {
        private final KnockCanvas knockCanvas;

        SpectrogramPointer(KnockCanvas canvas) {
            knockCanvas = canvas;
        }

        @Override
        public void mouseMoved(MouseEvent e) {
            knockCanvas.setMousePosition(e.getX(), e.getY());
            setHovering(true);
            float[] mags = knockCanvas.getCurrentMouseMagnitudes();
            if (hasNonZeroValue(mags))
                magnitudes.processValues(mags);
        }

        @Override
        public void mouseExited(MouseEvent e) {
            knockCanvas.setMousePosition(-1, -1);
            setHovering(false);
        }

        private boolean hasNonZeroValue(float[] values) {
            for (float v : values)
                if (v != 0f) return true;
            return false;
        }
    }

}
