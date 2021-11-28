package com.rusefi.output;

import com.opensr5.ini.field.IniField;
import com.rusefi.*;

import java.io.IOException;
import java.io.Writer;
import java.util.List;

import static com.rusefi.ToolUtil.EOL;

public class TsOutput {
    private final StringBuilder settingContextHelp = new StringBuilder();
    private final ReaderState state;
    private final boolean longForm;

    public TsOutput(ReaderState state, boolean longForm) {
        this.state = state;
        this.longForm = longForm;
    }

    public StringBuilder getSettingContextHelp() {
        return settingContextHelp;
    }

    private int writeTunerStudio(ConfigField configField, String prefix, Writer tsHeader, int tsPosition, ConfigField next, int bitIndex) throws IOException {
        String nameWithPrefix = prefix + configField.getName();

        if (configField.isDirective() && configField.getComment() != null) {
            tsHeader.write(configField.getComment());
            tsHeader.write(EOL);
            return tsPosition;
        }

        ConfigStructure cs = configField.getState().structures.get(configField.getType());
        if (configField.getComment() != null && configField.getComment().trim().length() > 0 && cs == null) {
            settingContextHelp.append("\t" + nameWithPrefix + " = \"" + configField.getCommentContent() + "\"" + EOL);
        }
        state.variableRegistry.register(nameWithPrefix + "_offset", tsPosition);

        if (cs != null) {
            String extraPrefix = cs.withPrefix ? configField.getName() + "_" : "";
            return writeTunerStudio(cs, prefix + extraPrefix, tsHeader, tsPosition);
        }

        if (configField.isBit()) {
            tsHeader.write(nameWithPrefix + " = bits, U32,");
            tsHeader.write(" " + tsPosition + ", [");
            tsHeader.write(bitIndex + ":" + bitIndex);
            tsHeader.write("]");
            if (longForm)
                tsHeader.write(", \"" + configField.getFalseName() + "\", \"" + configField.getTrueName() + "\"");
            tsHeader.write(EOL);

            tsPosition += configField.getSize(next);
            return tsPosition;
        }

        if (configField.getState().tsCustomLine.containsKey(configField.getType())) {
            String bits = configField.getState().tsCustomLine.get(configField.getType());
            if (!bits.startsWith("bits")) {
                bits = handleTsInfo(bits, 5);
            }

            bits = bits.replaceAll("@OFFSET@", "" + tsPosition);
            tsHeader.write(nameWithPrefix + " = " + bits);

            tsPosition += configField.getState().tsCustomSize.get(configField.getType());
        } else if (configField.getTsInfo() == null) {
            throw new IllegalArgumentException("Need TS info for " + configField.getName() + " at " + prefix);
        } else if (configField.getArraySizes().length == 0) {
            tsHeader.write(nameWithPrefix + " = scalar, ");
            tsHeader.write(TypesHelper.convertToTs(configField.getType()) + ",");
            tsHeader.write(" " + tsPosition + ",");
            tsHeader.write(" " + handleTsInfo(configField.getTsInfo(), 1));
            tsPosition += configField.getSize(next);
        } else if (configField.getSize(next) == 0) {
            // write nothing for empty array
            // TS does not like those
        } else {
            tsHeader.write(nameWithPrefix + " = array, ");
            tsHeader.write(TypesHelper.convertToTs(configField.getType()) + ",");
            tsHeader.write(" " + tsPosition + ",");
            tsHeader.write(" [");
            boolean first = true;
            for (int size : configField.getArraySizes()) {
                if (first) {
                    first = false;
                } else {
                    tsHeader.write("x");
                }
                tsHeader.write(Integer.toString(size));
            }
            tsHeader.write("], " + handleTsInfo(configField.getTsInfo(), 1));

            tsPosition += configField.getSize(next);
        }
        tsHeader.write(EOL);
        return tsPosition;
    }

    protected int writeTunerStudio(ConfigStructure configStructure, String prefix, Writer tsHeader, int tsPosition) throws IOException {
        List<ConfigField> tsFields = configStructure.tsFields;
        FieldIterator iterator = new FieldIterator(tsFields);
        int prevTsPosition = tsPosition;
        for (int i = 0; i < tsFields.size(); i++) {
            iterator.start(i);

            // if duplicate names, use previous position
            if (iterator.cf.getName().equals(iterator.prev.getName())) {
                tsPosition = prevTsPosition;
            }

            // Update 'prev' state needed for duplicate names recognition
            if (!iterator.cf.isDirective()) {
                prevTsPosition = tsPosition;
                iterator.prev = iterator.cf;
            }

            tsPosition = writeTunerStudio(iterator.cf, prefix, tsHeader, tsPosition, iterator.next, iterator.bitState.get());

            iterator.end();
        }
        return tsPosition;
    }


    private String handleTsInfo(String tsInfo, int multiplierIndex) {
        try {
            String[] fields = tsInfo.split("\\,");
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
            if (!longForm) {
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
            throw new IllegalStateException("While parsing " + tsInfo, e);
        }
    }
}
