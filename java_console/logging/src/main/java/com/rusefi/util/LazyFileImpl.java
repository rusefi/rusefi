package com.rusefi.util;

import com.devexperts.logging.Logging;

import com.github.difflib.DiffUtils;
import com.github.difflib.UnifiedDiffUtils;
import com.github.difflib.patch.*;

import java.io.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;
import java.util.Arrays;
import java.util.List;
import java.util.Scanner;
import java.util.regex.Pattern;

import static com.devexperts.logging.Logging.getLogging;

/**
 * This file would override file content only of content has changed, disregarding the magic tag line.
 */
public class LazyFileImpl implements LazyFile {
    private static final Logging log = getLogging(LazyFileImpl.class);

    private final String filename;
    private final String patchfile;

    private final StringBuffer content = new StringBuffer();
    private final StringBuffer contentWithoutTag = new StringBuffer();

    public LazyFileImpl(String filename) {
        this(filename, null);
    }

		public LazyFileImpl(String filename, String patchfile) {
        this.filename = filename;
				this.patchfile = patchfile;
    }

    @Override
    public void write(String line) {
        content.append(line);
        String[] lines = line.split("\\r?\\n");
        for (String subLine : lines) {
            if (!subLine.toLowerCase().contains(LazyFile.LAZY_FILE_TAG_LOWER)) {
                contentWithoutTag.append(subLine + "\n");
            }
        }
    }

    public static String unifySpaces(String line) {
        line = line.replace("\r", "");
        return line.replaceAll("\n[\n]+", "\n");
    }

    @Override
    public void close() throws IOException {
        if (LazyFile.TEST.equals(filename))
            return;
        String fileContent = unifySpaces(readCurrentContent(filename));
        String newContent = unifySpaces(contentWithoutTag.toString());

        if (fileContent.equals(newContent)) {
            log.info(getClass().getSimpleName() + ": Not updating " + filename + " since looks to be the same content, new content size=" + contentWithoutTag.length());
            return;
        }
        for (int i = 0; i < Math.min(fileContent.length(), newContent.length()); i++) {
            if (fileContent.charAt(i) != newContent.charAt(i)) {
                log.info(getClass().getSimpleName() + " " + filename + ": Not same at " + i);
                if (i > 15 && i < fileContent.length() - 6 && i < newContent.length() - 6) {
                    log.info("file       " + fileContent.substring(i - 15, i + 5));
                    log.info("newContent " + newContent.substring(i - 15, i + 5));
                }
                break;
            }
        }

				if (patchfile != null && !patchfile.trim().isEmpty()) {
						List<String> fileLines = Arrays.asList(fileContent.split("\\r?\\n"));
						List<String> newLines  = Arrays.asList(newContent.split("\\r?\\n"));
						
						Patch<String> patch = DiffUtils.diff(fileLines, newLines);
						List<String> unifiedDiff = UnifiedDiffUtils.generateUnifiedDiff(
																																						"a/" + filename,
																																						"b/" + filename,
																																						fileLines,
																																						patch,
																																						3
																																						);

						Files.write(Paths.get(patchfile), unifiedDiff,
												StandardOpenOption.CREATE, StandardOpenOption.APPEND);
				}

        Files.createDirectories(Paths.get(new File(filename).getParent()));
        Writer fw = new PrintWriter(new OutputStreamWriter(new FileOutputStream(filename), IoUtils.CHARSET));
        fw.write(content.toString());
        fw.close();
    }

    private String readCurrentContent(String filename) throws IOException {
        if (LazyFile.TEST.equals(filename))
            return "";
        if (!new File(filename).exists()) {
            log.info(filename + " does not exist considering empty current content");
            return "";
        }
        Scanner in = new Scanner(Paths.get(filename), IoUtils.CHARSET.name());
        Pattern pat = Pattern.compile(".*\\R|.+\\z");
        String line;
        StringBuilder sb = new StringBuilder();
        while ((line = in.findWithinHorizon(pat, 0)) != null) {
            if (!line.toLowerCase().contains(LazyFile.LAZY_FILE_TAG_LOWER))
                sb.append(line);
        }
        return sb.toString();
    }
}
