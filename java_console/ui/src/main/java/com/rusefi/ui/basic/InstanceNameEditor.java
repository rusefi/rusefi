package com.rusefi.ui.basic;

import com.rusefi.core.preferences.storage.PersistentConfiguration;

import javax.swing.*;
import javax.swing.event.AncestorEvent;
import javax.swing.event.AncestorListener;
import javax.swing.text.AttributeSet;
import javax.swing.text.BadLocationException;
import javax.swing.text.PlainDocument;

import java.awt.*;

import static com.rusefi.core.preferences.storage.PersistentConfiguration.getConfig;

public class InstanceNameEditor {
    private static final String INSTANCE_NAME = "INSTANCE_NAME";

    private InstanceNameEditor() {}

    public static final InstanceNameEditor INSTANCE = new InstanceNameEditor();

    public void editInstanceName(final Component parentComponent) {
        final InstanceNameTextField instanceNameField = new InstanceNameTextField(loadInstanceName());
        instanceNameField.addAncestorListener(new RequestFocusListener());

        final int response = JOptionPane.showOptionDialog(
            parentComponent,
            new Object[] {
                "Instance name:",
                instanceNameField,
                "up to 64 latin letters, digits, spaces and tabs are allowed"
            },
            "Edit instance name",
            JOptionPane.OK_CANCEL_OPTION,
            JOptionPane.PLAIN_MESSAGE,
            null,
            null,
            null
        );
        if (response == JOptionPane.OK_OPTION) {
            saveInstanceName(instanceNameField.getText());
        }
    }

    public static String loadInstanceName() {
        return getConfig().getRoot().getProperty(INSTANCE_NAME, "");
    }

    private static void saveInstanceName(final String newInstanceName) {
        getConfig().getRoot().setProperty(INSTANCE_NAME, newInstanceName);
        PersistentConfiguration.getConfig().save();
    }

    private static class InstanceNameTextField extends JTextField {
        InstanceNameTextField(final String initialTextToDisplay) {
            super(new InstanceNameDocument(), initialTextToDisplay, 0);
        }

        private static class InstanceNameDocument extends PlainDocument {
            @Override
            public void insertString(int offs, String str, AttributeSet a) throws BadLocationException {
                super.insertString(offs, getAllowedStringToInsert(str), a);
            }

            private String getAllowedStringToInsert(final String originalString) {
                final String filteredString = PanamaArgumentValidator.secureInstanceName(originalString);
                final int allowedLengthToInsert = MAX_LENGTH - getLength();
                final String result = filteredString.substring(0, Math.min(allowedLengthToInsert, filteredString.length()));
                if (!result.equals(originalString)) {
                    java.awt.Toolkit.getDefaultToolkit().beep();
                }
                return result;
            }

            private static final int MAX_LENGTH = 64;
        }
    }

    // The following class requests focus on a component (see https://tips4java.wordpress.com/2010/03/14/dialog-focus/
    // and https://github.com/tips4java/tips4java/blob/main/source/RequestFocusListener.java for details)
    private static class RequestFocusListener implements AncestorListener
    {
        private boolean removeListener;

        public RequestFocusListener()
        {
            this(true);
        }

        public RequestFocusListener(boolean removeListener)
        {
            this.removeListener = removeListener;
        }

        @Override
        public void ancestorAdded(final AncestorEvent e)
        {
            final JComponent component = e.getComponent();
            component.requestFocusInWindow();

            if (removeListener) {
                component.removeAncestorListener(this);
            }
        }

        @Override
        public void ancestorMoved(final AncestorEvent e) {}

        @Override
        public void ancestorRemoved(final AncestorEvent e) {}
    }
}
