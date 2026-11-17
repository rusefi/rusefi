package com.rusefi.ui.engine;

import com.devexperts.logging.FileLogger;
import com.devexperts.logging.Logging;
import com.rusefi.config.generated.Integration;
import com.rusefi.core.EngineState;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.ui.*;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ui.util.URLLabel;
import com.rusefi.ui.basic.UiHelper;
import com.rusefi.ui.util.UiUtils;
import com.rusefi.ui.widgets.AnyCommand;
import com.rusefi.ui.widgets.CommandIcon;
import com.rusefi.ui.widgets.JoinedButtons;
import com.rusefi.ui.widgets.ScaledIcon;
import com.rusefi.ui.widgets.SegmentedControl;
import com.rusefi.ui.widgets.ToolbarStrip;
import com.rusefi.waves.EngineChart;
import com.rusefi.waves.EngineChartParser;
import com.rusefi.waves.EngineReport;

import javax.imageio.ImageIO;
import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.*;
import java.util.List;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.config.generated.Integration.*;

/**
 * Engine Sniffer control consists of a set of {@link UpDownImage}
 * <p/>
 * <p/>
 * Date: 6/23/13
 * Andrey Belomutskiy, (c) 2013-2020
 *
 * @see EngineSnifferStatusPanel status bar
 * @see com.rusefi.ui.test.WavePanelSandbox
 */
public class EngineSnifferPanel {
    private static final Logging log = getLogging(EngineSnifferPanel.class);
    private static final int EFI_DEFAULT_CHART_SIZE = 180;
    public static final Comparator<String> INSTANCE = new ImageOrderComparator();
    private static final String HELP_URL = "http://rusefi.com/wiki/index.php?title=Manual:DevConsole#Digital_Chart";
    public static final String HELP_TEXT = "Engine Sniffer help";
    static final String EMPTY_TITLE = "Waiting for engine sniffer data";
    private static final String EMPTY_DETAIL = "The chart appears with the first capture that contains signal edges. "
        + "The ECU records captures only while RPM is below the Engine Sniffer Threshold setting.";
    static final String PLOT_LIGHT = "Light";
    static final String PLOT_DARK = "Dark";
    static final String DARK_PLOT_KEY = "dark_plot";
    private static final String CHART_CARD = "chart";
    private static final String EMPTY_CARD = "empty";

    private final JPanel chartPanel = new JPanel(new BorderLayout());
    /** The scrolling chart, or the empty-state card until a capture has events */
    private final JPanel chartOrEmpty = new JPanel(new CardLayout());
    /**
     * chartPanel in the center and warning panel on the bottom
     */
    private final JPanel mainPanel = new JPanel(new BorderLayout());

    /**
     * imageName -> UpDownImage
     */
    private final TreeMap<String, UpDownImage> images = new TreeMap<>(INSTANCE);
    /**
     * this is the panel which displays all {@link UpDownImage} using {@link GridLayout}
     */
    private final JPanel imagePanel = new JPanel(rowsLayout(1)) {
        @Override
        public Dimension getPreferredSize() {
            // the visible chart width (beside the channel column) times the zoom
            int width = chartScroll == null ? chartPanel.getWidth() : chartScroll.getViewport().getWidth();
            Dimension s = super.getPreferredSize();
            return new Dimension((int) (width * zoomControl.getZoomProvider().getZoomValue()), s.height);
        }
    };
    /**
     * Channel names, pins and event counts, one {@link SnifferRowLabel} per row of {@link #imagePanel} in
     * the same order; the scroll pane's row header, so it stays put while a zoomed chart scrolls sideways
     */
    private final JPanel labelPanel = new JPanel(rowsLayout(1));
    private final Map<String, SnifferRowLabel> labels = new HashMap<>();
    private JScrollPane chartScroll;

    private final ZoomControl zoomControl = new ZoomControl();
    private final EngineSnifferStatusPanel statusPanel = new EngineSnifferStatusPanel();
    /** Light until the saved choice is applied at the end of the constructor */
    private SnifferPalette palette = SnifferPalette.LIGHT;
    private final UpDownImage crank = createImage(Integration.PROTOCOL_CRANK1);
    private final ChartScrollControl scrollControl;
    private AnyCommand command;

    private boolean isPaused;
    private final Node config;

    public EngineSnifferPanel(UIContext uiContext, Node config) {
        this.config = config;
        statusPanel.setTimeAxisTranslator(crank.createTranslator());

        final JButton pauseButton = UiUtils.createPauseButton();
        pauseButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                setPaused(pauseButton, !isPaused);
            }
        });

        JButton clearButton = UiUtils.createClearButton();
        clearButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                for (UpDownImage image : images.values())
                    image.setWaveReport(EngineReport.MOCK, null);
                for (SnifferRowLabel label : labels.values())
                    label.setEvents(0);
                setPaused(pauseButton, false);
                showChart(false);
            }
        });

        JButton saveImageButton = UiUtils.createSaveImageButton();
        saveImageButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                saveImage();
            }
        });


        // toolbar like the Messages tab [tag:console_theme]: RPM, command box, chart actions, zoom
        JPanel actions = new JPanel(new FlowLayout(FlowLayout.LEFT, JoinedButtons.ROW_GAP, 0));
        actions.setOpaque(false);
        actions.add(new RpmLabel(uiContext, 2).getContent());

        if (!uiContext.getLinkManager().isLogViewer()) {
            command = AnyCommand.createField(uiContext, config, "chartsize " + EFI_DEFAULT_CHART_SIZE, true, true);
            actions.add(command.getContent());
        }

        actions.add(new JoinedButtons(clearButton, saveImageButton, pauseButton));
        actions.add(zoomControl);

        // plot colours independent of the console theme, like the Knock Analyzer and Log Viewer plots
        JToggleButton light = SegmentedControl.segment(PLOT_LIGHT);
        JToggleButton dark = SegmentedControl.segment(PLOT_DARK);
        light.setToolTipText("Chart with a light background");
        dark.setToolTipText("Chart with a dark background");
        ButtonGroup plots = new ButtonGroup();
        plots.add(light);
        plots.add(dark);
        boolean darkPlot = config.getBoolProperty(UiHelper.plotThemeKey(DARK_PLOT_KEY), UiHelper.isDarkTheme());
        (darkPlot ? dark : light).setSelected(true);
        light.addActionListener(e -> setDarkPlot(false));
        dark.addActionListener(e -> setDarkPlot(true));
        actions.add(Box.createHorizontalStrut(JoinedButtons.ROW_GAP));
        JLabel plotLabel = new JLabel("Plot");
        plotLabel.setForeground(UIManager.getColor("Label.disabledForeground"));
        actions.add(plotLabel);
        actions.add(new SegmentedControl(light, dark));

        scrollControl = ChartRepository.getInstance().createControls(chart -> displayChart(chart));
        if (uiContext.getLinkManager().isLogViewer())
            actions.add(scrollControl.getContent());

        // GridBagLayout centres the link vertically in the toolbar
        JPanel help = new JPanel(new GridBagLayout());
        help.setOpaque(false);
        help.setBorder(BorderFactory.createEmptyBorder(0, 0, 0, JoinedButtons.ROW_GAP));
        help.add(new URLLabel(HELP_TEXT, HELP_URL));
        JPanel upperPanel = new ToolbarStrip(new BorderLayout());
        upperPanel.setBorder(BorderFactory.createEmptyBorder(8, 0, 9, JoinedButtons.ROW_END_INSET - JoinedButtons.ROW_GAP));
        upperPanel.add(actions, BorderLayout.CENTER);
        upperPanel.add(help, BorderLayout.EAST);

        chartPanel.add(upperPanel, BorderLayout.NORTH);
        // chart on a white card [tag:console_theme]; 1 px gaps between rows show the divider colour
        labelPanel.setBackground(themed("Separator.foreground", Color.LIGHT_GRAY));
        chartScroll = new JScrollPane(imagePanel, JScrollPane.VERTICAL_SCROLLBAR_NEVER, JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
        chartScroll.setBorder(BorderFactory.createEmptyBorder());
        chartScroll.setRowHeaderView(labelPanel);
        JPanel corner = new JPanel();
        corner.setBackground(cardBackground());
        chartScroll.setCorner(ScrollPaneConstants.LOWER_LEFT_CORNER, corner);
        JPanel chartCard = new JPanel(new BorderLayout());
        chartCard.setBorder(BorderFactory.createLineBorder(themed("Console.Card.borderColor", Color.LIGHT_GRAY)));
        chartCard.add(chartScroll, BorderLayout.CENTER);
        JPanel chartPage = new JPanel(new BorderLayout());
        chartPage.setBorder(BorderFactory.createEmptyBorder(12, 12, 12, 12));
        chartPage.add(chartCard, BorderLayout.CENTER);
        chartOrEmpty.add(chartPage, CHART_CARD);
        chartOrEmpty.add(createEmptyCard(), EMPTY_CARD);
        showChart(false);
        chartPanel.add(chartOrEmpty, BorderLayout.CENTER);
        chartPanel.add(statusPanel.infoPanel, BorderLayout.SOUTH);

        zoomControl.listener = () -> {
            System.out.println("onZoomChange");
            /**
             * We have scroll pane size which depends on zoom, that's a long chain of dependencies
             */
            AutoupdateUtil.trueLayoutAndRepaint(imagePanel.getParent());
        };

        resetImagePanel();
        setDarkPlot(darkPlot);

        uiContext.getLinkManager().getEngineState().registerStringValueAction(Integration.PROTOCOL_ENGINE_SNIFFER, new EngineState.ValueCallback<String>() {
            @Override
            public void onUpdate(String value) {
                if (isPaused)
                    return;
                displayChart(value);
            }
        });

        mainPanel.add(chartPanel, BorderLayout.CENTER);
    }

    private void setPaused(JButton pauseButton, boolean isPaused) {
        this.isPaused = isPaused;
        UiUtils.setPauseButtonText(pauseButton, this.isPaused);
    }

    public void setOutpinListener(EngineState engineState) {
        engineState.registerStringValueAction(Integration.PROTOCOL_OUTPIN, new EngineState.ValueCallback<String>() {
            @Override
            public void onUpdate(String value) {
                String[] pinInfo = value.split("@");
                if (pinInfo.length != 2)
                    return;
                String channel = pinInfo[0];
                String pin = pinInfo[1];
                UpDownImage image = images.get(channel);
                ChannelNaming.INSTANCE.channelName2PhysicalPin.put(channel, pin);
                if (image != null)
                    image.setPhysicalPin(pin);
                SnifferRowLabel label = labels.get(channel);
                if (label != null)
                    label.setPin(pin);
            }
        });
    }

    private void resetImagePanel() {
        imagePanel.removeAll();
        labelPanel.removeAll();
        imagePanel.add(crank);
        images.put(Integration.PROTOCOL_CRANK1, crank);
        labelPanel.add(createLabel(Integration.PROTOCOL_CRANK1));
    }

    public void displayChart(String value) {
        EngineChart map = EngineChartParser.unpackToMap(value);

        StringBuilder revolutions = map.get(TOP_DEAD_CENTER_MESSAGE);

        statusPanel.setRevolutions(revolutions);

        // Create images for any new keys
        for (String imageName : map.getMap().keySet()) {
            createSecondaryImage(imageName);
        }

        // Update existing images
        int events = 0;
        for (String imageName : images.keySet()) {
            UpDownImage image = images.get(imageName);
            if (image == null)
                throw new IllegalStateException("image not found for " + imageName);

            StringBuilder sb = map.getMap().get(imageName);
            String report = sb == null ? "" : sb.toString();

            image.setRevolutions(revolutions);
            List<EngineReport.UpDown> list = EngineReport.parse(report);
            EngineReport wr = new EngineReport(list);
            image.setWaveReport(wr, revolutions);
            events += list.size();
            SnifferRowLabel label = labels.get(imageName);
            if (label != null)
                label.setEvents(list.size());
        }

        boolean hasEvents = events > 0;
        // Repaint now that we've updated state
        SwingUtilities.invokeLater(() -> {
            showChart(hasEvents);
            AutoupdateUtil.trueLayoutAndRepaint(imagePanel);
        });
    }

    /** The chart once a capture has signal edges, otherwise a message saying what the chart is waiting for. */
    private void showChart(boolean show) {
        ((CardLayout) chartOrEmpty.getLayout()).show(chartOrEmpty, show ? CHART_CARD : EMPTY_CARD);
    }

    /** True while the waiting message is shown instead of the chart. */
    boolean isEmptyStateShownForTests() {
        return !chartOrEmpty.getComponent(0).isVisible();
    }

    /** Icon, title and explanation centred on a card, like the Knock Analyzer's empty state. */
    private static JComponent createEmptyCard() {
        JLabel icon = new JLabel(new ScaledIcon(CommandIcon.TRIGGER, 2.5));
        JLabel title = new JLabel(EMPTY_TITLE);
        Font base = UIManager.getFont("Label.font");
        if (base != null) {
            title.setFont(base.deriveFont(Font.BOLD, base.getSize2D() + 2));
        }
        JLabel text = new JLabel("<html><div style='text-align:center;width:380px'>" + EMPTY_DETAIL + "</div></html>");
        text.setForeground(UIManager.getColor("Label.disabledForeground"));

        JPanel card = new JPanel(new GridBagLayout());
        Color background = UIManager.getColor("Console.Card.background");
        card.setBackground(background != null ? background : Color.WHITE);
        Color border = UIManager.getColor("Console.Card.borderColor");
        card.setBorder(BorderFactory.createCompoundBorder(
            BorderFactory.createLineBorder(border != null ? border : Color.LIGHT_GRAY),
            BorderFactory.createEmptyBorder(28, 32, 28, 32)));
        GridBagConstraints c = new GridBagConstraints();
        c.gridx = 0;
        c.insets = new Insets(0, 0, 12, 0);
        card.add(icon, c);
        c.insets = new Insets(0, 0, 6, 0);
        card.add(title, c);
        c.insets = new Insets(0, 0, 0, 0);
        card.add(text, c);

        JPanel page = new JPanel(new GridBagLayout());
        page.setBorder(BorderFactory.createEmptyBorder(12, 12, 12, 12));
        page.add(card);
        return page;
    }

    public JPanel getPanel() {
        return mainPanel;
    }

    private void createSecondaryImage(String name) {
        if (images.containsKey(name)) {
            // already created, skip
            return;
        }

        // Don't render a row for the TDC mark
        if (Integration.TOP_DEAD_CENTER_MESSAGE.equalsIgnoreCase(name)) {
            return;
        }

        int index = getInsertIndex(name, images.keySet());

        log.info("Engine sniffer register channel " + name + " at idx " + index);

        UpDownImage image = createImage(name);
        images.put(name, image);
        image.setTranslator(crank.createTranslator());
        imagePanel.add(image, index);
        imagePanel.setLayout(rowsLayout(images.size()));
        labelPanel.add(createLabel(name), index);
        labelPanel.setLayout(rowsLayout(images.size()));
    }

    public static int getInsertIndex(String name, Set<String> strings) {
        String[] mapKeys = new String[strings.size()];
        int pos = 0;
        for (String key : strings)
            mapKeys[pos++] = key;

//        int index = -Arrays.binarySearch(mapKeys, name) - 1;
        int index = -Arrays.binarySearch(mapKeys, 0, mapKeys.length, name, INSTANCE) - 1;
        if (index >= strings.size())
            index = -1;
        return index;
    }

    private void saveImage() {
        int rpm = RpmModel.getInstance().getValue();
        double maf = 0;
        String fileName = FileLogger.getDate() + "rpm_" + rpm + "_maf_" + maf + ".png";

        JFileChooser fc = UiUtils.getFileChooser(new FileNameExtensionFilter("PNG files", "png"));
        fc.setSelectedFile(new File(fileName));
        if (fc.showSaveDialog(mainPanel) != JFileChooser.APPROVE_OPTION)
            return;
        try {
            ImageIO.write(renderChartImage(), "png", fc.getSelectedFile());
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }
    }

    /** The channel column and the whole chart (also the part scrolled out of view) side by side. */
    BufferedImage renderChartImage() {
        int labelWidth = labelPanel.getWidth();
        BufferedImage image = new BufferedImage(Math.max(1, labelWidth + imagePanel.getWidth()),
            Math.max(1, imagePanel.getHeight()), BufferedImage.TYPE_INT_RGB);
        Graphics2D g = image.createGraphics();
        try {
            labelPanel.paint(g);
            g.translate(labelWidth, 0);
            imagePanel.paint(g);
        } finally {
            g.dispose();
        }
        return image;
    }

    private SnifferRowLabel createLabel(String name) {
        SnifferRowLabel label = new SnifferRowLabel(name, ChannelNaming.INSTANCE.channelName2PhysicalPin.get(name));
        label.setBackground(cardBackground());
        labels.put(name, label);
        return label;
    }

    /** One column of rows with a 1 px gap between them, painted in the panel's divider colour. */
    private static GridLayout rowsLayout(int rows) {
        return new GridLayout(rows, 1, 0, 1);
    }

    private static Color cardBackground() {
        return themed("Console.Card.background", Color.WHITE);
    }

    /** Pulse colours by channel family, background and line colours of the current light or dark plot. */
    private void applyPalette(String name, UpDownImage image) {
        SnifferPalette.Role role = SnifferPalette.Role.of(name);
        image.setSignalBody(palette.fill(role));
        image.setSignalBorder(palette.outline(role));
        image.setBackground(palette.background);
        image.setStyle(new UpDownImage.Style(palette.text, palette.mutedText, palette.durationText,
            palette.timeScale, palette.engineCycle, true));
    }

    private void setDarkPlot(boolean dark) {
        palette = SnifferPalette.of(dark);
        config.setBoolProperty(UiHelper.plotThemeKey(DARK_PLOT_KEY), dark);
        images.forEach(this::applyPalette);
        imagePanel.setBackground(palette.divider);
        statusPanel.setLegendColors(palette.engineCycle, palette.timeScale);
        imagePanel.repaint();
    }

    boolean isDarkPlotForTests() {
        return palette.dark;
    }

    private static Color themed(String key, Color fallback) {
        Color color = UIManager.getColor(key);
        return color != null ? color : fallback;
    }

    private UpDownImage createImage(final String name) {
        UpDownImage image = new UpDownImage(name, new UpDownImage.PinNameSource() {
            @Override
            public String getPinByName(String name) {
                return ChannelNaming.INSTANCE.channelName2PhysicalPin.get(name);
            }

            @Override
            public boolean isSimulationMode() {
                return false;
            }
        }) {
            @Override
            protected boolean isShowTdcLabel() {
                /**
                 * TDC label is only displayed on the bottom UpDown image
                 */
                return name.equals(images.lastKey());
            }
        };
        applyPalette(name, image);
        // the channel column shows the name and event count
        image.setRenderName(false);
        image.addMouseMotionListener(statusPanel.motionAdapter);
        return image;
    }

    public void reloadFile() {
        displayChart(ChartRepository.getInstance().getChart(0));
        scrollControl.reset();
    }

    public ActionListener getTabSelectedListener() {
        return new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (command != null)
                    command.requestFocus();
            }
        };
    }

    /**
     * The job of this comparator is to place Spark charts before Injection charts
     */
    static class ImageOrderComparator implements Comparator<String> {
        @Override
        public int compare(String o1, String o2) {
            return fixNameForNicerOrder(o1).compareTo(fixNameForNicerOrder(o2));
        }

        String fixNameForNicerOrder(String s) {
            if (s.toLowerCase().startsWith("t"))
                return "a" + s; // let's place this at the top
            if (s.toLowerCase().startsWith("hip"))
                return "z" + s; // let's place this at the bottom
            if (s.toLowerCase().startsWith("spa"))
                return "d" + s;
            return s;
        }
    }
}
