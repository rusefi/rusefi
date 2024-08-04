package com.rusefi.ui.logview;

import com.devexperts.logging.FileLogger;
import com.rusefi.ConsoleUI;
import com.rusefi.FileLog;
import com.rusefi.config.generated.Fields;
import com.rusefi.core.EngineState;
import com.rusefi.file.FileUtils;
import com.rusefi.ui.ChartRepository;
import com.rusefi.ui.LogDownloader;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.engine.EngineSnifferPanel;
import com.rusefi.ui.util.UiUtils;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.io.File;
import java.io.FileFilter;
import java.util.Arrays;

/**
 * This tab is the entry point of rusEfi own log browser
 * <p/>
 * <p/>
 * 7/27/13
 * Andrey Belomutskiy, (c) 2013-2020
 * @see LogDownloader
 */
public class LogViewer extends JPanel {
    public static final FileFilter FILE_FILTER = new FileFilter() {
        @Override
        public boolean accept(File pathname) {
            return pathname.getName().contains("MAIN_rfi_report");
        }
    };
    public static final String DEFAULT_LOG_LOCATION = FileLogger.DIR;
    private final JLabel folderLabel = new JLabel();
    private final JLabel fileLabel = new JLabel();
    private final DefaultListModel<FileItem> fileListModel = new DefaultListModel<FileItem>();
    private final JList<FileItem> fileList = new JList<FileItem>(fileListModel);
    private final UIContext uiContext;
    private final EngineSnifferPanel engineSnifferPanel;
    private String currentFolder;
    private static JPanel descPanel = new JPanel();

    public LogViewer(UIContext uiContext, EngineSnifferPanel engineSnifferPanel) {
        super(new BorderLayout());
        this.uiContext = uiContext;
        this.engineSnifferPanel = engineSnifferPanel;

        setBackground(Color.green);

        /**
         * scan the default folder on start-up
         */
        openFolder(DEFAULT_LOG_LOCATION);

        JPanel folderPanel = new JPanel(new FlowLayout());

        JButton folderButton = new JButton("Open folder");
        folderButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                JFileChooser chooser = new JFileChooser(currentFolder);
                chooser.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
                int result = chooser.showSaveDialog(LogViewer.this);
                if (result == JFileChooser.APPROVE_OPTION)
                    openFolder(chooser.getSelectedFile().getAbsolutePath());
            }
        });

        folderPanel.add(folderButton);
        folderPanel.add(folderLabel);
        folderPanel.add(fileLabel);

        add(folderPanel, BorderLayout.NORTH);

        refreshCountPanel();

        JPanel boxPanel = new JPanel();
        boxPanel.setLayout(new BoxLayout(boxPanel, BoxLayout.X_AXIS));
        boxPanel.setBorder(BorderFactory.createLineBorder(Color.cyan));

        fileList.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                if (e.getClickCount() == 2) {
                    FileItem selectedItem = fileList.getSelectedValue();
                    openFile(selectedItem.getFile());
                }
            }
        });

        JScrollPane scrollPane = new JScrollPane(fileList);
        boxPanel.add(scrollPane);
        boxPanel.add(descPanel);

        add(boxPanel);
    }

    private void refreshCountPanel() {
        descPanel.removeAll();
        descPanel.add(new JLabel("Total digital charts: "));
        descPanel.add(new JLabel("" + ChartRepository.getInstance().getSize()));
        UiUtils.trueRepaint(descPanel);
    }

    private void openFolder(String folderName) {
        folderLabel.setText("Current folder: " + folderName);
        currentFolder = folderName;

        File folder = new File(folderName);
        if (!folder.isDirectory())
            throw new IllegalStateException("Not directory: " + folder);

        File[] files = folder.listFiles(FILE_FILTER);
        Arrays.sort(files);

        fileListModel.removeAllElements();
        for (File file : files)
            fileListModel.addElement(getFileDesc(file));

        int index = 0;

        while (files.length > index && uiContext.getLinkManager().isLogViewer()) {
            File file = files[index];
            if (file.getName().endsWith(FileLog.currentLogName)) {
                /**
                 * we do not want to view the log file we've just started to produce.
                 * We are here if the logs are newer then our current time - remember about fime zone differences
                 */
                index++;
                continue;
            }
            openFile(file);
            break;
        }
    }

    private FileItem getFileDesc(File file) {
        return new FileItem(file);
    }


//        JPanel upperPanel = new JPanel(new FlowLayout());
//
//
//        JButton next = new JButton("next");
//        next.addActionListener(new ActionListener() {
//            @Override
//            public void actionPerformed(ActionEvent e) {
//                currentChartIndex++;
//                refreshChart();
//
//            }
//        });
//
//        upperPanel.add(next);
//
//
//        refreshChart();


    //    private void refreshChart() {
//        String chart = ChartRepository.getConfig().getChart(currentChartIndex);
//    }
//
//    public static void main(String[] args) {
//        String filename = "a.csv";
//
//
    private void openFile(File file) {
        fileLabel.setText("Current file: " + file.getName());
        String filename = file.getAbsolutePath();
        EngineState.EngineStateListener listener = new EngineState.EngineStateListenerImpl();

        ChartRepository.getInstance().clear();
        EngineState engineState = new EngineState(listener);
        // this is pretty dirty, better OOP desperately needed
        ConsoleUI.engineSnifferPanel.setOutpinListener(engineState);
        engineState.registerStringValueAction(Fields.PROTOCOL_ENGINE_SNIFFER, new EngineState.ValueCallback<String>() {
            @Override
            public void onUpdate(String value) {
                FileLog.MAIN.logLine("Got wave_chart: " + value);

                ChartRepository.getInstance().addChart(value);
            }
        });
        FileUtils.readFile2(filename, engineState);

        if (ChartRepository.getInstance().getSize() > 0)
            engineSnifferPanel.reloadFile();
        refreshCountPanel();
    }
}
