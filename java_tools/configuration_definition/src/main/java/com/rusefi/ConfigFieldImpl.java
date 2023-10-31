package com.rusefi;

import com.devexperts.logging.Logging;
import com.opensr5.ini.field.EnumIniField;
import com.rusefi.core.Pair;
import com.rusefi.output.ConfigStructure;
import com.rusefi.output.DataLogConsumer;
import com.rusefi.output.JavaFieldsConsumer;

import java.util.Arrays;
import java.util.Objects;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.output.JavaSensorsConsumer.quote;

import org.jetbrains.annotations.Nullable;

/**
 * This is an immutable model of an individual field
 * Andrey Belomutskiy, (c) 2013-2020
 * 1/15/15
 */
public class ConfigFieldImpl implements ConfigField {
    private static final Logging log = getLogging(ConfigFieldImpl.class);
    public static final ConfigFieldImpl VOID = new ConfigFieldImpl(null, "", null, null, null, new int[0], null, false, false, false, null, null);

    private static final String typePattern = "([\\w\\d_]+)(\\[([\\w\\d]+)(\\sx\\s([\\w\\d]+))?(\\s([\\w\\d]+))?\\])?";

    private static final String namePattern = "[[\\w\\d\\s_]]+";
    private static final String commentPattern = ";([^;]*)";

    private static final Pattern FIELD = Pattern.compile(typePattern + "\\s(" + namePattern + ")(" + commentPattern + ")?(;(.*))?");

    private static final Pattern DIRECTIVE = Pattern.compile("#(if\\s" + namePattern + "|else|elif\\s\" + namePattern + \"|endif)");

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
    private final ReaderStateImpl state;
    private final boolean fsioVisible;
    private final boolean hasAutoscale;
    private final String trueName;
    private final String falseName;
    private boolean isFromIterate;
    private String iterateOriginalName;
    private int iterateIndex;

    /**
     * todo: one day someone should convert this into a builder
     */
    public ConfigFieldImpl(ReaderStateImpl state,
                           String name,
                           String comment,
                           String arraySizeAsText,
                           String type,
                           int[] arraySizes,
                           @Nullable String tsInfo,
                           boolean isIterate,
                           boolean fsioVisible,
                           boolean hasAutoscale,
                           String trueName,
                           String falseName) {
        this.fsioVisible = fsioVisible;
        this.hasAutoscale = hasAutoscale;
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
        this.tsInfo = tsInfo == null ? null : state.getVariableRegistry().applyVariables(tsInfo);
        this.isIterate = isIterate;
        if (tsInfo != null) {
            String[] tokens = getTokens();
            if (tokens.length > 1) {
                String scale = tokens[1].trim();
                double scaleDouble;
                try {
                    scaleDouble = Double.parseDouble(scale);
                } catch (NumberFormatException ignore) {
                    scaleDouble = -1.0;
                }
                if (!hasAutoscale && scaleDouble != 1) {
                    throw new IllegalStateException("Unexpected scale of " + scale + " without autoscale on " + this);
                }
            }
        }
    }

    private static int getSize(VariableRegistry variableRegistry, String s) {
        if (variableRegistry.intValues.containsKey(s)) {
            return variableRegistry.intValues.get(s);
        }
        return Integer.parseInt(s);
    }

    @Override
    public ConfigStructure getStructureType() {
        return getState().getStructures().get(getType());
    }

    @Override
    public boolean isArray() {
        return arraySizeVariableName != null || arraySizes.length != 0;
    }

    @Override
    public String getArraySizeVariableName() {
        return arraySizeVariableName;
    }

    @Override
    public String getTrueName() {
        return trueName;
    }

    @Override
    public String getFalseName() {
        return falseName;
    }

    @Override
    public boolean isBit() {
        return BOOLEAN_T.equalsIgnoreCase(type);
    }

    @Override
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
    public static ConfigFieldImpl parse(ReaderStateImpl state, String line) {
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
        validateComment(comment);
        String type = matcher.group(1);
        int[] arraySizes;
        String arraySizeAsText;
        if (matcher.group(5) != null) {
            arraySizeAsText = matcher.group(3) + "][" + matcher.group(5);
            arraySizes = new int[2];
            arraySizes[0] = getSize(state.getVariableRegistry(), matcher.group(3));
            arraySizes[1] = getSize(state.getVariableRegistry(), matcher.group(5));
        } else if (matcher.group(3) != null) {
            arraySizeAsText = matcher.group(3);
            arraySizes = new int[1];
            arraySizes[0] = getSize(state.getVariableRegistry(), arraySizeAsText);
        } else {
            arraySizes = new int[0];
            arraySizeAsText = null;
        }
        String tsInfo = matcher.group(12);

        boolean isIterate = "iterate".equalsIgnoreCase(matcher.group(7));


        ConfigFieldImpl field = new ConfigFieldImpl(state, name, comment, arraySizeAsText, type, arraySizes,
                tsInfo, isIterate, isFsioVisible, hasAutoscale, null, null);
        if (log.debugEnabled())
            log.debug("type " + type);
        if (log.debugEnabled())
            log.debug("name " + name);
        if (log.debugEnabled())
            log.debug("comment " + comment);

        return field;
    }

    private static void validateComment(String comment) {
        if (comment == null)
            return;
        comment = comment.trim();
        if (comment.isEmpty())
            return;
        if (comment.charAt(0) == '"' && !EnumIniField.isQuoted(comment))
            throw new MaybeSemicolorWasMissedException("This comment looks like semicolon was missed: " + comment);
    }

    public static boolean isPreprocessorDirective(String line) {
        Matcher matcher = DIRECTIVE.matcher(line);
        return matcher.matches();
    }

    @Override
    public int getSize(ConfigField next) {
        if (isBit() && next.isBit()) {
            // we have a protection from 33+ bits in a row in BitState, see BitState.TooManyBitsInARow
            return 0;
        }
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
                ", arraySizes=" + Arrays.toString(arraySizes) +
                '}';
    }

    @Override
    public int[] getArraySizes() {
        return arraySizes;
    }

    @Override
    public String getComment() {
        if (comment == null)
            return null;
        return comment.trim();
    }

    /**
     * field name without structure name
     *
     * @see JavaFieldsConsumer#writeJavaFields prefix parameter for structure name
     */
    @Override
    public String getName() {
        return name;
    }

    /**
     * @see com.rusefi.newparse.parsing.Type
     */
    @Override
    public String getType() {
        return type;
    }

    @Override
    public int getElementSize() {
        return isVoid() ? 0 : TypesHelper.getElementSize(state, type);
    }

    /**
     * this property of array expands field into a bunch of variables like field1 field2 field3 etc
     */
    @Override
    public boolean isIterate() {
        return isIterate;
    }

    @Override
    public boolean isHasAutoscale() {
        return hasAutoscale;
    }

    @Override
    public ReaderState getState() {
        return state;
    }

    @Override
    public String getTsInfo() {
        return tsInfo;
    }

    @Override
    public boolean isFsioVisible() {
        return fsioVisible;
    }

    @Override
    public String autoscaleSpec() {
        Pair<Integer, Integer> pair = autoscaleSpecPair();
        if (pair == null)
            return null;
        return pair.first + ", " + pair.second;
    }

    @Override
    public double autoscaleSpecNumber() {
        Pair<Integer, Integer> pair = autoscaleSpecPair();
        if (pair == null)
            return 1;
        return 1.0 * pair.second / pair.first;
    }

    @Override
    public Pair<Integer, Integer> autoscaleSpecPair() {
        if (!hasAutoscale) {
            return null;
        }
        if (tsInfo == null)
            throw new IllegalArgumentException("tsInfo expected with autoscale: " + this);
        String[] tokens = getTokens();
        if (tokens.length < 2)
            throw new IllegalArgumentException("Second comma-separated token expected in [" + tsInfo + "] for " + name);

        String scale = tokens[1].trim();
        double factor;
        if (scale.startsWith("{") && scale.endsWith("}")) {
            // Handle just basic division, not a full fledged eval loop
            scale = scale.substring(1, scale.length() - 1);
            String[] parts = scale.split("/");
            if (parts.length != 2)
                throw new IllegalArgumentException(name + ": Two parts of division expected in " + scale);
            factor = Double.parseDouble(parts[0]) / Double.parseDouble(parts[1]);
        } else {
            factor = Double.parseDouble(scale);
        }
        int mul, div;
        if (factor < 1.d) {
            mul = (int) Math.round(1. / factor);
            div = 1;
        } else {
            mul = 1;
            div = (int) factor;
        }
        // Verify accuracy
        double factor2 = ((double) div) / mul;
        double accuracy = Math.abs((factor2 / factor) - 1.);
        if (accuracy > 0.0000001) {
            // Don't want to deal with exception propogation; this should adequately not compile
            throw new IllegalStateException("$*@#$* Cannot accurately represent autoscale for " + tokens[1]);
        }

        return new Pair<>(mul, div);
    }

    private String[] getTokens() {
        if (tsInfo == null)
            return new String[0];
        return tsInfo.split(",");
    }

    @Override
    public String getUnits() {
        String[] tokens = getTokens();
        if (tokens.length == 0)
            return "";
        return unquote(tokens[0].trim());
    }

    @Override
    public double getMin() {
        String[] tokens = getTokens();
        if (tokens.length < 4)
            return -1;
        return Double.parseDouble(tokens[3]);
    }

    @Override
    public double getMax() {
        String[] tokens = getTokens();
        if (tokens.length < 5)
            return -1;
        return Double.parseDouble(tokens[4]);
    }

    @Override
    public int getDigits() {
        String[] tokens = getTokens();
        if (tokens.length < 6)
            return 0;
        return Integer.parseInt(tokens[5].trim());
    }

    @Override
    public String getCategory() {
        String[] tokens = getTokens();
        if (tokens.length < 7)
            return null;
        return tokens[6].trim();
    }

    // see testUnquote
    public static String unquote(String token) {
        return VariableRegistry.unquote(token);
    }

    public void setFromIterate(String iterateOriginalName, int iterateIndex) {
        this.iterateOriginalName = iterateOriginalName;
        this.iterateIndex = iterateIndex;
        this.isFromIterate = true;
    }

    @Override
    public String getIterateOriginalName() {
        return iterateOriginalName;
    }

    @Override
    public int getIterateIndex() {
        return iterateIndex;
    }

    @Override
    public boolean isFromIterate() {
        return isFromIterate;
    }

    /**
     * todo: find more usages for this method?
     * @see DataLogConsumer.getComment
     */
    public String getCommentOrName() {
        if (comment == null || comment.trim().isEmpty())
            return quote(name);
        return comment;
    }

    @Override
    public String getCommentTemplated() {
        return state.getVariableRegistry().applyVariables(getComment());
    }
}

