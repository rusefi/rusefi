package com.rusefi;

import com.rusefi.output.*;
import com.rusefi.util.IoUtils;
import com.rusefi.util.LazyFile;
import com.rusefi.util.SystemOut;

import java.io.*;
import java.math.BigInteger;
import java.net.URI;
import java.nio.file.*;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.*;
import java.util.zip.CRC32;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 1/12/15
 */
@SuppressWarnings("StringConcatenationInsideStringBufferAppend")
public class ConfigDefinition {
    public static final String EOL = "\n";
    private static final String SIGNATURE_HASH = "SIGNATURE_HASH";
    public static String MESSAGE;

    public static String TOOL = "(unknown script)";
    private static final String ROM_RAIDER_XML_TEMPLATE = "rusefi_template.xml";
    public static final String KEY_DEFINITION = "-definition";
    private static final String KEY_ROM_INPUT = "-romraider";
    public static final String KEY_TS_DESTINATION = "-ts_destination";
    private static final String KEY_C_DESTINATION = "-c_destination";
    private static final String KEY_C_FSIO_CONSTANTS = "-c_fsio_constants";
    private static final String KEY_C_FSIO_GETTERS = "-c_fsio_getters";
    private static final String KEY_C_FSIO_NAMES = "-c_fsio_names";
    private static final String KEY_C_FSIO_STRING = "-c_fsio_strings";
    private static final String KEY_C_DEFINES = "-c_defines";
    private static final String KEY_WITH_C_DEFINES = "-with_c_defines";
    private static final String KEY_JAVA_DESTINATION = "-java_destination";
    private static final String KEY_ROMRAIDER_DESTINATION = "-romraider_destination";
    private static final String KEY_FIRING = "-firing_order";
    public static final String KEY_PREPEND = "-prepend";
    public static final String KEY_SIGNATURE = "-signature";
    public static final String KEY_SIGNATURE_DESTINATION = "-signature_destination";
    public static final String KEY_CACHE = "-cache";
    public static final String KEY_CACHE_ZIP_FILE = "-cache_zip_file";
    private static final String KEY_SKIP = "-skip";
    private static final String KEY_ZERO_INIT = "-initialize_to_zero";
    public static boolean needZeroInit = true;
    public static String definitionInputFile = null;

    public static String getGeneratedAutomaticallyTag() {
        return LazyFile.LAZY_FILE_TAG + "ConfigDefinition.jar based on " + TOOL + " ";
    }

    public static void main(String[] args) {
        try {
            doJob(args);
        } catch (Throwable e) {
            SystemOut.println(e);
            e.printStackTrace();
            SystemOut.close();
            System.exit(-1);
        }
        SystemOut.close();
    }

    private static void doJob(String[] args) throws IOException {
        if (args.length < 2) {
            SystemOut.println("Please specify\r\n"
                    + KEY_DEFINITION + " x\r\n"
                    + KEY_TS_DESTINATION + " x\r\n"
                    + KEY_C_DESTINATION + " x\r\n"
                    + KEY_JAVA_DESTINATION + " x\r\n"
            );
            return;
        }

        SystemOut.println("Invoked with " + Arrays.toString(args));

        String tsPath = null;
        String destCHeaderFileName = null;
        String destCDefinesFileName = null;
        String destCFsioConstantsFileName = null;
        String destCFsioGettersFileName = null;
        String namesCFileName = null;
        String stringsCFileName = null;
        String javaDestinationFileName = null;
        String romRaiderDestination = null;
        List<String> prependFiles = new ArrayList<>();
        String skipRebuildFile = null;
        String romRaiderInputFile = null;
        String firingEnumFileName = null;
        String cachePath = null;
        String cacheZipFile = null;
        String signatureDestination = null;
        String signaturePrependFile = null;
        CHeaderConsumer.withC_Defines = true;

        // used to update other files
        List<String> inputFiles = new ArrayList<>();
        // disable the lazy checks because we use timestamps to detect changes
        LazyFile.setLazyFileEnabled(false);

        ReaderState state = new ReaderState();

        for (int i = 0; i < args.length - 1; i += 2) {
            String key = args[i];
            switch (key) {
                case "-tool":
                    ConfigDefinition.TOOL = args[i + 1];
                    break;
                case KEY_DEFINITION:
                    definitionInputFile = args[i + 1];
                    inputFiles.add(definitionInputFile);
                    break;
                case KEY_TS_DESTINATION:
                    tsPath = args[i + 1];
                    break;
                case KEY_C_DESTINATION:
                    destCHeaderFileName = args[i + 1];
                    break;
                case KEY_C_FSIO_GETTERS:
                    destCFsioGettersFileName = args[i + 1];
                    break;
                case KEY_C_FSIO_STRING:
                    stringsCFileName = args[i + 1];
                    break;
                case KEY_C_FSIO_NAMES:
                    namesCFileName = args[i + 1];
                    break;
                case KEY_C_FSIO_CONSTANTS:
                    destCFsioConstantsFileName = args[i + 1];
                    break;
                case KEY_ZERO_INIT:
                    needZeroInit = Boolean.parseBoolean(args[i + 1]);
                    break;
                case KEY_WITH_C_DEFINES:
                    CHeaderConsumer.withC_Defines = Boolean.parseBoolean(args[i + 1]);
                    break;
                case KEY_C_DEFINES:
                    destCDefinesFileName = args[i + 1];
                    break;
                case KEY_JAVA_DESTINATION:
                    javaDestinationFileName = args[i + 1];
                    break;
                case KEY_FIRING:
                    firingEnumFileName = args[i + 1];
                    inputFiles.add(firingEnumFileName);
                    break;
                case KEY_ROMRAIDER_DESTINATION:
                    romRaiderDestination = args[i + 1];
                    break;
                case KEY_PREPEND:
                    prependFiles.add(args[i + 1]);
                    inputFiles.add(args[i + 1]);
                    break;
                case KEY_SIGNATURE:
                    signaturePrependFile = args[i + 1];
                    prependFiles.add(args[i + 1]);
                    // don't add this file to the 'inputFiles'
                    break;
                case KEY_SIGNATURE_DESTINATION:
                    signatureDestination = args[i + 1];
                    break;
                case EnumToString.KEY_ENUM_INPUT_FILE:
                    String inputFile = args[i + 1];
                    state.enumsReader.process(".", inputFile);
                    SystemOut.println(state.enumsReader.getEnums() + " total enumsReader");
                    break;
                case KEY_CACHE:
                    cachePath = args[i + 1];
                    break;
                case KEY_CACHE_ZIP_FILE:
                    cacheZipFile = args[i + 1];
                    break;
                case KEY_SKIP:
                    // is this now not needed in light if LazyFile surving the same goal of not changing output unless needed?
                    skipRebuildFile = args[i + 1];
                    break;
                case "-ts_output_name":
                    TSProjectConsumer.TS_FILE_OUTPUT_NAME = args[i + 1];
                    break;
                case KEY_ROM_INPUT:
                    String inputFilePath = args[i + 1];
                    romRaiderInputFile = inputFilePath + File.separator + ROM_RAIDER_XML_TEMPLATE;
                    inputFiles.add(romRaiderInputFile);
                    break;
            }
        }

        // used to update .ini files
        List<String> inputAllFiles = new ArrayList<>(inputFiles);
        boolean needToUpdateTsFiles = false;
        if (tsPath != null) {
            inputAllFiles.add(TSProjectConsumer.getTsFileInputName(tsPath));
        }

        if (tsPath != null) {
            SystemOut.println("Check the input/output TS files:");
            needToUpdateTsFiles = checkIfOutputFilesAreOutdated(inputAllFiles, cachePath, cacheZipFile);
        }
        SystemOut.println("Check the input/output other files:");
        boolean needToUpdateOtherFiles = checkIfOutputFilesAreOutdated(inputFiles, cachePath, cacheZipFile);
        if (!needToUpdateTsFiles && !needToUpdateOtherFiles) {
            SystemOut.println("All output files are up-to-date, nothing to do here!");
            return;
        }

        // get CRC32 of given input files
        long crc32 = 0;
        for (String iFile : inputAllFiles) {
            long c = getCrc32(iFile) & 0xffffffffL;
            SystemOut.println("CRC32 from " + iFile + " = " + c);
            crc32 ^= c;
        }
        SystemOut.println("CRC32 from all input files = " + crc32);
        // store the CRC32 as a built-in variable
        if (tsPath != null) // nasty trick - do not insert signature into live data files
            VariableRegistry.INSTANCE.register(SIGNATURE_HASH, "" + crc32);

        if (firingEnumFileName != null) {
            SystemOut.println("Reading firing from " + firingEnumFileName);
            VariableRegistry.INSTANCE.register("FIRINGORDER", FiringOrderTSLogic.invoke(firingEnumFileName));
        }
        MESSAGE = getGeneratedAutomaticallyTag() + definitionInputFile + " " + new Date();

        SystemOut.println("Reading definition from " + definitionInputFile);

        String currentMD5 = getDefinitionMD5(definitionInputFile);

        if (skipRebuildFile != null) {
            boolean nothingToDoHere = needToSkipRebuild(skipRebuildFile, currentMD5);
            if (nothingToDoHere) {
                SystemOut.println("Nothing to do here according to " + skipRebuildFile + " hash " + currentMD5);
                return;
            }
        }

        for (String prependFile : prependFiles)
            readPrependValues(VariableRegistry.INSTANCE, prependFile);

        BufferedReader definitionReader = new BufferedReader(new InputStreamReader(new FileInputStream(definitionInputFile), IoUtils.CHARSET.name()));

        List<ConfigurationConsumer> destinations = new ArrayList<>();
        if (tsPath != null && needToUpdateTsFiles) {
            CharArrayWriter tsWriter = new CharArrayWriter();
            destinations.add(new TSProjectConsumer(tsWriter, tsPath, state));

            VariableRegistry tmpRegistry = new VariableRegistry();
            // store the CRC32 as a built-in variable
            tmpRegistry.register(SIGNATURE_HASH, "" + crc32);
            readPrependValues(tmpRegistry, signaturePrependFile);
            destinations.add(new SignatureConsumer(signatureDestination, tmpRegistry));
        }
        if (needToUpdateOtherFiles) {
            if (destCHeaderFileName != null) {
                destinations.add(new CHeaderConsumer(destCHeaderFileName));
            }
            if (javaDestinationFileName != null) {
                destinations.add(new FileJavaFieldsConsumer(state, javaDestinationFileName));
            }

            if (destCFsioConstantsFileName != null || destCFsioGettersFileName != null) {
                destinations.add(new FileFsioSettingsConsumer(state,
                        destCFsioConstantsFileName,
                        destCFsioGettersFileName,
                        namesCFileName,
                        stringsCFileName));
            }
        }

        if (destinations.isEmpty())
            throw new IllegalArgumentException("No destinations specified");
        state.readBufferedReader(definitionReader, destinations);


        if (destCDefinesFileName != null && needToUpdateOtherFiles)
            VariableRegistry.INSTANCE.writeDefinesToFile(destCDefinesFileName);

        if (romRaiderDestination != null && romRaiderInputFile != null && needToUpdateOtherFiles) {
            processTextTemplate(romRaiderInputFile, romRaiderDestination);
        }
        if (skipRebuildFile != null) {
            SystemOut.println("Writing " + currentMD5 + " to " + skipRebuildFile);
            PrintWriter writer = new PrintWriter(new FileWriter(skipRebuildFile));
            writer.write(currentMD5);
            writer.close();
        }

        saveCachedInputFiles(inputAllFiles, cachePath, cacheZipFile);
    }

    private static boolean needToSkipRebuild(String skipRebuildFile, String currentMD5) throws IOException {
        if (currentMD5 == null || !(new File(skipRebuildFile).exists()))
            return false;
        String finishedMD5 = new BufferedReader(new FileReader(skipRebuildFile)).readLine();
        return finishedMD5 != null && finishedMD5.equals(currentMD5);
    }

    private static String getDefinitionMD5(String fullFileName) throws IOException {
        File source = new File(fullFileName);
        FileInputStream fileInputStream = new FileInputStream(fullFileName);
        byte[] content = new byte[(int) source.length()];
        if (fileInputStream.read(content) != content.length)
            return "";
        return getMd5(content);
    }

    public static void readPrependValues(VariableRegistry registry, String prependFile) throws IOException {
        BufferedReader definitionReader = new BufferedReader(new FileReader(prependFile));
        String line;
        while ((line = definitionReader.readLine()) != null) {
            line = trimLine(line);
            /**
             * we should ignore empty lines and comments
             */
            if (ReaderState.isEmptyDefinitionLine(line))
                continue;
            if (startsWithToken(line, ReaderState.DEFINE)) {
                processDefine(registry, line.substring(ReaderState.DEFINE.length()).trim());
            }
        }
    }

    private static void processTextTemplate(String inputFileName, String outputFileName) throws IOException {
        SystemOut.println("Reading from " + inputFileName);
        SystemOut.println("Writing to " + outputFileName);

        VariableRegistry.INSTANCE.put("generator_message", ConfigDefinition.getGeneratedAutomaticallyTag() + new Date());

        File inputFile = new File(inputFileName);

        BufferedReader fr = new BufferedReader(new FileReader(inputFile));
        LazyFile fw = new LazyFile(outputFileName);

        String line;
        while ((line = fr.readLine()) != null) {
            line = VariableRegistry.INSTANCE.applyVariables(line);
            fw.write(line + ConfigDefinition.EOL);
        }
        fw.close();
    }

    static String trimLine(String line) {
        line = line.trim();
        line = line.replaceAll("\\s+", " ");
        return line;
    }

    static boolean startsWithToken(String line, String token) {
        return line.startsWith(token + " ") || line.startsWith(token + "\t");
    }


    public static String getComment(String comment, int currentOffset) {
        return "\t/**" + EOL + packComment(comment, "\t") + "\t * offset " + currentOffset + EOL + "\t */" + EOL;
    }

    public static String packComment(String comment, String linePrefix) {
        if (comment == null)
            return "";
        if (comment.trim().isEmpty())
            return "";
        String result = "";
        for (String line : comment.split("\\\\n")) {
            result += linePrefix + " * " + line + EOL;
        }
        return result;
    }

    public static int getSize(String s) {
        if (VariableRegistry.INSTANCE.intValues.containsKey(s)) {
            return VariableRegistry.INSTANCE.intValues.get(s);
        }
        return Integer.parseInt(s);
    }

    static void processDefine(VariableRegistry registry, String line) {
        int index = line.indexOf(' ');
        String name;
        if (index == -1) {
            name = line;
            line = "";
        } else {
            name = line.substring(0, index);
            line = line.substring(index).trim();
        }
        if (VariableRegistry.isNumeric(line)) {
            Integer v = Integer.valueOf(line);
            registry.register(name, v);
        } else {
            registry.register(name, line);
        }
    }

    private static String getMd5(byte[] content) {
        try {
            // Static getInstance method is called with hashing MD5
            MessageDigest md = MessageDigest.getInstance("MD5");

            // digest() method is called to calculate message digest
            //  of an input digest() return array of byte
            byte[] messageDigest = md.digest(content);

            // Convert byte array into signum representation
            BigInteger no = new BigInteger(1, messageDigest);

            // Convert message digest into hex value
            String hashtext = no.toString(16);
            while (hashtext.length() < 32) {
                hashtext = "0" + hashtext;
            }
            return hashtext;
        } catch (NoSuchAlgorithmException e) {
            // For specifying wrong message digest algorithms
            throw new RuntimeException(e);
        }
    }

    private static boolean checkIfOutputFilesAreOutdated(List<String> inputFiles, String cachePath, String cacheZipFile) {
        if (cachePath == null)
            return true;
        // find if any input file was changed from the cached version
        for (String iFile : inputFiles) {
            File newFile = new File(iFile);
            try {
                byte[] f1 = Files.readAllBytes(newFile.toPath());
                byte[] f2;
                if (cacheZipFile != null) {
                    f2 = unzipFileContents(cacheZipFile, cachePath + File.separator + iFile);
                } else {
                    String cachedFileName = getCachedInputFileName(newFile.getName(), cachePath);
                    File cachedFile = new File(cachedFileName);
                    f2 = Files.readAllBytes(cachedFile.toPath());
                }
                boolean isEqual = Arrays.equals(f1, f2);
                if (!isEqual) {
                    SystemOut.println("* the file " + iFile + " is changed!");
                    return true;
                } else {
                    SystemOut.println("* the file " + iFile + " is NOT changed!");
                }
            } catch (java.io.IOException e) {
                SystemOut.println("* cannot validate the file " + iFile + ", so assuming it's changed.");
                return true;
            }
        }
        SystemOut.println("* all the files are up-to-date!");
        return false;
    }

    private static boolean saveCachedInputFiles(List<String> inputFiles, String cachePath, String cacheZipFile) throws IOException {
        if (cachePath == null) {
            SystemOut.println("* cache storage is disabled.");
            return false;
        }
        // copy all input files to the cache
        if (cacheZipFile != null) {
            zipAddFiles(cacheZipFile, inputFiles, cachePath);
        } else {
            for (String iFile : inputFiles) {
                File newFile = new File(iFile);
                File cachedFile = new File(getCachedInputFileName(newFile.getName(), cachePath));
                cachedFile.mkdirs();
                try {
                    Files.copy(newFile.toPath(), cachedFile.toPath(), StandardCopyOption.REPLACE_EXISTING);
                } catch (java.io.IOException e) {
                    SystemOut.println("* cannot store the cached file for " + iFile);
                    throw e;
                }
            }
        }
        SystemOut.println("* input files copied to the cached folder");
        return true;
    }

    private static String getCachedInputFileName(String inputFile, String cachePath) {
        return cachePath + File.separator + inputFile;
    }

    private static long getCrc32(String fileName) throws IOException {
        File file = new File(fileName);
        byte[] f1 = Files.readAllBytes(file.toPath());
        CRC32 c = new CRC32();
        c.update(f1, 0, f1.length);
        return c.getValue();
    }

    private static void deleteFile(String fileName) throws IOException {
        File file = new File(fileName);
        // todo: validate?
        file.delete();
    }

    private static byte[] unzipFileContents(String zipFileName, String fileName) throws IOException {
        ZipInputStream zis = new ZipInputStream(new FileInputStream(zipFileName));
        ZipEntry zipEntry;
        byte[] data = null;
        while ((zipEntry = zis.getNextEntry()) != null) {
            Path zippedName = Paths.get(zipEntry.getName()).normalize();
            Path searchName = Paths.get(fileName).normalize();
            if (zippedName.equals(searchName) && zipEntry.getSize() >= 0) {
                int offset = 0;
                byte[] tmpData = new byte[(int) zipEntry.getSize()];
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

    private static boolean zipAddFiles(String zipFileName, List<String> fileNames, String zipPath) throws IOException {
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
}
