package com.rusefi;

import com.rusefi.tune.xml.Msq;
import com.rusefi.xml.XmlUtil;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import javax.swing.filechooser.FileSystemView;
import java.io.File;

public class TsTuneReader {
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
        JFileChooser fr = new JFileChooser();
        FileSystemView fw = fr.getFileSystemView();
        File defaultDirectory = fw.getDefaultDirectory();
        System.out.println(defaultDirectory);

        return defaultDirectory + File.separator + "TunerStudioProjects" + File.separator + ecuName + File.separator + "CurrentTune.msq";
    }
}
