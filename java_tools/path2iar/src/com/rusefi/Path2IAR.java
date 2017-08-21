package com.rusefi;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * (c) Andrey Belomutskiy
 * 3/30/2017.
 */
public class Path2IAR {
    private static final String EOL = "\n";
    private static final String RELATIVE = "..\\..\\firmware/ChibiOS/os";
    private static String includes = "";

    public static void main(String[] args) throws IOException {
        String result = process("  ", new File(RELATIVE));


        System.out.println("Result:" + EOL + EOL + EOL + EOL + EOL + EOL);
        System.out.println(result.length());

        new FileOutputStream("group.txt", false).write(result.getBytes());

        System.out.println("Headers:" + EOL + EOL + EOL + EOL + EOL + EOL);
        System.out.println(includes);
    }

    private static String process(String offset, File folder) {
        System.out.println("Folder " + folder);
        if (!folder.isDirectory())
            throw new IllegalStateException("Not a directory: " + folder);


        includes += "<state>$PROJ_DIR$\\..\\ChibiOS\\os" + folder.getPath().substring(RELATIVE.length()) + "</state>" + EOL;


        String group = offset + "<group>\n" +
                offset + "    <name>" + folder.getName() + "</name>\n";


        for (String fileName : folder.list()) {
            File file = new File(folder.getPath() + File.separator + fileName);
            System.out.println(file);

            if (file.isDirectory()) {
                group += process(offset + "  ", file);
                continue;
            }

            if (!fileName.endsWith(".cpp") && !fileName.endsWith(".c") && !fileName.endsWith("h"))
                continue;


            String name = file.getPath().substring(RELATIVE.length());

            group += offset + "<file>\n" +
                    offset + "  <name>$PROJ_DIR$\\..\\ChibiOS\\os" + name + "</name>\n" +
                    offset + "</file>\n";

        }




        return group + offset + "</group>" + EOL;

    }
}
