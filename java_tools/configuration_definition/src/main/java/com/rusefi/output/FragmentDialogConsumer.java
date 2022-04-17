package com.rusefi.output;

import com.rusefi.ConfigField;
import com.rusefi.ReaderState;
import org.jetbrains.annotations.NotNull;

import java.io.IOException;

import static com.rusefi.output.JavaSensorsConsumer.quote;

public class FragmentDialogConsumer implements ConfigurationConsumer {
    private final StringBuilder graphLines = new StringBuilder();

    private final StringBuilder indicatorPanel = new StringBuilder();
    private final String fragmentName;
    private boolean hasIndicators;

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
            int writeOneField(FieldIterator iterator, ConfigField _configField, String prefix, int tsPosition, ConfigField _next, int _bitIndex, ConfigField prev) {
                ConfigField configField = iterator.cf;
                ConfigField next = iterator.next;
                int bitIndex = iterator.bitState.get();

                if (configField.getName().startsWith(ConfigStructure.UNUSED_BIT_PREFIX))
                    return 0;

                if (configField.isBit()) {

                    if (!hasIndicators) {
                        hasIndicators = true;
                        indicatorPanel.append("indicatorPanel = " + getPanelName() + ", 2\n");
                    }
                    indicatorPanel.append("\tindicator = {" + configField.getName() + "}, \"No\", \"Yes\"\n");

                }

                graphLines.append("\tgraphLine = " + configField.getName() + "\n");


                return 0;
            }
        };
        fieldsStrategy.run(readerState, structure, 0);

    }

    @NotNull
    private String getPanelName() {
        return fragmentName + "IndicatorPanel";
    }

    public String getContent() {

        String dialogDeclaration = "dialog = " + fragmentName + "Dialog, " + quote(fragmentName) + "\n";

        String indicatorPanelUsageLine = (indicatorPanel.length() > 0) ? "\tpanel = " + getPanelName() + "\n" : "";


        String liveGraphControlDeclaration = "liveGraph = " + getGraphControlName() +
                ", " + quote("Graph") + ", South\n";

        return indicatorPanel + "\n" +
                liveGraphControlDeclaration +
                graphLines + "\n" +
                dialogDeclaration +
                indicatorPanelUsageLine +
                "\tpanel = " + getGraphControlName() + "\n"
                ;
    }

    @NotNull
    private String getGraphControlName() {
        return fragmentName + "Graph";
    }
}
