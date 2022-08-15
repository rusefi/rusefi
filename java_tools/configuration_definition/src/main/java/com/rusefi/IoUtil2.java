package com.rusefi;

import com.rusefi.newparse.ParseState;
import com.rusefi.newparse.parsing.Definition;
import com.rusefi.util.SystemOut;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.nio.file.Files;
import java.util.List;
import java.util.zip.CRC32;

public class IoUtil2 {
    static String readFile(String fileName) {
        String line;
        StringBuilder stringBuilder = new StringBuilder();
        String ls = System.getProperty("line.separator");
        try {

            try (BufferedReader reader = new BufferedReader(new FileReader(fileName))) {
                while (true) {
                    if (!((line = reader.readLine()) != null)) break;
                    stringBuilder.append(line);
                    stringBuilder.append(ls);
                }

                return stringBuilder.toString();
            }
        } catch (IOException e) {
            return "";
        }
    }

    private static long getCrc32(String fileName) throws IOException {
        File file = new File(fileName);
        byte[] fileContent = Files.readAllBytes(file.toPath());
        for (int i = 0; i < fileContent.length; i++) {
            byte aByte = fileContent[i];
            if (aByte == '\r')
                throw new IllegalStateException("CR \\r 0x0D byte not allowed in cacheable content " + fileName + " at index=" + i);
        }
        CRC32 c = new CRC32();
        c.update(fileContent, 0, fileContent.length);
        return c.getValue();
    }


    static long getCrc32(List<String> inputFileNames) throws IOException {
        // get CRC32 of given input files
        long crc32 = 0;
        for (String fileName : inputFileNames) {
            long c = getCrc32(fileName) & 0xffffffffL;
            SystemOut.println("CRC32 from " + fileName + " = " + c);
            crc32 ^= c;
        }
        SystemOut.println("CRC32 from all input files = " + crc32);
        return crc32;
    }

    static void signatureHash(ReaderState state, ParseState parseState, String tsPath, long crc32) {
        // store the CRC32 as a built-in variable

        // nasty trick - do not insert signature into live data files
        if (tsPath != null) {
            state.variableRegistry.register(ConfigDefinition.SIGNATURE_HASH, "" + crc32);
            parseState.addDefinition(ConfigDefinition.SIGNATURE_HASH, Long.toString(crc32), Definition.OverwritePolicy.NotAllowed);
        }
    }
}
