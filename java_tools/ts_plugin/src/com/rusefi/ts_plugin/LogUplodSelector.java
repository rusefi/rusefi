package com.rusefi.ts_plugin;

import com.rusefi.ui.util.FrameHelper;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import javax.swing.filechooser.FileSystemView;
import java.io.File;

public class LogUplodSelector {
    private JPanel content = new JPanel(new VerticalFlowLayout());

    public static void main(String[] args) {
        new FrameHelper().showFrame(new LogUplodSelector().getContent());
    }


    public LogUplodSelector() {

        // https://stackoverflow.com/questions/9677692/getting-my-documents-path-in-java
        String projectName = "dev";
        String folder = FileSystemView.getFileSystemView().getDefaultDirectory().getPath() + File.separator + "TunerStudioProjects"
                + File.separator + projectName + File.separator + "DataLogs";


        for (String fileName : new File(folder).list((dir, name) -> name.endsWith(".mlg"))) {
            System.out.println(fileName);


        }

    }

    private JComponent getContent() {
        return content;
    }

}
