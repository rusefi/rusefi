package com.rusefi.ui.wizard;

import com.rusefi.core.net.PropertiesHolder;

import javax.swing.JComponent;
import javax.swing.JTextField;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.text.AbstractDocument;
import javax.swing.text.AttributeSet;
import javax.swing.text.BadLocationException;
import javax.swing.text.DocumentFilter;

/**
 * VIN-specific wiring: prefills from the {@code vin_number_prefix} property in
 * {@code shared_io.properties}, strips whitespace on every insert/paste,
 * caps the field at 17 characters, and only considers the field valid at exactly 17.
 */
final class VinFieldPolicy implements WizardFieldPolicy {
    private static final int VIN_LENGTH = 17;
    private static final String PREFIX_PROPERTY = "vin_number_prefix";

    @Override
    public void decorate(JComponent editor, Runnable onValidityChange) {
        if (!(editor instanceof JTextField)) return;
        JTextField tf = (JTextField) editor;
        ((AbstractDocument) tf.getDocument()).setDocumentFilter(new DocumentFilter() {
            @Override
            public void insertString(FilterBypass fb, int offset, String string, AttributeSet attr) throws BadLocationException {
                replace(fb, offset, 0, string, attr);
            }

            @Override
            public void replace(FilterBypass fb, int offset, int length, String text, AttributeSet attrs) throws BadLocationException {
                String incoming = text == null ? "" : text.replaceAll("\\s+", "");
                int available = VIN_LENGTH - (fb.getDocument().getLength() - length);
                if (available < 0) available = 0;
                if (incoming.length() > available) incoming = incoming.substring(0, available);
                super.replace(fb, offset, length, incoming, attrs);
            }
        });
        tf.getDocument().addDocumentListener(new DocumentListener() {
            public void insertUpdate(DocumentEvent e) { onValidityChange.run(); }
            public void removeUpdate(DocumentEvent e) { onValidityChange.run(); }
            public void changedUpdate(DocumentEvent e) { onValidityChange.run(); }
        });
    }

    @Override
    public String transformLoadedValue(String current) {
        if (current != null && !current.trim().isEmpty()) return current;
        String prefix = PropertiesHolder.getProperty(PREFIX_PROPERTY, "");
        if (prefix == null) return "";
        prefix = prefix.replaceAll("\\s+", "");
        if (prefix.length() > VIN_LENGTH) prefix = prefix.substring(0, VIN_LENGTH);
        return prefix;
    }

    @Override
    public String transformSavedValue(String text) {
        return text == null ? "" : text.replaceAll("\\s+", "");
    }

    @Override
    public boolean isValid(JComponent editor) {
        if (!(editor instanceof JTextField)) return true;
        return ((JTextField) editor).getText().length() == VIN_LENGTH;
    }
}
