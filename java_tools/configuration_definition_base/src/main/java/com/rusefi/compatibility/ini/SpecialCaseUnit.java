package com.rusefi.compatibility.ini;

import com.opensr5.ini.field.IniField;
import org.jetbrains.annotations.Nullable;

import static com.rusefi.TokenUtils.tokenizeWithBraces;
import static com.rusefi.TokenUtils.tokensToString;
import static com.rusefi.output.JavaSensorsConsumer.quote;

/**
 * Handles the SPECIAL_CASE_* unit markers that appear in older rusefi units
 */
public class SpecialCaseUnit {

    public static final String TEMPERATURE = "SPECIAL_CASE_TEMPERATURE";
    public static final String PRESSURE    = "SPECIAL_CASE_PRESSURE";
    public static final String SPEED       = "SPECIAL_CASE_SPEED";

    private static final String CELSIUS    = quote("C");
    private static final String FAHRENHEIT = quote("F");
    private static final double TO_FAHRENHEIT_SCALE     = 1.8;
    private static final double TO_FAHRENHEIT_TRANSLATE = 17.77777;

    private static final String KPA               = quote("kPa");
    private static final String PSI               = quote("psi");
    private static final double TO_PSI_SCALE      = 0.145038;
    private static final double TO_PSI_TRANSLATE  = 0.0;

    private static final String KMH              = quote("kmh");
    private static final String MPH              = quote("mph");
    private static final double TO_MPH_SCALE     = 0.62137119;
    private static final double TO_MPH_TRANSLATE = 0.0;

    private SpecialCaseUnit() {
    }

    /**
     * If {@code units} begins with one of the SPECIAL_CASE_* sentinels, rewrites
     * {@code tsInfo} to the metric variant of that physical dimension and returns it.
     * Returns {@code null} when {@code units} is not a special-case marker.
     *
     * @param units  the raw units string from the config field
     * @param tsInfo the raw TsInfo string to transform
     * @return the rewritten TsInfo string, or {@code null} if not a special case
     */
    @Nullable
    public static String resolveSpecialCase(String units, String tsInfo) {
        if (units.startsWith(TEMPERATURE))
            return formatTemperatureTsInfo(tsInfo, false);
        if (units.startsWith(PRESSURE))
            return formatPressureTsInfo(tsInfo, false);
        if (units.startsWith(SPEED))
            return formatSpeedTsInfo(tsInfo, false);
        return null;
    }

    public static String formatTemperatureTsInfo(String tsInfo, boolean isImperial) {
        if (tsInfo == null || tsInfo.trim().isEmpty())
            return "";
        String[] fields = tokenizeWithBraces(tsInfo);
        double scale = IniField.parseDouble(fields[1]);
        if (isImperial) {
            fields[0] = FAHRENHEIT;
            fields[1] = String.valueOf(TO_FAHRENHEIT_SCALE * scale);
            fields[2] = String.valueOf(TO_FAHRENHEIT_TRANSLATE * scale);
            fields[3] = String.valueOf(celsiusToFahrenheit(IniField.parseDouble(fields[3]))); // min
            fields[4] = String.valueOf(celsiusToFahrenheit(IniField.parseDouble(fields[4]))); // max
        } else {
            fields[0] = CELSIUS;
        }
        return tokensToString(fields);
    }

    public static String formatPressureTsInfo(String tsInfo, boolean isImperial) {
        if (tsInfo == null || tsInfo.trim().isEmpty())
            return "";
        String[] fields = tokenizeWithBraces(tsInfo);
        double scale = IniField.parseDouble(fields[1]);
        if (isImperial) {
            fields[0] = PSI;
            fields[1] = String.valueOf(TO_PSI_SCALE * scale);
            fields[2] = String.valueOf(TO_PSI_TRANSLATE * scale);
            fields[3] = String.valueOf(kPaToPsi(IniField.parseDouble(fields[3]))); // min
            fields[4] = String.valueOf(kPaToPsi(IniField.parseDouble(fields[4]))); // max
        } else {
            fields[0] = KPA;
        }
        return tokensToString(fields);
    }

    public static String formatSpeedTsInfo(String tsInfo, boolean isImperial) {
        if (tsInfo == null || tsInfo.trim().isEmpty())
            return "";
        String[] fields = tokenizeWithBraces(tsInfo);
        double scale = IniField.parseDouble(fields[1]);
        if (isImperial) {
            fields[0] = MPH;
            fields[1] = String.valueOf(TO_MPH_SCALE * scale);
            fields[2] = String.valueOf(TO_MPH_TRANSLATE * scale);
            fields[3] = String.valueOf(kmhToMph(IniField.parseDouble(fields[3]))); // min
            fields[4] = String.valueOf(kmhToMph(IniField.parseDouble(fields[4]))); // max
        } else {
            fields[0] = KMH;
        }
        return tokensToString(fields);
    }

    private static double celsiusToFahrenheit(double celsius) {
        return celsius * TO_FAHRENHEIT_SCALE + 32;
    }

    private static double kPaToPsi(double kPa) {
        return kPa * TO_PSI_SCALE;
    }

    private static double kmhToMph(double kmh) {
        return kmh * TO_MPH_SCALE;
    }
}
