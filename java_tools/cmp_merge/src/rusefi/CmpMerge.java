package rusefi;

import java.io.*;
import java.util.ArrayList;
import java.util.List;

/**
 * 12/8/13
 * (c) Andrey Belomutskiy
 */
public class CmpMerge {
    private CmpMerge() {
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

            List<String> list = readFileToList(sourceFile);

            for (String line : list.subList(1, list.size() - 2))
                bw.write(line + "\r\n");
        }

        bw.write("\r\nEndListe\r\n");
        bw.close();
    }

    static List<String> readFileToList(String fileName) throws IOException {
        checkExistence(fileName);

        List<String> result = new ArrayList<String>();

        System.out.println("Reading " + fileName);
        String line;
        BufferedReader br = new BufferedReader(new FileReader(fileName));

        while (((line = br.readLine()) != null))
            result.add(line);
        return result;
    }

    private static void checkExistence(String fileName) {
        if (!new File(fileName).isFile()) {
            System.err.println("File not found: " + fileName);
            System.exit(-1);
        }
    }
}
