package com.rusefi.output;

import com.rusefi.ConfigField;
import com.rusefi.ReaderState;
import org.jetbrains.annotations.NotNull;

import java.io.IOException;

import static com.rusefi.output.JavaSensorsConsumer.quote;

public class FragmentDialogConsumer implements ConfigurationConsumer {
    private final StringBuilder graphList = new StringBuilder();

    private final StringBuilder indicatorPanel = new StringBuilder();
    private final String fragmentName;
    private final String variableNameSuffix;
    private final String variableNamePrefix;
    private boolean hasIndicators;
    private int graphLinesCounter;
    private int linesInCurrentGraph;
    private int currentGraphIndex;

    public FragmentDialogConsumer(String fragmentName, String variableNamePrefix, String variableNameSuffix) {
        this.fragmentName = fragmentName;
        this.variableNamePrefix = variableNamePrefix;
        this.variableNameSuffix = variableNameSuffix;
    }

    @Override
    public void handleEndStruct(ReaderState readerState, ConfigStructure structure) throws IOException {
        FieldsStrategy fieldsStrategy = new FieldsStrategy() {
            @Override
            int writeOneField(FieldIterator iterator, String prefix, int tsPosition) {
                ConfigField configField = iterator.cf;

                if (configField.isUnusedField())
                    return 0;

                ConfigStructure cs = configField.getStructureType();
                if (cs != null) {
                    String extraPrefix = cs.isWithPrefix() ? configField.getName() + "_" : "";
                    return writeFields(cs.getTsFields(), prefix + extraPrefix, tsPosition);
                }

                if (configField.isBit()) {
                    if (!hasIndicators) {
                        hasIndicators = true;
                        indicatorPanel.append("indicatorPanel = " + getPanelName() + ", 2\n");
                    }
                    indicatorPanel.append("\tindicator = {" + prefix + variableNamePrefix + configField.getName() + "}, " +
                            "\"" + configField.getName() + " No\", " +
                            "\"" + configField.getName() + " Yes\"" +
                            "\n");
                    return 0;
                }

                if (graphLinesCounter == 0)
                    startNewGraph();
                graphLinesCounter++;

                if (linesInCurrentGraph == 4) {
                    linesInCurrentGraph = 0;
                    startNewGraph();
                }

                graphList.append("\t\tgraphLine = " + prefix + variableNamePrefix + configField.getName() + "\n");
                linesInCurrentGraph++;


                return 0;
            }
        };
        fieldsStrategy.run(readerState, structure, 0);

    }

    private void startNewGraph() {
        currentGraphIndex++;
        graphList.append("\tliveGraph = " + getGraphControlName() +
                ", " + quote("Graph") + ", South\n");

    }

    @NotNull
    private String getPanelName() {
        return getFragmentNameWithSuffix() + "IndicatorPanel";
    }

    public String menuLine() {
        if (getContent().isEmpty())
            return "";
        return "\t\t\tsubMenu = " + getDialogName() + ", " + quote(getFragmentNameWithSuffix()) + "\n";
    }

    public String getContent() {
        if (graphLinesCounter > 40) {
            // too many lines - really looks like that huge first legacy model, not having fancy stuff for it
            return "";
        }

        String dialogDeclaration = "dialog = " + getDialogName() + ", " + quote(getFragmentNameWithSuffix()) + "\n";

        String indicatorPanelUsageLine = (indicatorPanel.length() > 0) ? "\tpanel = " + getPanelName() + "\n" : "";


        return indicatorPanel + "\n" +
                dialogDeclaration +
                indicatorPanelUsageLine +
                graphList +
                "\n"
                ;
    }

    @NotNull
    private String getDialogName() {
        return getFragmentNameWithSuffix() + "Dialog";
    }

    @NotNull
    private String getGraphControlName() {
        return getFragmentNameWithSuffix() + "_" + currentGraphIndex + "_Graph";
    }

    @NotNull
    private String getFragmentNameWithSuffix() {
        return fragmentName + variableNameSuffix;
    }
}
