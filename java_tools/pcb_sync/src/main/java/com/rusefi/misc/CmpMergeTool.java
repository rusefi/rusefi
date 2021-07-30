package com.rusefi.misc;

import com.rusefi.util.FileUtils;

import java.io.*;
import java.util.List;

/**
 * 12/8/13
 * (c) Andrey Belomutskiy
 */
public class CmpMergeTool {
    private CmpMergeTool() {
    }

    public static void main(String[] args) throws IOException {
        if (args.length < 2) {
            System.out.println("This tool merges .cmp KiCad component mappting files");
            System.out.println("usage:");
            System.out.println("NetListMerge DEST_FILE SOURCE_1 SOURCE_2 [SOURCE_x]*");
            return;
        }

        String destinationFile = args[0];

        System.out.println("Writing result into " + destinationFile);
        BufferedWriter bw = new BufferedWriter(new FileWriter(destinationFile));

        bw.write("Cmp-Mod V01 Created by CvPcb (2013-07-07 BZR 4022)-stable date = 01/12/2013 21:54:01\r\n");

        for (int i = 1; i < args.length; i++) {
            String sourceFile = args[i];

            List<String> list = FileUtils.readFileToList(sourceFile);

            for (String line : list.subList(1, list.size() - 2))
                bw.write(line + "\r\n");
        }

        bw.write("\r\nEndListe\r\n");
        bw.close();
    }
}
