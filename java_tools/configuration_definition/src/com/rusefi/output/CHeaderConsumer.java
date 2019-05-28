package com.rusefi.output;

import com.rusefi.ConfigDefinition;
import com.rusefi.ConfigField;
import com.rusefi.ConfigStructure;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;

import static com.rusefi.ConfigDefinition.EOL;

/**
 * Configuration consumer which writes C header file
 */
public class CHeaderConsumer implements ConfigurationConsumer {
    public static final String BOOLEAN_TYPE = "bool";
    private final BufferedWriter cHeader;

    public CHeaderConsumer(String destCHeader) throws IOException {
        System.out.println("Writing C header to " + destCHeader);
        cHeader = new BufferedWriter(new FileWriter(destCHeader));
        cHeader.write("// this section " + ConfigDefinition.MESSAGE + EOL);
        cHeader.write("// begin" + EOL);
        String id = destCHeader.replaceAll("[\\\\\\.\\/]", "_").toUpperCase();
        cHeader.write("#ifndef " + id + EOL);
        cHeader.write("#define " + id + EOL);
        cHeader.write("#include \"rusefi_types.h\"" + EOL);
    }

    public static String getHeaderText(ConfigField configField, int currentOffset, int bitIndex) {
        if (configField.isBit()) {
            String comment = "\t/**" + EOL + ConfigDefinition.packComment(configField.getCommentContent(), "\t") + "\toffset " + currentOffset + " bit " + bitIndex + " */" + EOL;
            return comment + "\t" + BOOLEAN_TYPE + " " + configField.getName() + " : 1;" + EOL;
        }

        String cEntry = ConfigDefinition.getComment(configField.getCommentContent(), currentOffset);

        if (configField.getArraySize() == 1) {
            // not an array
            cEntry += "\t" + configField.getType() + " " + configField.getName() + ";" + EOL;
        } else {
            cEntry += "\t" + configField.getType() + " " + configField.getName() + "[" + configField.arraySizeVariableName + "];" + EOL;
        }
        return cEntry;
    }

    @Override
    public void startFile() throws IOException {
    }

    @Override
    public void handleEndStruct(ConfigStructure structure) throws IOException {
        if (structure.comment != null) {
            cHeader.write("/**" + EOL + ConfigDefinition.packComment(structure.comment, "")  + EOL + "*/" + EOL);
        }

        cHeader.write("// start of " + structure.name + EOL);
        cHeader.write("struct " + structure.name + " {" + EOL);

        structure.bitState.reset();
        for (int i = 0; i < structure.cFields.size(); i++) {
            ConfigField cf = structure.cFields.get(i);
            cHeader.write(getHeaderText(cf, structure.currentOffset, structure.bitState.get()));
            ConfigField next = i == structure.cFields.size() - 1 ? ConfigField.VOID : structure.cFields.get(i + 1);

            structure.bitState.incrementBitIndex(cf, next);
            structure.currentOffset += cf.getSize(next);
        }

        cHeader.write("\t/** total size " + structure.currentOffset + "*/" + EOL);
        cHeader.write("};" + EOL + EOL);

        // https://stackoverflow.com/questions/1675351/typedef-struct-vs-struct-definitions
        cHeader.write("typedef struct " + structure.name + " " + structure.name + ";" + EOL + EOL);
    }

    @Override
    public void endFile() throws IOException {
        cHeader.write("#endif" + EOL);
        cHeader.write("// end" + EOL);
        cHeader.write("// this section " + ConfigDefinition.MESSAGE + EOL);
        cHeader.close();
    }
}
