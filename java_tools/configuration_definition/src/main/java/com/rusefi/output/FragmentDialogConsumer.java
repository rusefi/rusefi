package com.rusefi.output;

import com.rusefi.ConfigField;
import com.rusefi.ReaderState;
import com.rusefi.newparse.layout.ScalarLayout;
import org.jetbrains.annotations.NotNull;

import java.io.IOException;

import static com.rusefi.output.JavaSensorsConsumer.quote;

public class FragmentDialogConsumer {
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

    public void handleScalar(String name) {
        if (graphLinesCounter == 0)
            startNewGraph();
        graphLinesCounter++;

        if (linesInCurrentGraph == 4) {
            linesInCurrentGraph = 0;
            startNewGraph();
        }

        graphList.append("\t\tgraphLine = " + name + "\n");
        linesInCurrentGraph++;
    }

    public void handleBit(String name, String displayName) {
        if (!hasIndicators) {
            hasIndicators = true;
            indicatorPanel.append("indicatorPanel = " + getPanelName() + ", 2\n");
        }

        indicatorPanel.append("\tindicator = {" + name + "}, " +
                "\"" + displayName + " No\", " +
                "\"" + displayName + " Yes\"" +
                "\n");
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
