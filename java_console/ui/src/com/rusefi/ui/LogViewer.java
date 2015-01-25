package com.rusefi.ui;

import com.irnems.FileLog;
import com.irnems.core.EngineState;
import com.irnems.file.FileUtils;
import com.rusefi.ui.widgets.UpDownImage;
import com.rusefi.FIleItem;
import com.rusefi.io.LinkManager;

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
 * (c) Andrey Belomutskiy
 */
public class LogViewer extends JPanel {
    public static final FileFilter FILE_FILTER = new FileFilter() {
        @Override
        public boolean accept(File pathname) {
            return pathname.getName().contains("MAIN_rfi_report");
        }
    };
    public static final String DEFAULT_LOG_LOCATION = "out";
    private final JLabel folderLabel = new JLabel();
    private final JLabel fileLabel = new JLabel();
    private final DefaultListModel<FIleItem> fileListModel = new DefaultListModel<FIleItem>();
    private final JList<FIleItem> fileList = new JList<FIleItem>(fileListModel);
    private final WavePanel wavePanel;
    private String currentFolder;
    private static JPanel descPanel = new JPanel();


//    int currentChartIndex = 0;

    public LogViewer(WavePanel wavePanel) {
        super(new BorderLayout());
        this.wavePanel = wavePanel;

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
                    FIleItem selectedItem = fileList.getSelectedValue();
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
        UpDownImage.trueRepaint(descPanel);
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

        if (files.length > 0 && LinkManager.isLogViewer())
            openFile(files[0]);
    }

    private FIleItem getFileDesc(File file) {
        return new FIleItem(file);
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
        engineState.registerStringValueAction("wave_chart", new EngineState.ValueCallback<String>() {
            @Override
            public void onUpdate(String value) {
                FileLog.rlog("Got wave_chart: " + value);

                ChartRepository.getInstance().addChart(value);
            }
        });
        FileUtils.readFile2(filename, engineState);

        if (ChartRepository.getInstance().getSize() > 0)
            wavePanel.reloadFile();
        refreshCountPanel();
    }
}
