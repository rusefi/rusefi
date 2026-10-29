package com.rusefi.ui.widgets.tune;

import javax.swing.*;

/**
 * What the "Show on connector" button beside a Tuning pin selector does with the chosen pin
 * ({@code ts_name} value such as "B6 injector output 1"). A lambda gives {@link #show} only.
 */
@FunctionalInterface
public interface PinoutLink {
    /** Shows the pin in Pinout. */
    void show(String pin);

    /**
     * A card showing where the pin is, built fresh for each hover; null when the board's pinout
     * is not loaded or does not have this pin.
     */
    default JComponent preview(String pin) {
        return null;
    }

    /** A link that always asks {@code current} for the link to use, so callers can set it later. */
    static PinoutLink forwarding(java.util.function.Supplier<PinoutLink> current) {
        return new PinoutLink() {
            @Override
            public void show(String pin) {
                PinoutLink link = current.get();
                if (link != null) {
                    link.show(pin);
                }
            }

            @Override
            public JComponent preview(String pin) {
                PinoutLink link = current.get();
                return link == null ? null : link.preview(pin);
            }
        };
    }
}
