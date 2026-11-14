package com.rusefi.ui;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.DialogModel;
import com.opensr5.ini.ExpressionEvaluator;
import com.opensr5.ini.GroupMenuModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.MenuItem;
import com.opensr5.ini.MenuModel;
import com.opensr5.ini.PanelModel;
import com.opensr5.ini.SubMenuModel;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

/**
 * Assigning a pin from the Pinout tab: the reverse of choosing a pin in a Tuning pin selector. The
 * settings that can use a pin are the pin selectors ({@link PinoutLookup#isPinSetting}, analog inputs
 * included) whose choices include the pin's expanded {@code ts_name}, so the INI already says which outputs, analog inputs or
 * switch inputs suit it. A pin has one function: assigning it sets every other setting on that pin to
 * NONE.
 */
final class PinAssignments {
    private static final String NONE = "NONE";
    /** {@link #swapProblem} for two pins nothing uses: there is nothing to swap. */
    static final String BOTH_FREE = "both free";

    private PinAssignments() {
    }

    /** A pin setting that can use the pin, as Tuning shows it. */
    static final class Candidate {
        final String fieldKey;
        /** The setting's label in Tuning. */
        final String label;
        /** The title of the Tuning dialog holding it. */
        final String section;
        final String dialogKey;
        /** The pin chosen now, as its {@code ts_name}; null when none. */
        final String current;
        /** Whether Tuning hides or disables the setting now, e.g. until its feature is turned on. */
        final boolean hidden;
        /** Whether a change takes effect only after the ECU restarts ({@code requiresPowerCycle} in the INI). */
        final boolean needsRestart;
        /**
         * While {@link #hidden}: the labels of the settings its false conditions test (e.g. "Enable injection",
         * "Cylinders count"); empty otherwise, or when the conditions name no setting.
         */
        final List<String> hiddenBy;
        /**
         * While {@link #hidden}: where to turn it on, as {fieldKey, dialogKey} of the first such setting Tuning
         * shows; null to open this setting itself.
         */
        final String[] turnOnIn;

        Candidate(String fieldKey, String label, String section, String dialogKey, String current, boolean hidden) {
            this(fieldKey, label, section, dialogKey, current, hidden, false);
        }

        Candidate(String fieldKey, String label, String section, String dialogKey, String current, boolean hidden,
                  boolean needsRestart) {
            this(fieldKey, label, section, dialogKey, current, hidden, needsRestart, Collections.emptyList(), null);
        }

        Candidate(String fieldKey, String label, String section, String dialogKey, String current, boolean hidden,
                  boolean needsRestart, List<String> hiddenBy, String[] turnOnIn) {
            this.fieldKey = fieldKey;
            this.label = label;
            this.section = section;
            this.dialogKey = dialogKey;
            this.current = current;
            this.hidden = hidden;
            this.needsRestart = needsRestart;
            this.hiddenBy = hiddenBy;
            this.turnOnIn = turnOnIn;
        }

        /** Where "Assign and open in Tuning" goes: {fieldKey, dialogKey}. */
        String[] openTarget() {
            return turnOnIn != null ? turnOnIn : new String[]{fieldKey, dialogKey};
        }

        boolean uses(String tsName) {
            return tsName.equals(current);
        }

        @Override
        public String toString() {
            return label;
        }
    }

    /**
     * The settings shown in a Tuning dialog that can choose {@code tsName}, in Tuning order. Settings that no
     * dialog shows are left out: they cannot be opened in Tuning afterwards.
     */
    static List<Candidate> candidates(IniFileModel ini, ConfigurationImage image, String tsName) {
        if (ini == null || image == null || tsName == null || tsName.isEmpty()) {
            return Collections.emptyList();
        }
        return settings(ini, image, field -> ordinalOf(field, tsName) >= 0);
    }

    /**
     * Every pin setting shown in a Tuning dialog, in Tuning order: the rows of the Pinout tab's
     * Functions view.
     */
    static List<Candidate> settings(IniFileModel ini, ConfigurationImage image) {
        if (ini == null || image == null) {
            return Collections.emptyList();
        }
        return settings(ini, image, field -> true);
    }

    /** Whether the setting {@code fieldKey} has {@code tsName} among its pin choices. */
    static boolean accepts(IniFileModel ini, String fieldKey, String tsName) {
        IniField field = ini.getAllIniFields().get(fieldKey);
        return field instanceof EnumIniField && ordinalOf((EnumIniField) field, tsName) >= 0;
    }

    private static List<Candidate> settings(IniFileModel ini, ConfigurationImage image,
                                            java.util.function.Predicate<EnumIniField> include) {
        Map<String, DialogModel> dialogOf = dialogsByField(ini);
        Conditions conditions = new Conditions(ini, image, dialogOf);
        Map<String, IniField> fields = ini.getAllIniFields();
        List<Candidate> result = new ArrayList<>();
        for (Map.Entry<String, DialogModel.Field> entry : ini.getFieldsInUiOrder().entrySet()) {
            String key = entry.getKey();
            IniField field = fields.get(key);
            DialogModel dialog = dialogOf.get(key);
            if (!(field instanceof EnumIniField) || !PinoutLookup.isPinSetting(key, field) || dialog == null
                || !include.test((EnumIniField) field)) {
                continue;
            }
            DialogModel.Field meta = entry.getValue();
            String label = meta.getUiName() == null || meta.getUiName().isEmpty() ? key : meta.getUiName();
            String section = dialog.getUiName() == null || dialog.getUiName().isEmpty() ? dialog.getKey()
                : dialog.getUiName();
            List<String> off = conditions.offFor(meta, dialog);
            result.add(new Candidate(key, label, section, dialog.getKey(), currentPin(field, image),
                !off.isEmpty(), ini.getRequiresPowerCycle().contains(key),
                conditions.labelsOf(off, key), conditions.turnOnIn(off, key)));
        }
        return result;
    }

    /**
     * What hides or disables a setting in Tuning: its own enable/visible conditions, those of every panel
     * embedding its dialog, and those of the menu entry opening that page. The settings a false condition tests
     * explain why and say where to turn the setting on.
     */
    private static final class Conditions {
        private final IniFileModel ini;
        private final ConfigurationImage image;
        private final Map<String, DialogModel> dialogOf;
        /** Dialog key to the panel embedding it, and that panel's dialog key. */
        private final Map<String, Object[]> embeddedIn = new HashMap<>();
        /** Dialog key to the menu entry opening it. */
        private final Map<String, SubMenuModel> menuEntries = new HashMap<>();

        Conditions(IniFileModel ini, ConfigurationImage image, Map<String, DialogModel> dialogOf) {
            this.ini = ini;
            this.image = image;
            this.dialogOf = dialogOf;
            for (DialogModel dialog : ini.getDialogs().values()) {
                for (PanelModel panel : dialog.getPanels()) {
                    embeddedIn.putIfAbsent(panel.getPanelName(), new Object[]{panel, dialog.getKey()});
                }
            }
            for (MenuModel menu : ini.getMenus()) {
                collectMenuEntries(menu.getItems());
            }
        }

        private void collectMenuEntries(List<MenuItem> items) {
            for (MenuItem item : items) {
                if (item instanceof SubMenuModel) {
                    menuEntries.putIfAbsent(((SubMenuModel) item).getKey(), (SubMenuModel) item);
                } else if (item instanceof GroupMenuModel) {
                    collectMenuEntries(((GroupMenuModel) item).getItems());
                }
            }
        }

        /** The false conditions on the way from the menu to the field. */
        List<String> offFor(DialogModel.Field field, DialogModel dialog) {
            List<String> off = new ArrayList<>();
            addIfOff(off, field.getEnableExpression());
            addIfOff(off, field.getVisibleExpression());
            java.util.Set<String> seen = new java.util.HashSet<>();
            String key = dialog.getKey();
            while (key != null && seen.add(key)) {
                SubMenuModel entry = menuEntries.get(key);
                if (entry != null) {
                    // the page Tuning opens: its menu entry is the last condition
                    addIfOff(off, entry.getEnableExpression());
                    addIfOff(off, entry.getVisibleExpression());
                    break;
                }
                Object[] parent = embeddedIn.get(key);
                if (parent == null) {
                    break;
                }
                PanelModel panel = (PanelModel) parent[0];
                addIfOff(off, panel.getEnableExpression());
                addIfOff(off, panel.getVisibleExpression());
                key = (String) parent[1];
            }
            return off;
        }

        private void addIfOff(List<String> off, String expression) {
            if (isOff(expression, ini, image)) {
                off.add(expression);
            }
        }

        /** The settings the conditions test, in order, without the setting itself. */
        private List<String> settingKeys(List<String> off, String self) {
            java.util.Set<String> keys = new java.util.LinkedHashSet<>();
            for (String expression : off) {
                for (String variable : ExpressionEvaluator.extractVariables(expression)) {
                    if (!variable.equalsIgnoreCase(self) && ini.findIniField(variable).isPresent()) {
                        keys.add(variable);
                    }
                }
            }
            return new ArrayList<>(keys);
        }

        List<String> labelsOf(List<String> off, String self) {
            List<String> labels = new ArrayList<>();
            for (String key : settingKeys(off, self)) {
                labels.add(label(ini, key));
            }
            return labels;
        }

        /** The first tested setting a dialog shows, as {fieldKey, dialogKey}; null when none. */
        String[] turnOnIn(List<String> off, String self) {
            for (String key : settingKeys(off, self)) {
                DialogModel dialog = dialogOf.get(key);
                if (dialog != null) {
                    return new String[]{key, dialog.getKey()};
                }
            }
            return null;
        }
    }

    /** The candidates using {@code tsName} now. */
    static List<Candidate> users(List<Candidate> candidates, String tsName) {
        List<Candidate> users = new ArrayList<>();
        for (Candidate candidate : candidates) {
            if (candidate.uses(tsName)) {
                users.add(candidate);
            }
        }
        return users;
    }

    /**
     * Sets {@code fieldKey} to {@code tsName} and every other pin setting on that pin to NONE.
     *
     * @throws IllegalArgumentException when the setting cannot choose that pin
     */
    static void assign(IniFileModel ini, ConfigurationImage image, String fieldKey, String tsName) {
        IniField field = ini.getAllIniFields().get(fieldKey);
        int ordinal = field instanceof EnumIniField ? ordinalOf((EnumIniField) field, tsName) : -1;
        if (ordinal < 0) {
            throw new IllegalArgumentException(fieldKey + " cannot use " + tsName);
        }
        clear(ini, image, tsName, fieldKey);
        image.setBitValue((EnumIniField) field, ordinal);
    }

    /**
     * The Functions view's pin column: {@code tsName} as in {@link #assign}, or null to set just this setting
     * to NONE.
     *
     * @throws IllegalArgumentException when the setting cannot choose that pin, or has no NONE choice
     */
    static void setPin(IniFileModel ini, ConfigurationImage image, String fieldKey, String tsName) {
        if (tsName != null) {
            assign(ini, image, fieldKey, tsName);
            return;
        }
        IniField field = ini.getAllIniFields().get(fieldKey);
        int none = field instanceof EnumIniField ? ordinalOf((EnumIniField) field, NONE) : -1;
        if (none < 0) {
            throw new IllegalArgumentException(fieldKey + " has no " + NONE + " choice");
        }
        image.setBitValue((EnumIniField) field, none);
    }

    /** Sets every pin setting using {@code tsName} to NONE. */
    static void unassign(IniFileModel ini, ConfigurationImage image, String tsName) {
        clear(ini, image, tsName, null);
    }

    /**
     * Why pins {@code a} and {@code b} cannot swap settings, or null when they can: every setting on one must
     * be able to choose the other, and at least one of them must be used.
     */
    static String swapProblem(IniFileModel ini, ConfigurationImage image, String a, String b) {
        if (a.equals(b)) {
            return "same pin";
        }
        Map<String, EnumIniField> onA = settingsOn(ini, image, a);
        Map<String, EnumIniField> onB = settingsOn(ini, image, b);
        if (onA.isEmpty() && onB.isEmpty()) {
            return BOTH_FREE;
        }
        String problem = firstUnable(ini, onA, b);
        return problem != null ? problem : firstUnable(ini, onB, a);
    }

    /**
     * The labels of the settings a swap of {@code a} and {@code b} changes that take effect only after the ECU
     * restarts ({@link IniFileModel#getRequiresPowerCycle}); empty when the swap applies on Burn.
     */
    static List<String> swapRestartLabels(IniFileModel ini, ConfigurationImage image, String a, String b) {
        List<String> labels = new ArrayList<>();
        Map<String, EnumIniField> changed = new LinkedHashMap<>(settingsOn(ini, image, a));
        changed.putAll(settingsOn(ini, image, b));
        for (String key : changed.keySet()) {
            if (ini.getRequiresPowerCycle().contains(key)) {
                labels.add(label(ini, key));
            }
        }
        return labels;
    }

    /** The labels of the candidates that take effect only after the ECU restarts. */
    static List<String> restartLabels(List<Candidate> candidates) {
        List<String> labels = new ArrayList<>();
        for (Candidate candidate : candidates) {
            if (candidate.needsRestart) {
                labels.add(candidate.label);
            }
        }
        return labels;
    }

    /** The UI labels of the pin settings using {@code tsName} now, in Tuning order where shown. */
    static List<String> userLabels(IniFileModel ini, ConfigurationImage image, String tsName) {
        List<String> labels = new ArrayList<>();
        for (String key : settingsOn(ini, image, tsName).keySet()) {
            labels.add(label(ini, key));
        }
        return labels;
    }

    /**
     * Moves every setting on {@code a} to {@code b} and every setting on {@code b} to {@code a}.
     *
     * @throws IllegalArgumentException when {@link #swapProblem} reports one; the image is then unchanged
     */
    static void swap(IniFileModel ini, ConfigurationImage image, String a, String b) {
        String problem = swapProblem(ini, image, a, b);
        if (problem != null) {
            throw new IllegalArgumentException("Cannot swap " + a + " and " + b + ": " + problem);
        }
        Map<String, EnumIniField> onA = settingsOn(ini, image, a);
        Map<String, EnumIniField> onB = settingsOn(ini, image, b);
        for (EnumIniField field : onA.values()) {
            image.setBitValue(field, ordinalOf(field, b));
        }
        for (EnumIniField field : onB.values()) {
            image.setBitValue(field, ordinalOf(field, a));
        }
    }

    /** Every pin setting on {@code tsName}, shown in a dialog or not, keyed by field name in Tuning order. */
    private static Map<String, EnumIniField> settingsOn(IniFileModel ini, ConfigurationImage image, String tsName) {
        Map<String, EnumIniField> result = new LinkedHashMap<>();
        Map<String, IniField> fields = ini.getAllIniFields();
        for (String key : ini.getFieldsInUiOrder().keySet()) {
            addIfOn(result, key, fields.get(key), image, tsName);
        }
        for (Map.Entry<String, IniField> entry : fields.entrySet()) {
            addIfOn(result, entry.getKey(), entry.getValue(), image, tsName);
        }
        return result;
    }

    private static void addIfOn(Map<String, EnumIniField> result, String key, IniField field,
                                ConfigurationImage image, String tsName) {
        if (field instanceof EnumIniField && PinoutLookup.isPinSetting(key, field) && !result.containsKey(key)
            && tsName.equals(currentPin(field, image))) {
            result.put(key, (EnumIniField) field);
        }
    }

    private static String firstUnable(IniFileModel ini, Map<String, EnumIniField> settings, String target) {
        for (Map.Entry<String, EnumIniField> entry : settings.entrySet()) {
            if (ordinalOf(entry.getValue(), target) < 0) {
                return label(ini, entry.getKey()) + " cannot use it";
            }
        }
        return null;
    }

    private static String label(IniFileModel ini, String key) {
        DialogModel.Field meta = ini.getFieldsInUiOrder().get(key);
        return meta == null || meta.getUiName() == null || meta.getUiName().isEmpty() ? key : meta.getUiName();
    }

    private static void clear(IniFileModel ini, ConfigurationImage image, String tsName, String except) {
        for (Map.Entry<String, IniField> entry : ini.getAllIniFields().entrySet()) {
            IniField field = entry.getValue();
            if (!(field instanceof EnumIniField) || !PinoutLookup.isPinSetting(entry.getKey(), field)
                || entry.getKey().equals(except) || !tsName.equals(currentPin(field, image))) {
                continue;
            }
            int none = ordinalOf((EnumIniField) field, NONE);
            if (none >= 0) {
                image.setBitValue((EnumIniField) field, none);
            }
        }
    }

    /** The ordinal of the choice {@code value} (quotes ignored), or -1 when the setting has no such choice. */
    static int ordinalOf(EnumIniField field, String value) {
        EnumIniField.EnumKeyValueMap enums = field.getEnums();
        for (int ordinal = 0; ordinal <= enums.maxOrdinal(); ordinal++) {
            String choice = enums.get(ordinal);
            if (choice != null && value.equalsIgnoreCase(choice.replace("\"", "").trim())) {
                return ordinal;
            }
        }
        return -1;
    }

    private static String currentPin(IniField field, ConfigurationImage image) {
        try {
            return PinoutLookup.pinChoice(ConfigurationImageGetterSetter.getStringValue(field, image));
        } catch (RuntimeException e) {
            return null;
        }
    }

    private static boolean isOff(String expression, IniFileModel ini, ConfigurationImage image) {
        if (expression == null || expression.trim().isEmpty()) {
            return false;
        }
        try {
            Boolean on = ExpressionEvaluator.evaluateBooleanExpression(expression, ini, image);
            return on != null && !on;
        } catch (RuntimeException e) {
            // a condition the console cannot evaluate does not hide the setting, as when it gives no answer
            return false;
        }
    }

    /** Each field's innermost dialog: the dialog listing it as a field. */
    private static Map<String, DialogModel> dialogsByField(IniFileModel ini) {
        Map<String, DialogModel> result = new HashMap<>();
        for (DialogModel dialog : ini.getDialogs().values()) {
            for (DialogModel.Field field : dialog.getFields()) {
                result.putIfAbsent(field.getKey(), dialog);
            }
        }
        return result;
    }
}
