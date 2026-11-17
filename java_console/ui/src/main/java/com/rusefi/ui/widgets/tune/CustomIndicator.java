package com.rusefi.ui.widgets.tune;

import com.opensr5.ini.ExpressionEvaluator;
import com.opensr5.ini.IndicatorModel;
import com.opensr5.ini.IniFileModel;
import com.rusefi.core.preferences.storage.Node;
import org.jetbrains.annotations.Nullable;

import java.util.*;

/**
 * An indicator the user defined: a condition on output channels (for example {@code CLT > 100}),
 * a label and a colour for each state. It is shown in the indicator strip like the INI ones and
 * evaluated the same way.
 * <p>
 * Colours are the strip's themed tones, not free colours, so a custom chip follows light and dark
 * themes and a red chip means the same as a red INI chip.
 */
public final class CustomIndicator {
    /** Strip keys of custom indicators start with this; INI expressions never do. */
    static final String KEY_PREFIX = "custom:";
    static final String ID = "id";
    static final String EXPRESSION = "expression";
    static final String OFF_LABEL = "off";
    static final String ON_LABEL = "on";
    static final String OFF_TONE = "off_tone";
    static final String ON_TONE = "on_tone";
    /** Optional: older preferences and layout files have none. */
    static final String NAME = "name";
    /** The longest label or name, so a chip stays a chip. */
    static final int MAX_LABEL = 40;
    static final String[] PROPERTIES = {ID, EXPRESSION, OFF_LABEL, ON_LABEL, OFF_TONE, ON_TONE, NAME};

    /** The chip colours a custom indicator can use, as the INI colour pair that selects each themed tone. */
    public enum Tone {
        NEUTRAL("Grey", "white", "black"),
        GREEN("Green", "green", "black"),
        YELLOW("Yellow", "yellow", "black"),
        RED("Red", "red", "black"),
        BLUE("Blue", "blue", "white"),
        DARK("Dark", "black", "white");

        private final String title;
        final String iniBackground;
        final String iniForeground;

        Tone(String title, String iniBackground, String iniForeground) {
            this.title = title;
            this.iniBackground = iniBackground;
            this.iniForeground = iniForeground;
        }

        @Override
        public String toString() {
            return title;
        }

        @Nullable
        static Tone parse(String name) {
            for (Tone tone : values()) {
                if (tone.name().equalsIgnoreCase(name)) {
                    return tone;
                }
            }
            return null;
        }
    }

    private final String id;
    private final String expression;
    private final String offLabel;
    private final String onLabel;
    private final Tone offTone;
    private final Tone onTone;
    /** What menus and tooltips call the indicator; empty to use its labels. */
    private final String name;

    public CustomIndicator(String id, String expression, String offLabel, String onLabel, Tone offTone, Tone onTone) {
        this(id, expression, offLabel, onLabel, offTone, onTone, "");
    }

    public CustomIndicator(String id, String expression, String offLabel, String onLabel, Tone offTone, Tone onTone,
                           String name) {
        this.id = Objects.requireNonNull(id);
        this.expression = Objects.requireNonNull(expression).trim();
        this.offLabel = Objects.requireNonNull(offLabel);
        this.onLabel = Objects.requireNonNull(onLabel);
        this.offTone = Objects.requireNonNull(offTone);
        this.onTone = Objects.requireNonNull(onTone);
        this.name = Objects.requireNonNull(name).trim();
    }

    /** A new indicator with an id no other one has. */
    public static CustomIndicator create(String expression, String offLabel, String onLabel, Tone offTone, Tone onTone) {
        return new CustomIndicator(newId(), expression, offLabel, onLabel, offTone, onTone);
    }

    /** A short random id, unique among one user's indicators in practice. */
    static String newId() {
        return UUID.randomUUID().toString().substring(0, 8);
    }

    public String getId() {
        return id;
    }

    public String getExpression() {
        return expression;
    }

    public String getOffLabel() {
        return offLabel;
    }

    public String getOnLabel() {
        return onLabel;
    }

    public Tone getOffTone() {
        return offTone;
    }

    public Tone getOnTone() {
        return onTone;
    }

    public String getName() {
        return name;
    }

    /**
     * What the chip shows when no label is typed: the name, or without one the condition with its
     * operators as the dialog shows them; shortened to fit a chip.
     */
    static String defaultLabel(String name, String condition) {
        String label = name.trim().isEmpty()
            ? condition.trim().replace(">=", "≥").replace("<=", "≤").replace("!=", "≠").replace("==", "=")
            : name.trim();
        return label.length() <= MAX_LABEL ? label : label.substring(0, MAX_LABEL - 1).trim() + "…";
    }

    /**
     * This indicator under another name, its chip text following the name where it showed the default
     * (the old name or the condition); labels the user typed are kept.
     */
    public CustomIndicator renamed(String newName) {
        String newDefault = defaultLabel(newName, expression);
        return new CustomIndicator(id, expression,
            isDefaultLabel(offLabel) ? newDefault : offLabel,
            isDefaultLabel(onLabel) ? newDefault : onLabel,
            offTone, onTone, newName);
    }

    /**
     * Whether {@code label} is what the chip shows when no label is typed: from the name, or from the
     * condition (also when a name was added later, in an earlier version that kept the condition).
     */
    boolean isDefaultLabel(String label) {
        return label.equals(defaultLabel(name, expression)) || label.equals(defaultLabel("", expression));
    }

    /** This indicator under another name; an empty one goes back to naming it by its labels. */
    public CustomIndicator withName(String name) {
        return new CustomIndicator(id, expression, offLabel, onLabel, offTone, onTone, name);
    }

    String key() {
        return KEY_PREFIX + id;
    }

    IndicatorModel toModel() {
        // labels are plain text, never INI string functions, so braces are kept out
        return new IndicatorModel("{ " + expression + " }", offLabel, onLabel,
            offTone.iniBackground, offTone.iniForeground, onTone.iniBackground, onTone.iniForeground);
    }

    /** Output channels the condition reads, in the order it names them. */
    Set<String> channels() {
        return new LinkedHashSet<>(ExpressionEvaluator.extractVariables(expression));
    }

    /** The channels {@code ini} does not have; all of them when there is no INI yet. */
    List<String> missingChannels(@Nullable IniFileModel ini) {
        List<String> missing = new ArrayList<>();
        for (String channel : channels()) {
            if (ini == null || !hasChannel(ini, channel)) {
                missing.add(channel);
            }
        }
        return missing;
    }

    private static boolean hasChannel(IniFileModel ini, String channel) {
        if (ini.getAllOutputChannels().containsKey(channel)) {
            return true;
        }
        Map<String, String> expressions = ini.getExpressionOutputChannels();
        return expressions != null && expressions.containsKey(channel);
    }

    void write(Node node) {
        node.setProperty(ID, id);
        node.setProperty(EXPRESSION, expression);
        node.setProperty(OFF_LABEL, offLabel);
        node.setProperty(ON_LABEL, onLabel);
        node.setProperty(OFF_TONE, offTone.name());
        node.setProperty(ON_TONE, onTone.name());
        node.setProperty(NAME, name);
    }

    /** The indicator stored in {@code node}, or null when a field is missing or invalid. */
    @Nullable
    static CustomIndicator read(Node node) {
        String id = node.getProperty(ID, null);
        String expression = node.getProperty(EXPRESSION, null);
        String offLabel = node.getProperty(OFF_LABEL, null);
        String onLabel = node.getProperty(ON_LABEL, null);
        Tone offTone = Tone.parse(node.getProperty(OFF_TONE, ""));
        Tone onTone = Tone.parse(node.getProperty(ON_TONE, ""));
        if (id == null || id.trim().isEmpty() || expression == null || expression.trim().isEmpty()
            || offLabel == null || onLabel == null || offTone == null || onTone == null) {
            return null;
        }
        return new CustomIndicator(id, expression, offLabel, onLabel, offTone, onTone, node.getProperty(NAME, ""));
    }

    @Override
    public boolean equals(Object o) {
        if (!(o instanceof CustomIndicator)) {
            return false;
        }
        CustomIndicator other = (CustomIndicator) o;
        return id.equals(other.id) && expression.equals(other.expression) && offLabel.equals(other.offLabel)
            && onLabel.equals(other.onLabel) && offTone == other.offTone && onTone == other.onTone
            && name.equals(other.name);
    }

    @Override
    public int hashCode() {
        return Objects.hash(id, expression, offLabel, onLabel, offTone, onTone, name);
    }
}
