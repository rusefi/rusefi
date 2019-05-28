package com.rusefi;

import com.rusefi.output.CHeaderConsumer;
import com.rusefi.output.ConfigurationConsumer;
import com.rusefi.output.JavaFieldsConsumer;
import com.rusefi.output.TSProjectConsumer;

import java.io.*;
import java.math.BigInteger;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

/**
 * (c) Andrey Belomutskiy
 * 1/12/15
 */
@SuppressWarnings("StringConcatenationInsideStringBufferAppend")
public class ConfigDefinition {
    public static final String EOL = "\n";
    public static String MESSAGE;

    private static final String ROM_RAIDER_XML_TEMPLATE = "rusefi_template.xml";
    private static final String ROM_RAIDER_XML_OUTPUT = "rusefi.xml";
    private static final String KEY_DEFINITION = "-definition";
    private static final String KEY_ROM_INPUT = "-romraider";
    private static final String KEY_TS_DESTINATION = "-ts_destination";
    private static final String KEY_C_DESTINATION = "-c_destination";
    private static final String KEY_C_DEFINES = "-c_defines";
    private static final String KEY_CONSOLE_DESTINATION = "-java_destination";
    private static final String KEY_PREPEND = "-prepend";
    private static final String KEY_SKIP = "-skip";

    public static StringBuilder settingContextHelp = new StringBuilder();

    public static void main(String[] args) throws IOException {
        if (args.length < 2) {
            System.out.println("Please specify\r\n"
                    + KEY_DEFINITION + " x\r\n"
                    + KEY_TS_DESTINATION + " x\r\n"
                    + KEY_C_DESTINATION + " x\r\n"
                    + KEY_CONSOLE_DESTINATION + " x\r\n"
            );
            return;
        }

        String definitionInputFile = null;
        String tsPath = null;
        String destCHeader = null;
        String destCDefines = null;
        String javaConsolePath = null;
        String prependFile = null;
        String skipRebuildFile = null;
        String romRaiderInputFile = null;

        for (int i = 0; i < args.length - 1; i += 2) {
            String key = args[i];
            if (key.equals(KEY_DEFINITION)) {
                definitionInputFile = args[i + 1];
            } else if (key.equals(KEY_TS_DESTINATION)) {
                tsPath = args[i + 1];
            } else if (key.equals(KEY_C_DESTINATION)) {
                destCHeader = args[i + 1];
            } else if (key.equals(KEY_C_DEFINES)) {
                destCDefines = args[i + 1];
            } else if (key.equals(KEY_CONSOLE_DESTINATION)) {
                javaConsolePath = args[i + 1];
            } else if (key.equals(KEY_PREPEND)) {
                prependFile = args[i + 1];
            } else if (key.equals(KEY_SKIP)) {
                skipRebuildFile = args[i + 1];
            } else if (key.equals(KEY_ROM_INPUT)) {
                romRaiderInputFile = args[i + 1];
            }
        }

        MESSAGE = "was generated automatically by ConfigDefinition.jar based on " + definitionInputFile + " " + new Date();

        System.out.println("Reading from " + definitionInputFile);

        String currentMD5 = getDefinitionMD5(definitionInputFile);

        if (skipRebuildFile != null) {
            boolean nothingToDoHere = needToSkipRebuild(skipRebuildFile, currentMD5);
            if (nothingToDoHere) {
                System.out.println("Nothing to do here according to " + skipRebuildFile + " hash " + currentMD5);
                return;
            }
        }

        if (prependFile != null)
            readPrependValues(prependFile);

        BufferedReader definitionReader = new BufferedReader(new FileReader(definitionInputFile));
        ReaderState state = new ReaderState();

        List<ConfigurationConsumer> destinations = new ArrayList<>();
        if (destCHeader != null) {
            destinations.add(new CHeaderConsumer(destCHeader));
        }
        if (tsPath != null) {
            CharArrayWriter tsWriter = new CharArrayWriter();
            destinations.add(new TSProjectConsumer(tsWriter, tsPath, state));
        }
        if (javaConsolePath != null) {
            CharArrayWriter javaFieldsWriter = new CharArrayWriter();
            destinations.add(new JavaFieldsConsumer(javaFieldsWriter, state, javaConsolePath));
        }

        if (destinations.isEmpty())
            throw new IllegalArgumentException("No destinations specified");
        state.readBufferedReader(definitionReader, destinations);

        state.ensureEmptyAfterProcessing();


        if (destCDefines != null)
            VariableRegistry.INSTANCE.writeNumericsToFile(destCDefines);

        if (javaConsolePath != null && romRaiderInputFile != null) {
            String inputFileName = romRaiderInputFile + File.separator + ROM_RAIDER_XML_TEMPLATE;
            String outputFileName = javaConsolePath + File.separator + ROM_RAIDER_XML_OUTPUT;
            processTextTemplate(inputFileName, outputFileName);
        }
        if (skipRebuildFile != null) {
            System.out.println("Writing " + currentMD5 + " to " + skipRebuildFile);
            PrintWriter writer = new PrintWriter(new FileWriter(skipRebuildFile));
            writer.write(currentMD5);
            writer.close();
        }
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
        byte content[] = new byte[(int) source.length()];
        if (fileInputStream.read(content) != content.length)
            return "";
        return getMd5(content);
    }

    private static void readPrependValues(String prependFile) throws IOException {
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
                processDefine(line.substring(ReaderState.DEFINE.length()).trim());
            }

        }
    }

    private static void processTextTemplate(String inputFileName, String outputFileName) throws IOException {
        System.out.println("Reading from " + inputFileName);
        System.out.println("Writing to " + outputFileName);

        VariableRegistry.INSTANCE.put("generator_message", "Generated by ConfigDefinition utility on " + new Date());

        File inputFile = new File(inputFileName);

        File outputFile = new File(outputFileName);
        BufferedReader fr = new BufferedReader(new FileReader(inputFile));
        FileWriter fw = new FileWriter(outputFile);

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

    static void processDefine(String line) {
        int index = line.indexOf(' ');
        String name;
        if (index == -1) {
            name = line;
            line = "";
        } else {
            name = line.substring(0, index);
            line = line.substring(index).trim();
        }
        VariableRegistry.INSTANCE.register(name, line);
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
}
