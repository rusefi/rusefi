package com.rusefi.ui;

import com.devexperts.logging.Logging;
import com.rusefi.io.can.slcan.SlcanClient;

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

    public SlcanTab() {
        this(null);
    }

    public SlcanTab(Consumer<String> messageHandler) {
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
                text.append(String.format("%s %-30s %s%n",
                    timeFormat.format(new Date(record.wallClockMs)), record.frame.raw, record.frame.decode()));
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
        long firstMs = snapshot.get(0).wallClockMs;
        try (PrintWriter writer = new PrintWriter(Files.newBufferedWriter(file.toPath(), StandardCharsets.US_ASCII))) {
            writer.println("; rusEFI SLCAN capture " + new Date() + ", " + snapshot.size() + " frame(s)");
            writer.println("; relative-ms raw-slcan-line decoded");
            for (FrameRecord record : snapshot) {
                writer.printf("%d %s %s%n", record.wallClockMs - firstMs, record.frame.raw, record.frame.decode());
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
