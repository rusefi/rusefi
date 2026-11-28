package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.PcVariableArrayIniField;
import org.jetbrains.annotations.Nullable;

import java.nio.ByteOrder;
import java.util.HashSet;
import java.util.Set;

/**
 * [tag:unburned_marks] Tells which settings and table cells of the tune being edited differ from the
 * last burned tune (#10671), so the Tuning tab can mark them until the next burn.
 * <p>
 * The burned tune is {@link TuningToolbarWidget#getBaselineImage()}: the image as last burned, loaded
 * or reconciled on connect. Edits are sent to the ECU without burning, so "differs from the baseline"
 * means "not burned yet" (offline: "not saved").
 * <p>
 * Values are compared, not edits tracked, so a value typed back to its burned value is unmarked again.
 * On/off settings and other bit-field enums are compared only on their own bits: several of them share
 * one stored word, and a byte comparison would mark every neighbour of a changed bit.
 * <p>
 * Both images hold TunerStudio page 0 only. Fields on other pages (read separately by
 * {@link CalibrationDialogWidget}) and PC-side variables are never reported as changed.
 * Without a burned tune nothing is marked.
 */
public final class UnburnedChanges {
    private static final UnburnedChanges NONE = new UnburnedChanges(null, null);

    @Nullable
    private final ConfigurationImage current;
    @Nullable
    private final ConfigurationImage burned;
    private final int page;

    private UnburnedChanges(@Nullable ConfigurationImage current, @Nullable ConfigurationImage burned) {
        this(current, burned, 0);
    }

    private UnburnedChanges(@Nullable ConfigurationImage current, @Nullable ConfigurationImage burned, int page) {
        this.current = current;
        this.burned = burned;
        this.page = page;
    }

    /**
     * @param current the tune being edited
     * @param burned  the last burned tune; null marks nothing
     */
    public static UnburnedChanges of(@Nullable ConfigurationImage current, @Nullable ConfigurationImage burned) {
        return onPage(current, burned, 0);
    }

    static UnburnedChanges onPage(@Nullable ConfigurationImage current, @Nullable ConfigurationImage burned, int page) {
        if (current == null || burned == null || current.getByteOrder() != burned.getByteOrder()) {
            return NONE;
        }
        return new UnburnedChanges(current, burned, page);
    }

    public static UnburnedChanges none() {
        return NONE;
    }

    /** @return true when the field, or for a table or curve any of its cells, differs from the burned tune */
    public boolean isChanged(@Nullable IniField field) {
        if (!comparable(field)) {
            return false;
        }
        if (field instanceof EnumIniField) {
            return bitsDiffer((EnumIniField) field);
        }
        return bytesDiffer(field.getOffset(), field.getSize());
    }

    /** @return true when one cell of a table or curve differs from the burned tune */
    public boolean isCellChanged(@Nullable ArrayIniField field, int row, int col) {
        if (!comparable(field) || row < 0 || col < 0 || row >= field.getRows() || col >= field.getCols()) {
            return false;
        }
        return bytesDiffer(field.getOffset(row, col), field.getType().getStorageSize());
    }

    /** @return how many cells of a table or curve differ from the burned tune */
    public int countChangedCells(@Nullable ArrayIniField field) {
        if (!comparable(field)) {
            return 0;
        }
        int count = 0;
        for (int row = 0; row < field.getRows(); row++) {
            for (int col = 0; col < field.getCols(); col++) {
                if (bytesDiffer(field.getOffset(row, col), field.getType().getStorageSize())) {
                    count++;
                }
            }
        }
        return count;
    }

    /**
     * Counts changed values over many fields: one per setting, one per table or curve cell. Values stored
     * at the same place (fields that alias the same storage, e.g. a table shown in two units) count once.
     */
    public int countChanged(Iterable<? extends IniField> fields) {
        Set<Long> seen = new HashSet<>();
        int count = 0;
        for (IniField field : fields) {
            if (!comparable(field)) {
                continue;
            }
            if (field instanceof ArrayIniField) {
                ArrayIniField array = (ArrayIniField) field;
                if (!bytesDiffer(array.getOffset(), array.getSize())) {
                    continue; // most tables are unchanged: one pass over the bytes
                }
                int cellSize = array.getType().getStorageSize();
                for (int row = 0; row < array.getRows(); row++) {
                    for (int col = 0; col < array.getCols(); col++) {
                        int offset = array.getOffset(row, col);
                        if (bytesDiffer(offset, cellSize) && seen.add(key(offset, 0, cellSize * 8))) {
                            count++;
                        }
                    }
                }
            } else if (field instanceof EnumIniField) {
                EnumIniField e = (EnumIniField) field;
                if (bitsDiffer(e) && seen.add(key(e.getOffset(), e.getBitPosition(), e.getBitSize0() + 1))) {
                    count++;
                }
            } else if (bytesDiffer(field.getOffset(), field.getSize())
                && seen.add(key(field.getOffset(), 0, field.getSize() * 8))) {
                count++;
            }
        }
        return count;
    }

    /**
     * Copies the burned value of {@code field} into {@code target} exactly: bit-field enums only on their
     * own bits, so the settings sharing their word keep their edits. Does nothing for a field that is
     * never reported as changed.
     */
    public static void restore(IniField field, ConfigurationImage burned, ConfigurationImage target) {
        of(target, burned).copyField(field);
    }

    /** Copies the reference field into the current image, preserving unrelated bits. */
    void copyField(IniField field) {
        if (!comparable(field)) {
            return;
        }
        ConfigurationImage target = current;
        if (field instanceof EnumIniField) {
            EnumIniField e = (EnumIniField) field;
            int size = e.getSize();
            int bitCount = e.getBitSize0() + 1;
            long mask = bitCount >= 64 ? -1L : ((1L << bitCount) - 1) << e.getBitPosition();
            long value = (readUnsigned(target, e.getOffset(), size) & ~mask)
                | (readUnsigned(burned, e.getOffset(), size) & mask);
            writeUnsigned(target, e.getOffset(), size, value);
            return;
        }
        System.arraycopy(burned.getContent(), field.getOffset(), target.getContent(), field.getOffset(), field.getSize());
    }

    private boolean comparable(@Nullable IniField field) {
        if (field == null || current == null || burned == null) {
            return false;
        }
        if (field.getPageIndex() != page || field instanceof PcVariableArrayIniField) {
            return false;
        }
        int end = field.getOffset() + field.getSize();
        return field.getOffset() >= 0 && end >= field.getOffset() && end <= current.getSize() && end <= burned.getSize();
    }

    private boolean bytesDiffer(int offset, int size) {
        byte[] a = current.getContent();
        byte[] b = burned.getContent();
        for (int i = offset; i < offset + size; i++) {
            if (a[i] != b[i]) {
                return true;
            }
        }
        return false;
    }

    private boolean bitsDiffer(EnumIniField field) {
        int size = field.getSize();
        int bitCount = field.getBitSize0() + 1;
        int position = field.getBitPosition();
        long mask = bitCount >= 64 ? -1L : ((1L << bitCount) - 1) << position;
        long a = readUnsigned(current, field.getOffset(), size);
        long b = readUnsigned(burned, field.getOffset(), size);
        return ((a ^ b) & mask) != 0;
    }

    private static long readUnsigned(ConfigurationImage image, int offset, int size) {
        byte[] content = image.getContent();
        boolean little = image.getByteOrder() == ByteOrder.LITTLE_ENDIAN;
        long value = 0;
        for (int i = 0; i < size; i++) {
            int index = little ? offset + size - 1 - i : offset + i;
            value = (value << 8) | (content[index] & 0xFF);
        }
        return value;
    }

    private static void writeUnsigned(ConfigurationImage image, int offset, int size, long value) {
        byte[] content = image.getContent();
        boolean little = image.getByteOrder() == ByteOrder.LITTLE_ENDIAN;
        for (int i = 0; i < size; i++) {
            int index = little ? offset + i : offset + size - 1 - i;
            content[index] = (byte) (value >>> (8 * i));
        }
    }

    private static long key(int offset, int bitPosition, int bitCount) {
        return ((long) offset << 32) | ((long) (bitPosition & 0xFF) << 24) | (bitCount & 0xFFFFFF);
    }
}
