package com.rusefi.ldmp;

import com.rusefi.util.SystemOut;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import java.util.Stack;
import java.util.stream.Stream;

import static com.rusefi.ConfigDefinition.EOL;

public class LiveDocsMetaParser {
    private static final String DISPLAY_CONFIG = "DISPLAY_CONFIG";
    private static final String DISPLAY_FIELD = "DISPLAY_FIELD";
    private static final String DISPLAY_TEXT = "DISPLAY_TEXT";
    private static final String DISPLAY_SENSOR = "DISPLAY_SENSOR";
    private static final String DISPLAY_IF = "DISPLAY_IF";
    private static final String DISPLAY_ELSE = "DISPLAY_ELSE";
    private static final String DISPLAY_ENDIF = "DISPLAY_ENDIF";

    private static String readLineByLine(String filePath) throws IOException {
        StringBuilder contentBuilder = new StringBuilder();
        Stream<String> stream = Files.lines(Paths.get(filePath), StandardCharsets.UTF_8);
        stream.forEach(s -> contentBuilder.append(s).append("\n"));
        return contentBuilder.toString();
    }

    public static void main(String[] args) throws IOException {
        String fileName = args[0];
        String destinationPath = args[1];
        SystemOut.println(fileName);
        if (!new File(fileName).exists())
            throw new IllegalStateException("Not found " + fileName);
        String content = readLineByLine(fileName);
        List<Request> r = parse(content);
        SystemOut.println(r);


        String className = "ThermistorMeta";
        String javaCode = generateJavaCode(r, className);
        FileWriter fw = new FileWriter(destinationPath + "java_console/ui/src/com/rusefi/ldmp/generated/" + className + ".java");
        fw.write(javaCode);
        fw.close();
    }

    public static List<Request> parse(String string) {
        Stack<List<Request>> stack = new Stack<>();

        List<Request> result = new ArrayList<>();
        string = string.replaceAll("[()>]", " ");
        SystemOut.println(string);
        Scanner s = new Scanner(string);
        while (s.hasNext()) {
            String token = s.next();
            //SystemOut.println(token);
            if (DISPLAY_CONFIG.equals(token)) {
                if (s.hasNext()) {
                    String config = s.next();
                    SystemOut.println("REQ CONF " + config);
                    result.add(new ConfigRequest(config));
                }
            } else if (DISPLAY_TEXT.equals(token)) {
                if (s.hasNext()) {
                    String config = s.next();
                    SystemOut.println("REQ TEXT " + config);
                    result.add(new TextRequest(config));
                }
            } else if (DISPLAY_FIELD.equals(token)) {
                if (s.hasNext()) {
                    String config = s.next();
                    SystemOut.println("REQ FIELD " + config);
                    result.add(new FieldRequest(config));
                }
            } else if (DISPLAY_IF.equals(token)) {
                if (s.hasNext()) {
                    stack.push(result);

                    String variable = s.next();


                    String config = s.next();
                    SystemOut.println("REQ TEXT " + config);
                    IfRequest ifRequest = new IfRequest(variable);
                    result.add(ifRequest);

                    result = ifRequest.trueBlock;
                }
            } else if (DISPLAY_ELSE.equals(token)) {
                if (stack.isEmpty())
                    throw new IllegalStateException("No IF statement on stack");
                List<Request> onStack = stack.peek();
                if (onStack.isEmpty())
                    throw new IllegalStateException("Empty on stack");
                Request request = onStack.get(onStack.size() - 1);
                if (!(request instanceof IfRequest))
                    throw new IllegalStateException("Something not right " + request);
                IfRequest ifRequest = (IfRequest) request;

                result = ifRequest.falseBlock;
            } else if (DISPLAY_ENDIF.equals(token)) {
                if (stack.isEmpty())
                    throw new IllegalStateException("No IF statement on stack");
                result = stack.pop();

            } else if (DISPLAY_SENSOR.equals(token)) {
                if (s.hasNext()) {
                    String config = s.next();
                    SystemOut.println("REQ SENSOR " + config);
                    result.add(new SensorRequest(config));
                }
            }
        }

        if (!stack.isEmpty())
            throw new IllegalStateException("Unfinished");
        return result;
    }

    public static String generateJavaCode(List<Request> r, String className) {
        StringBuilder java = new StringBuilder("package com.rusefi.ldmp.generated;" + EOL + EOL +
                "import com.rusefi.ldmp.*;" + EOL + EOL +
                "public class " + className + " {" + EOL +
                "\tpublic static final Request[] CONTENT = new Request[]{" + EOL);

        for (Request request : r) {
            java.append(request.getJavaCode());
        }

        java.append("\t};" + EOL +
                "}");

        return java.toString();
    }
}
