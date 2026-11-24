package com.rusefi.ui.widgets.tune;

import com.formdev.flatlaf.FlatClientProperties;
import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.ScalarIniField;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import javax.swing.border.Border;
import java.awt.*;
import java.awt.geom.RoundRectangle2D;
import java.util.function.Consumer;
import java.util.function.Supplier;

/**
 * [tag:unburned_marks] Marks one setting row whose value differs from the last burned tune (#10671):
 * a light warning background with a bar in the row's empty left margin, and at the end of the row the
 * burned value with a Revert button. Painted by a border that keeps the row's own insets, so nothing
 * moves when the mark comes and goes; {@link FieldHighlight} wraps this border and keeps it.
 * <p>
 * {@link CalibrationFieldFactory} leaves an empty slot at the end of every setting row and a setter
 * for the row's editor ({@link #SLOT}, {@link #EDITOR}); {@link CalibrationDialogWidget} installs the
 * mark and refreshes it after every edit on the page.
 */
final class UnburnedRowMark {
    /** Client property on a setting row: the panel at the end of the row that holds the burned value. */
    static final String SLOT = "rusefi.unburnedSlot";
    /** Client property on a setting row: {@code Consumer<String>} showing a stored value in the editor. */
    static final String EDITOR = "rusefi.unburnedEditor";

    private static final int ARC = 10;
    /** Inside the 10 px strut that starts every setting row, like {@link FieldHighlight}'s bar. */
    private static final int BAR_WIDTH = 4;

    private final JComponent row;
    private final IniField field;
    private final JPanel slot;
    private final JLabel burnedLabel = new JLabel();
    private final JButton revertButton = new JButton("Revert");
    private boolean changed;

    private UnburnedRowMark(JComponent row, IniField field, JPanel slot) {
        this.row = row;
        this.field = field;
        this.slot = slot;
    }

    /** The empty, hidden slot a setting row ends with; filled when the setting is changed. */
    static JPanel createSlot() {
        JPanel slot = new JPanel();
        slot.setName("unburnedSlot");
        slot.setLayout(new BoxLayout(slot, BoxLayout.X_AXIS));
        slot.setOpaque(false);
        slot.setVisible(false);
        return slot;
    }

    /**
     * Installs the mark on a row made by {@link CalibrationFieldFactory#createFieldRow}; null when the row
     * has no slot (a label-only row).
     *
     * @param workingImage the image the row's editor writes to
     * @param burned       the last burned tune, or null
     * @param onReverted   called after Revert put the burned value back, like an edit of the row
     */
    @Nullable
    static UnburnedRowMark install(JComponent row, IniField field, Supplier<ConfigurationImage> workingImage,
                                   Supplier<ConfigurationImage> burned, Runnable onReverted) {
        Object slot = row.getClientProperty(SLOT);
        if (!(slot instanceof JPanel)) {
            return null;
        }
        UnburnedRowMark mark = new UnburnedRowMark(row, field, (JPanel) slot);
        mark.build(workingImage, burned, onReverted);
        row.setBorder(mark.new Mark(row.getBorder()));
        return mark;
    }

    private void build(Supplier<ConfigurationImage> workingImage, Supplier<ConfigurationImage> burned,
                       Runnable onReverted) {
        burnedLabel.setName("unburnedValue");
        burnedLabel.setForeground(foreground());
        revertButton.setName("unburnedRevert");
        revertButton.putClientProperty(FlatClientProperties.BUTTON_TYPE, FlatClientProperties.BUTTON_TYPE_TOOLBAR_BUTTON);
        revertButton.setForeground(foreground());
        revertButton.setFocusable(false);
        revertButton.setToolTipText(UnburnedWording.BURNED.revertTooltip);
        revertButton.addActionListener(e -> revert(workingImage.get(), burned.get(), onReverted));
        slot.add(Box.createHorizontalStrut(12));
        slot.add(burnedLabel);
        slot.add(Box.createHorizontalStrut(4));
        slot.add(revertButton);
    }

    /** Shows or hides the mark for {@code changes}. */
    void refresh(UnburnedChanges changes, @Nullable ConfigurationImage burned) {
        refresh(changes, burned, UnburnedWording.BURNED);
    }

    /** Shows or hides the mark for {@code changes}, naming the reference tune by {@code wording}. */
    void refresh(UnburnedChanges changes, @Nullable ConfigurationImage burned, UnburnedWording wording) {
        boolean nowChanged = burned != null && changes.isChanged(field);
        if (nowChanged) {
            burnedLabel.setText(wording.reference + ": " + burnedText(field, burned));
            revertButton.setToolTipText(wording.revertTooltip);
            revertButton.getAccessibleContext().setAccessibleName("Revert to " + burnedLabel.getText());
        }
        if (nowChanged == changed) {
            return;
        }
        changed = nowChanged;
        slot.setVisible(nowChanged);
        row.revalidate();
        row.repaint();
    }

    boolean isChanged() {
        return changed;
    }

    /** The burned value as the row shows values: units for numbers, On/Off for check boxes. */
    static String burnedText(IniField field, ConfigurationImage burned) {
        String raw = ConfigurationImageGetterSetter.getStringValue(field, burned);
        if (field instanceof EnumIniField) {
            EnumIniField enumField = (EnumIniField) field;
            if (CalibrationFieldFactory.isCheckboxEnum(enumField)) {
                return CalibrationFieldFactory.isCheckboxOn(raw) ? "On" : "Off";
            }
            return raw.replace("\"", "");
        }
        String units = field instanceof ScalarIniField ? field.getUnits() : null;
        return units == null || units.trim().isEmpty() ? raw : raw + " " + units.replace("\"", "").trim();
    }

    private void revert(@Nullable ConfigurationImage working, @Nullable ConfigurationImage burned, Runnable onReverted) {
        if (working == null || burned == null) {
            return;
        }
        Object editor = row.getClientProperty(EDITOR);
        if (editor instanceof Consumer) {
            // shown first: the editor writes what it shows, which can be rounded to the displayed digits
            @SuppressWarnings("unchecked")
            Consumer<String> setter = (Consumer<String>) editor;
            setter.accept(ConfigurationImageGetterSetter.getStringValue(field, burned));
        }
        // then the exact burned value, bit for bit
        UnburnedChanges.restore(field, burned, working);
        onReverted.run();
    }

    static Color bar() {
        return TuningStyle.color("Console.Tuning.unburned.bar", TuningStyle.dirty());
    }

    static Color background() {
        return TuningStyle.color("Console.Tuning.unburned.background", TuningStyle.cardBackground());
    }

    static Color foreground() {
        return TuningStyle.color("Console.Tuning.unburned.foreground", TuningStyle.dirty());
    }

    /** A warning-tinted rounded box with a bar on the left, around the row's own border, while changed. */
    private final class Mark implements Border {
        private final Border inner;

        Mark(Border inner) {
            this.inner = inner;
        }

        @Override
        public void paintBorder(Component c, Graphics g, int x, int y, int width, int height) {
            if (inner != null) {
                inner.paintBorder(c, g, x, y, width, height);
            }
            if (!changed) {
                return;
            }
            Graphics2D g2 = (Graphics2D) g.create();
            try {
                g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                // painted before the row's children, so the fill sits behind the label and editors
                g2.setColor(background());
                g2.fill(new RoundRectangle2D.Double(x, y, width, height, ARC, ARC));
                g2.setColor(bar());
                g2.fill(new RoundRectangle2D.Double(x + 2, y + 2, BAR_WIDTH, height - 4, BAR_WIDTH, BAR_WIDTH));
            } finally {
                g2.dispose();
            }
        }

        @Override
        public Insets getBorderInsets(Component c) {
            return inner != null ? inner.getBorderInsets(c) : new Insets(0, 0, 0, 0);
        }

        @Override
        public boolean isBorderOpaque() {
            return false;
        }
    }
}
