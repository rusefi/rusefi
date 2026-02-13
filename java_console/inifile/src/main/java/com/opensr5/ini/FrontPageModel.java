package com.opensr5.ini;

import java.util.ArrayList;
import java.util.List;

public class FrontPageModel {
    private final List<String> gaugeNames = new ArrayList<>();
    private final List<IndicatorModel> indicators = new ArrayList<>();

    public List<String> getGaugeNames() {
        return gaugeNames;
    }

    public List<IndicatorModel> getIndicators() {
        return indicators;
    }
}
