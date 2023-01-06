package com.rusefi;

import com.rusefi.core.Pair;
import com.rusefi.output.ConfigStructure;

public interface ConfigField {
    ConfigStructure getStructureType();

    boolean isArray();

    String getTrueName();

    String getFalseName();

    boolean isBit();

    boolean isDirective();

    int getSize(ConfigField next);

    int[] getArraySizes();

    String getComment();

    String getName();

    String getType();

    int getElementSize();

    boolean isIterate();

    boolean isHasAutoscale();

    ReaderState getState();

    String getTsInfo();

    boolean isFsioVisible();

    String autoscaleSpec();

    double autoscaleSpecNumber();

    Pair<Integer, Integer> autoscaleSpecPair();

    String[] getTokens();

    String getUnits();

    double getMin();

    double getMax();

    int getDigits();
}
