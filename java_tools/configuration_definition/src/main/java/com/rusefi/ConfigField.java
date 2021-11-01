package com.rusefi;

import com.rusefi.util.SystemOut;
import com.rusefi.test.ConfigFieldParserTest;

import java.util.Arrays;
import java.util.Objects;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * This is an immutable model of an individual field
 * Andrey Belomutskiy, (c) 2013-2020
 * 1/15/15
 */
public class ConfigField {
    public static final ConfigField VOID = new ConfigField(null, "", null, null, null, 1, null, false, false, null,null, -1, null, null);

    private static final String typePattern = "([\\w\\d_]+)(\\[([\\w\\d]+)(\\s([\\w\\d]+))?\\])?";
    private static final String namePattern = "[[\\w\\d\\s<>_]]+";
    private static final String commentPattern = ";([^;]*)";

    private static final Pattern FIELD = Pattern.compile(typePattern + "\\s(" + namePattern + ")(" + commentPattern + ")?(;(.*))?");

    private static final Pattern DIRECTIVE = Pattern.compile("#(if\\s" + namePattern + "|else|elif\\s\" + namePattern + \"|endif)");

    // we used to have a weird feature of only handing comments starting with + as tooltips
    // very unclear what was the value of it. todo; remove all of those and kill this tag?
    public static final char TS_COMMENT_TAG = '+';
    public static final String VOID_NAME = "";
    public static final String BOOLEAN_T = "boolean";
    public static final String DIRECTIVE_T = "directive";

    private final String name;
    private final String comment;
    public final String arraySizeVariableName;
    private final String type;
    private final int arraySize;

    private final String tsInfo;
    private final boolean isIterate;
    private final ReaderState state;
    private boolean fsioVisible;
    private final String autoscaleSpec;
    private final String individualName;
    private final int indexWithinArray;
    private final String trueName;
    private final String falseName;

    /**
     * todo: one day someone should convert this into a builder
     */
    public ConfigField(ReaderState state,
                       String name,
                       String comment,
                       String arraySizeAsText,
                       String type,
                       int arraySize,
                       String tsInfo,
                       boolean isIterate,
                       boolean fsioVisible,
                       String autoscaleSpec,
                       String individualName,
                       int indexWithinArray, String trueName, String falseName) {
        this.fsioVisible = fsioVisible;
        this.autoscaleSpec = autoscaleSpec;
        this.individualName = individualName;
        this.indexWithinArray = indexWithinArray;
        this.trueName = trueName == null ? "true" : trueName;
        this.falseName = falseName == null ? "false" : falseName;
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
        this.tsInfo = tsInfo == null ? null : state.variableRegistry.applyVariables(tsInfo);
        this.isIterate = isIterate;
    }

    public boolean isArray() {
        return arraySizeVariableName != null || arraySize != 1;
    }

    public String getTrueName() {
        return trueName;
    }

    public String getFalseName() {
        return falseName;
    }

    public String getCFieldName() {
        return getIndividualName() == null ? getName() : getIndividualName() + "["
                + (getIndexWithinArray() - 1) + "]";
    }

    public String getIndividualName() {
        return individualName;
    }

    public int getIndexWithinArray() {
        return indexWithinArray;
    }

    public boolean isBit() {
        return BOOLEAN_T.equalsIgnoreCase(type);
    }

    public boolean isDirective() {
        return DIRECTIVE_T.equalsIgnoreCase(type);
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

        String nameString = matcher.group(6).trim();
        String[] nameTokens = nameString.split("\\s");
        String name = nameTokens[nameTokens.length - 1];

        boolean isFsioVisible = Arrays.stream(nameTokens).anyMatch(s -> s.equalsIgnoreCase("fsio_visible"));

        String autoscaleSpec = null;
        for (String autoscaler : nameTokens) {
            if (!autoscaler.startsWith("autoscale")) {
                continue;
            }

            autoscaleSpec = autoscaler.split("[<>]")[1];
            break;
        }

        String comment = matcher.group(8);
        String type = matcher.group(1);
        int arraySize;
        String arraySizeAsText;
        if (matcher.group(3) != null) {
            arraySizeAsText = matcher.group(3);
            arraySize = ConfigDefinition.getSize(state.variableRegistry, arraySizeAsText);
        } else {
            arraySize = 1;
            arraySizeAsText = null;
        }
        String tsInfo = matcher.group(10);

        boolean isIterate = "iterate".equalsIgnoreCase(matcher.group(5));


        ConfigField field = new ConfigField(state, name, comment, arraySizeAsText, type, arraySize,
                tsInfo, isIterate, isFsioVisible, autoscaleSpec, null, -1, null, null);
        SystemOut.println("type " + type);
        SystemOut.println("name " + name);
        SystemOut.println("comment " + comment);

        return field;
    }

    public static boolean isPreprocessorDirective(ReaderState state, String line) {
        Matcher matcher = DIRECTIVE.matcher(line);
        return matcher.matches();
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

    public boolean isFsioVisible() {
        return fsioVisible;
    }

    public String autoscaleSpec() { return this.autoscaleSpec; }

    public String getUnits() {
        if (tsInfo == null)
            return "";
        String[] tokens = tsInfo.split("\\,");
        if (tokens.length == 0)
            return "";
        return unquote(tokens[0].trim());
    }

    private static String unquote(String token) {
        int length = token.length();
        if (length < 2)
            return token;
        if (token.charAt(0) == '\"')
            return token.substring(1, length - 1);
        return token;
    }
}

