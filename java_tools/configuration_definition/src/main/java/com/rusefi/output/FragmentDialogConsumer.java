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
    private boolean hasIndicators;
    private int graphLinesCounter;
    private int linesInCurrentGraph;
    private int currentGraphIndex;

    public FragmentDialogConsumer(String fragmentName) {
        this.fragmentName = fragmentName;
    }

    @Override
    public void startFile() {
    }

    @Override
    public void endFile() throws IOException {
    }


    @Override
    public void handleEndStruct(ReaderState readerState, ConfigStructure structure) throws IOException {
        FieldsStrategy fieldsStrategy = new FieldsStrategy() {
            @Override
            int writeOneField(FieldIterator iterator, String prefix, int tsPosition) {
                ConfigField configField = iterator.cf;

                if (configField.getName().startsWith(ConfigStructureImpl.ALIGNMENT_FILL_AT))
                    return 0;

                ConfigStructure cs = configField.getStructureType();
                if (cs != null) {
                    String extraPrefix = cs.isWithPrefix() ? configField.getName() + "_" : "";
                    return writeFields(cs.getTsFields(), prefix + extraPrefix, tsPosition);
                }

                if (configField.getName().startsWith(ConfigStructureImpl.UNUSED_BIT_PREFIX))
                    return 0;

                if (configField.isBit()) {
                    if (!hasIndicators) {
                        hasIndicators = true;
                        indicatorPanel.append("indicatorPanel = " + getPanelName() + ", 2\n");
                    }
                    indicatorPanel.append("\tindicator = {" + prefix + configField.getName() + "}, " +
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

                graphList.append("\t\tgraphLine = " + prefix + configField.getName() + "\n");
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
        return fragmentName + "IndicatorPanel";
    }

    public String menuLine() {
        if (getContent().isEmpty())
            return "";
        return "\t\t\tsubMenu = " + getDialogName() + ", " + quote(fragmentName) + "\n";
    }

    public String getContent() {
        if (graphLinesCounter > 40) {
            // too many lines - really looks like that huge first legacy model, not having fancy stuff for it
            return "";
        }

        String dialogDeclaration = "dialog = " + getDialogName() + ", " + quote(fragmentName) + "\n";

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
        return fragmentName + "Dialog";
    }

    @NotNull
    private String getGraphControlName() {
        return fragmentName + "_" + currentGraphIndex + "_Graph";
    }
}
