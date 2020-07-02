package com.rusefi.shared;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.URI;
import java.nio.file.*;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

public class FileUtil {
    public static void unzip(String zipFileName, String destPath) throws IOException {
        File destDir = new File(destPath);
        byte[] buffer = new byte[1024];
        ZipInputStream zis = new ZipInputStream(new FileInputStream(zipFileName));
        ZipEntry zipEntry = zis.getNextEntry();
        while (zipEntry != null) {
            File newFile = newFile(destDir, zipEntry);
            if (newFile.isDirectory()) {
                System.out.println("Nothing to do for directory " + newFile);
            } else {
                unzipFile(buffer, zis, newFile);
            }
            zipEntry = zis.getNextEntry();
        }
        zis.closeEntry();
        zis.close();
        System.out.println("Unzip " + zipFileName + " to " + destPath + " worked!");
    }

    public static byte [] unzipFileContents(String zipFileName, String fileName) throws IOException {
        ZipInputStream zis = new ZipInputStream(new FileInputStream(zipFileName));
        ZipEntry zipEntry;
        byte [] data = null;
        while ((zipEntry = zis.getNextEntry()) != null) {
            Path zippedName = Paths.get(zipEntry.getName()).normalize();
            Path searchName = Paths.get(fileName).normalize();
            if (zippedName.equals(searchName) && zipEntry.getSize() >= 0) {
                int offset = 0;
                byte [] tmpData = new byte[(int)zipEntry.getSize()];
                int bytesLeft = tmpData.length, bytesRead;
                while (bytesLeft > 0 && (bytesRead = zis.read(tmpData, offset, bytesLeft)) >= 0) {
                    offset += bytesRead;
                    bytesLeft -= bytesRead;
                }
                if (bytesLeft == 0) {
                    data = tmpData;
                } else {
                    System.out.println("Unzip: error extracting file " + fileName);
                }
                break;
            }
        }
        zis.closeEntry();
        zis.close();
        System.out.println("Unzip " + zipFileName + ": " + fileName + (data != null ? " extracted!" : " failed!"));
        return data;
    }

    public static boolean zipAddFiles(String zipFileName, List<String> fileNames, String zipPath) throws IOException {
        // requires Java7+
        Map<String, String> env = new HashMap<>();
        env.put("create", "true");
        Path path = Paths.get(zipFileName);
        URI uri = URI.create("jar:" + path.toUri());
        FileSystem fs = FileSystems.newFileSystem(uri, env);
        for (String fileName : fileNames) {
            String fileNameInZip = zipPath + File.separator + fileName;
            Path extFile = Paths.get(fileName);
            Path zippedFile = fs.getPath(fileNameInZip);
            Files.createDirectories(zippedFile.getParent());
            //fs.provider().checkAccess(zippedFile, AccessMode.READ);
            Files.copy(extFile, zippedFile, StandardCopyOption.REPLACE_EXISTING);
        }
        fs.close();
        return true;
    }

    private static void unzipFile(byte[] buffer, ZipInputStream zis, File newFile) throws IOException {
        System.out.println("Unzipping " + newFile);
        FileOutputStream fos = new FileOutputStream(newFile);
        int len;
        while ((len = zis.read(buffer)) > 0) {
            fos.write(buffer, 0, len);
        }
        fos.close();
    }

    private static File newFile(File destinationDir, ZipEntry zipEntry) throws IOException {
        File destFile = new File(destinationDir, zipEntry.getName());

        String destDirPath = destinationDir.getCanonicalPath();
        String destFilePath = destFile.getCanonicalPath();

        if (!destFilePath.startsWith(destDirPath + File.separator)) {
            throw new IOException("Entry is outside of the target dir: " + zipEntry.getName());
        }

        return destFile;
    }
}
