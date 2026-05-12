package com.rusefi.output;

import com.rusefi.*;
import com.rusefi.parse.TypesHelper;

import static com.rusefi.ToolUtil.EOL;

public class BaseCHeaderConsumer implements ConfigurationConsumer {
    private static final String BOOLEAN_TYPE = "bool";
    /**
     * This flag controls if we assign default zero value (useful while generating structures used for class inheritance)
     * versus not assigning default zero value like we need for non-class headers
     * This could be related to configuration header use-case versus "live data" (not very alive idea) use-case
     *
     * TODO: STATIC MUTABLE?! Not good code at all.
     */
    public static boolean needZeroInit = true;
    private final StringBuilder content = new StringBuilder();

    private static String getHeaderText(FieldIteratorWithOffset iterator) {
        ConfigField configField = iterator.cf;
        if (configField.isBit()) {
            // unused bits are needed for proper struct memsize
            String comment = "\t/**" + EOL + packComment(configField.getComment(), "\t") + "\toffset " + iterator.currentOffset + " bit " + iterator.bitState.get() + " */" + EOL;
            return comment + "\t" + BOOLEAN_TYPE + " " + configField.getName() + " : 1 {};" + EOL;
        }

        String cEntry = getComment(configField.getComment(), iterator.currentOffset, configField.getUnits());

        String typeName = configField.getTypeName();

        String autoscaleSpec = configField.autoscaleSpec();
        if (autoscaleSpec != null) {
            typeName = "scaled_channel<" + typeName + ", " + autoscaleSpec + ">";
        }

        if (!configField.isArray()) {
            // not an array
            cEntry += "\t" + typeName + " " + configField.getName();
            if (needZeroInit && TypesHelper.isPrimitive(configField.getTypeName())) {
                // we need this cast in case of enums
                cEntry += " = (" + configField.getTypeName() + ")0";
            }
            cEntry += ";" + EOL;
        } else {
            cEntry += "\t" + typeName + " " + configField.getName() + "[" + configField.getArraySizeVariableName() + "] = {};" + EOL;
        }
        return cEntry;
    }

    private static String getComment(String comment, int currentOffset, String units) {
        String start = "\t/**";
        String packedComment = packComment(comment, "\t");
        String unitsComment = units.isEmpty() ? "" : "\t * units: " + units + EOL;
        return start + EOL +
                packedComment +
                unitsComment +
                "\t * offset " + currentOffset + EOL + "\t */" + EOL;
    }

    public static String packComment(String comment, String linePrefix) {
        if (comment == null)
            return "";
        if (comment.trim().isEmpty())
            return "";
        String result = "";
        for (String line : comment.split("\\\\n")) {
            result += linePrefix + " * " + line + EOL;
        }
        return result;
    }

    @Override
    public void handleEndStruct(ReaderState readerState, ConfigStructure structure) {
        if (structure.getComment() != null) {
            content.append("/**" + EOL + packComment(structure.getComment(), "") + EOL + "*/" + EOL);
        }

        content.append("// start of " + structure.getName() + EOL);
        content.append("struct " + structure.getName() + " {" + EOL);

        FieldIteratorWithOffset iterator = new FieldIteratorWithOffset(structure.getcFields());
        // todo: reuse FieldsStrategy#loopIterator?
        for (int i = 0; i < structure.getcFields().size(); i++) {
            iterator.start(i);
            content.append(getHeaderText(iterator));

            iterator.currentOffset = iterator.adjustSize(iterator.currentOffset);
            iterator.end(0);
        }

        content.append("};" + EOL);
        int realSize = iterator.currentOffset;
        // https://stackoverflow.com/questions/621616/c-what-is-the-size-of-an-object-of-an-empty-class
        int cppSize = Math.max(1, realSize);
        content.append("static_assert(sizeof(" + structure.getName() + ") == " + cppSize + ");\n");
        content.append(EOL);
    }

    public String getContent() {
        return content.toString();
    }
}
