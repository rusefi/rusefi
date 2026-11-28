package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.CurveModel;
import com.opensr5.ini.DialogModel;
import com.opensr5.ini.ExpressionEvaluator;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.PanelModel;
import com.opensr5.ini.TableModel;
import com.opensr5.ini.TsStringFunction;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.PcVariableArrayIniField;
import com.opensr5.ini.field.ScalarIniField;
import com.opensr5.ini.field.StringIniField;
import com.rusefi.config.Field;
import com.rusefi.config.StringFormatter;

import java.math.BigDecimal;
import java.math.RoundingMode;
import java.nio.charset.StandardCharsets;
import java.util.*;

/**
 * A value-level review of an already migrated tune. This class does not parse or migrate images.
 * Fields are addressed by INI wire page and storage offsets, never by generated struct layout.
 */
final class TuneLoadPlan {
    private final List<Change> changes;
    private final List<String> warnings;
    private final Map<Integer, ConfigurationImage> current;
    private final Map<Integer, ConfigurationImage> incoming;

    static TuneLoadPlan create(IniFileModel ini, Map<Integer, ConfigurationImage> current,
                               Map<Integer, ConfigurationImage> incoming, List<String> warnings) {
        return new TuneLoadPlan(ini, current, incoming, warnings);
    }

    private TuneLoadPlan(IniFileModel ini, Map<Integer, ConfigurationImage> current,
                         Map<Integer, ConfigurationImage> incoming, List<String> initialWarnings) {
        this.current = copyMap(current);
        this.incoming = copyMap(incoming);
        this.warnings = new ArrayList<>(initialWarnings);
        List<FieldChange> candidates = new ArrayList<>();
        Map<String, String> displayNames = new HashMap<>();
        ini.getFieldsInUiOrder().forEach((key, value) -> displayNames.put(key, value.getUiName()));
        Map<String, String> titles = new HashMap<>();
        ini.getTables().values().forEach(t -> titles.put(t.getMap3dId(), t.getTitle()));
        ini.getCurves().values().forEach(c -> titles.put(c.getCurveId(), c.getTitle()));
        Set<String> seen = new HashSet<>();
        List<IniField> allFields = new ArrayList<>(ini.getAllIniFields().values());
        allFields.addAll(ini.getSecondaryIniFields().values());
        for (IniField field : allFields) {
            if (field instanceof PcVariableArrayIniField || ini.getPcVariableNames().contains(field.getName())) {
                continue;
            }
            if (!seen.add(field.getPageIndex() + ":" + field.getName() + ":" + field.getOffset())) { continue; }
            ConfigurationImage before = current.get(field.getPageIndex());
            ConfigurationImage after = incoming.get(field.getPageIndex());
            if (before == null || after == null) {
                continue;
            }
            if (!readable(field, before, after)) {
                this.warnings.add("Cannot review " + field.getName() + " on page " + field.getPageIndex());
                continue;
            }
            try {
                FieldChange change = fieldChange(ini, field, this.current, this.incoming, displayNames, titles);
                if (change != null) {
                    candidates.add(change);
                }
            } catch (RuntimeException e) {
                this.warnings.add("Cannot review " + field.getName() + ": " + e.getMessage());
            }
        }
        this.changes = group(ini, candidates, displayNames, this.current, this.incoming);
    }

    List<Change> changes() { return changes; }
    List<String> warnings() { return Collections.unmodifiableList(warnings); }

    /** Returns deep-cloned current pages with only selected INI-covered values copied from incoming. */
    Map<Integer, ConfigurationImage> select(Set<Change> selected) {
        return select(selected, current);
    }

    /**
     * Applies selected incoming representations over a compatible destination snapshot. Values outside
     * the selected INI representation remain as they were in that destination.
     */
    Map<Integer, ConfigurationImage> select(Set<Change> selected,
                                            Map<Integer, ConfigurationImage> destination) {
        Map<Integer, ConfigurationImage> result = copyMap(destination);
        for (Change change : selected) {
            for (FieldChange detail : change.fields) {
                ConfigurationImage from = incoming.get(detail.field.getPageIndex());
                ConfigurationImage to = result.get(detail.field.getPageIndex());
                ConfigurationImage original = current.get(detail.field.getPageIndex());
                copyValue(detail.field, from, original, to);
            }
        }
        return result;
    }

    private static Map<Integer, ConfigurationImage> copyMap(Map<Integer, ConfigurationImage> images) {
        Map<Integer, ConfigurationImage> result = new LinkedHashMap<>();
        images.forEach((page, image) -> result.put(page, image.clone()));
        return result;
    }

    private static boolean readable(IniField field, ConfigurationImage a, ConfigurationImage b) {
        int end = field.getOffset() + field.getSize();
        return field.getOffset() >= 0 && field.getSize() >= 0 && end >= field.getOffset()
            && end <= a.getSize() && end <= b.getSize()
            && a.getByteOrder() == b.getByteOrder();
    }

    private static FieldChange fieldChange(IniFileModel ini, IniField field, Map<Integer, ConfigurationImage> previous,
                                           Map<Integer, ConfigurationImage> incoming,
                                           Map<String, String> displayNames, Map<String, String> titles) {
        if (field instanceof PcVariableArrayIniField) {
            return null;
        }
        ConfigurationImage a = previous.get(field.getPageIndex());
        ConfigurationImage b = incoming.get(field.getPageIndex());
        UnburnedChanges raw = UnburnedChanges.onPage(b, a, field.getPageIndex());
        List<Detail> details = new ArrayList<>();
        if (field instanceof ArrayIniField) {
            ArrayIniField array = (ArrayIniField) field;
            int size = array.getType().getStorageSize();
            for (int row = 0; row < array.getRows(); row++) {
                for (int col = 0; col < array.getCols(); col++) {
                    int offset = array.getOffset(row, col);
                    if (raw.isCellChanged(array, row, col)) {
                        String before = arrayValue(array, a, row, col);
                        String after = arrayValue(array, b, row, col);
                        details.add(new Detail(array.getName() + "[" + row + "," + col + "]",
                            before == null || before.equals(after) ? bytes(a, offset, size) : before,
                            after == null || before.equals(after) ? bytes(b, offset, size) : after));
                    }
                }
            }
        } else if (field instanceof EnumIniField) {
            EnumIniField e = (EnumIniField) field;
            if (raw.isChanged(e)) {
                String before = safeValue(field, a), after = safeValue(field, b);
                details.add(new Detail(displayNames.getOrDefault(field.getName(), field.getName()),
                    before == null ? bytes(a, field.getOffset(), field.getSize()) : before,
                    after == null ? bytes(b, field.getOffset(), field.getSize()) : after));
            }
        } else if (field instanceof StringIniField) {
            String before = ascii(a, field);
            String after = ascii(b, field);
            if (!before.equals(after)) {
                String name = field.getName();
                details.add(new Detail(name.equalsIgnoreCase("luaScript") ? "Lua script" : displayNames.getOrDefault(name, name),
                    before, after));
            }
        } else if (field instanceof ScalarIniField) {
            if (raw.isChanged(field)) {
                details.add(scalarDetail(ini, (ScalarIniField) field, previous, incoming,
                    titles.getOrDefault(field.getName(), displayNames.getOrDefault(field.getName(), field.getName()))));
            }
        } else if (raw.isChanged(field)) {
            String before = safeValue(field, a), after = safeValue(field, b);
            if (before != null && before.equals(after)) {
                before = bytes(a, field.getOffset(), field.getSize());
                after = bytes(b, field.getOffset(), field.getSize());
            }
            details.add(new Detail(titles.getOrDefault(field.getName(),
                displayNames.getOrDefault(field.getName(), field.getName())),
                before == null ? bytes(a, field.getOffset(), field.getSize()) : before,
                after == null ? bytes(b, field.getOffset(), field.getSize()) : after));
        }
        return details.isEmpty() ? null : new FieldChange(field, details);
    }

    private static Detail scalarDetail(IniFileModel ini, ScalarIniField field, Map<Integer, ConfigurationImage> previous,
                                       Map<Integer, ConfigurationImage> incoming, String label) {
        Double before = scalarDisplayValue(ini, field, previous);
        Double after = scalarDisplayValue(ini, field, incoming);
        // If either conversion is unavailable, compare both sides in base units without a dynamic label.
        boolean display = before != null && after != null;
        String beforeUnit = display ? displayText(field.getUnits(), ini, previous) : "";
        String afterUnit = display ? displayText(field.getUnits(), ini, incoming) : "";
        if (!display) {
            before = baseScalarValue(field, previous);
            after = baseScalarValue(field, incoming);
        }
        int digits = IniField.parseDigits(field.getDigits());
        String a = scalarText(before, digits, beforeUnit);
        String b = scalarText(after, digits, afterUnit);
        if (a.equals(b) && !before.equals(after) && Double.isFinite(before) && Double.isFinite(after)) {
            String[] precise = distinguishRoundedValues(before, after, digits);
            a = withUnit(precise[0], beforeUnit);
            b = withUnit(precise[1], afterUnit);
        }
        return new Detail(label, a, b);
    }

    private static Double scalarDisplayValue(IniFileModel ini, ScalarIniField field, Map<Integer, ConfigurationImage> pages) {
        String scaleExpression = field.getScaleExpression();
        String translateExpression = field.getTranslateExpression();
        if (scaleExpression == null && translateExpression == null) {
            return baseScalarValue(field, pages);
        }
        Double scale = displayAttribute(scaleExpression, field.getMultiplier(), ini, pages);
        Double translate = displayAttribute(translateExpression, field.getSerializationOffset(), ini, pages);
        return scale == null || translate == null ? null : (rawScalarValue(field, pages) + translate) * scale;
    }

    private static Double displayAttribute(String expression, double base, IniFileModel ini, Map<Integer, ConfigurationImage> pages) {
        if (expression == null) {
            return base;
        }
        Double resolved = ExpressionEvaluator.tryEvaluateWithContext(expression, expressionVariables(expression, ini, pages));
        return resolved != null && Double.isFinite(resolved) ? resolved : null;
    }

    private static double baseScalarValue(ScalarIniField field, Map<Integer, ConfigurationImage> pages) {
        return rawScalarValue(field, pages) * field.getMultiplier() + field.getSerializationOffset();
    }

    private static double rawScalarValue(ScalarIniField field, Map<Integer, ConfigurationImage> pages) {
        return field.getType().readRawValue(pages.get(field.getPageIndex()).getByteBuffer(field.getOffset(), field.getSize()));
    }

    private static String scalarText(double value, int digits, String unit) {
        return withUnit(Double.isFinite(value) ? StringFormatter.niceToString(value, digits) : Double.toString(value), unit);
    }

    private static String withUnit(String value, String unit) {
        unit = displayUnit(unit);
        return unit.isEmpty() ? value : value + " " + unit;
    }

    private static String displayUnit(String unit) {
        if ("C".equals(unit) || "F".equals(unit)) {
            return "°" + unit;
        }
        return unit;
    }

    private static String arrayValue(ArrayIniField field, ConfigurationImage image, int row, int col) {
        try {
            double value = new Field(field.getName(), field.getOffset(row, col), field.getType())
                .getValue(image, field.getMultiplier());
            return format(field, value);
        } catch (RuntimeException e) {
            return null;
        }
    }

    private static String safeValue(IniField field, ConfigurationImage image) {
        try {
            return ConfigurationImageGetterSetter.getStringValue(field, image).trim();
        } catch (RuntimeException e) {
            return null;
        }
    }

    static String ascii(ConfigurationImage image, IniField field) {
        byte[] data = image.getContent();
        int end = field.getOffset();
        int limit = end + field.getSize();
        while (end < limit && data[end] != 0) { end++; }
        return new String(data, field.getOffset(), end - field.getOffset(), StandardCharsets.US_ASCII);
    }

    private static String bytes(ConfigurationImage image, int offset, int size) {
        StringBuilder result = new StringBuilder();
        for (int i = offset; i < offset + size; i++) {
            if (i > offset) { result.append(' '); }
            result.append(String.format("%02X", image.getContent()[i] & 0xff));
        }
        return result.toString();
    }

    private static void copyValue(IniField field, ConfigurationImage from, ConfigurationImage original,
                                  ConfigurationImage to) {
        if (field instanceof ArrayIniField) {
            ArrayIniField array = (ArrayIniField) field;
            UnburnedChanges raw = UnburnedChanges.onPage(from, original, field.getPageIndex());
            int size = array.getType().getStorageSize();
            for (int row = 0; row < array.getRows(); row++) {
                for (int col = 0; col < array.getCols(); col++) {
                    int offset = array.getOffset(row, col);
                    if (raw.isCellChanged(array, row, col)) {
                        System.arraycopy(from.getContent(), offset, to.getContent(), offset, size);
                    }
                }
            }
            return;
        }
        UnburnedChanges.onPage(to, from, field.getPageIndex()).copyField(field);
    }

    private static List<Change> group(IniFileModel ini, List<FieldChange> fields, Map<String, String> displayNames,
                                      Map<Integer, ConfigurationImage> previous, Map<Integer, ConfigurationImage> incoming) {
        List<List<FieldChange>> groups = new ArrayList<>();
        Map<String, String> linkedLabels = new HashMap<>();
        for (TableModel table : ini.getTables().values()) {
            Set<String> names = new HashSet<>(Arrays.asList(table.getZBinsConstant(),
                table.getXBinsConstant(), table.getYBinsConstant(), table.getMap3dId(), table.getTableId()));
            names.remove(null);
            addLinkedGroup(fields, groups, names);
            names.forEach(n -> linkedLabels.put(n, table.getTitle() == null ? table.getTableId() : table.getTitle()));
        }
        for (CurveModel curve : ini.getCurves().values()) {
            Set<String> names = new HashSet<>(curve.getYBinsSeries());
            names.add(curve.getCurveId());
            names.add(curve.getxBins());
            names.remove(null);
            addLinkedGroup(fields, groups, names);
            names.forEach(n -> linkedLabels.put(n, curve.getTitle() == null ? curve.getCurveId() : curve.getTitle()));
        }
        groupSettingPanels(ini, fields, groups, linkedLabels);
        for (FieldChange field : fields) {
            if (groups.stream().noneMatch(group -> group.contains(field))) {
                groups.add(new ArrayList<>(Collections.singletonList(field)));
            }
        }
        boolean merged;
        do {
            merged = false;
            outer: for (int i = 0; i < groups.size(); i++) {
                for (int j = i + 1; j < groups.size(); j++) {
                    if (overlapGroups(groups.get(i), groups.get(j))) {
                        groups.get(i).addAll(groups.remove(j));
                        merged = true;
                        break outer;
                    }
                }
            }
        } while (merged);
        List<Change> result = new ArrayList<>();
        for (List<FieldChange> group : groups) {
            String name = group.stream().map(f -> linkedLabels.get(f.field.getName()))
                .filter(Objects::nonNull).findFirst().orElse(null);
            if (name == null) {
                String key = group.get(0).field.getName();
                name = title(displayNames.get(key), key);
                if (group.size() > 1) {
                    name += " (shared storage)";
                }
            }
            List<TableComparison> previews = comparisons(ini, group, displayNames, previous, incoming);
            if ("Target AFR Table".equals(name) || "Target Lambda Table".equals(name)) {
                name = previews.stream().map(t -> t.title).filter(t -> t.equals("Target mixture")
                    || t.equals("Target AFR Table") || t.equals("Target Lambda Table")).findFirst().orElse(name);
            }
            result.add(new Change(name, group, previews));
        }
        return Collections.unmodifiableList(result);
    }

    private static void groupSettingPanels(IniFileModel ini, List<FieldChange> fields,
                                           List<List<FieldChange>> groups, Map<String, String> linkedLabels) {
        for (DialogModel parent : ini.getDialogs().values()) {
            for (PanelModel panel : parent.getPanels()) {
                DialogModel child = panel.resolveDialog(ini);
                if (child == null || child == parent || child.getUiName() == null || child.getUiName().isBlank()) {
                    continue;
                }
                List<FieldChange> group = new ArrayList<>();
                for (DialogModel.Field control : child.getFields()) {
                    if (control.getKey() == null) {
                        continue;
                    }
                    for (FieldChange field : fields) {
                        if (control.getKey().equalsIgnoreCase(field.field.getName())
                                && !(field.field instanceof ArrayIniField) && !(field.field instanceof StringIniField)
                                && !linkedLabels.containsKey(field.field.getName()) && !group.contains(field)) {
                            group.add(field);
                        }
                    }
                }
                if (!group.isEmpty()) {
                    groups.add(group);
                    String parentTitle = title(parent.getUiName(), "");
                    String label = parentTitle.isEmpty() ? child.getUiName() : parentTitle + " — " + child.getUiName();
                    group.forEach(field -> linkedLabels.put(field.field.getName(), label));
                }
            }
        }
    }

    private static List<TableComparison> comparisons(IniFileModel ini, List<FieldChange> group,
                                                     Map<String, String> displayNames, Map<Integer, ConfigurationImage> previous,
                                                     Map<Integer, ConfigurationImage> incoming) {
        Set<String> changed = new TreeSet<>(String.CASE_INSENSITIVE_ORDER);
        group.forEach(f -> changed.add(f.field.getName()));
        List<TableComparison> result = new ArrayList<>();
        Set<String> shown = new HashSet<>();
        boolean targetMixtureShown = false;
        for (TableModel table : ini.getTables().values()) {
            if (!touches(changed, table.getZBinsConstant(), table.getXBinsConstant(), table.getYBinsConstant())) {
                continue;
            }
            ArrayIniField values = array(ini, table.getZBinsConstant());
            if (values == null || !readableOnBoth(values, previous, incoming)) {
                continue;
            }
            if (!tableVisible(table, ini, previous) && !tableVisible(table, ini, incoming)) {
                continue;
            }
            String tableTitle = title(table.getTitle(), "Calibration table");
            if ("mapEstimateTableTbl".equalsIgnoreCase(table.getTableId())) {
                // The per-side headings below report kPa or psi, not the static "(kPa)" in the INI title.
                tableTitle = tableTitle.replace(" (kPa)", "");
            }
            if ("afrTableTbl".equalsIgnoreCase(table.getTableId()) || "lambdaTableTbl".equalsIgnoreCase(table.getTableId())) {
                if (!displayText(values.getUnits(), ini, previous).equals(displayText(values.getUnits(), ini, incoming))) {
                    if (targetMixtureShown) {
                        continue;
                    }
                    targetMixtureShown = true;
                    tableTitle = "Target mixture";
                }
            }
            ArrayIniField x = array(ini, table.getXBinsConstant());
            ArrayIniField y = array(ini, table.getYBinsConstant());
            shown.add(values.getName());
            if (x != null) {
                shown.add(x.getName());
            }
            if (y != null) {
                shown.add(y.getName());
            }
            result.add(gridComparison(tableTitle, ini, values, x, y,
                table.getXLabel(), table.getYLabel(), previous, incoming));
        }
        for (CurveModel curve : ini.getCurves().values()) {
            ArrayIniField x = array(ini, curve.getxBins());
            if (!readableOnBoth(x, previous, incoming)) {
                continue;
            }
            List<String> series = curve.getYBinsSeries();
            boolean omittedSeries = series.stream().map(name -> array(ini, name)).anyMatch(y -> !readableOnBoth(y, previous, incoming));
            for (int i = 0; i < series.size(); i++) {
                String name = series.get(i);
                ArrayIniField y = array(ini, name);
                if (!touches(changed, curve.getxBins(), name) || !readableOnBoth(y, previous, incoming)
                        || x.getRows() * x.getCols() != y.getRows() * y.getCols()) {
                    continue;
                }
                shown.add(x.getName());
                shown.add(y.getName());
                boolean multipleStoredSeries = series.size() > 1 && !omittedSeries;
                result.add(curveComparison(ini, curve, x, y, previous, incoming, curve.getYSeriesLabel(i),
                    multipleStoredSeries, omittedSeries));
            }
        }
        for (FieldChange field : group) {
            if (field.field instanceof ArrayIniField && shown.add(field.field.getName())) {
                ArrayIniField values = (ArrayIniField) field.field;
                result.add(gridComparison(title(displayNames.get(values.getName()), values.getName()), ini, values,
                    null, null, null, null, previous, incoming));
            }
        }
        return List.copyOf(result);
    }

    private static boolean tableVisible(TableModel table, IniFileModel ini, Map<Integer, ConfigurationImage> pages) {
        String expression = table.getVisibilityExpression();
        return expression == null || !Boolean.FALSE.equals(ExpressionEvaluator.evaluateBooleanExpression(
            expression, expressionVariables(expression, ini, pages)));
    }

    private static boolean touches(Set<String> changed, String... names) {
        return Arrays.stream(names).anyMatch(name -> name != null && changed.contains(name));
    }

    private static ArrayIniField array(IniFileModel ini, String name) {
        if (name == null) {
            return null;
        }
        return ini.findIniField(name).filter(ArrayIniField.class::isInstance).map(ArrayIniField.class::cast).orElse(null);
    }

    private static boolean available(ArrayIniField field, Map<Integer, ConfigurationImage> pages) {
        if (field == null) {
            return false;
        }
        if (field instanceof PcVariableArrayIniField) {
            return ((PcVariableArrayIniField) field).hasDefaultValues();
        }
        ConfigurationImage image = pages.get(field.getPageIndex());
        return image != null && readable(field, image, image);
    }

    private static boolean readableOnBoth(ArrayIniField field, Map<Integer, ConfigurationImage> previous,
                                          Map<Integer, ConfigurationImage> incoming) {
        return available(field, previous) && available(field, incoming);
    }

    private static TableComparison gridComparison(String title, IniFileModel ini, ArrayIniField values,
                                                    ArrayIniField x, ArrayIniField y, String xLabel, String yLabel,
                                                    Map<Integer, ConfigurationImage> previous, Map<Integer, ConfigurationImage> incoming) {
        ArrayDisplay beforeX = new ArrayDisplay(x, ini, previous), afterX = new ArrayDisplay(x, ini, incoming);
        ArrayDisplay beforeY = new ArrayDisplay(y, ini, previous), afterY = new ArrayDisplay(y, ini, incoming);
        ArrayDisplay beforeZ = new ArrayDisplay(values, ini, previous), afterZ = new ArrayDisplay(values, ini, incoming);
        String[][] a = new String[values.getRows() + 1][values.getCols() + 1];
        String[][] b = new String[values.getRows() + 1][values.getCols() + 1];
        a[0][0] = axisTitle(yLabel, beforeY, "Row", ini, previous) + " / " + axisTitle(xLabel, beforeX, "Column", ini, previous);
        b[0][0] = axisTitle(yLabel, afterY, "Row", ini, incoming) + " / " + axisTitle(xLabel, afterX, "Column", ini, incoming);
        for (int col = 0; col < values.getCols(); col++) {
            setComparedValues(a, b, 0, col + 1, beforeX.bin(col), afterX.bin(col), beforeX.digits, afterX.digits);
        }
        for (int row = 0; row < values.getRows(); row++) {
            setComparedValues(a, b, row + 1, 0, beforeY.bin(row), afterY.bin(row), beforeY.digits, afterY.digits);
            for (int col = 0; col < values.getCols(); col++) {
                setComparedValues(a, b, row + 1, col + 1, beforeZ.values[row][col], afterZ.values[row][col],
                    beforeZ.digits, afterZ.digits);
            }
        }
        return new TableComparison(title, a, b, beforeZ.units, afterZ.units, null, null, false);
    }

    private static void setComparedValues(String[][] previous, String[][] incoming, int row, int col, Double before, Double after,
                                          int previousDigits, int incomingDigits) {
        String a = format(before, previousDigits);
        String b = format(after, incomingDigits);
        if (before != null && after != null && !before.equals(after) && Double.isFinite(before) && Double.isFinite(after)
                && a.equals(b)) {
            String[] precise = distinguishRoundedValues(before, after, Math.max(previousDigits, incomingDigits));
            a = precise[0];
            b = precise[1];
        }
        previous[row][col] = a;
        incoming[row][col] = b;
    }

    private static String[] distinguishRoundedValues(double before, double after, int digits) {
        for (int precision = Math.max(0, Math.min(digits, 9)) + 1; precision <= 9; precision++) {
            String a = BigDecimal.valueOf(before).setScale(precision, RoundingMode.HALF_UP).toPlainString();
            String b = BigDecimal.valueOf(after).setScale(precision, RoundingMode.HALF_UP).toPlainString();
            if (!a.equals(b)) {
                return new String[]{a, b};
            }
        }
        return new String[]{Double.toString(before), Double.toString(after)};
    }

    private static double[][] withoutMafPadding(double[][] points) {
        int end = points.length;
        // maf.cpp fillTheRest uses this pair to pad the binary-search table, not as sensor calibration.
        while (end > 0 && points[end - 1][0] == 2010 + end - 1 && points[end - 1][1] == 3000) {
            end--;
        }
        return end == points.length ? points : Arrays.copyOf(points, end);
    }

    private static TableComparison curveComparison(IniFileModel ini, CurveModel curve, ArrayIniField x, ArrayIniField y,
                                                    Map<Integer, ConfigurationImage> previous,
                                                    Map<Integer, ConfigurationImage> incoming, String seriesLabel,
                                                    boolean multipleStoredSeries, boolean omittedSeries) {
        ArrayDisplay beforeX = new ArrayDisplay(x, ini, previous), afterX = new ArrayDisplay(x, ini, incoming);
        ArrayDisplay beforeY = new ArrayDisplay(y, ini, previous), afterY = new ArrayDisplay(y, ini, incoming);
        double[][] oldPoints = curvePoints(x, beforeX, beforeY);
        double[][] newPoints = curvePoints(x, afterX, afterY);
        String[][] a = new String[oldPoints.length + 1][2];
        String[][] b = new String[newPoints.length + 1][2];
        String yLabel = multipleStoredSeries ? title(seriesLabel, y.getName()) : seriesLabel;
        a[0] = new String[]{axisTitle(curve.getXLabel(), beforeX, "Input", ini, previous),
            axisTitle(yLabel, beforeY, "Output", ini, previous)};
        b[0] = new String[]{axisTitle(curve.getXLabel(), afterX, "Input", ini, incoming),
            axisTitle(yLabel, afterY, "Output", ini, incoming)};
        for (int i = 0; i < oldPoints.length; i++) {
            setComparedValues(a, b, i + 1, 0, oldPoints[i][0], newPoints[i][0], beforeX.digits, afterX.digits);
            setComparedValues(a, b, i + 1, 1, oldPoints[i][1], newPoints[i][1], beforeY.digits, afterY.digits);
        }
        if ("mafDecodingBins".equalsIgnoreCase(x.getName()) && "mafDecoding".equalsIgnoreCase(y.getName())) {
            oldPoints = withoutMafPadding(oldPoints);
            newPoints = withoutMafPadding(newPoints);
        }
        String curveTitle = title(curve.getTitle(), "Calibration curve");
        if (multipleStoredSeries) {
            curveTitle += " — " + title(displayText(yLabel, ini, incoming), y.getName());
        }
        return new TableComparison(curveTitle, a, b, "", "", oldPoints, newPoints, omittedSeries);
    }

    private static double[][] curvePoints(ArrayIniField x, ArrayDisplay xDisplay, ArrayDisplay yDisplay) {
        double[][] points = new double[x.getRows() * x.getCols()][2];
        for (int i = 0; i < points.length; i++) {
            points[i][0] = xDisplay.bin(i);
            points[i][1] = yDisplay.bin(i);
        }
        return points;
    }

    private static final class ArrayDisplay {
        final ArrayIniField field;
        final Double[][] values;
        final String units;
        final int digits;

        ArrayDisplay(ArrayIniField field, IniFileModel ini, Map<Integer, ConfigurationImage> pages) {
            this.field = field;
            if (field == null) {
                values = null;
                units = "";
                digits = 0;
                return;
            }
            String digitSpec = field.getDigits();
            Double resolvedDigits = digitSpec != null && digitSpec.trim().startsWith("{")
                ? displayAttribute(digitSpec, 3, ini, pages) : null;
            digits = resolvedDigits != null ? Math.max(0, Math.min(9, resolvedDigits.intValue())) : IniField.parseDigits(digitSpec);
            Double scale = displayAttribute(field.getScaleExpression(), field.getMultiplier(), ini, pages);
            Double translate = displayAttribute(field.getTranslateExpression(), field.getTranslate(), ini, pages);
            if (field instanceof PcVariableArrayIniField) {
                values = available(field, pages) ? ((PcVariableArrayIniField) field).getValues() : null;
                units = displayUnit(displayText(field.getUnits(), ini, pages));
            } else {
                units = scale != null && translate != null ? displayUnit(displayText(field.getUnits(), ini, pages)) : "";
                if (!available(field, pages)) {
                    values = null;
                    return;
                }
                double multiplier = scale != null ? scale : field.getMultiplier();
                double translation = translate != null ? translate : field.getTranslate();
                ConfigurationImage image = pages.get(field.getPageIndex());
                values = new Double[field.getRows()][field.getCols()];
                for (int row = 0; row < values.length; row++) {
                    for (int col = 0; col < values[row].length; col++) {
                        double raw = field.getType().readRawValue(image.getByteBuffer(field.getOffset(row, col),
                            field.getType().getStorageSize()));
                        values[row][col] = (raw + translation) * multiplier;
                    }
                }
            }
        }

        Double bin(int index) {
            if (field == null || index >= field.getRows() * field.getCols()) {
                return (double) index + 1;
            }
            return values == null ? null : values[index / field.getCols()][index % field.getCols()];
        }
    }

    private static String format(ArrayIniField field, double value) {
        return format(value, IniField.parseDigits(field.getDigits()));
    }

    private static String format(Double value, int digits) {
        return value != null && Double.isFinite(value) ? StringFormatter.niceToString(value, digits) : "—";
    }

    private static String axisTitle(String label, ArrayDisplay display, String fallback,
                                    IniFileModel ini, Map<Integer, ConfigurationImage> pages) {
        String name = displayText(label, ini, pages);
        String unit = display.units;
        if (!name.isEmpty() && !unit.isEmpty() && !name.equals(unit) && !name.endsWith(" (" + unit + ")")
                && (TsStringFunction.containsStringFunction(label)
                    || display.field != null && TsStringFunction.containsStringFunction(display.field.getUnits()))) {
            return name + " (" + unit + ")";
        }
        return title(name, title(unit, fallback));
    }

    private static String displayText(String raw, IniFileModel ini, Map<Integer, ConfigurationImage> pages) {
        String resolved = TsStringFunction.resolveDisplayLabelWithVariables(raw, ini, pages.get(0),
            expression -> expressionVariables(expression, ini, pages)).trim();
        return resolved.startsWith("{") ? "" : resolved;
    }

    private static Map<String, Double> expressionVariables(String expression, IniFileModel ini, Map<Integer, ConfigurationImage> pages) {
        return ExpressionEvaluator.resolveVariables(expression, ini, field -> {
            ConfigurationImage image = pages.get(field.getPageIndex());
            return image != null && readable(field, image, image) ? image.readNumericValue(field) : null;
        });
    }

    private static String title(String label, String fallback) {
        return label == null || label.trim().isEmpty() ? fallback : label;
    }

    private static void addLinkedGroup(List<FieldChange> fields, List<List<FieldChange>> groups, Set<String> names) {
        List<FieldChange> linked = new ArrayList<>();
        for (FieldChange field : fields) {
            if (names.contains(field.field.getName())) { linked.add(field); }
        }
        if (!linked.isEmpty()) { groups.add(linked); }
    }

    private static boolean overlapGroups(List<FieldChange> a, List<FieldChange> b) {
        for (FieldChange first : a) {
            for (FieldChange second : b) {
                if (overlaps(first.field, second.field)) { return true; }
            }
        }
        return false;
    }

    private static boolean overlaps(IniField a, IniField b) {
        if (a.getPageIndex() != b.getPageIndex()) { return false; }
        if (a instanceof EnumIniField && b instanceof EnumIniField && a.getOffset() == b.getOffset()) {
            EnumIniField ea = (EnumIniField) a, eb = (EnumIniField) b;
            long ma = ((1L << (ea.getBitSize0() + 1)) - 1) << ea.getBitPosition();
            long mb = ((1L << (eb.getBitSize0() + 1)) - 1) << eb.getBitPosition();
            return (ma & mb) != 0;
        }
        return a.getOffset() < b.getOffset() + b.getSize() && b.getOffset() < a.getOffset() + a.getSize();
    }

    private static final class FieldChange {
        final IniField field;
        final List<Detail> details;
        FieldChange(IniField field, List<Detail> details) { this.field = field; this.details = details; }
    }

    static final class Change {
        private final String label;
        private final List<FieldChange> fields;
        private final Detail lua;
        final List<TableComparison> tables;
        Change(String label, List<FieldChange> fields, List<TableComparison> tables) {
            this.label = label;
            this.fields = fields;
            this.lua = fields.stream().filter(f -> isLuaField(f.field)).flatMap(f -> f.details.stream()).findFirst().orElse(null);
            this.tables = tables;
        }
        String label() { return label; }
        String current() { return summary(true); }
        String incoming() { return summary(false); }
        private String summary(boolean before) {
            if (lua != null) { return before ? lua.current() : lua.incoming(); }
            int count = details().size();
            if (count > 1) { return count + " values changed"; }
            Detail detail = details().get(0);
            return before ? detail.current() : detail.incoming();
        }
        List<Detail> details() {
            List<Detail> all = new ArrayList<>();
            fields.forEach(f -> all.addAll(f.details));
            return Collections.unmodifiableList(all);
        }
        List<Detail> settings() {
            List<Detail> settings = new ArrayList<>();
            fields.stream().filter(f -> !(f.field instanceof ArrayIniField) && !isLuaField(f.field))
                .forEach(f -> settings.addAll(f.details));
            return settings;
        }

        private static boolean isLuaField(IniField field) {
            return field instanceof StringIniField && field.getName().equalsIgnoreCase("luaScript");
        }

        boolean isLua() { return lua != null; }
    }

    static final class Detail {
        private final String label, current, incoming;

        Detail(String label, String current, String incoming) {
            this.label = label; this.current = current; this.incoming = incoming;
        }
        String label() { return label; }
        String current() { return current; }
        String incoming() { return incoming; }
    }

    static final class TableComparison {
        final String title;
        final String[][] previous, incoming;
        final String previousUnits, incomingUnits;
        final double[][] previousPoints, incomingPoints;
        final boolean omittedSeries;

        TableComparison(String title, String[][] previous, String[][] incoming, String previousUnits, String incomingUnits,
                        double[][] previousPoints, double[][] incomingPoints, boolean omittedSeries) {
            this.title = title;
            this.previous = previous;
            this.incoming = incoming;
            this.previousUnits = previousUnits;
            this.incomingUnits = incomingUnits;
            this.previousPoints = previousPoints;
            this.incomingPoints = incomingPoints;
            this.omittedSeries = omittedSeries;
        }

        boolean hasSharedCurveAxes() {
            return Arrays.equals(previous[0], incoming[0]);
        }

        boolean isChanged(int row, int column) {
            return !previous[row][column].equals(incoming[row][column]);
        }

        boolean hasVisibleDifference() {
            return !previousUnits.equals(incomingUnits) || !Arrays.deepEquals(previous, incoming);
        }
    }
}
