package com.rusefi.ui.widgets;

import com.devexperts.logging.Logging;
import com.rusefi.FileLog;
import com.rusefi.UiVersion;
import com.rusefi.core.io.BundleUtil;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.ui.StatusWindow;

import javax.swing.*;
import java.awt.*;
import java.awt.datatransfer.StringSelection;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.ToolUtil.EOL;

public class StatusPanel extends JPanel implements UpdateOperationCallbacks {
    private static final Logging log = getLogging(StatusWindow.class);

    private static final Color LIGHT_RED = new Color(255, 102, 102);
    private static final Color LIGHT_GREEN = new Color(102, 255, 102);
    private final JTextArea logTextArea = new JTextArea();
    private final JLabel bottomStatusLabel = new JLabel();

    public StatusPanel(final int height) {
        super(new BorderLayout());

        logTextArea.setLineWrap(true);
        JScrollPane messagesScroll = new JScrollPane(logTextArea, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED) {
            @Override
            public Dimension getPreferredSize() {
                return new Dimension(400, height);
            }
        };
        super.add(messagesScroll, BorderLayout.CENTER);
        super.add(bottomStatusLabel, BorderLayout.SOUTH);
        clear();
    }

    public void setErrorState() {
        logTextArea.setBackground(LIGHT_RED);
        copyContentToClipboard();
    }

    @Override
    public void done() {
        setSuccessState();
    }

    @Override
    public void error() {
        setErrorState();
    }

    @Override
    public void warning() {
        logTextArea.setBackground(Color.YELLOW);
    }

    public void setSuccessState() {
        logTextArea.setBackground(LIGHT_GREEN);
    }

    public void copyContentToClipboard() {
        // kludge: due to 'append' method using invokeLater even while on AWT thread we also need invokeLater to
        // actually get overall status message
        SwingUtilities.invokeLater(() -> {
            final String contentWithoutNullTerminators = logTextArea.getText().replace("\0", EOL);
            try {
                Toolkit.getDefaultToolkit().getSystemClipboard()
                    .setContents(new StringSelection(contentWithoutNullTerminators), null);
            } catch (Throwable e) {
                log.error("getSystemClipboard error " + e, e);
            }
        });

        logLine("hint: error state is already in your clipboard, please use PASTE or Ctrl-V while reporting issues");
    }

    @Override
    public void clear() {
        logTextArea.setText("");
        logTextArea.setBackground(Color.WHITE);
        logLine("Console version " + UiVersion.CONSOLE_VERSION);
        log.info(FileLog.getOsName() + " " + System.getProperty("os.version"));
        logLine("Bundle " + BundleUtil.readBundleFullNameNotNull());
    }

    @Override
    public void log(final String string, final boolean breakLineOnTextArea, final boolean sendToLogger) {
        // todo: check if AWT thread and do not invokeLater if already on AWT thread
        SwingUtilities.invokeLater(() -> {
            String s = string.replaceAll(Character.toString((char) 219), "");
            if (sendToLogger) {
                log.info(s);
            }
            // We do not want to print null-terminator on text area to avoid problems like #7402 and #7431:
            String stringForTestArea = s.replaceAll("\0", "");
            if (breakLineOnTextArea) {
                stringForTestArea += "\r\n";
            }
            logTextArea.append(stringForTestArea);
            AutoupdateUtil.trueLayoutAndRepaint(logTextArea);
        });
    }

    public void setStatus(String status) {
        bottomStatusLabel.setText(status);
    }
}
