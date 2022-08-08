package com.rusefi.ui;

import com.devexperts.logging.Logging;
import com.rusefi.NamedThreadFactory;
import com.rusefi.io.stream.PCanIoStream;
import com.rusefi.tools.CANConnectorStartup;
import com.rusefi.ui.util.FrameHelper;
import com.rusefi.ui.util.UiUtils;

import javax.swing.*;
import java.awt.*;
import java.io.IOException;

import static com.devexperts.logging.Logging.getLogging;

public class PcanConnectorUI {
    private static final Logging log = getLogging(PcanConnectorUI.class);

    public static void show() {
        FrameHelper frame = new FrameHelper(WindowConstants.EXIT_ON_CLOSE);

        JPanel panel = new JPanel(new BorderLayout());
        panel.add(new JLabel("Running PCAN connector for TS"), BorderLayout.NORTH);
        JTextArea logTextArea = new JTextArea();
        panel.add(logTextArea, BorderLayout.CENTER);

        StatusConsumer statusConsumer = string -> SwingUtilities.invokeLater(() -> {
            log.info(string);
            logTextArea.append(string + "\r\n");
            UiUtils.trueLayout(logTextArea);
        });

        new NamedThreadFactory("PCAN-connector").newThread(() -> {
            PCanIoStream stream = PCanIoStream.createStream(statusConsumer);
            try {
                if (stream != null)
                    CANConnectorStartup.start(stream, statusConsumer);
            } catch (IOException e) {
                statusConsumer.append("Error " + e);
            }
        }).start();

        frame.showFrame(panel);
    }
}
