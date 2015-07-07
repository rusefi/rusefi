package com.rusefi.ui;

import com.rusefi.core.MessagesCentral;
import com.rusefi.io.CommandQueue;
import com.rusefi.io.InvocationConfirmationListener;
import com.rusefi.ui.util.UiUtils;
import org.jetbrains.annotations.NotNull;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 * (c) Andrey Belomutskiy
 * 6/20/2015.
 */
public class LogsManager {
    /**
     * see LS_RESPONSE in firmware source code
     */
    private static final String LS_RESPONSE = "ls_result";
    private static final String LS_ENTRY_PREFIX = "logfile";
    private final JPanel content = new JPanel(new BorderLayout());

    private final JPanel logFiles = new JPanel(new VerticalFlowLayout());

    public LogsManager() {
        UiUtils.showLoadingMessage(content);
        MessagesCentral.getInstance().addListener(new MessagesCentral.MessageListener() {
            @Override
            public void onMessage(Class clazz, String message) {
                if (message.startsWith(LS_RESPONSE)) {
                    resetLogsList();
                } else if (message.startsWith(LS_ENTRY_PREFIX)) {
                    message = message.substring(LS_ENTRY_PREFIX.length());
                    int colonIndex = message.indexOf(':');
                    if (colonIndex == -1)
                        return;
                    long size;
                    try {
                        size = Integer.parseInt(message.substring(0, colonIndex));
                    } catch (NumberFormatException e) {
                        return;
                    }
                    JPanel logFileEntry = createFilePanel(message.substring(colonIndex + 1), size);

                    logFiles.add(logFileEntry);
                }
            }
        });
    }

    @NotNull
    private JPanel createFilePanel(final String name, long size) {
        JLabel label = new JLabel(name + " [" + size + " bytes]");

        JPanel logFileEntry = new JPanel(new FlowLayout());
        logFileEntry.add(label);

        JButton removeFile = new JButton("[delete]");
        removeFile.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                CommandQueue.getInstance().write("del " + name);
            }
        });

        logFileEntry.add(removeFile);
        return logFileEntry;
    }

    private void resetLogsList() {
        logFiles.removeAll();
    }

    public JPanel getContent() {
        return content;
    }

    public void showContent() {
        content.removeAll();

        JButton refresh = new JButton("Refresh");
        refresh.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                listDirectory();
            }
        });

        content.add(refresh, BorderLayout.NORTH);
        content.add(logFiles, BorderLayout.CENTER);
        listDirectory();
    }

    private void listDirectory() {
//        CommandQueue.getInstance().write("ls /", CommandQueue.DEFAULT_TIMEOUT, InvocationConfirmationListener.VOID,
//                false);
    }
}
