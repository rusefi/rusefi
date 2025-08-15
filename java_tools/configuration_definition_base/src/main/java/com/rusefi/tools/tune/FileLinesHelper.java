package com.rusefi.tools.tune;

import com.rusefi.RootHolder;
import org.jetbrains.annotations.NotNull;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class FileLinesHelper {
    public static @NotNull List<String> readAllLinesWithRoot(String fileName) throws IOException {
        return readAllLines2(RootHolder.ROOT + fileName);
    }

    // something is weird about Files.readAllLines turned much safer to have our own method
    public static List<String> readAllLines2(String fileName) throws IOException {
        try (BufferedReader reader = new BufferedReader(new FileReader(fileName))) {
            List<String> result = new ArrayList<>();
            for (;;) {
                String line = reader.readLine();
                if (line == null)
                    break;
                result.add(line);
            }
            return result;
        }
    }
}
