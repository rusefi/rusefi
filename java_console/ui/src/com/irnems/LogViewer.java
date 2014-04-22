package com.irnems;

import com.irnems.core.EngineState;
import com.irnems.file.FileUtils;
import com.irnems.ui.WavePanel;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FileFilter;

/**
 * 7/27/13
 * (c) Andrey Belomutskiy
 */
public class LogViewer extends JPanel {
    public static final FileFilter FILE_FILTER = new FileFilter() {
        @Override
        public boolean accept(File pathname) {
            return pathname.getName().startsWith("rfi_report");
        }
    };
    private final JLabel folderLabel = new JLabel();
    private final DefaultListModel<String> fileListModel = new DefaultListModel<String>();
    private final JList<String> fileList = new JList<String>(fileListModel);
    private String currentFolder;


//    int currentChartIndex = 0;

    public LogViewer() {
        super(new BorderLayout());

        setBackground(Color.green);

        // todo: this is not perfect
        openFolder("out");

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

        folderPanel.setBackground(Color.red);

        add(folderPanel, BorderLayout.NORTH);


        JPanel descPanel = new JPanel();
        descPanel.add(new JLabel("Total digital charts: "));
        descPanel.add(new JLabel("" + ChartRepository.getInstance().getSize()));


        JPanel boxPanel = new JPanel();
        boxPanel.setLayout(new BoxLayout(boxPanel, BoxLayout.X_AXIS));
        boxPanel.setBorder(BorderFactory.createLineBorder(Color.cyan));

        boxPanel.add(fileList);
        boxPanel.add(descPanel);


        add(boxPanel);
    }

    private void openFolder(String folderName) {
        folderLabel.setText(folderName);
        currentFolder = folderName;

        File folder = new File(folderName);
        if (!folder.isDirectory())
            throw new IllegalStateException("Not directory: " + folder);

        File[] files = folder.listFiles(FILE_FILTER);
        fileList.removeAll();
        for (File file : files)
            fileListModel.addElement(getFileDesc(file));

        if (files.length > 0)
            openFile(files[0]);

    }

    private String getFileDesc(File file) {
        return file.getName() + " " + file.getUsableSpace();
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
//        String chart = ChartRepository.getInstance().getChart(currentChartIndex);
//    }
//
//    public static void main(String[] args) {
//        String filename = "a.csv";
//
//
    private void openFile(File file) {
        String filename = file.getAbsolutePath();
        EngineState.EngineStateListener listener = new EngineState.EngineStateListenerImpl() {
        };

        ChartRepository.getInstance().clear();
        EngineState engineState = new EngineState(listener);
        engineState.registerStringValueAction("wave_chart", new EngineState.ValueCallback<String>() {
            @Override
            public void onUpdate(String value) {
                FileLog.rlog("chart " + value);

                ChartRepository.getInstance().addChart(value);

            }
        });
        FileUtils.readFile2(filename, engineState);

        if (ChartRepository.getInstance().getSize() > 0)
            WavePanel.getInstance().displayChart(ChartRepository.getInstance().getChart(0));
    }
}