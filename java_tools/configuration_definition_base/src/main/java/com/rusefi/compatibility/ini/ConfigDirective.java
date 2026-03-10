package com.rusefi.compatibility.ini;

import com.rusefi.ConfigField;
import com.rusefi.ConfigFieldImpl;
import com.rusefi.ReaderState;
import org.jetbrains.annotations.Nullable;

public class ConfigDirective {

    private ConfigDirective() {
    }

    /**
     * Returns true if the given field represents a preprocessor directive
     */
    public static boolean isDirective(ConfigField field) {
        return IniPreprocessor.isDirectiveType(field.getTypeName());
    }

    /**
     * @param state the current reader state
     * @param line  the raw directive line
     * @return a new {@link ConfigFieldImpl} tagged as a directive
     */
    public static ConfigFieldImpl makeDirectiveField(ReaderState state, String line) {
        return new ConfigFieldImpl(state, "", line, null,
                IniPreprocessor.DIRECTIVE_TYPE, new int[0], null, false, false,
                null, null);
    }

    /**
     * Returns the text that should be emitted verbatim into the TunerStudio
     * output for a directive field, or {@code null} if the field is not a
     * directive (or has no comment).
     */
    @Nullable
    public static String getDirectiveOutput(ConfigField field) {
        if (!isDirective(field))
            return null;
        return field.getComment();
    }
}
