package com.rusefi;

import java.io.*;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;
import java.util.Stack;

/**
 * (c) Andrey Belomutskiy
 * 1/12/15
 */
@SuppressWarnings("StringConcatenationInsideStringBufferAppend")
public class ConfigDefinition {
    public static final String EOL = "\n";
    private static final String INPUT_FILE_NAME = "rusefi_config.txt";
    private static final String MESSAGE = "was generated automatically by ConfigDefinition.jar based on " + INPUT_FILE_NAME + " " + new Date();
    private static final String TS_FILE_INPUT_NAME = "rusefi.input";
    private static final String TS_FILE_OUTPUT_NAME = "rusefi.ini";
    private static final String STRUCT_NO_PREFIX = "struct_no_prefix ";
    private static final String STRUCT = "struct ";
    private static final String END_STRUCT = "end_struct";
    private static final String CUSTOM = "custom";
    private static final String DEFINE = "#define";
    private static final String BIT = "bit";
    private static final String CONFIG_DEFINITION_START = "CONFIG_DEFINITION_START";
    private static final String CONFIG_DEFINITION_END = "CONFIG_DEFINITION_END";
    private static final String ROM_RAIDER_XML = "rusefi.xml";
    private static final String ENGINE_CONFIGURATION_GENERATED_STRUCTURES_H = "engine_configuration_generated_structures.h";
    private static final String FIELDS_JAVA = "models/src/com/rusefi/config/Fields.java";
    private static int totalTsSize;

    private static Stack<ConfigStructure> stack = new Stack<>();
    public static Map<String, ConfigStructure> structures = new HashMap<>();
    public static Map<String, String> tsCustomLine = new HashMap<>();
    public static Map<String, Integer> tsCustomSize = new HashMap<>();
    public static StringBuilder settingContextHelp = new StringBuilder();

    public static void main(String[] args) throws IOException {
        if (args.length != 4) {
            System.out.println("Please specify path to '" + INPUT_FILE_NAME + "' file, path to " + TS_FILE_INPUT_NAME +
                    " and destination folder");
            return;
        }

        String inputPath = args[0];
        String tsPath = args[1];
        String headerDestinationFolder = args[2];
        String javaConsolePath = args[3];
        String fullFileName = inputPath + File.separator + INPUT_FILE_NAME;
        System.out.println("Reading from " + fullFileName);
        String destCHeader = headerDestinationFolder + File.separator + ENGINE_CONFIGURATION_GENERATED_STRUCTURES_H;
        System.out.println("Writing C header to " + destCHeader);

        BufferedWriter cHeader = new BufferedWriter(new FileWriter(destCHeader));

        BufferedReader br = new BufferedReader(new FileReader(fullFileName));

        CharArrayWriter tsWriter = new CharArrayWriter();

        CharArrayWriter javaFieldsWriter = new CharArrayWriter();

        processFile(br, cHeader, tsWriter, javaFieldsWriter);

        BufferedWriter javaFields = new BufferedWriter(new FileWriter(javaConsolePath + File.separator + FIELDS_JAVA));
        javaFields.write("package com.rusefi.config;" + EOL + EOL);
        javaFields.write("// this file " + MESSAGE + EOL);
        javaFields.write("public class Fields {" + EOL);
        javaFields.write(VariableRegistry.INSTANCE.getJavaConstants());
        javaFields.write(javaFieldsWriter.toString());
        javaFields.write("}" + EOL);
        javaFields.close();


        BufferedWriter tsHeader = writeTunerStudioFile(tsPath, tsWriter.toString());

        if (!stack.isEmpty())
            throw new IllegalStateException("Unclosed structure: " + stack.peek().name);

        cHeader.close();
        tsHeader.close();

        VariableRegistry.INSTANCE.writeNumericsToFile(headerDestinationFolder);

        processTextTemplate(inputPath + File.separator + ROM_RAIDER_XML, javaConsolePath + File.separator + ROM_RAIDER_XML);
    }

    private static BufferedWriter writeTunerStudioFile(String tsPath, String fieldsSection) throws IOException {
        TsFileContent tsContent = readTsFile(tsPath);
        System.out.println("Got " + tsContent.getPrefix().length() + "/" + tsContent.getPostfix().length() + " of " + TS_FILE_INPUT_NAME);

        BufferedWriter tsHeader = new BufferedWriter(new FileWriter(tsPath + File.separator + TS_FILE_OUTPUT_NAME));
        tsHeader.write(tsContent.getPrefix());

        tsHeader.write("; " + CONFIG_DEFINITION_START + ConfigDefinition.EOL);
        tsHeader.write("; this section " + MESSAGE + ConfigDefinition.EOL + ConfigDefinition.EOL);
        tsHeader.write("pageSize            = " + totalTsSize + ConfigDefinition.EOL);
        tsHeader.write("page = 1" + ConfigDefinition.EOL);
        tsHeader.write(fieldsSection);
        if (settingContextHelp.length() > 0) {
            tsHeader.write("[SettingContextHelp]" + ConfigDefinition.EOL);
            tsHeader.write(settingContextHelp.toString() + ConfigDefinition.EOL + ConfigDefinition.EOL);
        }
        tsHeader.write("; " + CONFIG_DEFINITION_END + ConfigDefinition.EOL);
        tsHeader.write(tsContent.getPostfix());
        return tsHeader;
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

    private static TsFileContent readTsFile(String tsPath) throws IOException {
        BufferedReader r = new BufferedReader(new FileReader(tsPath + File.separator + TS_FILE_INPUT_NAME));

        StringBuilder prefix = new StringBuilder();
        StringBuilder postfix = new StringBuilder();

        boolean isBeforeStartTag = true;
        boolean isAfterEndTag = false;
        String line;
        while ((line = r.readLine()) != null) {
            if (line.contains(CONFIG_DEFINITION_START)) {
                isBeforeStartTag = false;
                continue;
            }
            if (line.contains(CONFIG_DEFINITION_END)) {
                isAfterEndTag = true;
                continue;
            }

            if (isBeforeStartTag)
                prefix.append(line + ConfigDefinition.EOL);

            if (isAfterEndTag)
                postfix.append(VariableRegistry.INSTANCE.applyVariables(line) + ConfigDefinition.EOL);
        }
        r.close();
        return new TsFileContent(prefix.toString(), postfix.toString());
    }

    private static void processFile(BufferedReader br, BufferedWriter cHeader, Writer tsHeader, CharArrayWriter javaFieldsWriter) throws IOException {
        String line;

        String message = "// this section " + MESSAGE + EOL;
        cHeader.write(message);
        cHeader.write("// begin" + EOL);
        cHeader.write("#ifndef ENGINE_CONFIGURATION_GENERATED_H_" + EOL);
        cHeader.write("#define ENGINE_CONFIGURATION_GENERATED_H_" + EOL);
        cHeader.write("#include \"rusefi_types.h\"" + EOL);

        while ((line = br.readLine()) != null) {
            line = line.trim();
            line = line.replaceAll("\\s+", " ");
            /**
             * we should ignore empty lines and comments
             */
            if (line.length() == 0 || line.startsWith("!"))
                continue;

            if (line.startsWith(STRUCT)) {
                handleStartStructure(line.substring(STRUCT.length()), true);
            } else if (line.startsWith(STRUCT_NO_PREFIX)) {
                handleStartStructure(line.substring(STRUCT_NO_PREFIX.length()), false);
            } else if (line.startsWith(END_STRUCT)) {
                handleEndStruct(cHeader, tsHeader, javaFieldsWriter);
            } else if (line.startsWith(BIT)) {
                line = line.substring(BIT.length() + 1).trim();

                String bitName;
                String comment;
                if (!line.contains(";")) {
                    bitName = line;
                    comment = "";
                } else {
                    int index = line.indexOf(";");
                    bitName = line.substring(0, index);
                    comment = line.substring(index + 1);
                }

                ConfigField bitField = new ConfigField(bitName, comment, true, null, null, 0, null, false);
                stack.peek().addBoth(bitField);

            } else if (startsWithToken(line, CUSTOM)) {
                line = line.substring(CUSTOM.length() + 1).trim();
                int index = line.indexOf(' ');
                String name = line.substring(0, index);
                line = line.substring(index).trim();
                index = line.indexOf(' ');
                String customSize = line.substring(0, index);

                String tunerStudioLine = line.substring(index).trim();
                tunerStudioLine = VariableRegistry.INSTANCE.applyVariables(tunerStudioLine);
                int size;
                try {
                    size = Integer.parseInt(customSize);
                } catch (NumberFormatException e) {
                    throw new IllegalStateException("Size in " + line);
                }
                tsCustomSize.put(name, size);
                tsCustomLine.put(name, tunerStudioLine);

            } else {
                processLine(line);
            }
        }
        cHeader.write("#endif" + EOL);
        cHeader.write("// end" + EOL);
        cHeader.write(message);
    }

    private static boolean startsWithToken(String line, String token) {
        return line.startsWith(token + " ") || line.startsWith(token + "\t");
    }

    private static void handleStartStructure(String line, boolean withPrefix) {
        String name;
        String comment;
        if (line.contains(" ")) {
            int index = line.indexOf(' ');
            name = line.substring(0, index);
            comment = line.substring(index + 1).trim();
        } else {
            name = line;
            comment = null;
        }
        ConfigStructure structure = new ConfigStructure(name, comment, withPrefix);
        stack.push(structure);
        System.out.println("Starting structure " + structure.name);
    }

    private static void handleEndStruct(Writer cHeader, Writer tsHeader, CharArrayWriter javaFieldsWriter) throws IOException {
        if (stack.isEmpty())
            throw new IllegalStateException("Unexpected end_struct");
        ConfigStructure structure = stack.pop();
        System.out.println("Ending structure " + structure.name);
        structure.addAlignmentFill();

        ConfigDefinition.structures.put(structure.name, structure);

        structure.headerWrite(cHeader);

        if (stack.isEmpty()) {
            totalTsSize = structure.writeTunerStudio("", tsHeader, 0);
            tsHeader.write("; total TS size = " + totalTsSize + EOL);
            VariableRegistry.INSTANCE.register("TOTAL_CONFIG_SIZE", totalTsSize);

            structure.writeJavaFields("", javaFieldsWriter, 0);
        }
    }

    private static void processLine(String line) throws IOException {
        /**
         * for example
         * #define CLT_CURVE_SIZE 16
         */
        if (startsWithToken(line, DEFINE)) {
            processDefine(line.substring(DEFINE.length()).trim());
            return;
        }

        ConfigField cf = ConfigField.parse(line);
        if (cf == null)
            throw new IllegalStateException("Cannot parse line [" + line + "]");

        if (stack.isEmpty())
            throw new IllegalStateException(cf.name + ": Not enclosed in a struct");
        ConfigStructure structure = stack.peek();

        if (cf.isIterate) {
            structure.addC(cf);
            for (int i = 1; i <= cf.arraySize; i++) {
                ConfigField element = new ConfigField(cf.name + i, cf.comment, false, null,
                        cf.type, 1, cf.tsInfo, false);
                structure.addTs(element);
            }
        } else {
            structure.addBoth(cf);
        }
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
        if (VariableRegistry.INSTANCE.intValues.containsKey(s))
            return VariableRegistry.INSTANCE.intValues.get(s);
        return Integer.parseInt(s);
    }

    private static void processDefine(String line) {
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
}
