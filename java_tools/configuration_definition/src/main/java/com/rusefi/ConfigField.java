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
    public static final ConfigField VOID = new ConfigField(null, "", null, null, null, new int[0], null, false, false, false, null, -1, null, null);

    private static final String typePattern = "([\\w\\d_]+)(\\[([\\w\\d]+)(\\sx\\s([\\w\\d]+))?(\\s([\\w\\d]+))?\\])?";

    private static final String namePattern = "[[\\w\\d\\s_]]+";
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
    private final int[] arraySizes;

    private final String tsInfo;
    private final boolean isIterate;
    private final ReaderState state;
    private boolean fsioVisible;
    private final boolean hasAutoscale;
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
                       int[] arraySizes,
                       String tsInfo,
                       boolean isIterate,
                       boolean fsioVisible,
                       boolean hasAutoscale,
                       String individualName,
                       int indexWithinArray, String trueName, String falseName) {
        this.fsioVisible = fsioVisible;
        this.hasAutoscale = hasAutoscale;
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
        this.arraySizes = arraySizes;
        this.tsInfo = tsInfo == null ? null : state.variableRegistry.applyVariables(tsInfo);
        this.isIterate = isIterate;
    }

    public boolean isArray() {
        return arraySizeVariableName != null || arraySizes.length != 0;
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

        String nameString = matcher.group(8).trim();
        String[] nameTokens = nameString.split("\\s");
        String name = nameTokens[nameTokens.length - 1];

        boolean isFsioVisible = Arrays.stream(nameTokens).anyMatch(s -> s.equalsIgnoreCase("fsio_visible"));

        boolean hasAutoscale = false;
        for (String autoscaler : nameTokens) {
            if (autoscaler.equals("autoscale")) {
                hasAutoscale = true;
                break;
            }
        }

        String comment = matcher.group(10);
        String type = matcher.group(1);
        int[] arraySizes;
        String arraySizeAsText;
        if (matcher.group(5) != null) {
            arraySizeAsText = matcher.group(3) + "][" + matcher.group(5);
	    arraySizes = new int[2];
	    arraySizes[0] = ConfigDefinition.getSize(state.variableRegistry, matcher.group(3));
	    arraySizes[1] = ConfigDefinition.getSize(state.variableRegistry, matcher.group(5));
	} else if (matcher.group(3) != null) {
            arraySizeAsText = matcher.group(3);
            arraySizes = new int[1];
	    arraySizes[0] = ConfigDefinition.getSize(state.variableRegistry, arraySizeAsText);
        } else {
            arraySizes = new int[0];
            arraySizeAsText = null;
        }
        String tsInfo = matcher.group(12);

        boolean isIterate = "iterate".equalsIgnoreCase(matcher.group(7));


        ConfigField field = new ConfigField(state, name, comment, arraySizeAsText, type, arraySizes,
                tsInfo, isIterate, isFsioVisible, hasAutoscale, null, -1, null, null);
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
	int size = getElementSize();
	for (int s : arraySizes) {
	    size *= s;
	}
        return size;
    }

    @Override
    public String toString() {
        return "ConfigField{" +
                "name='" + name + '\'' +
                ", type='" + type + '\'' +
                ", arraySizes=" + arraySizes +
                '}';
    }


    public String getCommentContent() {
        if (comment == null || comment.isEmpty())
            return comment;
        return comment.charAt(0) == TS_COMMENT_TAG ? comment.substring(1) : comment;
    }

    public int[] getArraySizes() {
        return arraySizes;
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

    public String autoscaleSpec() {
        if (!hasAutoscale) {
            return null;
        }
        String[] tokens = tsInfo.split("\\,");
        String scale = tokens[1].trim();
        double factor;
        if (scale.startsWith("{") && scale.endsWith("}")) {
            // Handle just basic division, not a full fledged eval loop
            scale = scale.substring(1, scale.length() - 1);
            String[] parts = scale.split("/");
            factor = Double.parseDouble(parts[0]) / Double.parseDouble(parts[1]);
        } else {
            factor = Double.parseDouble(scale);
        }
        int mul, div;
        if (factor < 1.d) {
            mul = (int)Math.round(1. / factor);
            div = 1;
        } else {
            mul = 1;
            div = (int)factor;
        }
        // Verify accuracy
        double factor2 = ((double)div) / mul;
        double accuracy = Math.abs((factor2 / factor) - 1.);
        if (accuracy > 0.0000001) {
            // Don't want to deal with exception propogation; this should adequately not compile
            return "$*@#$* Cannot accurately represent autoscale for " + tokens[1];
        }
        
        return mul + ", " + div;
    }

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

