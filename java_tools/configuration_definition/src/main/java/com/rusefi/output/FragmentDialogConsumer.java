package com.rusefi.output;

import com.rusefi.ConfigField;
import com.rusefi.ReaderState;

import java.io.IOException;

public class FragmentDialogConsumer implements ConfigurationConsumer {
    private final StringBuilder sb = new StringBuilder();

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
            int writeOneField(ConfigField configField, String prefix, int tsPosition, ConfigField next, int bitIndex, ConfigField prev) throws IOException {
                if (configField.getName().startsWith(ConfigStructure.UNUSED_BIT_PREFIX))
                    return 0;

                if (configField.isBit()) {

                    if (!hasIndicators) {
                        hasIndicators = true;
                        indicatorPanel.append("indicatorPanel = " +
                                fragmentName +
                                "IndicatorPanel, 2\n");
                    }
                    indicatorPanel.append("indicator = {" + configField.getName() + "}, \"No\", \"Yes\"\n");

                }


                return 0;
            }
        };
        fieldsStrategy.run(readerState, structure);

    }

    public String getContent() {
        return indicatorPanel + sb.toString();
    }
}
