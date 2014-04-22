package com.rusefi.util;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * Generic file utilities
 * <p/>
 * 12/9/13
 * (c) Andrey Belomutskiy
 */
public class FileUtils {
    private FileUtils() {
    }

    /**
     * @param fileName
     * @return Full content of the file as one String
     * @throws IOException
     */
    public static String readFile(String fileName) throws IOException {
        checkExistence(fileName);

        System.out.println("Reading " + fileName);
        StringBuilder sb = new StringBuilder();
        String line;
        BufferedReader br = new BufferedReader(new FileReader(fileName));

        while (((line = br.readLine()) != null))
            sb.append(line).append("\r\n");
        br.close();
        return sb.toString();
    }

    private static void checkExistence(String fileName) {
        if (!new File(fileName).isFile()) {
            System.err.println("File not found: " + fileName);
            System.exit(-1);
        }
    }

    public static List<String> readFileToList(String fileName) throws IOException {
        checkExistence(fileName);

        List<String> result = new ArrayList<String>();

        System.out.println("Reading " + fileName);
        String line;
        BufferedReader br = new BufferedReader(new FileReader(fileName));

        while (((line = br.readLine()) != null))
            result.add(line);
        return result;
    }
}
