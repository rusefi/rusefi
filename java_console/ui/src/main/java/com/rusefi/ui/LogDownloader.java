package com.rusefi.ui;

import com.rusefi.ConsoleUI;
import com.rusefi.core.MessagesCentral;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.ui.logview.LogViewer;
import com.rusefi.ui.util.UiUtils;
import org.jetbrains.annotations.NotNull;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import static com.rusefi.ui.util.UiUtils.createOnTopParent;

/**
 * This tab is about uploading log files from ECU.
 *
 * Andrey Belomutskiy, (c) 2013-2020
 * 6/20/2015.
 * @see LogViewer
 */
public class LogDownloader {
    /**
     * see LS_RESPONSE in firmware source code
     */
    private static final String LS_RESPONSE = "ls_result";
    private static final String LS_ENTRY_PREFIX = "logfile";
    private static final String DELETE = "[delete]";
    private static final String DOWNLOAD = "[download]";
    private final JPanel content = new JPanel(new BorderLayout());

    private final JPanel logFiles = new JPanel(new VerticalFlowLayout());
    private final Timer timer = new Timer(3000, new ActionListener() {
        @Override
        public void actionPerformed(ActionEvent e) {
            listDirectory();
        }
    });
    private final UIContext uiContext;

    public LogDownloader(UIContext uiContext) {
        this.uiContext = uiContext;
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
                    AutoupdateUtil.trueLayoutAndRepaint(logFiles.getParent());
                }
            }
        });
    }

    @NotNull
    private JPanel createFilePanel(final String name, long size) {
        JLabel label = new JLabel(name + " [" + size + " bytes]");

        JPanel logFileEntry = new JPanel(new FlowLayout());
        logFileEntry.add(label);

        JButton removeFile = new JButton(DELETE);
        removeFile.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                int i = JOptionPane.showConfirmDialog(createOnTopParent(), ("Do you really want to delete " + name + "?"),
                        UIManager.getString("OptionPane.titleText"),
                        JOptionPane.YES_NO_OPTION);
                if (i == JOptionPane.YES_OPTION) {
                    uiContext.getCommandQueue().write("del " + name);
                    timer.restart();
                }
            }
        });

        JButton downloadFile = new JButton(DOWNLOAD);
        downloadFile.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                uiContext.getCommandQueue().write("get_file 0 " + name);
            }
        });


        logFileEntry.add(removeFile);
        logFileEntry.add(downloadFile);
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
/*
        CommandQueue.getInstance().write("ls /", CommandQueue.DEFAULT_TIMEOUT, InvocationConfirmationListener.VOID,
                false);
 */
    }
}
