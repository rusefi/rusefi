package com.rusefi.util;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.StandardCopyOption;
import java.util.Arrays;
import java.util.List;

public class CachingStrategy {
    public static boolean checkIfOutputFilesAreOutdated(List<String> inputFileNames, String cachePath, String cacheZipFile) {
        if (cachePath == null)
            return true;
        SystemOut.println("Check the input/output other files:");
        // find if any input file was changed from the cached version
        for (String inputFileName : inputFileNames) {
            File inputFile = new File(inputFileName);
            try {
                byte[] inputFileContent = Files.readAllBytes(inputFile.toPath());
                byte[] f2;
                if (cacheZipFile != null) {
                    f2 = ZipUtil.unzipFileContents(cacheZipFile, cachePath + File.separator + inputFileName);
                } else {
                    String cachedFileName = getCachedInputFileName(cachePath, inputFile.getName());
                    SystemOut.println("* cache ZIP file not specified, reading " + cachedFileName + " vs " + inputFileName);
                    /**
                     * todo: do we have a bug in this branch? how often do we simply read same 'inputFile'?
                     */
                    File cachedFile = new File(cachedFileName);
                    f2 = Files.readAllBytes(cachedFile.toPath());
                }
                boolean isEqual = Arrays.equals(inputFileContent, f2);
                if (!isEqual) {
                    SystemOut.println("* the file " + inputFileName + " is changed!");
                    return true;
                } else {
                    SystemOut.println("* the file " + inputFileName + " is NOT changed!");
                }
            } catch (IOException e) {
                SystemOut.println("* cannot validate the file " + inputFileName + ", so assuming it's changed.");
                return true;
            }
        }
        SystemOut.println("* all the files are up-to-date!");
        return false;
    }

    public static boolean saveCachedInputFiles(List<String> inputFiles, String cachePath, String cacheZipFile) throws IOException {
        if (cachePath == null) {
            SystemOut.println("* cache storage is disabled.");
            return false;
        }
        // copy all input files to the cache
        if (cacheZipFile != null) {
            ZipUtil.zipAddFiles(cacheZipFile, inputFiles, cachePath);
        } else {
            for (String iFile : inputFiles) {
                File newFile = new File(iFile);
                File cachedFile = new File(getCachedInputFileName(cachePath, newFile.getName()));
                cachedFile.mkdirs();
                try {
                    Files.copy(newFile.toPath(), cachedFile.toPath(), StandardCopyOption.REPLACE_EXISTING);
                } catch (IOException e) {
                    SystemOut.println("* cannot store the cached file for " + iFile);
                    throw e;
                }
            }
        }
        SystemOut.println("* input files copied to the cached folder");
        return true;
    }

    private static String getCachedInputFileName(String cachePath, String inputFile) {
        return cachePath + File.separator + inputFile;
    }
}
