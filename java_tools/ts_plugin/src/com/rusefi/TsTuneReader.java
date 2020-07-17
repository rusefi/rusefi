package com.rusefi;

import com.rusefi.tune.xml.Msq;
import com.rusefi.xml.XmlUtil;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import javax.swing.filechooser.FileSystemView;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.Properties;

public class TsTuneReader {
    private static final String TS_USER_FILE = System.getProperty("user.home") + File.separator + ".efiAnalytics" + File.separator + "tsUser.properties";

    public static void main(String[] args) throws Exception {
        String ecuName = "dev";

        Msq tune = readTsTune(ecuName);
        System.out.println(tune);
    }

    public static Msq readTsTune(String ecuName) throws Exception {
        String fileName = getTsTuneFileName(ecuName);
        return XmlUtil.readModel(Msq.class, fileName);
    }

    @NotNull
    public static String getTsTuneFileName(String ecuName) {
        String projectsDir = getProjectsDir();

        return projectsDir + File.separator + ecuName + File.separator + "CurrentTune.msq";
    }

    public static String getProjectsDir() {
        try {
            Properties tsUser = new Properties();
            tsUser.load(new FileInputStream(TS_USER_FILE));
            // reading TS properties
            return tsUser.getProperty("projectsDir");
        } catch (IOException e) {
            JFileChooser fr = new JFileChooser();
            FileSystemView fw = fr.getFileSystemView();
            File defaultDirectory = fw.getDefaultDirectory();

            // fallback mechanism just in case
            return defaultDirectory + File.separator + "TunerStudioProjects";
        }
    }

    @NotNull
    public static String getProjectModeFileName(String projectName) {
        return getProjectsDir() +
                    File.separator + projectName +
                    File.separator + "projectCfg" +
                    File.separator + "mainController.ini";
    }
}
