package com.rusefi;

import com.rusefi.test.ConfigFieldParserTest;

import java.io.IOException;
import java.io.Writer;
import java.util.Objects;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * This is an immutable model of an individual field
 * (c) Andrey Belomutskiy
 * 1/15/15
 */
public class ConfigField {
    public static final ConfigField VOID = new ConfigField(null, "", null, null, null, 1, null, false);

    private static final String typePattern = "([\\w\\d_]+)(\\[([\\w\\d]+)(\\s([\\w\\d]+))?\\])?";
    private static final String namePattern = "[[\\w\\d\\s_]]+";
    private static final String commentPattern = ";([^;]*)";

    private static final Pattern FIELD = Pattern.compile(typePattern + "\\s(" + namePattern + ")(" + commentPattern + ")?(;(.*))?");

    public static final char TS_COMMENT_TAG = '+';
    public static final String ENUM_SUFFIX = "_enum";
    public static final String VOID_NAME = "";
    public static final String BOOLEAN_T = "boolean";

    private final String name;
    private final String comment;
    public final String arraySizeVariableName;
    private final String type;
    private final int arraySize;

    private final String tsInfo;
    private final boolean isIterate;
    private final ReaderState state;

    public ConfigField(ReaderState state,
                       String name,
                       String comment,
                       String arraySizeAsText,
                       String type,
                       int arraySize,
                       String tsInfo,
                       boolean isIterate) {
        Objects.requireNonNull(name, comment + " " + type);
        assertNoWhitespaces(name);
        this.name = name;

        if (!isVoid())
            Objects.requireNonNull(state);
        this.state = state;
        this.comment = comment;

        if (!isVoid())
            Objects.requireNonNull(type);
        this.type = type;
        this.arraySizeVariableName = arraySizeAsText;
        this.arraySize = arraySize;
        this.tsInfo = tsInfo;
        this.isIterate = isIterate;
    }

    public boolean isBit() {
        return BOOLEAN_T.equalsIgnoreCase(type);
    }

    private boolean isVoid() {
        return name.equals(VOID_NAME);
    }

    public static void assertNoWhitespaces(String name) {
        if (name.contains(" ") || name.contains("\t"))
            throw new IllegalArgumentException("Invalid name: " + name);
    }

    /**
     * @see ConfigFieldParserTest#testParseLine()
     */
    public static ConfigField parse(ReaderState state, String line) {
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
        ConfigField field = new ConfigField(state, name, comment, arraySizeAsText, type, arraySize,
                tsInfo, isIterate);
        System.out.println("type " + type);
        System.out.println("name " + name);
        System.out.println("comment " + comment);

        return field;
    }

    public int getSize(ConfigField next) {
        if (isBit() && next.isBit())
            return 0;
        if (isBit())
            return 4;
        return getElementSize() * arraySize;
    }

    @Override
    public String toString() {
        return "ConfigField{" +
                "name='" + name + '\'' +
                ", type='" + type + '\'' +
                ", arraySize=" + arraySize +
                '}';
    }


    public void writeJavaFieldName(Writer javaFieldsWriter, String nameWithPrefix, int tsPosition) throws IOException {
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

    public int getArraySize() {
        return arraySize;
    }

    public String getComment() {
        return comment;
    }

    /**
     * field name without structure name
     *
     * @see JavaFieldsConsumer#writeJavaFields prefix parameter for structure name
     */
    public String getName() {
        return name;
    }

    public String getType() {
        return type;
    }

    public int getElementSize() {
        return isVoid() ? 0 : TypesHelper.getElementSize(state, type);
    }

    /**
     * this property of array expands field into a bunch of variables like field1 field2 field3 etc
     */
    public boolean isIterate() {
        return isIterate;
    }

    public ReaderState getState() {
        return state;
    }

    public String getTsInfo() {
        return tsInfo;
    }
}

