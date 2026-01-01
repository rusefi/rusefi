package com.rusefi.output;

import com.opensr5.ini.field.IniField;
import com.rusefi.ConfigField;
import com.rusefi.ConfigFieldImpl;
import com.rusefi.ReaderState;
import com.rusefi.parse.Type;
import com.rusefi.parse.TypesHelper;
import com.rusefi.PinType;

import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.TreeSet;
import java.util.stream.Collectors;

import static com.rusefi.TokenUtils.tokenizeWithBraces;
import static com.rusefi.TokenUtils.tokensToString;
import static com.rusefi.ToolUtil.EOL;
import static com.rusefi.output.JavaSensorsConsumer.quote;

/**
 * Same code is used to generate [Constants] and [OutputChannels] bodies, with just one flag controlling the minor
 * difference in behaviours
 */
@SuppressWarnings({"StringConcatenationInsideStringBufferAppend", "DanglingJavadoc"})
public class TsOutput {
    private final StringBuilder settingContextHelp = new StringBuilder();
    private final boolean isConstantsSection;
    private final StringBuilder tsHeader = new StringBuilder();
    private final TreeSet<String> usedNames = new TreeSet<>(String.CASE_INSENSITIVE_ORDER);
//    private final String metricUnitsConditionalStart = "#if USE_METRIC_UNITS" + EOL;
//    private final String metricUnitsConditionalElse = "#else" + EOL;
//    private final String metricUnitsConditionalEnd = "#endif" + EOL;
    private final String temperatureCelsiusUnit = quote("C");
    private final String temperatureFahrenheitUnit = quote("F");
    private final Double temperatureToFahrenheitScale = 1.8;
    private final Double temperatureToFahrenheitTranslate = 17.77777;

    private final String pressureMetricUnit = quote("kPa");
    private final String pressureImperialUnit = quote("psi");
    private final Double pressureToPsiScale = 0.145038;
    private final Double pressureToPsiTranslate = 0.0;

    private final String speedMetricUnit = quote("kmh");
    private final String speedImperialUnit = quote("mph");
    private final Double KmhToMphScale = 0.62137119;
    private final Double KmhToMphTranslate = 0.0;

    public TsOutput(boolean longForm) {
        this.isConstantsSection = longForm;
    }

    public String getContent() {
        return tsHeader.toString();
    }

    public String getSettingContextHelp() {
        return settingContextHelp.toString();
    }

    public int run(ReaderState state, ConfigStructure structure, int structureStartingTsPosition, String temporaryLineComment, String variableNamePrefix) {
        FieldsStrategy strategy = new FieldsStrategy() {

			int writeFieldJob(String nameWithPrefix, ConfigFieldImpl configField, ConfigField next, int tsPosition,
					int bitIndex, String prefix, ConfigStructure cs) {
                if (configField.getState().getTsCustomLine().containsKey(configField.getTypeName())) {
                    // todo: rename 'bits' to 'customLine' or something since _not_ bits for array?
                    String bits = configField.getState().getTsCustomLine().get(configField.getTypeName());
                    if (!bits.startsWith("bits")) {
                        // 'array' would be handled here
                        bits = handleTsInfo(configField, bits, 5);
                    }

                    bits = bits.replaceAll("@OFFSET@", "" + tsPosition);
                    tsHeader.append(nameWithPrefix + " = " + bits);

                    if (!configField.getName().equals(next.getName()))
                        tsPosition += configField.getState().getTsCustomSize().get(configField.getTypeName());
                } else if (configField.getArraySizes().length == 0) {
                    tsHeader.append(temporaryLineComment + nameWithPrefix + " = scalar, ");
                    tsHeader.append(TypesHelper.convertToTs(configField.getTypeName()) + ",");
                    tsHeader.append(" " + tsPosition + ",");
                    tsHeader.append(" " + handleTsInfo(configField, configField.getTsInfo(), 1));
                    if (!configField.getName().equals(next.getName()))
                        tsPosition += configField.getSize(next);
                } else if (configField.getSize(next) == 0) {
                    // write nothing for empty array
                    // TS does not like those
                } else {
                    tsHeader.append(nameWithPrefix + " = array, ");
                    tsHeader.append(TypesHelper.convertToTs(configField.getTypeName()) + ",");
                    tsHeader.append(" " + tsPosition + ",");
                    tsHeader.append(" [");
                    boolean first = true;
                    List<Integer> list = Arrays.stream(configField.getArraySizes()).boxed().collect(Collectors.toList());
                    Collections.reverse(list);
                    for (int size : list) {
                        if (first) {
                            first = false;
                        } else {
                            tsHeader.append("x");
                        }
                        tsHeader.append(size);
                    }
                    tsHeader.append("], " + handleTsInfo(configField, configField.getTsInfo(), 1));

                    if (!configField.getName().equals(next.getName()))
                        tsPosition += configField.getSize(next);
                }
                tsHeader.append(EOL);
                return tsPosition;
            }

			@Override
			public int writeOneField(FieldIterator it, String prefix, int tsPosition) {
				ConfigFieldImpl configField = (ConfigFieldImpl) it.cf;
				ConfigField next = it.next;
				int bitIndex = it.bitState.get();
				String nameWithPrefix = prefix + variableNamePrefix + configField.getName();
				String originalUnits = configField.getUnits();
				String originalTsInfo = configField.getTsInfo();
                ConfigStructure cs = configField.getStructureType();

                /**
                 * in 'Constants' section we have conditional sections and this check is not smart enough to handle those right
                 * A simple solution would be to allow only one variable per each conditional section - would be simpler not to check against previous field
                 */
                if (!usedNames.add(nameWithPrefix)
                        && !isConstantsSection
                        && !configField.isUnusedField()) {
                    throw new IllegalStateException(nameWithPrefix + " already present: " + configField);
                }

                // note that we need to handle account for unused bits size below!
                if (configField.getName().startsWith(ConfigStructureImpl.ALIGNMENT_FILL_AT)) {
                    return it.adjustSize(tsPosition);
                }

                if (configField.isDirective() && configField.getComment() != null) {
                    tsHeader.append(configField.getComment());
                    tsHeader.append(EOL);
                    return tsPosition;
                }

                if (cs == null) {
                    String comment = "";

                    if (configField.getComment() != null && configField.getComment().trim().length() > 0) {
                        String commentContent = configField.getCommentTemplated();
                        comment = ConfigFieldImpl.unquote(commentContent);
                    }


                    PinType pinType = PinType.findByOutputEnum(configField.getTypeName());
                    if (pinType != null) {
                        try {
                            String url = state.getVariableRegistry().applyVariables("@@PINOUT_URL@@");
                            if (comment != "") {
                                comment += "\\n";
                            }
                            comment += ConfigFieldImpl.unquote(url) + "?highlight=class~" + pinType.name().toLowerCase();
                        } catch (IllegalStateException ignore) {}
                    }

                    if (comment != "") {
                        settingContextHelp.append(temporaryLineComment + "\t" + nameWithPrefix + " = " + quote(comment) + EOL);
                    }
                }

                if (cs != null) {
                    String extraPrefix = cs.isWithPrefix() ? configField.getName() + "_" : "";
                    return writeFields(cs.getTsFields(), prefix + extraPrefix, tsPosition);
                }

                if (configField.isBit()) {
                    if (!configField.getName().startsWith(ConfigStructureImpl.UNUSED_BIT_PREFIX)) {
                        tsHeader.append(temporaryLineComment + nameWithPrefix + " = bits, U32,");
                        tsHeader.append(" " + tsPosition + ", [");
                        tsHeader.append(bitIndex + ":" + bitIndex);
                        tsHeader.append("]");
                        if (isConstantsSection)
                            tsHeader.append(", \"" + configField.getFalseName() + "\", \"" + configField.getTrueName() + "\"");
                        tsHeader.append(EOL);
                    }

                    return it.adjustSize(tsPosition);
                }

                 // if the units are SPECIAL_CASE_TEMPERATURE, we are going to deal with a temperature-based config
                // so we need to edit the unit first on C degree, and then on F degree, also the TS conditional is added here
                if (originalUnits.startsWith("SPECIAL_CASE_TEMPERATURE")) {
                    // first the Celsius case, and save the index after writing the field
                    configField.setTsInfo(formatTemperatureTsInfo(originalTsInfo, false));
  //                  tsHeader.append(metricUnitsConditionalStart);
                    int newIndex = writeFieldJob(nameWithPrefix, configField, next, tsPosition, bitIndex, nameWithPrefix, cs);
//                    tsHeader.append(metricUnitsConditionalElse);
//                    // now the fahrenheit case:
//                    configField.setTsInfo(formatTemperatureTsInfo(originalTsInfo, true));
//                    writeFieldJob(nameWithPrefix, configField, next, tsPosition, bitIndex, nameWithPrefix, cs);
//                    tsHeader.append(metricUnitsConditionalEnd);
                    configField.setTsInfo(originalTsInfo);
                    return newIndex;
                }
                // equal structure as temperature case, now with kPa and psi as units
                if (originalUnits.startsWith("SPECIAL_CASE_PRESSURE")) {
                    configField.setTsInfo(formatPressureTsInfo(originalTsInfo, false));
//                    tsHeader.append(metricUnitsConditionalStart);
                    int newIndex = writeFieldJob(nameWithPrefix, configField, next, tsPosition, bitIndex, nameWithPrefix, cs);
//                    tsHeader.append(metricUnitsConditionalElse);
//                    // now the psi case:
//                    configField.setTsInfo(formatPressureTsInfo(originalTsInfo, true));
//                    writeFieldJob(nameWithPrefix, configField, next, tsPosition, bitIndex, nameWithPrefix, cs);
//                    tsHeader.append(metricUnitsConditionalEnd);
                    configField.setTsInfo(originalTsInfo);
                    return newIndex;
                }
                if (originalUnits.startsWith("SPECIAL_CASE_SPEED")) {
                    configField.setTsInfo(formatSpeedTsInfo(originalTsInfo, false));
//                    tsHeader.append(metricUnitsConditionalStart);
                    int newIndex = writeFieldJob(nameWithPrefix, configField, next, tsPosition, bitIndex, nameWithPrefix, cs);
//                    tsHeader.append(metricUnitsConditionalElse);
//                    // now the psi case:
//                    configField.setTsInfo(formatSpeedTsInfo(originalTsInfo, true));
//                    writeFieldJob(nameWithPrefix, configField, next, tsPosition, bitIndex, nameWithPrefix, cs);
//                    tsHeader.append(metricUnitsConditionalEnd);
                    configField.setTsInfo(originalTsInfo);
                    return newIndex;
                }

				return writeFieldJob(nameWithPrefix, configField, next, tsPosition, bitIndex, nameWithPrefix, cs);
			}
		};
        structureStartingTsPosition = strategy.run(state, structure, structureStartingTsPosition);

        if (state.isStackEmpty()) {
            tsHeader.append("; total TS size = " + structureStartingTsPosition + EOL);
        }
        return structureStartingTsPosition;
    }

    private double celsiusToFahrenheit(double celsius){
        return celsius * temperatureToFahrenheitScale + 32;
    }

    private double kPaToPsi(double kPa){
        return kPa * pressureToPsiScale;
    }

    private double KmhToMph(double Kmh){
        return Kmh * KmhToMphScale;
    }

    public String formatTemperatureTsInfo(String tsInfo, boolean isImperial){
        if (tsInfo == null || tsInfo.trim().isEmpty()) {
            // this case is handle by handleTsInfo, so we return a empty string
            return "";
        }
        String[] fields = tokenizeWithBraces(tsInfo);
        Double scale = IniField.parseDouble(fields[1]);

         if (isImperial){
            // override scale/translate & units, convert min-max
            fields[0] = temperatureFahrenheitUnit;
            fields[1] = String.valueOf( temperatureToFahrenheitScale * scale );
            fields[2] = String.valueOf( temperatureToFahrenheitTranslate * scale );
            fields[3] = String.valueOf( celsiusToFahrenheit( IniField.parseDouble(fields[3]) ) ); // min
            fields[4] = String.valueOf( celsiusToFahrenheit( IniField.parseDouble(fields[4]) ) ); // max
         } else {
            // override units
            fields[0] = temperatureCelsiusUnit;
         }

          return tokensToString(fields);
    }

    public String formatPressureTsInfo(String tsInfo, boolean isImperial) {
        if (tsInfo == null || tsInfo.trim().isEmpty()) {
            // this case is handle by handleTsInfo, so we return a empty string
            return "";
        }

        String[] fields = tokenizeWithBraces(tsInfo);
        Double scale = IniField.parseDouble(fields[1]);

         if (isImperial){
                    // override scale/translate & units, convert min-max
                    fields[0] = pressureImperialUnit;
                    fields[1] = String.valueOf( pressureToPsiScale * scale );
                    fields[2] = String.valueOf( pressureToPsiTranslate * scale );
                    fields[3] = String.valueOf( kPaToPsi( IniField.parseDouble(fields[3]) ) ); // min
                    fields[4] = String.valueOf( kPaToPsi( IniField.parseDouble(fields[4]) ) ); // max
        } else {
            // override units
            fields[0] = pressureMetricUnit;
        }
        return tokensToString(fields);
    }

    public String formatSpeedTsInfo(String tsInfo, boolean isImperial) {
        if (tsInfo == null || tsInfo.trim().isEmpty()) {
            // this case is handle by handleTsInfo, so we return a empty string
            return "";
        }

        String[] fields = tokenizeWithBraces(tsInfo);
        Double scale = IniField.parseDouble(fields[1]);

         if (isImperial){
                    // override scale/translate & units, convert min-max
                    fields[0] = speedImperialUnit;
                    fields[1] = String.valueOf( KmhToMphScale * scale );
                    fields[2] = String.valueOf( KmhToMphTranslate * scale );
                    fields[3] = String.valueOf( KmhToMph( IniField.parseDouble(fields[3]) ) ); // min
                    fields[4] = String.valueOf( KmhToMph( IniField.parseDouble(fields[4]) ) ); // max
        } else {
            // override units
            fields[0] = speedMetricUnit;
        }
        return tokensToString(fields);
    }

    private String handleTsInfo(ConfigField configField, String tsInfo, int multiplierIndex) {
        if (tsInfo == null || tsInfo.trim().isEmpty()) {
            // default units and scale
            if (isConstantsSection) {
                if (configField.getTypeName().equalsIgnoreCase(Type.U16.cType) || configField.getTypeName().equalsIgnoreCase(Type.S16.cType))
                    return quote("") + ", 1, 0, 0, 32000, 0";
                return quote("") + ", 1, 0, 0, 100, 0";
            }
            return quote("") + ", 1, 0";
        }
        try {
            String[] fields = tokenizeWithBraces(tsInfo);
            if (fields.length > multiplierIndex) {
                /**
                 * Evaluate static math on .ini layer to simplify rusEFI java and rusEFI PHP project consumers
                 * https://github.com/rusefi/web_backend/issues/97
                 */
                double val = IniField.parseDouble(fields[multiplierIndex]);

                if (val == 0) {
                    fields[multiplierIndex] = " 0";
                } else if (val == 1) {
                    fields[multiplierIndex] = " 1";
                } else {
                    fields[multiplierIndex] = " " + val;
                }
            }

            if (!isConstantsSection) {
                String[] subarray = new String[3];
                System.arraycopy(fields, 0, subarray, 0, subarray.length);
                fields = subarray;
            }
            return tokensToString(fields);
        } catch (Throwable e) {
            throw new IllegalStateException("While parsing [" + tsInfo + "] of " + configField, e);
        }
    }
}
