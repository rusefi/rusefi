package com.rusefi.ui;

import com.devexperts.logging.Logging;
import com.rusefi.io.can.HexUtil;
import com.rusefi.io.can.slcan.SlcanClient;
import com.opensr5.ini.DialogModel;
import com.opensr5.ini.IniFileModel;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.ScalarIniField;

import javax.swing.*;
import javax.swing.Timer;
import java.awt.*;
import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.text.SimpleDateFormat;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.function.Consumer;

import static com.devexperts.logging.Logging.getLogging;

/**
 * CAN sniffer tab around {@link SlcanClient}: scans serial ports for the rusEFI CAN sniffer
 * SLCAN channel (skipping the TunerStudio console port) and keeps reconnecting.
 * <p>
 * Top-right square shows connection state: green = SLCAN port located and connected,
 * red = scanning/disconnected. Most of the area shows the last {@link #VISIBLE_FRAMES}
 * frames, newest first. 'Record' collects frames into an unlimited internal buffer showing
 * recording duration and buffer size, 'Stop' ends recording and offers a 'Save file as'
 * dialog, 'Reset' clears the buffer.
 * <p>
 * Shown in ConsoleUI behind the 'show_slcan_sniffer' flag; construct lazily (the reader
 * thread starts scanning serial ports as soon as the tab is instantiated).
 *
 * @see com.rusefi.UiProperties#isSlcanSnifferEnabled()
 */
public class SlcanTab {
    private static final Logging log = getLogging(SlcanTab.class);

    private static final int VISIBLE_FRAMES = 30;
    private static final int READ_TIMEOUT_MS = 1000;
    // if the bus is silent for this long we poke the adapter to detect an unplugged port
    private static final int LIVENESS_POLL_PERIOD_MS = 3000;
    private static final int RESCAN_PERIOD_MS = 2000;

    private final SimpleDateFormat timeFormat = new SimpleDateFormat("HH:mm:ss.SSS");

    private final Object lock = new Object();
    private final List<FrameRecord> buffer = new ArrayList<>(); // unlimited recording buffer
    private final ArrayDeque<FrameRecord> lastFrames = new ArrayDeque<>(); // newest first

    private volatile boolean recording;
    private volatile long recordStartMs;
    private volatile String connectedPort; // null while disconnected

    private final JPanel content = new JPanel(new BorderLayout());
    private final JPanel indicator = new JPanel();
    private final JLabel statusLabel = new JLabel();
    private final JTextArea framesArea = new JTextArea();
    private final JButton resetButton = new JButton("Reset");
    private final JButton recordButton = new JButton("Record");
    private final JButton stopButton = new JButton("Stop");
    private final Consumer<String> messageHandler;
    private final UIContext uiContext;

    public SlcanTab(UIContext uiContext) {
        this(uiContext, null);
    }

    public SlcanTab(UIContext uiContext, Consumer<String> messageHandler) {
        this.uiContext = uiContext;
        this.messageHandler = messageHandler != null
            ? messageHandler
            : message -> JOptionPane.showMessageDialog(content, message, "SLCAN", JOptionPane.INFORMATION_MESSAGE);
        JPanel buttons = new JPanel(new FlowLayout(FlowLayout.LEFT));
        buttons.add(resetButton);
        buttons.add(recordButton);
        buttons.add(stopButton);
        stopButton.setEnabled(false);

        indicator.setPreferredSize(new Dimension(22, 22));
        indicator.setBackground(Color.RED);
        indicator.setBorder(BorderFactory.createLineBorder(Color.DARK_GRAY));
        JPanel indicatorWrapper = new JPanel(new FlowLayout(FlowLayout.RIGHT));
        indicatorWrapper.setOpaque(false);
        indicatorWrapper.add(indicator);

        JPanel top = new JPanel(new BorderLayout());
        top.add(buttons, BorderLayout.WEST);
        top.add(statusLabel, BorderLayout.CENTER);
        top.add(indicatorWrapper, BorderLayout.EAST);

        JPanel busPanel = createBusPanel();
        if (busPanel != null) {
            top.add(busPanel, BorderLayout.SOUTH);
        }

        framesArea.setEditable(false);
        framesArea.setFont(new Font(Font.MONOSPACED, Font.PLAIN, 13));

        content.add(top, BorderLayout.NORTH);
        content.add(new JScrollPane(framesArea), BorderLayout.CENTER);

        resetButton.addActionListener(e -> {
            synchronized (lock) {
                buffer.clear();
            }
            recordStartMs = System.currentTimeMillis();
        });
        recordButton.addActionListener(e -> {
            recordStartMs = System.currentTimeMillis();
            recording = true;
            recordButton.setEnabled(false);
            stopButton.setEnabled(true);
        });
        stopButton.addActionListener(e -> {
            recording = false;
            recordButton.setEnabled(true);
            stopButton.setEnabled(false);
            saveBuffer();
        });

        new Timer(250, e -> refresh()).start();

        Thread reader = new Thread(this::readerLoop, "SLCAN tab reader");
        reader.setDaemon(true);
        reader.start();
    }

    public JPanel getContent() {
        return content;
    }

    private void readerLoop() {
        while (true) {
            SlcanClient client = SlcanClient.findAndConnect(log::info);
            if (client == null) {
                try {
                    Thread.sleep(RESCAN_PERIOD_MS);
                } catch (InterruptedException e) {
                    return;
                }
                continue;
            }
            connectedPort = client.getPort();
            try {
                long lastActivity = System.currentTimeMillis();
                while (true) {
                    String line = client.readLine(READ_TIMEOUT_MS);
                    if (line == null) {
                        if (System.currentTimeMillis() - lastActivity > LIVENESS_POLL_PERIOD_MS) {
                            // frames-read timeout happens on a silent bus too, so a write is the
                            // real liveness signal: it throws if the port is gone. The Fxx response
                            // comes back as a non-frame line below and gets ignored.
                            client.pollStatus();
                            lastActivity = System.currentTimeMillis();
                        }
                        continue;
                    }
                    lastActivity = System.currentTimeMillis();
                    SlcanClient.Frame frame = SlcanClient.Frame.parse(line);
                    if (frame == null) {
                        continue;
                    }
                    onFrame(new FrameRecord(System.currentTimeMillis(), frame));
                }
            } catch (IOException e) {
                log.info(client.getPort() + ": disconnected: " + e);
            } finally {
                connectedPort = null;
                client.close();
            }
        }
    }

    private void onFrame(FrameRecord record) {
        synchronized (lock) {
            lastFrames.addFirst(record);
            while (lastFrames.size() > VISIBLE_FRAMES) {
                lastFrames.removeLast();
            }
            if (recording) {
                buffer.add(record);
            }
        }
    }

    /**
     * Swing timer tick: connection indicator, status line, last-frames view.
     */
    private void refresh() {
        String port = connectedPort;
        indicator.setBackground(port != null ? new Color(0, 160, 0) : Color.RED);
        indicator.setToolTipText(port != null ? "Connected to " + port : "No SLCAN port");

        int bufferSize;
        StringBuilder text = new StringBuilder();
        synchronized (lock) {
            bufferSize = buffer.size();
            for (FrameRecord record : lastFrames) {
                text.append(String.format("%s %s%n",
                    timeFormat.format(new Date(record.wallClockMs)), record.frame.decode()));
            }
        }

        String status = port != null ? "Connected to " + port : "Scanning for SLCAN port...";
        if (recording) {
            long elapsed = (System.currentTimeMillis() - recordStartMs) / 1000;
            status += String.format(" | RECORDING %d:%02d", elapsed / 60, elapsed % 60);
        }
        status += " | " + bufferSize + " frame(s) in buffer";
        statusLabel.setText(status);

        String newText = text.toString();
        if (!newText.equals(framesArea.getText())) {
            framesArea.setText(newText);
            framesArea.setCaretPosition(0);
        }
    }

    private JPanel createBusPanel() {
        if (uiContext == null) {
            return null;
        }
        IniFileModel ini = uiContext.iniFileState.getIniFileModel();
        if (ini == null) {
            return null;
        }

        JPanel busPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        busPanel.add(new JLabel("Listen:"));

        for (int i = 1; i <= 3; i++) {
            String dialogKey = "canHw" + i;
            DialogModel dialog = ini.getDialogs().get(dialogKey);
            if (dialog == null) {
                continue;
            }

            String busName = dialog.getUiName();
            String fieldName1 = "canSniffer" + i + "_listenOurs";
            String fieldName2 = "canSniffer" + i + "_read";

            // Verify the fields exist
            try {
                ini.getIniField(fieldName1);
                ini.getIniField(fieldName2);
            } catch (Exception e) {
                // Skip if not found
                continue;
            }

            JCheckBox cb = new JCheckBox(busName);

            uiContext.addConfigImageListener(image -> {
                updateCheckbox(cb, fieldName1, fieldName2, ini, image);
            });

            // Initial state
            BinaryProtocol bp = uiContext.getBinaryProtocol();
            if (bp != null) {
                ConfigurationImage image = bp.getBinaryProtocolState().getConfigurationImage();
                if (image != null) {
                    updateCheckbox(cb, fieldName1, fieldName2, ini, image);
                }
            }

            cb.addActionListener(e -> {
                BinaryProtocol bp2 = uiContext.getBinaryProtocol();
                if (bp2 == null) return;
                ConfigurationImage image = bp2.getBinaryProtocolState().getConfigurationImage();
                if (image == null) return;

                int newValue = cb.isSelected() ? 1 : 0;
                applyValue(image, ini.getIniField(fieldName1), newValue);
                applyValue(image, ini.getIniField(fieldName2), newValue);

                ConfigurationImage snapshot = image.clone();
                uiContext.getLinkManager().submit(() -> bp2.uploadChangesWithoutBurn(snapshot));
                uiContext.fireConfigImageChanged(image);
            });

            busPanel.add(cb);
        }

        return busPanel;
    }

    private void updateCheckbox(JCheckBox cb, String field1, String field2, IniFileModel ini, ConfigurationImage image) {
        Double v1 = image.readNumericValue(ini.getIniField(field1));
        Double v2 = image.readNumericValue(ini.getIniField(field2));
        if (v1 == null || v2 == null) return;

        boolean b1 = v1 != 0;
        boolean b2 = v2 != 0;

        if (b1 == b2) {
            cb.setSelected(b1);
            cb.setBackground(null);
        } else {
            cb.setSelected(true); // show as checked but grayed
            cb.setBackground(java.awt.Color.LIGHT_GRAY);
        }
    }

    private void applyValue(ConfigurationImage image, IniField field, int newValue) {
        if (field instanceof EnumIniField) {
            image.setBitValue((EnumIniField) field, newValue);
        } else if (field instanceof ScalarIniField) {
            ScalarIniField sf = (ScalarIniField) field;
            java.nio.ByteBuffer bb = image.getByteBuffer(sf.getOffset(), sf.getSize());
            sf.getType().writeRawValue(bb, newValue);
        }
    }

    private void saveBuffer() {
        List<FrameRecord> snapshot;
        synchronized (lock) {
            snapshot = new ArrayList<>(buffer);
        }
        if (snapshot.isEmpty()) {
            messageHandler.accept("Buffer is empty, nothing to save");
            return;
        }
        JFileChooser chooser = new JFileChooser();
        chooser.setDialogTitle("Save file as");
        chooser.setSelectedFile(new File("slcan_" + new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date()) + ".txt"));
        if (chooser.showSaveDialog(content) != JFileChooser.APPROVE_OPTION) {
            return;
        }
        File file = chooser.getSelectedFile();
        try (PrintWriter writer = new PrintWriter(Files.newBufferedWriter(file.toPath(), StandardCharsets.US_ASCII))) {
            for (FrameRecord record : snapshot) {
                String hexData = record.frame.rtr ? "" : HexUtil.asString(record.frame.data);
                writer.printf("(%d.%06d) can0 %X#%s%n", record.wallClockMs / 1000, (record.wallClockMs % 1000) * 1000, record.frame.id, hexData);
            }
        } catch (IOException e) {
            messageHandler.accept("Failed to save: " + e);
            return;
        }
        messageHandler.accept("Saved " + snapshot.size() + " frame(s) to " + file.getAbsolutePath());
    }

    private static class FrameRecord {
        final long wallClockMs;
        final SlcanClient.Frame frame;

        FrameRecord(long wallClockMs, SlcanClient.Frame frame) {
            this.wallClockMs = wallClockMs;
            this.frame = frame;
        }
    }
}
