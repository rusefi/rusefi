package com.rusefi.tools.tune;

import com.rusefi.RootHolder;
import org.jetbrains.annotations.NotNull;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;

public class FileLinesHelper {
    static @NotNull List<String> readAllLines(String pathRelativeToRoot) throws IOException {
        // some technical debt?! local runs (?!) seem to fail if relative paths is used?!
        // todo: maybe re-implement 'Files.readAllLines' without fragile 'Path' API?
        String absolutePath = new File(RootHolder.ROOT).getAbsolutePath();
        return Files.readAllLines(Paths.get(absolutePath + pathRelativeToRoot));
    }

    public static @NotNull List<String> readAllLinesWithRoot(String fileName) throws IOException {
        // 'getAbsolutePath' seems to somehow help some tests? something is weird
        return Files.readAllLines(Paths.get(new File(RootHolder.ROOT + fileName).getAbsolutePath()));
    }
}
