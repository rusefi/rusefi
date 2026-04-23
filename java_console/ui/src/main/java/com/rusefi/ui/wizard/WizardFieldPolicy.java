package com.rusefi.ui.wizard;

import javax.swing.JComponent;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;

/**
 * Per-field extension hook for {@link GenericFieldsPanel}. Register a policy in
 * {@link Registry#MAP} to apply field-specific prefill, input constraints, or
 * validation rules without teaching the generic panel about that field.
 */
public interface WizardFieldPolicy {
    WizardFieldPolicy DEFAULT = new WizardFieldPolicy() {};

    static WizardFieldPolicy forField(String fieldName) {
        return Registry.MAP.getOrDefault(fieldName, DEFAULT);
    }

    /** Attach live input constraints to the editor; call {@code onValidityChange} when state affecting {@link #isValid} shifts. */
    default void decorate(JComponent editor, Runnable onValidityChange) {}

    /** Transform the value loaded from the config image before it is placed in the editor (e.g. apply a default). */
    default String transformLoadedValue(String current) { return current; }

    /** Transform the editor's current text before it is written back to the config image. */
    default String transformSavedValue(String text) { return text; }

    /** Whether the current editor state is acceptable to advance past this step. */
    default boolean isValid(JComponent editor) { return true; }

    /** Holds the field-name → policy registry. New policies get a one-line entry here. */
    final class Registry {
        static final Map<String, WizardFieldPolicy> MAP;
        static {
            Map<String, WizardFieldPolicy> m = new HashMap<>();
            m.put("vinNumber", new VinFieldPolicy());
            MAP = Collections.unmodifiableMap(m);
        }

        private Registry() {}
    }
}
