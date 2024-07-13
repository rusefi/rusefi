package com.rusefi.ui;

import com.devexperts.logging.Logging;
import com.rusefi.core.io.BundleUtil;
import com.rusefi.core.rusEFIVersion;
import com.rusefi.core.ui.FrameHelper;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.ui.util.UiUtils;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.awt.datatransfer.StringSelection;

import static com.devexperts.logging.Logging.getLogging;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 3/7/2015
 */
public class StatusWindow implements StatusConsumer, UpdateOperationCallbacks {
    private static final Logging log = getLogging(StatusWindow.class);

    private static final Color LIGHT_RED = new Color(255, 102, 102);
    private static final Color LIGHT_GREEN = new Color(102, 255, 102);
    // todo: extract driver from console bundle? find a separate driver bundle?
    private final JTextArea logTextArea = new JTextArea();
    private final JPanel content = new JPanel(new BorderLayout());
    private final JLabel bottomStatusLabel = new JLabel();
    @NotNull
    protected final FrameHelper frameHelper = new FrameHelper();

    public StatusWindow() {
        logTextArea.setLineWrap(true);
        JScrollPane messagesScroll = new JScrollPane(logTextArea, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED) {
            @Override
            public Dimension getPreferredSize() {
                return new Dimension(400, 400);
            }
        };
        content.add(messagesScroll, BorderLayout.CENTER);
        content.add(bottomStatusLabel, BorderLayout.SOUTH);

        append("Console version " + rusEFIVersion.CONSOLE_VERSION, true);
        append("Windows " + System.getProperty("os.version"), true);
        append("Bundle " + BundleUtil.readBundleFullNameNotNull(), true);
    }

    @NotNull
    public FrameHelper getFrameHelper() {
        return frameHelper;
    }

    public JPanel getContent() {
        return content;
    }


    public void setErrorState() {
        logTextArea.setBackground(LIGHT_RED);
        copyContentToClipboard();
    }

  @Override
  public void log(final String message, final boolean breakLineOnTextArea) {
    append(message, breakLineOnTextArea);
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

    public JFrame getFrame() {
        return frameHelper.getFrame();
    }

    public void showFrame(String title) {
        frameHelper.getFrame().setTitle(title);
        frameHelper.showFrame(content, false);
        UiUtils.centerWindow(frameHelper.getFrame());
        logTextArea.setText(""); // let's remove stuff from previous invocation
    }

    @Override
    public void append(final String string, final boolean breakLineOnTextArea) {
        // todo: check if AWT thread and do not invokeLater if already on AWT thread
        SwingUtilities.invokeLater(() -> {
            String s = string.replaceAll(Character.toString((char) 219), "");
            log.info(s);
            String stringForTestArea = s;
            if (breakLineOnTextArea) {
                stringForTestArea += "\r\n";
            }
            logTextArea.append(stringForTestArea);
            UiUtils.trueLayout(logTextArea);
        });
    }

    public void copyContentToClipboard() {
        // kludge: due to 'append' method using invokeLater even while on AWT thread we also need invokeLater to
        // actually get overall status message
        SwingUtilities.invokeLater(() -> Toolkit.getDefaultToolkit().getSystemClipboard()
                .setContents(new StringSelection(logTextArea.getText()), null));

        append("hint: error state is already in your clipboard, please use PASTE or Ctrl-V while reporting issues", true);
    }

    public void setStatus(String status) {
        bottomStatusLabel.setText(status);
    }
}
