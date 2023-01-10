package com.rusefi.output;

import com.opensr5.ini.field.IniField;
import com.rusefi.ConfigField;
import com.rusefi.ConfigFieldImpl;
import com.rusefi.ReaderState;
import com.rusefi.TypesHelper;
import com.rusefi.newparse.parsing.Type;

import java.util.TreeSet;

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

    public TsOutput(boolean longForm) {
        this.isConstantsSection = longForm;
    }

    public String getContent() {
        return tsHeader.toString();
    }

    public String getSettingContextHelp() {
        return settingContextHelp.toString();
    }

    public int run(ReaderState state, ConfigStructure structure, int sensorTsPosition) {
        FieldsStrategy strategy = new FieldsStrategy() {
            @Override
            public int writeOneField(FieldIterator it, String prefix, int tsPosition) {
                ConfigField configField = it.cf;
                ConfigField next = it.next;
                int bitIndex = it.bitState.get();
                String nameWithPrefix = prefix + configField.getName();

                /**
                 * in 'Constants' section we have conditional sections and this check is not smart enough to handle those right
                 * A simple solution would be to allow only one variable per each conditional section - would be simpler not to check against previous field
                 */
                if (!usedNames.add(nameWithPrefix)
                        && !isConstantsSection
                        && !configField.getName().startsWith(ConfigStructureImpl.ALIGNMENT_FILL_AT)
                        && !configField.getName().startsWith(ConfigStructure.UNUSED_ANYTHING_PREFIX)) {
                    throw new IllegalStateException(nameWithPrefix + " already present: " + configField);
                }

                if (configField.getName().startsWith(ConfigStructureImpl.ALIGNMENT_FILL_AT)) {
                    tsPosition += configField.getSize(next);
                    return tsPosition;
                }

                if (configField.isDirective() && configField.getComment() != null) {
                    tsHeader.append(configField.getComment());
                    tsHeader.append(EOL);
                    return tsPosition;
                }

                ConfigStructure cs = configField.getStructureType();
                if (configField.getComment() != null && configField.getComment().trim().length() > 0 && cs == null) {
                    String commentContent = configField.getCommentTemplated();
                    commentContent = ConfigFieldImpl.unquote(commentContent);
                    settingContextHelp.append("\t" + nameWithPrefix + " = " + quote(commentContent) + EOL);
                }

                if (cs != null) {
                    String extraPrefix = cs.isWithPrefix() ? configField.getName() + "_" : "";
                    return writeFields(cs.getTsFields(), prefix + extraPrefix, tsPosition);
                }

                if (configField.isBit()) {
                    if (!configField.getName().startsWith(ConfigStructureImpl.UNUSED_BIT_PREFIX)) {
                        tsHeader.append(nameWithPrefix + " = bits, U32,");
                        tsHeader.append(" " + tsPosition + ", [");
                        tsHeader.append(bitIndex + ":" + bitIndex);
                        tsHeader.append("]");
                        if (isConstantsSection)
                            tsHeader.append(", \"" + configField.getFalseName() + "\", \"" + configField.getTrueName() + "\"");
                        tsHeader.append(EOL);
                    }

                    tsPosition += configField.getSize(next);
                    return tsPosition;
                }

                if (configField.getState().getTsCustomLine().containsKey(configField.getType())) {
                    String bits = configField.getState().getTsCustomLine().get(configField.getType());
                    if (!bits.startsWith("bits")) {
                        bits = handleTsInfo(configField, bits, 5);
                    }

                    bits = bits.replaceAll("@OFFSET@", "" + tsPosition);
                    tsHeader.append(nameWithPrefix + " = " + bits);

                    if (!configField.getName().equals(next.getName()))
                        tsPosition += configField.getState().getTsCustomSize().get(configField.getType());
                } else if (configField.getArraySizes().length == 0) {
                    tsHeader.append(nameWithPrefix + " = scalar, ");
                    tsHeader.append(TypesHelper.convertToTs(configField.getType()) + ",");
                    tsHeader.append(" " + tsPosition + ",");
                    tsHeader.append(" " + handleTsInfo(configField, configField.getTsInfo(), 1));
                    if (!configField.getName().equals(next.getName()))
                        tsPosition += configField.getSize(next);
                } else if (configField.getSize(next) == 0) {
                    // write nothing for empty array
                    // TS does not like those
                } else {
                    tsHeader.append(nameWithPrefix + " = array, ");
                    tsHeader.append(TypesHelper.convertToTs(configField.getType()) + ",");
                    tsHeader.append(" " + tsPosition + ",");
                    tsHeader.append(" [");
                    boolean first = true;
                    for (int size : configField.getArraySizes()) {
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
        };
        sensorTsPosition = strategy.run(state, structure, sensorTsPosition);

        if (state.isStackEmpty()) {
            tsHeader.append("; total TS size = " + sensorTsPosition + EOL);
        }
        return sensorTsPosition;
    }

    private String handleTsInfo(ConfigField configField, String tsInfo, int multiplierIndex) {
        if (tsInfo == null || tsInfo.trim().isEmpty()) {
            // default units and scale
            if (isConstantsSection) {
                if (configField.getType().equalsIgnoreCase(Type.U16.cType) || configField.getType().equalsIgnoreCase(Type.S16.cType))
                    return quote("") + ", 1, 0, 0, 32000, 0";
                return quote("") + ", 1, 0, 0, 100, 0";
            }
            return quote("") + ", 1, 0";
        }
        try {
            String[] fields = tsInfo.split(",");
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
            StringBuilder sb = new StringBuilder();
            if (!isConstantsSection) {
                String[] subarray = new String[3];
                System.arraycopy(fields, 0, subarray, 0, subarray.length);
                fields = subarray;
            }
            for (String f : fields) {
                if (sb.length() > 0) {
                    sb.append(",");
                }
                sb.append(f);
            }
            return sb.toString();
        } catch (Throwable e) {
            throw new IllegalStateException("While parsing [" + tsInfo + "] of " + configField, e);
        }
    }
}
