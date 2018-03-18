package com.rusefi;

import com.rusefi.test.ConfigDefinitionTest;

import java.io.IOException;
import java.io.Writer;
import java.util.HashSet;
import java.util.Set;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import static com.rusefi.ConfigDefinition.EOL;

/**
 * This is an immutable model of an individual field
 * (c) Andrey Belomutskiy
 * 1/15/15
 */
public class ConfigField {
    public static final ConfigField VOID = new ConfigField("", null, false, null, null, 1, null, false);

    private static final String typePattern = "([\\w\\d_]+)(\\[([\\w\\d]+)(\\s([\\w\\d]+))?\\])?";
    private static final String namePattern = "[[\\w\\d\\s_]]+";
    private static final String commentPattern = ";([^;]*)";

    private static final Pattern FIELD = Pattern.compile(typePattern + "\\s(" + namePattern + ")(" + commentPattern + ")?(;(.*))?");
    private static final Set<String> javaEnums = new HashSet<>();
    private static final String BOOLEAN_TYPE = "bool";

    public static final int LENGTH = 24;
    private static final char TS_COMMENT_TAG = '+';
    public static final String ENUM_SUFFIX = "_enum";

    /**
     * field name without structure name
     * @see #writeJavaFields prefix parameter for structure name
     */
    public final String name;
    public final String comment;
    public final boolean isBit;
    public final String arraySizeAsText;
    public final String type;
    public final int arraySize;

    public final String tsInfo;
    public final int elementSize;
    /**
     * this property of array expands field into a bunch of variables like field1 field2 field3 etc
     */
    public final boolean isIterate;

    public ConfigField(String name, String comment, boolean isBit, String arraySizeAsText, String type,
                       int arraySize, String tsInfo, boolean isIterate) {
        if (name == null)
            throw new NullPointerException(comment + " " + isBit + " " + type);
        assertNoWhitespaces(name);
        this.name = name;
        this.comment = comment;
        this.isBit = isBit;
        this.arraySizeAsText = arraySizeAsText;
        this.type = type;
        if (type == null) {
            elementSize = 0;
        } else {
            elementSize = TypesHelper.getElementSize(type);
        }
        this.arraySize = arraySize;
        this.tsInfo = tsInfo;
        this.isIterate = isIterate;
    }

    public static void assertNoWhitespaces(String name) {
        if (name.contains(" ") || name.contains("\t"))
            throw new IllegalArgumentException("Invalid name: " + name);
    }

    /**
     * @see ConfigDefinitionTest#testParseLine()
     */
    public static ConfigField parse(String line) {
        Matcher matcher = FIELD.matcher(line);
        if (!matcher.matches())
            return null;

        String name = matcher.group(6).trim();
        String comment = matcher.group(8);
        String type = matcher.group(1);
        int arraySize;
        String arraySizeAsText;
        if (matcher.group(3) != null) {
            arraySizeAsText = matcher.group(3);
            arraySize = ConfigDefinition.getSize(arraySizeAsText);
        } else {
            arraySize = 1;
            arraySizeAsText = null;
        }
        String tsInfo = matcher.group(10);

        boolean isIterate = "iterate".equalsIgnoreCase(matcher.group(5));
        ConfigField field = new ConfigField(name, comment, false, arraySizeAsText, type, arraySize,
                tsInfo, isIterate);
        System.out.println("type " + type);
        System.out.println("name " + name);
        System.out.println("comment " + comment);

        return field;
    }

    int getSize(ConfigField next) {
        if (isBit && next.isBit)
            return 0;
        if (isBit)
            return 4;
        return elementSize * arraySize;
    }

    String getHeaderText(int currentOffset, int bitIndex) {
        if (isBit) {
            String comment = "\t/**" + EOL + ConfigDefinition.packComment(getCommentContent(), "\t") + "\toffset " + currentOffset + " bit " + bitIndex + " */" + EOL;
            return comment + "\t" + BOOLEAN_TYPE + " " + name + " : 1;" + EOL;
        }

        String cEntry = ConfigDefinition.getComment(getCommentContent(), currentOffset);

        if (arraySize == 1) {
            // not an array
            cEntry += "\t" + type + " " + name + ";" + EOL;
        } else {
            cEntry += "\t" + type + " " + name + "[" + arraySizeAsText + "];" + EOL;
        }
        return cEntry;
    }

    @Override
    public String toString() {
        return "ConfigField{" +
                "name='" + name + '\'' +
                ", type='" + type + '\'' +
                ", arraySize=" + arraySize +
                ", elementSize=" + elementSize +
                '}';
    }

    public int writeTunerStudio(String prefix, Writer tsHeader, int tsPosition, ConfigField next, int bitIndex) throws IOException {
        String nameWithPrefix = prefix + name;

        VariableRegistry.INSTANCE.register(nameWithPrefix + "_offset", tsPosition);

        ConfigStructure cs = ConfigDefinition.structures.get(type);
        if (cs != null) {
            String extraPrefix = cs.withPrefix ? name + "_" : "";
            return cs.writeTunerStudio(prefix + extraPrefix, tsHeader, tsPosition);
        }

        if (isBit) {
            tsHeader.write("\t" + addTabsUpTo(nameWithPrefix, LENGTH));
            tsHeader.write("= bits,    U32,   ");
            tsHeader.write("\t" + tsPosition + ", [");
            tsHeader.write(bitIndex + ":" + bitIndex);
            tsHeader.write("], \"false\", \"true\"");
            tsHeader.write(EOL);

            tsPosition += getSize(next);
            return tsPosition;
        }

        if (ConfigDefinition.tsCustomLine.containsKey(type)) {
            String bits = ConfigDefinition.tsCustomLine.get(type);
            tsHeader.write("\t" + addTabsUpTo(nameWithPrefix, LENGTH));
            int size = ConfigDefinition.tsCustomSize.get(type);
//            tsHeader.headerWrite("\t" + size + ",");
            //          tsHeader.headerWrite("\t" + tsPosition + ",");
            bits = bits.replaceAll("@OFFSET@", "" + tsPosition);
            tsHeader.write("\t = " + bits);

            tsPosition += size;
        } else if (tsInfo == null) {
            tsHeader.write(";no TS info - skipping " + prefix + name + " offset " + tsPosition);
            tsPosition += arraySize * elementSize;
        } else if (arraySize != 1) {
            tsHeader.write("\t" + addTabsUpTo(nameWithPrefix, LENGTH) + "\t\t= array, ");
            tsHeader.write(TypesHelper.convertToTs(type) + ",");
            tsHeader.write("\t" + tsPosition + ",");
            tsHeader.write("\t[" + arraySize + "],");
            tsHeader.write("\t" + tsInfo);

            tsPosition += arraySize * elementSize;
        } else {
            tsHeader.write("\t" + addTabsUpTo(nameWithPrefix, LENGTH) + "\t\t= scalar, ");
            tsHeader.write(TypesHelper.convertToTs(type) + ",");
            tsHeader.write("\t" + tsPosition + ",");
            tsHeader.write("\t" + tsInfo);
            tsPosition += arraySize * elementSize;
        }
        tsHeader.write(EOL);
        return tsPosition;
    }

    private String addTabsUpTo(String name, int length) {
        StringBuilder result = new StringBuilder(name);
        int currentLength = name.length();
        while (currentLength < length) {
            result.append("\t");
            currentLength += 4;
        }
        return result.toString();
    }

    public int writeJavaFields(String prefix, Writer javaFieldsWriter, int tsPosition, ConfigField next, int bitIndex) throws IOException {
        ConfigStructure cs = ConfigDefinition.structures.get(type);
        if (cs != null) {
            String extraPrefix = cs.withPrefix ? name + "_" : "";
            return cs.writeJavaFields(prefix + extraPrefix, javaFieldsWriter, tsPosition);
        }

        String nameWithPrefix = prefix + name;

        if (comment != null && comment.startsWith(TS_COMMENT_TAG + "")) {
            ConfigDefinition.settingContextHelp.append("\t" + nameWithPrefix + " = \"" + getCommentContent() + "\"" + EOL);
        }

        if (isBit) {
            writeJavaFieldName(javaFieldsWriter, nameWithPrefix, tsPosition);
            javaFieldsWriter.append("FieldType.BIT, " + bitIndex + ");" + EOL);
            tsPosition += getSize(next);
            return tsPosition;
        }


        if (arraySize != 1) {
            // todo: array support
        } else if (TypesHelper.isFloat(type)) {
            writeJavaFieldName(javaFieldsWriter, nameWithPrefix, tsPosition);
            javaFieldsWriter.write("FieldType.FLOAT);" + EOL);
        } else {
            String enumOptions = VariableRegistry.INSTANCE.get(type + ENUM_SUFFIX);

            if (enumOptions != null && !javaEnums.contains(type)) {
                javaEnums.add(type);
                javaFieldsWriter.write("\tpublic static final String[] " + type + " = {" + enumOptions + "};" + EOL);
            }

            writeJavaFieldName(javaFieldsWriter, nameWithPrefix, tsPosition);
            javaFieldsWriter.write("FieldType.INT");
            if (enumOptions != null) {
                javaFieldsWriter.write(", " + type);
            }
            javaFieldsWriter.write(");" + EOL);
        }

        tsPosition += arraySize * elementSize;

        return tsPosition;
    }

    private void writeJavaFieldName(Writer javaFieldsWriter, String nameWithPrefix, int tsPosition) throws IOException {
        javaFieldsWriter.write("\tpublic static final Field ");
        javaFieldsWriter.write(nameWithPrefix.toUpperCase());
        javaFieldsWriter.write(" = Field.create(\"" + nameWithPrefix.toUpperCase() + "\", "
                + tsPosition + ", ");
    }

    public String getCommentContent() {
        if (comment == null || comment.isEmpty())
            return comment;
        return comment.charAt(0) == TS_COMMENT_TAG ? comment.substring(1) : comment;
    }
}
