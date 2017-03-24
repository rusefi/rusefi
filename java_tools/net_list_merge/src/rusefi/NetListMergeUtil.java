package rusefi;

import java.io.*;

/**
 * 12/8/13
 * (c) Andrey Belomutskiy
 */
public class NetListMergeUtil {
    static void writeResult(String content, String fileName) throws IOException {
        System.out.println("Writing result into " + fileName);
        BufferedWriter bw = new BufferedWriter(new FileWriter(fileName));
        bw.write(content);
        bw.close();
    }

    static String readFile(String fileName) throws IOException {
        checkExistence(fileName);

        System.out.println("Reading " + fileName);
        StringBuilder sb = new StringBuilder();
        String line;
        BufferedReader br = new BufferedReader(new FileReader(fileName));

        while (((line = br.readLine()) != null))
            sb.append(line).append("\r\n");
        return sb.toString();
    }

    private static void checkExistence(String fileName) {
        if (!new File(fileName).isFile()) {
            System.err.println("File not found: " + fileName);
            System.exit(-1);
        }
    }

    static void validateSection(String section) {
        int counter = 0;
        for (int i = 0; i < section.length(); i++) {
            char c = section.charAt(i);
            if (c == '(')
                counter++;
            else if (c == ')')
                counter--;
            if (counter < 0)
                throw new IllegalStateException("Invalid section: " + section);
        }
        if (counter != 0)
            throw new IllegalStateException("Invalid section: " + section + " balance: " + counter);
    }
}
