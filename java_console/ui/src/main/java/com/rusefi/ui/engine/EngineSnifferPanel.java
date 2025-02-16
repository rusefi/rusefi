package com.rusefi.ui.engine;

import com.devexperts.logging.Logging;
import com.rusefi.FileLog;
import com.rusefi.config.generated.Fields;
import com.rusefi.config.generated.Integration;
import com.rusefi.core.EngineState;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.ui.*;
import com.rusefi.ui.config.BitConfigField;
import com.rusefi.ui.config.ConfigUiField;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ui.util.URLLabel;
import com.rusefi.ui.util.UiUtils;
import com.rusefi.ui.widgets.AnyCommand;
import com.rusefi.waves.EngineChart;
import com.rusefi.waves.EngineChartParser;
import com.rusefi.waves.EngineReport;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
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
    public static final String HELP_TEXT = "Click here for online help";

    private final JPanel chartPanel = new JPanel(new BorderLayout());
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
    private final JPanel imagePanel = new JPanel(new GridLayout(1, 1)) {
        @Override
        public Dimension getPreferredSize() {
            Dimension d = chartPanel.getSize();
            Dimension s = super.getPreferredSize();
            return new Dimension((int) (d.width * zoomControl.getZoomProvider().getZoomValue()), s.height);
        }
    };

    private final ZoomControl zoomControl = new ZoomControl();
    private final EngineSnifferStatusPanel statusPanel = new EngineSnifferStatusPanel();
    private final UpDownImage crank = createImage(Integration.PROTOCOL_CRANK1);
    private final ChartScrollControl scrollControl;
    private AnyCommand command;

    private boolean isPaused;

    public EngineSnifferPanel(UIContext uiContext, Node config) {
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
                setPaused(pauseButton, false);
            }
        });

        JButton saveImageButton = UiUtils.createSaveImageButton();
        saveImageButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                saveImage();
            }
        });


        JPanel upperPanel = new JPanel(new FlowLayout(FlowLayout.LEFT, 5, 0));
        upperPanel.add(clearButton);
        upperPanel.add(saveImageButton);
        upperPanel.add(pauseButton);
        upperPanel.add(new RpmLabel(uiContext,2).getContent());

        if (!uiContext.getLinkManager().isLogViewer()) {
            command = AnyCommand.createField(uiContext, config, "chartsize " + EFI_DEFAULT_CHART_SIZE, true, true);
            upperPanel.add(command.getContent());
        }

        upperPanel.add(zoomControl);

        scrollControl = ChartRepository.getInstance().createControls(chart -> displayChart(chart));
        if (uiContext.getLinkManager().isLogViewer())
            upperPanel.add(scrollControl.getContent());

        upperPanel.add(new URLLabel(HELP_TEXT, HELP_URL));

        JPanel bottomPanel = new JPanel(new BorderLayout());

        if (!uiContext.getLinkManager().isLogViewer()) {
            JPanel lowerButtons = new JPanel(new FlowLayout(FlowLayout.RIGHT, 5, 0));
            lowerButtons.add(new ConfigUiField(uiContext, Fields.GLOBALTRIGGERANGLEOFFSET, "Trigger Offset").getContent());
            lowerButtons.add(new BitConfigField(uiContext, Fields.VERBOSETRIGGERSYNCHDETAILS, "Verbose trigger Sync").getContent());
            lowerButtons.add(new BitConfigField(uiContext, Fields.VERBOSEVVTDECODING, "Verbose VVT Sync").getContent());
            lowerButtons.add(new BitConfigField(uiContext, Fields.ENGINESNIFFERFOCUSONINPUTS, "Focus On Inputs").getContent());
            lowerButtons.add(new ConfigUiField(uiContext, Fields.ENGINECHARTSIZE, "Engine Sniffer size").getContent());
            lowerButtons.add(new ConfigUiField(uiContext, Fields.ENGINESNIFFERRPMTHRESHOLD, "RPM threshold").getContent());
            lowerButtons.add(new BitConfigField(uiContext, Fields.INVERTPRIMARYTRIGGERSIGNAL, "Invert Primary Input").getContent());
            bottomPanel.add(lowerButtons, BorderLayout.NORTH);
        }

        bottomPanel.add(statusPanel.infoPanel, BorderLayout.SOUTH);

        chartPanel.add(upperPanel, BorderLayout.NORTH);
        JScrollPane pane = new JScrollPane(imagePanel, JScrollPane.VERTICAL_SCROLLBAR_NEVER, JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
        chartPanel.add(pane, BorderLayout.CENTER);
        chartPanel.add(bottomPanel, BorderLayout.SOUTH);

        zoomControl.listener = () -> {
            System.out.println("onZoomChange");
            /**
             * We have scroll pane size which depends on zoom, that's a long chain of dependencies
             */
            AutoupdateUtil.trueLayout(imagePanel.getParent());
        };

        resetImagePanel();

        uiContext.getLinkManager().getEngineState().registerStringValueAction(Integration.PROTOCOL_ENGINE_SNIFFER, new EngineState.ValueCallback<String>() {
            @Override
            public void onUpdate(String value) {
                if (isPaused)
                    return;
                displayChart(value);
            }
        });

        mainPanel.add(chartPanel, BorderLayout.CENTER);
        mainPanel.add(new WarningPanel(config).getPanel(config), BorderLayout.SOUTH);
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
            }
        });
    }

    private void resetImagePanel() {
        imagePanel.removeAll();
        imagePanel.add(crank);
        images.put(Integration.PROTOCOL_CRANK1, crank);
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
        }

        // Repaint now that we've updated state
        SwingUtilities.invokeLater(() -> AutoupdateUtil.trueLayout(imagePanel));
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
        imagePanel.setLayout(new GridLayout(images.size(), 1));
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
        double maf = SensorCentral.getInstance().getValue(Sensor.MAF);
        String fileName = FileLog.getDate() + "rpm_" + rpm + "_maf_" + maf + ".png";

        UiUtils.saveImageWithPrompt(fileName, mainPanel, imagePanel);
    }

    private UpDownImage createImage(final String name) {
        Color signalBody = Color.lightGray;
        Color signalBorder = Color.blue;

        if (name.startsWith("tach") || name.startsWith("dizzy")) {
            signalBody = Color.yellow;
        } else if (name.startsWith("t")) {
            // trigger
        } else if (name.startsWith("r")) {
            // trailing coil
            signalBody = new Color(0xffa400); // golden yellow
        } else if (name.startsWith(PROTOCOL_COIL_SHORT_PREFIX)) {
            // coil
            signalBody = Color.darkGray;
        } else if (name.startsWith(PROTOCOL_INJ_SHORT_PREFIX) || name.startsWith(PROTOCOL_INJ_STAGE2_SHORT_PREFIX)) {
            // injection
            signalBody = Color.green;
        } else if (name.startsWith("map")) {
            signalBody = Color.pink;
        } else {
            signalBody = Color.gray;
        }

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
        image.setSignalBody(signalBody);
        image.setSignalBorder(signalBorder);
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
