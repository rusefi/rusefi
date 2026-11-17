package com.rusefi.ui.logview;

import com.rusefi.core.preferences.storage.Node;

import java.util.ArrayList;
import java.util.List;
import java.util.Locale;

/**
 * How the user chose to scale signals, kept by signal label in the console config so a signal is scaled the same
 * way in every log: to its whole-log range (the default, nothing stored), to the visible time (rescaling as the
 * view zooms and pans), or to a fixed custom range.
 */
final class SignalScales {
    enum Mode {
        WHOLE_LOG, VISIBLE, CUSTOM
    }

    private static final String VISIBLE = "visible";
    private static final String CUSTOM = "custom:";

    private final Node node;
    private final List<Runnable> listeners = new ArrayList<>();

    /** Choices that are forgotten with the viewer, e.g. in the sandbox. */
    SignalScales() {
        this(new Node());
    }

    SignalScales(Node node) {
        this.node = node;
    }

    /** Called after every change, e.g. to save the config. */
    void addListener(Runnable listener) {
        listeners.add(listener);
    }

    private void changed() {
        for (Runnable listener : listeners) {
            listener.run();
        }
    }

    Mode mode(String label) {
        String value = node.getProperty(label, null);
        if (VISIBLE.equals(value)) {
            return Mode.VISIBLE;
        }
        return custom(label) != null ? Mode.CUSTOM : Mode.WHOLE_LOG;
    }

    /** The custom {min, max}, or null when the signal has no valid custom range. */
    double[] custom(String label) {
        String value = node.getProperty(label, null);
        if (value == null || !value.startsWith(CUSTOM)) {
            return null;
        }
        String[] parts = value.substring(CUSTOM.length()).split(":");
        try {
            double min = Double.parseDouble(parts[0]);
            double max = Double.parseDouble(parts[1]);
            return Double.isFinite(min) && Double.isFinite(max) && min < max ? new double[]{min, max} : null;
        } catch (RuntimeException corrupt) {
            return null;
        }
    }

    void setWholeLog(String label) {
        node.getConfig().remove(label);
        changed();
    }

    void setVisible(String label) {
        node.setProperty(label, VISIBLE);
        changed();
    }

    /** @param range {min, max} with min below max */
    void setCustom(String label, double[] range) {
        node.setProperty(label, String.format(Locale.ROOT, "%s%s:%s", CUSTOM, range[0], range[1]));
        changed();
    }
}
