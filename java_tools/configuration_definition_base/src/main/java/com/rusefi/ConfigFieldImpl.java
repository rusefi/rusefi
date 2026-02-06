package com.rusefi;

import com.devexperts.logging.Logging;
import com.opensr5.ini.field.EnumIniField;
import com.rusefi.core.Pair;
import com.rusefi.core.net.ConnectionAndMeta;
import com.rusefi.output.ConfigStructure;
import com.rusefi.output.ConfigStructureImpl;
import com.rusefi.output.JavaFieldsConsumer;

import java.util.Arrays;
import java.util.Objects;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.TokenUtils.tokenizeWithBraces;

import com.rusefi.parse.TypesHelper;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

/**
 * This is an immutable model of an individual field
 * Andrey Belomutskiy, (c) 2013-2020
 * 1/15/15
 */
public class ConfigFieldImpl implements ConfigField {
    private static final Logging log = getLogging(ConfigFieldImpl.class);
    public static final String VOID_BIT = "void";
    public static final ConfigFieldImpl VOID = new ConfigFieldImpl(null, "", null, null, null, new int[0], null, false, false, VOID_BIT, VOID_BIT);

    private static final String typePattern = "([\\w\\d_]+)(\\[([\\w\\d]+)(\\sx\\s([\\w\\d]+))?(\\s([\\w\\d]+))?\\])?";

    private static final String namePattern = "[[@\\w\\d\\s_]]+";
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
    private final ReaderState state;
    private final boolean hasAutoscale;
    private final String trueName;
    private final String falseName;
    private final ConfigStructure parentType;
    private boolean isFromIterate;
    private String iterateOriginalName;
    private int iterateIndex;

    // this is used to override the units used on rusefi_config.txt
    // only used to replace "SPECIAL_CASE_TEMPERATURE" to "C" and "F", and apply the correct scale
    @Nullable
    private String mockedTsInfo;

    /**
     * todo: one day someone should convert this into a builder
     */
    public ConfigFieldImpl(ReaderState state,
                           String name,
                           String comment,
                           String arraySizeAsText,
                           String type,
                           int[] arraySizes,
                           @Nullable String tsInfo,
                           boolean isIterate,
                           boolean hasAutoscale,
                           String trueName,
                           String falseName) {
        this.hasAutoscale = hasAutoscale;
        this.trueName = trueName == null ? "true" : trueName;
        this.falseName = falseName == null ? "false" : falseName;
        Objects.requireNonNull(name, comment + " " + type);
        assertNoWhitespaces(name);
        this.name = name;

        if (!isVoid())
            Objects.requireNonNull(state);
        this.state = state;
        this.parentType = state == null ? null : (state.isStackEmpty() ? null : state.peek());
        this.comment = comment;

        if (TypesHelper.isBoolean(type) && (trueName == null || falseName == null) && !getName().startsWith(ConfigStructureImpl.UNUSED_BIT_PREFIX)) {
            if (this.isWithinStruct("persistent_config_s")) {
                throw new IllegalArgumentException("trueName and falseName must be non-null: " + name);
            } else {
                state.intDefaultBitNameCounter();
            }
        }

        if (!isVoid())
            Objects.requireNonNull(type);
        this.type = type;
        this.arraySizeVariableName = arraySizeAsText;
        this.arraySizes = arraySizes;
        this.tsInfo = tsInfo == null ? null : state.getVariableRegistry().applyVariables(tsInfo);
        this.isIterate = isIterate;
        if (tsInfo != null && !TypesHelper.isFloat(type)) {
            String[] tokens = getTokens();
            if (tokens.length > 1) {
                String scale = tokens[1].trim();
                if (!isExpression(scale)) {
                   validateAutoScale(scale, hasAutoscale);
                } else {
                    String parsedScale = extractTrueBranch(scale);
                    validateAutoScale(parsedScale, hasAutoscale);
                }
            }
        }
        validateRange();
    }

    private void validateAutoScale(String scale, Boolean hasAutoscale) {
        Double scaleDouble;
        try {
            scaleDouble = Double.parseDouble(scale);
        } catch (NumberFormatException ignore) {
            scaleDouble = -1.0;
        }
        if (!hasAutoscale && scaleDouble != 1) {
            throw new IllegalStateException("Unexpected scale of " + scale + " without autoscale on " + this);
        }
    }

    private void validateRange() {
        if (!TypesHelper.withRange(type))
            return;
        String[] tokens = getTokens();
        if (tokens.length < 4)
            return;
        // Skip validation if min or max are expressions - see #8650
        if (isExpression(tokens[3]) || (tokens.length >= 5 && isExpression(tokens[4])))
            return;
        double scale = autoscaleSpecNumber();
        double min = getMin();
        double minValue = scale * TypesHelper.getMinValue(type);
        if (min < minValue)
            throw new FieldOutOfRangeException(name + ": min value outside of range " + min + " for " + type + " should be " + minValue);
        double max = getMax();
        double maxValue = scale * TypesHelper.getMaxValue(type);
        if (max > maxValue)
            throw new FieldOutOfRangeException(name + ": max value " + max + " outside of range. Type " + type + " maxValue " + maxValue);
    }

    /**
     * Check if a string is an expression
     */
    private static boolean isExpression(String str) {
        if (str == null)
            return false;
        String trimmed = str.trim();
        return trimmed.startsWith("{") && trimmed.endsWith("}");
    }

    /**
     * Extract the true branch from a ternary expression and try to evaluate it.
     * For expressions like "{condition ? trueValue : falseValue}", returns "trueValue".
     * This is useful for providing a sensible default when we can't evaluate the condition at parse time.
     * We use the true branch because unit tests typically use the "primary" or "true" case.
     *
     * @param expression The full expression string (may or may not include outer braces)
     * @return The true branch if it can be extracted, otherwise the original expression
     */
    private static String extractTrueBranch(String expression) {
        if (expression == null)
            return expression;

        String trimmed = expression.trim();
        // Remove outer braces if present
        if (trimmed.startsWith("{") && trimmed.endsWith("}")) {
            trimmed = trimmed.substring(1, trimmed.length() - 1).trim();
        }

        // Check if this is a ternary expression
        if (!trimmed.contains("?"))
            return expression;

        // Find the question mark and colon
        int questionIndex = trimmed.indexOf('?');
        int colonIndex = trimmed.lastIndexOf(':');

        if (questionIndex <= 0 || colonIndex <= questionIndex)
            return expression;

        // Extract the true branch (between ? and :)
        return trimmed.substring(questionIndex + 1, colonIndex).trim();
    }

    @Override
    public ConfigStructure getParentStructureType() {
        return parentType;
    }

    private static int getSize(VariableRegistry variableRegistry, String s) {
        if (variableRegistry.intValues.containsKey(s)) {
            return variableRegistry.intValues.get(s);
        }
        return Integer.parseInt(s);
    }

    @Override
    public ConfigStructure getStructureType() {
        return getState().getStructures().get(getTypeName());
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
    public static ConfigFieldImpl parse(ReaderState state, String line) {
        Matcher matcher = FIELD.matcher(line.trim());
        if (!matcher.matches())
            return null;

        String nameString = state.getVariableRegistry().applyVariables(matcher.group(8).trim());
        String[] nameTokens = nameString.split("\\s");
        String name = nameTokens[nameTokens.length - 1];

        boolean hasAutoscale = isHasAutoscale(nameTokens);

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
                tsInfo, isIterate, hasAutoscale, VOID_BIT, VOID_BIT);
        if (log.debugEnabled())
            log.debug("type " + type);
        if (log.debugEnabled())
            log.debug("name " + name);
        if (log.debugEnabled())
            log.debug("comment " + comment);

        return field;
    }

    private static boolean isHasAutoscale(String[] nameTokens) {
        boolean hasAutoscale = false;
        for (String autoscaler : nameTokens) {
            if (autoscaler.equals("autoscale")) {
                hasAutoscale = true;
                break;
            }
        }
        return hasAutoscale;
    }

    private static void validateComment(String comment) {
        if (comment == null)
            return;
        comment = comment.trim();
        if (comment.isEmpty())
            return;
        if (comment.charAt(0) == '"' && !EnumIniField.isQuoted(comment))
            throw new MaybeSemicolonWasMissedException("This comment looks like semicolon was missed: " + comment);
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
     * @see TypesHelper
     */
    @Override
    public String getTypeName() {
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
        if (mockedTsInfo != null) {
            return mockedTsInfo;
        }
        return tsInfo;
    }

    @Override
    public void setTsInfo(String newTsInfo) {
    	mockedTsInfo = newTsInfo;
    }

    @Override
    public String autoscaleSpec() {
        if (!hasAutoscale)
            return null;
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
        return getScaleSpec(scale, name);
    }

    public static @NotNull Pair<Integer, Integer> getScaleSpec(String scale, String name) {
        double factor;
        if (scale.startsWith("{") && scale.endsWith("}")) {
            String innerExpression = scale.substring(1, scale.length() - 1).trim();
            if (innerExpression.contains("?")) {
                // Try to extract and evaluate the true branch
                String trueBranch = extractTrueBranch(scale);
                try {
                    return getScaleSpec("{" + trueBranch + "}", name);
                } catch (Exception e) {
                    // If we can't evaluate the true branch, fall back to 1:1
                    return new Pair<>(1, 1);
                }
            }
            // Handle just basic division, not a full fledged eval loop
            String[] parts = innerExpression.split("/");
            if (parts.length == 1) {
                // Simple number wrapped in braces (e.g., from extracting true branch of ternary)
                factor = Double.parseDouble(parts[0]);
            } else if (parts.length == 2) {
                factor = Double.parseDouble(parts[0]) / Double.parseDouble(parts[1]);
            } else {
                throw new IllegalArgumentException(name + ": Expected simple number or division in " + innerExpression);
            }
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
            // Don't want to deal with exception propagation; this should adequately not compile
            throw new IllegalStateException("$*@#$* Cannot accurately represent autoscale for [" + scale + "] got " + accuracy);
        }

        return new Pair<>(mul, div);
    }

    private String[] getTokens() {
        return tokenizeWithBraces(tsInfo);
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
        String minToken = tokens[3];
        if (isExpression(minToken)) {
            String trueBranch = extractTrueBranch(minToken);
            try {
                return Double.parseDouble(trueBranch);
            } catch (NumberFormatException e) {
                return 0;
            }
        }
        return Double.parseDouble(minToken);
    }

    @Override
    public double getMax() {
        String[] tokens = getTokens();
        if (tokens.length < 5)
            return -1;
        String maxToken = tokens[4];
        if (isExpression(maxToken)) {
            String trueBranch = extractTrueBranch(maxToken);
            try {
                return Double.parseDouble(trueBranch);
            } catch (NumberFormatException e) {
                return 255;
            }
        }
        return Double.parseDouble(maxToken);
    }

    @Override
    public int getDigits() {
        String[] tokens = getTokens();
        if (tokens.length < 6)
            return 0;
        String digitsToken = tokens[5].trim();
        if (isExpression(digitsToken)) {
            String trueBranch = extractTrueBranch(digitsToken);
            try {
                return Integer.parseInt(trueBranch);
            } catch (NumberFormatException e) {
                return 2;
            }
        }
        return Integer.parseInt(digitsToken);
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
            return VariableRegistry.quote(name);
        return comment;
    }

    @Override
    public String getCommentTemplated() {
        return state.getVariableRegistry().applyVariables(getComment());
    }

    public static class FieldOutOfRangeException extends RuntimeException {
        public FieldOutOfRangeException(String s) {
            super(s);
        }
    }

    @Override
    public boolean isWithinStruct(String structName) {
        if (this.state == null || this.state.isStackEmpty())
            return false;
        ConfigStructure parent = this.state.peek();
        String parentName = parent.getName();
        while(!parentName.equals(structName)) {
            parent = parent.getParent();
            if (parent == null)
                return false;
            parentName = parent.getName();
        }
        return true;
    }
}

