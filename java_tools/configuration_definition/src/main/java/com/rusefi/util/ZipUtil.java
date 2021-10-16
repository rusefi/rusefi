package com.rusefi.util;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.net.URI;
import java.nio.file.*;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

public class ZipUtil {
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

    public static byte[] unzipFileContents(String zipFileName, String fileName) throws IOException {
        ZipInputStream zis = new ZipInputStream(new FileInputStream(zipFileName));
        ZipEntry zipEntry;
        byte[] data = null;
        while ((zipEntry = zis.getNextEntry()) != null) {
            Path zippedName = Paths.get(zipEntry.getName()).normalize();
            Path searchName = Paths.get(fileName).normalize();
            if (zippedName.equals(searchName)) {
                ByteArrayOutputStream baos = new ByteArrayOutputStream();
                byte[] buffer = new byte[4096];
                int read;
                while ((read = zis.read(buffer)) != -1) {
                    baos.write(buffer, 0, read);
                }
                data = baos.toByteArray();
                break;
            }
        }
        zis.closeEntry();
        zis.close();
        System.out.println("Unzip " + zipFileName + ": " + fileName + (data != null ? " extracted!" : " failed!"));
        return data;
    }
}
