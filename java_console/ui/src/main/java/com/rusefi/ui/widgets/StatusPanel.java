package com.rusefi.ui.widgets;

import com.devexperts.logging.Logging;
import com.rusefi.FileLog;
import com.rusefi.core.io.BundleUtil;
import com.rusefi.core.rusEFIVersion;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.ui.StatusConsumer;
import com.rusefi.ui.StatusWindow;
import com.rusefi.ui.util.UiUtils;

import javax.swing.*;
import java.awt.*;
import java.awt.datatransfer.StringSelection;

import static com.devexperts.logging.Logging.getLogging;

public class StatusPanel extends JPanel implements UpdateOperationCallbacks, StatusConsumer {
    private static final Logging log = getLogging(StatusWindow.class);

    private static final Color LIGHT_RED = new Color(255, 102, 102);
    private static final Color LIGHT_GREEN = new Color(102, 255, 102);
    private final JTextArea logTextArea = new JTextArea();
    private final JLabel bottomStatusLabel = new JLabel();

    public StatusPanel() {
        super(new BorderLayout());

        logTextArea.setLineWrap(true);
        JScrollPane messagesScroll = new JScrollPane(logTextArea, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED) {
            @Override
            public Dimension getPreferredSize() {
                return new Dimension(400, 400);
            }
        };
        super.add(messagesScroll, BorderLayout.CENTER);
        super.add(bottomStatusLabel, BorderLayout.SOUTH);
        appendLine("Console version " + rusEFIVersion.CONSOLE_VERSION);
        appendLine(FileLog.getOsName() + " " + System.getProperty("os.version"));
        appendLine("Bundle " + BundleUtil.readBundleFullNameNotNull());
    }

    public void setErrorState() {
        logTextArea.setBackground(LIGHT_RED);
        copyContentToClipboard();
    }

    @Override
    public void log(final String message, final boolean breakLineOnTextArea, final boolean sendToLogger) {
        append(message, breakLineOnTextArea, sendToLogger);
    }

    @Override
    public void done() {
        setSuccessState();
    }

    @Override
    public void error() {
        setErrorState();
    }

    public void setSuccessState() {
        logTextArea.setBackground(LIGHT_GREEN);
    }

    public void copyContentToClipboard() {
        // kludge: due to 'append' method using invokeLater even while on AWT thread we also need invokeLater to
        // actually get overall status message
        SwingUtilities.invokeLater(() -> Toolkit.getDefaultToolkit().getSystemClipboard()
            .setContents(new StringSelection(logTextArea.getText()), null));

        appendLine("hint: error state is already in your clipboard, please use PASTE or Ctrl-V while reporting issues");
    }

    public void clear() {
        logTextArea.setText("");
    }

    @Override
    public void append(final String string, final boolean breakLineOnTextArea, final boolean sendToLogger) {
        // todo: check if AWT thread and do not invokeLater if already on AWT thread
        SwingUtilities.invokeLater(() -> {
            String s = string.replaceAll(Character.toString((char) 219), "");
            if (sendToLogger) {
                log.info(s);
            }
            String stringForTestArea = s;
            if (breakLineOnTextArea) {
                stringForTestArea += "\r\n";
            }
            logTextArea.append(stringForTestArea);
            UiUtils.trueLayout(logTextArea);
        });
    }

    public void setStatus(String status) {
        bottomStatusLabel.setText(status);
    }

    @Override
    public void appendStatus(final String string, final boolean breakLineOnTextArea, final boolean sendToLogger) {
        append(string, breakLineOnTextArea, sendToLogger);
    }
}
