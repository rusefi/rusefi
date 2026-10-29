package com.rusefi.ui;

import java.awt.image.BufferedImage;
import java.util.List;
import java.util.Locale;
import java.util.regex.Pattern;

/**
 * Links Tuning's pin selectors to the Pinout connectors. A pin selector is an enum setting whose
 * key follows the rusEFI convention {@code .*pins?\d*} ({@code fuelPumpPin}, {@code injectionPins1});
 * its value, the pin choice, equals a connector pin's expanded {@code ts_name}.
 */
public final class PinoutLookup {
    private static final Pattern PIN_FIELD = Pattern.compile(".*pins?\\d*");

    private PinoutLookup() {
    }

    /** Whether the setting {@code fieldKey} chooses a pin. */
    public static boolean isPinField(String fieldKey) {
        return fieldKey != null && PIN_FIELD.matcher(fieldKey.toLowerCase(Locale.ROOT)).matches();
    }

    /**
     * The pin chosen by a pin selector value, without the INI quotes; null for no pin
     * (empty, NONE or INVALID).
     */
    public static String pinChoice(Object value) {
        if (value == null) {
            return null;
        }
        String choice = value.toString().replace("\"", "").trim();
        return choice.isEmpty() || "NONE".equalsIgnoreCase(choice) || "INVALID".equalsIgnoreCase(choice)
            ? null : choice;
    }

    /** Where a pin choice is on the board: the connector, in switch order, and the pin. */
    static final class Match {
        final int connector;
        final String connectorTitle;
        /** The connector drawing; null when the board has none. */
        final BufferedImage image;
        /** Every pin of that connector, the match included. */
        final List<PinoutPin> connectorPins;
        final PinoutPin pin;

        Match(int connector, String connectorTitle, BufferedImage image, List<PinoutPin> connectorPins, PinoutPin pin) {
            this.connector = connector;
            this.connectorTitle = connectorTitle;
            this.image = image;
            this.connectorPins = connectorPins;
            this.pin = pin;
        }
    }

    /** The first connector pin for {@code value}, or null when there is no pin choice or no such pin. */
    static Match find(List<PinoutConnectorPage> connectors, Object value) {
        String choice = pinChoice(value);
        if (choice == null) {
            return null;
        }
        for (int i = 0; i < connectors.size(); i++) {
            PinoutConnectorPage page = connectors.get(i);
            for (PinoutPin pin : page.pins) {
                if (choice.equals(pin.tsName)) {
                    return new Match(i, page.title, page.image, page.pins, pin);
                }
            }
        }
        return null;
    }
}
