package com.rusefi.tune.ve;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.TableModel;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.config.generated.VariableRegistryValues;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Optional;

/**
 * Binds {@code veTableTbl} from an active INI + ConfigurationImage.
 * <p>
 * Validates load mode, reads axes, encodes generated VE Z values into a cloned
 * image, and proves that only VE Z bytes change.
 */
public class VeTableBinding {

    private static final String FUEL_ALGORITHM_FIELD   = "fuelAlgorithm";
    private static final String VE_OVERRIDE_FIELD      = "veOverrideMode";
    private static final String SEPARATE_IDLE_VE_FIELD = "useSeparateIdleVe";

    public static class BindingError extends Exception {
        public BindingError(String message) {
            super(message);
        }
    }

    /** Resolved and validated binding for veTableTbl. */
    public static class BoundTable {
        public final ArrayIniField rpmField;    // X bins (1D)
        public final ArrayIniField mapField;    // Y bins (1D)
        public final ArrayIniField veField;     // Z values (2D)
        public final double[] rpmAxis;          // decoded RPM values from image
        public final double[] mapAxis;          // decoded load values from image (kPa)
        public final double[][] currentVe;      // decoded current VE[loadRow][rpmCol]
        public final int nLoad;
        public final int nRpm;
        public final List<String> effectiveVeWarnings;

        BoundTable(ArrayIniField rpmField, ArrayIniField mapField, ArrayIniField veField,
                   double[] rpmAxis, double[] mapAxis, double[][] currentVe,
                   List<String> effectiveVeWarnings) {
            this.rpmField = rpmField;
            this.mapField = mapField;
            this.veField  = veField;
            this.rpmAxis  = rpmAxis;
            this.mapAxis  = mapAxis;
            this.currentVe = currentVe;
            this.nLoad = mapAxis.length;
            this.nRpm  = rpmAxis.length;
            this.effectiveVeWarnings = Collections.unmodifiableList(new ArrayList<>(effectiveVeWarnings));
        }
    }

    /**
     * Resolve and validate {@code veTableTbl} from the active INI and image.
     *
     * @throws BindingError when the table cannot be bound (unsupported mode, bad axes, etc.)
     */
    public static BoundTable bind(IniFileModel ini, ConfigurationImage image) throws BindingError {
        TableModel table = ini.getTable("veTableTbl");
        if (table == null) {
            throw new BindingError("veTableTbl not found in INI.");
        }

        ArrayIniField rpmField = requireArrayField(ini, table.getXBinsConstant(), "RPM axis (X)");
        ArrayIniField mapField = requireArrayField(ini, table.getYBinsConstant(), "load axis (Y)");
        ArrayIniField veField  = requireArrayField(ini, table.getZBinsConstant(), "VE values (Z)");

        // Axes must be 1D (cols == 1 for curves in TS INI)
        if (rpmField.getCols() != 1) {
            throw new BindingError("RPM axis field '" + rpmField.getName() + "' must be 1D (cols==1).");
        }
        if (mapField.getCols() != 1) {
            throw new BindingError("Load axis field '" + mapField.getName() + "' must be 1D (cols==1).");
        }

        int nRpm  = rpmField.getRows();
        int nLoad = mapField.getRows();

        if (veField.getCols() != nRpm) {
            throw new BindingError("VE Z cols (" + veField.getCols() +
                ") != RPM axis length (" + nRpm + ").");
        }
        if (veField.getRows() != nLoad) {
            throw new BindingError("VE Z rows (" + veField.getRows() +
                ") != load axis length (" + nLoad + ").");
        }

        checkRange(image, rpmField);
        checkRange(image, mapField);
        checkRange(image, veField);

        checkLoadMode(ini, image);

        double[] rpmAxis = readAxis(image, rpmField, nRpm);
        double[] mapAxis = readAxis(image, mapField, nLoad);
        validateAscendingFinite(rpmAxis, "RPM axis");
        validateAscendingFinite(mapAxis, "load axis");

        double[][] currentVe = readZ(image, veField, nLoad, nRpm);
        List<String> warnings = buildEffectiveVeWarnings(ini, image);

        return new BoundTable(rpmField, mapField, veField, rpmAxis, mapAxis, currentVe, warnings);
    }

    /**
     * Encode a generated VE surface into a clone of {@code source}.
     * Asserts byte-identity: every byte outside the VE Z field must be unchanged.
     *
     * @param source   current working image (not mutated)
     * @param bt       bound table from {@link #bind}
     * @param veTable  generated values veTable[loadRow][rpmCol]
     * @return cloned image with only VE Z bytes changed
     * @throws BindingError on dimension mismatch or byte-identity violation
     */
    public static ConfigurationImage applyToClone(ConfigurationImage source,
                                                   BoundTable bt,
                                                   double[][] veTable) throws BindingError {
        if (veTable.length != bt.nLoad) {
            throw new BindingError("veTable load rows (" + veTable.length +
                ") != bound nLoad (" + bt.nLoad + ").");
        }
        for (int r = 0; r < veTable.length; r++) {
            if (veTable[r].length != bt.nRpm) {
                throw new BindingError("veTable row " + r + " length (" + veTable[r].length +
                    ") != bound nRpm (" + bt.nRpm + ").");
            }
        }

        ConfigurationImage clone = source.clone();
        ArrayIniField z = bt.veField;
        double multiplier = z.getMultiplier();

        for (int r = 0; r < bt.nLoad; r++) {
            for (int c = 0; c < bt.nRpm; c++) {
                int byteOffset  = z.getOffset(r, c);
                int storageSize = z.getType().getStorageSize();
                ByteBuffer bb   = clone.getByteBuffer(byteOffset, storageSize);
                int rawEncoded  = (int) Math.round(veTable[r][c] / multiplier);
                z.getType().writeRawValue(bb, rawEncoded);
            }
        }

        // Prove only VE Z bytes changed
        byte[] before = source.getContent();
        byte[] after  = clone.getContent();
        int zStart = z.getOffset();
        int zEnd   = zStart + z.getSize();
        for (int i = 0; i < before.length; i++) {
            if ((i < zStart || i >= zEnd) && before[i] != after[i]) {
                throw new BindingError("Byte " + i + " outside VE Z range [" +
                    zStart + "," + zEnd + ") changed during encoding.");
            }
        }

        return clone;
    }

    /**
     * Decode VE Z values from a (possibly cloned) image back into a double array.
     * Use this to read back quantized values after {@link #applyToClone}.
     */
    public static double[][] decodeZ(ConfigurationImage image, BoundTable bt) {
        return readZ(image, bt.veField, bt.nLoad, bt.nRpm);
    }

    // -- private helpers --

    private static ArrayIniField requireArrayField(IniFileModel ini, String name, String role)
            throws BindingError {
        if (name == null || name.isEmpty()) {
            throw new BindingError("INI table missing " + role + " field name.");
        }
        Optional<IniField> opt = ini.findIniField(name);
        if (!opt.isPresent()) {
            throw new BindingError(role + " field '" + name + "' not found in INI.");
        }
        IniField field = opt.get();
        if (!(field instanceof ArrayIniField)) {
            throw new BindingError(role + " field '" + name + "' is not an ArrayIniField.");
        }
        return (ArrayIniField) field;
    }

    private static void checkRange(ConfigurationImage image, ArrayIniField field) throws BindingError {
        int end = field.getOffset() + field.getSize();
        if (field.getOffset() < 0 || end > image.getSize()) {
            throw new BindingError("Field '" + field.getName() + "' byte range [" +
                field.getOffset() + "," + end + ") exceeds image size (" + image.getSize() + ").");
        }
    }

    private static void checkLoadMode(IniFileModel ini, ConfigurationImage image) throws BindingError {
        int fuelAlgo = readOrdinal(ini, image, FUEL_ALGORITHM_FIELD, "fuelAlgorithm");
        if (fuelAlgo != VariableRegistryValues.engine_load_mode_e_LM_SPEED_DENSITY) {
            throw new BindingError(
                "fuelAlgorithm ordinal=" + fuelAlgo +
                " is not LM_SPEED_DENSITY (" +
                VariableRegistryValues.engine_load_mode_e_LM_SPEED_DENSITY +
                "). archetype-base-ve-v1 requires Speed Density.");
        }
        int veOverride = readOrdinal(ini, image, VE_OVERRIDE_FIELD, "veOverrideMode");
        if (veOverride != VariableRegistryValues.ve_override_e_VE_None &&
            veOverride != VariableRegistryValues.ve_override_e_VE_MAP) {
            throw new BindingError(
                "veOverrideMode ordinal=" + veOverride +
                " is not VE_None (0) or VE_MAP (1). VE_TPS and other overrides are not supported.");
        }
    }

    private static int readOrdinal(IniFileModel ini, ConfigurationImage image,
                                    String fieldName, String role) throws BindingError {
        Optional<IniField> opt = ini.findIniField(fieldName);
        if (!opt.isPresent()) {
            throw new BindingError("INI field '" + fieldName + "' (" + role + ") not found.");
        }
        Double v = image.readNumericValue(opt.get());
        if (v == null) {
            throw new BindingError("Cannot read numeric ordinal for '" + fieldName +
                "' — field type not supported by readNumericValue.");
        }
        return (int) Math.round(v);
    }

    private static double[] readAxis(ConfigurationImage image, ArrayIniField field, int count) {
        double[] axis = new double[count];
        double multiplier = field.getMultiplier();
        for (int i = 0; i < count; i++) {
            // 1D array: col index is always 0
            int byteOffset  = field.getOffset(i, 0);
            int storageSize = field.getType().getStorageSize();
            ByteBuffer bb   = image.getByteBuffer(byteOffset, storageSize);
            axis[i] = field.getType().readRawValue(bb) * multiplier;
        }
        return axis;
    }

    private static double[][] readZ(ConfigurationImage image, ArrayIniField field, int nLoad, int nRpm) {
        double[][] z = new double[nLoad][nRpm];
        double multiplier = field.getMultiplier();
        for (int r = 0; r < nLoad; r++) {
            for (int c = 0; c < nRpm; c++) {
                int byteOffset  = field.getOffset(r, c);
                int storageSize = field.getType().getStorageSize();
                ByteBuffer bb   = image.getByteBuffer(byteOffset, storageSize);
                z[r][c] = field.getType().readRawValue(bb) * multiplier;
            }
        }
        return z;
    }

    private static void validateAscendingFinite(double[] axis, String name) throws BindingError {
        for (int i = 0; i < axis.length; i++) {
            if (!Double.isFinite(axis[i])) {
                throw new BindingError(name + "[" + i + "] is not finite: " + axis[i]);
            }
            if (i > 0 && axis[i] <= axis[i - 1]) {
                throw new BindingError(name + " is not strictly ascending at index " + i +
                    ": " + axis[i - 1] + " >= " + axis[i]);
            }
        }
    }

    private static List<String> buildEffectiveVeWarnings(IniFileModel ini, ConfigurationImage image) {
        List<String> warnings = new ArrayList<>();
        Optional<IniField> idleVeOpt = ini.findIniField(SEPARATE_IDLE_VE_FIELD);
        if (idleVeOpt.isPresent()) {
            Double v = image.readNumericValue(idleVeOpt.get());
            if (v != null && v.intValue() != 0) {
                warnings.add("Separate idle VE is enabled: the generated primary table may not " +
                    "be the effective VE at idle RPM.");
            }
        }
        return warnings;
    }
}
