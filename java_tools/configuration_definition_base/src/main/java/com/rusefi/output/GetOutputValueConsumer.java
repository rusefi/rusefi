package com.rusefi.output;

import com.rusefi.ConfigField;
import com.rusefi.ReaderState;
import com.rusefi.parse.TypesHelper;
import com.rusefi.output.variables.VariableRecord;
import com.rusefi.util.LazyFile;
import org.jetbrains.annotations.NotNull;

import java.io.IOException;
import java.util.*;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.stream.Collectors;

import static com.rusefi.output.GetConfigValueConsumer.getCompareName;
import static com.rusefi.output.SdCardFieldsContent.getNamePrefix;

/**
 * here we generate C++ code needed for https://wiki.rusefi.com/Lua-Scripting#getoutputname implementation
 *
 * @see GetConfigValueConsumer
 */
@SuppressWarnings("StringConcatenationInsideStringBufferAppend")
public class GetOutputValueConsumer implements ConfigurationConsumer {
    private final List<VariableRecord> getterPairs = new ArrayList<>();
    private final String fileName;
    private final LazyFile.LazyFileFactory fileFactory;

    public String[] expressions = {"engine->outputChannels"};
    public String[] names;
    public boolean moduleMode;
    public String currentEngineModule;
    public String conditional;
    public String additionalHeaders = "";
    public Boolean isPtr = false;

    public GetOutputValueConsumer(String fileName, LazyFile.LazyFileFactory fileFactory) {
        this.fileName = fileName;
        this.fileFactory = fileFactory;
    }

    @Override
    public void handleEndStruct(ReaderState state, ConfigStructure structure) throws IOException {
        if (state.isStackEmpty()) {
            for (int i = 0; i < expressions.length; i++) {
                String namePrefix = getNamePrefix(i, names);
                String expression = expressions[i];
                PerFieldWithStructuresIterator iterator = new PerFieldWithStructuresIterator(state, structure.getTsFields(), "",
                    (readerState, cf, prefix, currentPosition, perFieldWithStructuresIterator) -> processOutput(cf, prefix, expression, namePrefix), ".");
                iterator.loop(0);
            }
        }
    }

    private String processOutput(ConfigField cf, String prefix, String expression, String namePrefix) {
        if (cf.isUnusedField())
            return "";

        if (cf.isArray() || cf.isFromIterate())
            return "";
        if (!TypesHelper.isPrimitive(cf.getTypeName()) && !TypesHelper.isBoolean(cf.getTypeName())) {
            return "";
        }

        String userName = namePrefix + prefix + cf.getName();
        String javaName;
        if (moduleMode) {
            javaName = "engine->module<" + currentEngineModule + ">()->" + prefix;
        } else {
            javaName = expression + (isPtr ? "->" : ".") + prefix;
        }

        getterPairs.add(new VariableRecord(userName, javaName + cf.getName(), null, conditional));


        return "";
    }

    @Override
    public void endFile() throws IOException {
        GetConfigValueConsumer.writeStringToFile(fileName, getContent(), fileFactory);
    }

    public String getContent() {
        StringBuilder switchBody = new StringBuilder();

        StringBuilder getterBody = getGetters(switchBody, getterPairs);

        String fullSwitch = wrapSwitchStatement(switchBody);

        return
                GetConfigValueConsumer.getHeader(getClass()) +
                additionalHeaders +
                "float getOutputValueByHash(const int hash) {\n" +
                fullSwitch +
                getterBody + "\treturn EFI_ERROR_CODE;\n" +
                    "}\n" +
                    "float getOutputValueByName(const char *name) {\n" +
                    "\tint hash = djb2lowerCase(name);\n" +
                    "\treturn getOutputValueByHash(hash);\n" +
                    "}\n"
                ;
    }

    @NotNull
    static String wrapSwitchStatement(StringBuilder switchBody) {
        String fullSwitch = switchBody.length() == 0 ? "" :
                (

                        "\tswitch(hash) {\n" + switchBody + "\t}\n");
        return fullSwitch;
    }

    static void BuildGetters(String conditional, List<VariableRecord> getterPairs, StringBuilder switchBody, StringBuilder getterBody, HashMap<Integer, AtomicInteger> hashConflicts ) {
        if (conditional != null) {
            switchBody.append("#if " + conditional + "\n");
        }

        for (VariableRecord pair : getterPairs) {
            String returnLine = "\t\treturn " + pair.getFullName() + ";\n";

            int hash = HashUtil.hash(pair.getUserName());
            if (hashConflicts.get(hash).get() == 1) {
                switchBody.append("// " + pair.getUserName() + "\n");
                switchBody.append("\t\tcase " + hash + ":\n");
                switchBody.append("\t" + returnLine);
            } else {
                getterBody.append(getCompareName(pair.getUserName()));
                getterBody.append(returnLine);
            }
        }

        if (conditional != null) {
            switchBody.append("#endif\n");
        }
    }

    @NotNull
    static StringBuilder getGetters(StringBuilder switchBody, List<VariableRecord> getterPairs) {
        StringBuilder getterBody = new StringBuilder();

        HashMap<Integer, AtomicInteger> hashConflicts = getHashConflicts(getterPairs);

        Map<java.util.Optional<String>, List<VariableRecord>> byConditional = getterPairs.stream().collect(
            Collectors.groupingBy(
                v -> java.util.Optional.ofNullable(v.getConditional())
            )
        );

        Comparator<java.util.Optional<String>> optCmp = Comparator.comparing(
            (java.util.Optional<String> o) -> o.orElse(null),
            Comparator.nullsFirst(String::compareTo)
        );

        byConditional.entrySet().stream().sorted(Map.Entry.comparingByKey(optCmp)).forEach(
            e -> BuildGetters(
                e.getKey().orElse(null),
                e.getValue(),
                switchBody,
                getterBody,
                hashConflicts
            )
        );

        return getterBody;
    }

    @NotNull
    static HashMap<Integer, AtomicInteger> getHashConflicts(List<VariableRecord> getterPairs) {
        HashMap<Integer, AtomicInteger> hashConflicts = new HashMap<>();
        for (VariableRecord pair : getterPairs) {
            hashConflicts.computeIfAbsent(HashUtil.hash(pair.getUserName()), integer -> new AtomicInteger(0)).incrementAndGet();
        }
        return hashConflicts;
    }

}
