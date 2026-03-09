package com.rusefi;

import com.rusefi.core.Pair;
import com.rusefi.output.ConfigStructure;

import static com.rusefi.output.ConfigStructureImpl.ALIGNMENT_FILL_AT;
import static com.rusefi.output.DataLogConsumer.UNUSED;

public interface ConfigField {
    ConfigField VOID = new ConfigField() {
        @Override
        public ConfigStructure getStructureType() {
            return null;
        }

        @Override
        public ConfigStructure getParentStructureType() {
            return null;
        }

        @Override
        public boolean isArray() {
            return false;
        }

        @Override
        public String getArraySizeVariableName() {
            return null;
        }

        @Override
        public String getTrueName() {
            return null;
        }

        @Override
        public String getFalseName() {
            return null;
        }

        @Override
        public boolean isBit() {
            return false;
        }

        @Override
        public int getSize(ConfigField next) {
            return 0;
        }

        @Override
        public int[] getArraySizes() {
            return new int[0];
        }

        @Override
        public String getComment() {
            return null;
        }

        @Override
        public String getName() {
            return null;
        }

        @Override
        public String getTypeName() {
            return null;
        }

        @Override
        public int getElementSize() {
            return 0;
        }

        @Override
        public boolean isIterate() {
            return false;
        }

        @Override
        public boolean isHasAutoscale() {
            return false;
        }

        @Override
        public ReaderState getState() {
            return null;
        }

        @Override
        public String getTsInfo() {
            return null;
        }
        @Override
        public String autoscaleSpec() {
            return null;
        }

        @Override
        public double autoscaleSpecNumber() {
            return 0;
        }

        @Override
        public Pair<Integer, Integer> autoscaleSpecPair() {
            return null;
        }

        @Override
        public String getUnits() {
            return null;
        }

        @Override
        public double getMin() {
            return 0;
        }

        @Override
        public double getMax() {
            return 0;
        }

        @Override
        public String getCategory() {
            return "";
        }

        @Override
        public int getDigits() {
            return 0;
        }

        @Override
        public String getIterateOriginalName() {
            return null;
        }

        @Override
        public int getIterateIndex() {
            return 0;
        }

        @Override
        public boolean isFromIterate() {
            return false;
        }

        @Override
        public String getCommentTemplated() {
            return null;
        }
        @Override
        public void setTsInfo(String newtsInfo) {
        }
        @Override
        public boolean isWithinStruct(String structName) {
            return false;
        }
    };

    default boolean isUnusedField() {
        return getName().contains(UNUSED) || getName().contains(ALIGNMENT_FILL_AT);
    }

    default String getOriginalArrayName() {
    // FIXME: this method fails in case of a array of structs with a array inside (ie only GPPWM at the moment)
        if (isFromIterate()) {
            return getIterateOriginalName() + "[" + (getIterateIndex() - 1) + "]";
        } else {
            return getName();
        }
    }

    ConfigStructure getParentStructureType();

    String getTypeName();

    ConfigStructure getStructureType();

    boolean isArray();

    String getArraySizeVariableName();

    String getTrueName();

    String getFalseName();

    boolean isBit();

    int getSize(ConfigField next);

    int[] getArraySizes();

    String getComment();

    String getName();


    int getElementSize();

    boolean isIterate();

    boolean isHasAutoscale();

    ReaderState getState();

    String getTsInfo();
    String autoscaleSpec();

    double autoscaleSpecNumber();

    Pair<Integer, Integer> autoscaleSpecPair();

    String getUnits();

    double getMin();

    double getMax();

    int getDigits();

    String getCategory();

    String getIterateOriginalName();

    int getIterateIndex();

    /**
     * this is about array syntax: sometimes we handle those as arrays and sometimes we expand into field1, field2, field3
     */
    boolean isFromIterate();

    String getCommentTemplated();

    void setTsInfo(String tsInfo);

    boolean isWithinStruct(String structName);
}
