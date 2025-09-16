package com.rusefi;

import com.rusefi.ts.TsHelper;
import com.rusefi.tune.xml.Msq;
import com.rusefi.xml.XmlUtil;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import javax.swing.filechooser.FileSystemView;
import java.io.File;
import java.io.IOException;
import java.util.Properties;

public class TsTuneReader {

    public static Msq readTsTune(String ecuName) throws Exception {
        String fileName = getTsTuneFileName(ecuName);
        return XmlUtil.readModel(Msq.class, new File(fileName));
    }

    @NotNull
    public static String getTsTuneFileName(String ecuName) {
        String projectsDir = getProjectsDir();

        return projectsDir + File.separator + ecuName + File.separator + TsHelper.CURRENT_TUNE_MSQ;
    }

    public static String getProjectsDir() {
        try {
            Properties tsUser = TsHelper.readTsProperties();
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
            File.separator + TsHelper.MAIN_CONTROLLER_PATH;
    }
}
