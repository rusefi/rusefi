package com.rusefi.ui;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.DialogModel;
import com.opensr5.ini.ExpressionEvaluator;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Assigning a pin from the Pinout tab: the reverse of choosing a pin in a Tuning pin selector. The
 * settings that can use a pin are the pin selectors ({@link PinoutLookup#isPinField}) whose choices
 * include the pin's expanded {@code ts_name}, so the INI already says which outputs, analog inputs or
 * switch inputs suit it. A pin has one function: assigning it sets every other setting on that pin to
 * NONE.
 */
final class PinAssignments {
    private static final String NONE = "NONE";

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

        Candidate(String fieldKey, String label, String section, String dialogKey, String current, boolean hidden) {
            this.fieldKey = fieldKey;
            this.label = label;
            this.section = section;
            this.dialogKey = dialogKey;
            this.current = current;
            this.hidden = hidden;
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
        Map<String, DialogModel> dialogOf = dialogsByField(ini);
        Map<String, IniField> fields = ini.getAllIniFields();
        List<Candidate> result = new ArrayList<>();
        for (Map.Entry<String, DialogModel.Field> entry : ini.getFieldsInUiOrder().entrySet()) {
            String key = entry.getKey();
            IniField field = fields.get(key);
            DialogModel dialog = dialogOf.get(key);
            if (!(field instanceof EnumIniField) || !PinoutLookup.isPinField(key) || dialog == null
                || ordinalOf((EnumIniField) field, tsName) < 0) {
                continue;
            }
            DialogModel.Field meta = entry.getValue();
            String label = meta.getUiName() == null || meta.getUiName().isEmpty() ? key : meta.getUiName();
            String section = dialog.getUiName() == null || dialog.getUiName().isEmpty() ? dialog.getKey()
                : dialog.getUiName();
            result.add(new Candidate(key, label, section, dialog.getKey(), currentPin(field, image),
                isOff(meta.getVisibleExpression(), ini, image) || isOff(meta.getEnableExpression(), ini, image)));
        }
        return result;
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

    /** Sets every pin setting using {@code tsName} to NONE. */
    static void unassign(IniFileModel ini, ConfigurationImage image, String tsName) {
        clear(ini, image, tsName, null);
    }

    private static void clear(IniFileModel ini, ConfigurationImage image, String tsName, String except) {
        for (Map.Entry<String, IniField> entry : ini.getAllIniFields().entrySet()) {
            IniField field = entry.getValue();
            if (!(field instanceof EnumIniField) || !PinoutLookup.isPinField(entry.getKey())
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
        Boolean on = ExpressionEvaluator.evaluateBooleanExpression(expression, ini, image);
        return on != null && !on;
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
