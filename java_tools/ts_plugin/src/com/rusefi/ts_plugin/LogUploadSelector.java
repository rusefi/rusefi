package com.rusefi.ts_plugin;

import com.rusefi.TsTuneReader;
import com.rusefi.ui.util.FrameHelper;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import java.io.File;

public class LogUploadSelector {
    private JPanel content = new JPanel(new VerticalFlowLayout());

    public static void main(String[] args) {
        new FrameHelper().showFrame(new LogUploadSelector().getContent());
    }


    public LogUploadSelector() {

        String projectName = "dev";
        String folder = TsTuneReader.getProjectsDir()
                + File.separator + projectName + File.separator + "DataLogs";


        for (String fileName : new File(folder).list((dir, name) -> name.endsWith(".mlg"))) {
            System.out.println(fileName);


        }

    }

    private JComponent getContent() {
        return content;
    }

}
