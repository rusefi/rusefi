package com.rusefi.ldmp;

import com.rusefi.util.SystemOut;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.*;
import java.util.stream.Stream;

import static com.rusefi.ConfigDefinition.EOL;

public class LiveDocsMetaParser {
    private static final String DISPLAY_TAG = "DISPLAY_TAG";
    private static final String DISPLAY_CONFIG = "DISPLAY_CONFIG";
    private static final String DISPLAY_PREFIX = "DISPLAY_PREFIX";
    private static final String DISPLAY_FIELD = "DISPLAY_FIELD";
    private static final String DISPLAY_STATE = "DISPLAY_STATE";
    private static final String DISPLAY_TEXT = "DISPLAY_TEXT";
    private static final String DISPLAY_SENSOR = "DISPLAY_SENSOR";
    private static final String DISPLAY_IF = "DISPLAY_IF";
    private static final String DISPLAY_ELSE = "DISPLAY_ELSE";
    private static final String DISPLAY_ENDIF = "DISPLAY_ENDIF";
    private static final char QUOTE_SYMBOL = '"';
    public static final String TODO_MAKE_THIS_DESTINATION_PARAMETER = "/ui/src/main/java/com/rusefi/ldmp/generated/";
    private static StringBuilder prefix = new StringBuilder();

    private static String readLineByLine(String filePath) throws IOException {
        StringBuilder contentBuilder = new StringBuilder();
        Stream<String> stream = Files.lines(Paths.get(filePath), StandardCharsets.UTF_8);
        stream.forEach(s -> contentBuilder.append(s).append("\n"));
        return contentBuilder.toString();
    }

    public static void main(String[] args) throws IOException {
        if (args.length != 2)
            throw new IllegalArgumentException("Two arguments expected but " + Arrays.toString(args));
        String destinationPath = args[0];
        String fileName = args[1];
        handleMetaData(destinationPath, fileName);
    }

    public static void handleMetaData(String destinationPath, String inputFileName) throws IOException {
        MetaInfo metaInfo = getMetaFromFile(inputFileName);
        SystemOut.println(metaInfo);

        String className = getClassName(inputFileName);
        String javaCode = generateJavaCode(metaInfo, className);
        String fullDestination = destinationPath + TODO_MAKE_THIS_DESTINATION_PARAMETER;
        new File(fullDestination).mkdirs();
        FileWriter fw = new FileWriter(fullDestination + className + ".java");
        fw.write(javaCode);
        fw.close();
        SystemOut.close();
    }

    private static MetaInfo getMetaFromFile(String fileName) throws IOException {
        SystemOut.println(fileName);
        if (!new File(fileName).exists())
            throw new IllegalStateException("Not found " + fileName);
        String content = readLineByLine(fileName);
        return parse(content, fileName);
    }

    public static MetaInfo parse(String string, String context) {
        Stack<List<Request>> stack = new Stack<>();


        MetaInfo meta = new MetaInfo();

        List<Request> result = meta.start("CONTENT");
        string = string.replaceAll("[()>.]", " ");
        SystemOut.println(string);
        Scanner s = new Scanner(string);
        while (s.hasNext()) {
            String token = s.next();
            //SystemOut.println(token);
            if (DISPLAY_CONFIG.equalsIgnoreCase(token)) {
                if (s.hasNext()) {
                    String config = s.next();
                    SystemOut.println("REQ CONF " + config);
                    result.add(new ConfigRequest(config));
                }
            } else if (DISPLAY_STATE.equalsIgnoreCase(token)) {
                if (s.hasNext()) {
                    String state = s.next();
                    meta.stateStack.push(state);                }
            } else if (DISPLAY_TEXT.equalsIgnoreCase(token)) {
                if (s.hasNext()) {
                    String config = s.next();
                    if (config.startsWith(String.valueOf(QUOTE_SYMBOL))) {
                        config = config.substring(1);
                        while (!config.endsWith(String.valueOf(QUOTE_SYMBOL))) {
                            String next = s.next();
                            config += ' ' + next;
                        }
                        config = config.substring(0, config.length() - 1);
                    }
                    SystemOut.println("Display test " + config);
                    result.add(new TextRequest(config));
                }
            } else if (DISPLAY_TAG.equalsIgnoreCase(token)) {
                if (s.hasNext()) {
                    String tag = s.next();
                    SystemOut.println("REQ TAG " + tag);
                    result = meta.start(tag);
                }
            } else if (DISPLAY_PREFIX.equalsIgnoreCase(token)) {
                if (s.hasNext()) {
                    String current = s.next();
                    prefix.append(current + "_");
                }
            } else if (DISPLAY_FIELD.equalsIgnoreCase(token)) {
                if (s.hasNext()) {
                    String fieldName = prefix + s.next();
                    SystemOut.println("REQ FIELD " + fieldName);
                    result.add(new FieldRequest(meta.getStateContext(), fieldName));
                    prefix.setLength(0);
                }
            } else if (DISPLAY_IF.equalsIgnoreCase(token)) {
                if (s.hasNext()) {
                    stack.push(result);

                    String conditionVariable = s.next();

                    SystemOut.println("if Condition " + conditionVariable);
                    IfRequest ifRequest = new IfRequest(meta.getStateContext(), conditionVariable);
                    result.add(ifRequest);

                    result = ifRequest.trueBlock;
                }
            } else if (DISPLAY_ELSE.equalsIgnoreCase(token)) {
                if (stack.isEmpty())
                    throw new IllegalStateException("No IF statement on stack while we have DISPLAY_ELSE while " + context);
                List<Request> onStack = stack.peek();
                if (onStack.isEmpty())
                    throw new IllegalStateException("Empty on stack");
                Request request = onStack.get(onStack.size() - 1);
                if (!(request instanceof IfRequest))
                    throw new IllegalStateException("Something not right " + request);
                IfRequest ifRequest = (IfRequest) request;

                result = ifRequest.falseBlock;
            } else if (DISPLAY_ENDIF.equalsIgnoreCase(token)) {
                if (stack.isEmpty())
                    throw new IllegalStateException("No IF statement on stack");
                result = stack.pop();

            } else if (DISPLAY_SENSOR.equalsIgnoreCase(token)) {
                if (s.hasNext()) {
                    String config = s.next();
                    SystemOut.println("REQ SENSOR " + config);
                    result.add(new SensorRequest(config));
                }
            }
        }

        if (!stack.isEmpty())
            throw new IllegalStateException("Unfinished");
        return meta;
    }

    public static String generateJavaCode(MetaInfo r, String className) {
        StringBuilder java = new StringBuilder("package com.rusefi.ldmp.generated;" + EOL + EOL +
                "import com.rusefi.ldmp.*;" + EOL + EOL +
                "public class " + className + " {" + EOL
        );

        for (Map.Entry<String, List<Request>> e : r.map.entrySet()) {
            List<Request> list = e.getValue();
            if (list.isEmpty())
                continue;

            java.append("\tpublic static final Request[] " +
                    e.getKey() +
                    " = new Request[]{" + EOL);
            java.append(Request.getGeneratedJavaCode(list));
            java.append("\t};" + EOL);
        }

        java.append("}");
        return java.toString();
    }

    private static String toProperCase(String s) {
        return s.substring(0, 1).toUpperCase() + s.substring(1).toLowerCase();
    }

    public static String getClassName(String cppSourceName) {
        int lastDotIndex = cppSourceName.lastIndexOf('.');
        if (lastDotIndex != -1)
            cppSourceName = cppSourceName.substring(0, lastDotIndex);
        int lastSlashIndex = cppSourceName.lastIndexOf('/');
        if (lastDotIndex != -1)
            cppSourceName = cppSourceName.substring(lastSlashIndex + 1);

        String[] parts = cppSourceName.split("_");
        String camelCaseString = "";
        for (String part : parts) {
            camelCaseString = camelCaseString + toProperCase(part);
        }
        return camelCaseString + "Meta";
    }
}
