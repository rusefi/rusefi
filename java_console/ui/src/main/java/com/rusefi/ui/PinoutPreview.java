package com.rusefi.ui;

import com.rusefi.ui.widgets.tune.TuningStyle;

import javax.swing.*;
import java.awt.*;

/**
 * Where a Tuning pin choice is, shown while hovering the "Show on connector" button: the pin and its
 * connector, the connector drawing with that pin ringed and the others faded, then what the pin is.
 * Without a drawing, or when the YAML does not place the pin, the card has the text only.
 */
final class PinoutPreview {
    static final String CLICK_HINT = "Click to show it in Pinout";
    private static final int DRAWING_WIDTH = 520;
    private static final int MAX_DRAWING_HEIGHT = 320;

    private PinoutPreview() {
    }

    static JComponent create(PinoutLookup.Match match) {
        PinoutPin pin = match.pin;
        JPanel card = new JPanel(new BorderLayout(0, 10));
        TuningStyle.styleCard(card, 12, 14);

        JLabel title = new JLabel(pin.name, new PinoutConnectorPage.MarkerIcon(
            PinoutDrawing.fills(pin, PinoutDrawing.ColorMode.TYPE),
            PinoutDrawing.dashed(pin, PinoutDrawing.ColorMode.TYPE), 14, null), SwingConstants.LEFT);
        title.setFont(title.getFont().deriveFont(Font.BOLD, title.getFont().getSize2D() + 2));
        title.setIconTextGap(8);
        JPanel header = new JPanel(new FlowLayout(FlowLayout.LEFT, 8, 0));
        header.setOpaque(false);
        header.add(title);
        header.add(muted(new JLabel(match.connectorTitle)));
        card.add(header, BorderLayout.NORTH);

        if (match.image != null && pin.isPlaced()) {
            PinoutDrawing drawing = new PinoutDrawing();
            drawing.setData(match.image, match.connectorPins);
            drawing.setSelectedPin(pin.name);
            drawing.setShown(other -> other == pin);
            double aspect = (double) match.image.getHeight() / match.image.getWidth();
            drawing.setPreferredSize(new Dimension(DRAWING_WIDTH,
                (int) Math.min(MAX_DRAWING_HEIGHT, Math.round(DRAWING_WIDTH * aspect))));
            card.add(drawing, BorderLayout.CENTER);
        }

        JPanel facts = new JPanel(new GridBagLayout());
        facts.setOpaque(false);
        int row = 0;
        if (!pin.function.isEmpty()) {
            addFact(facts, row++, "Function", pin.function);
        }
        addFact(facts, row++, "Type", pin.type.type.label);
        if (!pin.wire.isEmpty()) {
            addFact(facts, row++, "Pigtail", pin.wire);
        }
        addFact(facts, row++, "Tune use", PinoutConnectorPage.useText(pin));
        JLabel hint = muted(new JLabel(CLICK_HINT));
        GridBagConstraints c = new GridBagConstraints();
        c.gridy = row;
        c.gridwidth = 2;
        c.anchor = GridBagConstraints.WEST;
        c.insets = new Insets(6, 0, 0, 0);
        facts.add(hint, c);
        card.add(facts, BorderLayout.SOUTH);
        return card;
    }

    private static void addFact(JPanel facts, int row, String label, String value) {
        GridBagConstraints c = new GridBagConstraints();
        c.gridy = row;
        c.anchor = GridBagConstraints.WEST;
        c.insets = new Insets(0, 0, 4, 12);
        facts.add(muted(new JLabel(label)), c);
        c.gridx = 1;
        c.weightx = 1;
        c.fill = GridBagConstraints.HORIZONTAL;
        c.insets = new Insets(0, 0, 4, 0);
        facts.add(new JLabel(value), c);
    }

    private static <T extends JComponent> T muted(T component) {
        component.setForeground(UIManager.getColor("Label.disabledForeground"));
        return component;
    }
}
