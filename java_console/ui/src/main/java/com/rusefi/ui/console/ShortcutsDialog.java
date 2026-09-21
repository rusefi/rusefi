package com.rusefi.ui.console;

import javax.swing.*;
import java.awt.*;
import java.awt.event.KeyEvent;

/** User-facing shortcut legend. Keep this text in sync with Console key bindings. */
final class ShortcutsDialog extends JDialog {
    private static final String LEGEND = String.join("\n",
            "KEYBOARD SHORTCUTS",
            "",
            "Keep this window open while using the Console. Shortcuts act on the focused "
                    + "control or window; click back in the Console to use its shortcuts. Keys below "
                    + "are fixed, not configurable. Ctrl means Control, including on macOS for the "
                    + "Console's explicit Ctrl shortcuts. Standard editing and menu keys may vary "
                    + "with your operating system and keyboard layout.",
            "",
            "CONNECTION AND BINARY LOGGING",
            "Ctrl+R  Connect / reconnect to the ECU.",
            "Ctrl+D  Disconnect from the ECU.",
            "Ctrl+S  Start or stop binary data logging in the main Console window.",
            "        Starting opens the log file chooser and requires a connected ECU.",
            "        If Save tune is checked, recording starts after the tune is saved.",
            "        Press Ctrl+S again to stop recording or cancel a pending tune capture.",
            "        This shortcut controls logging; use File > Save Tune to save a tune.",
            "",
            "TUNING TABLES",
            "Click a table cell first. Editing shortcuts operate on the selected data cells "
                    + "in an editable table, not on the axis column or a read-only comparison.",
            "H  Interpolate horizontally between the selected endpoints in each row.",
            "V  Interpolate vertically between the selected endpoints in each column.",
            "/  Interpolate the selection: horizontal for one row, vertical for one column,",
            "   or between the four corners for a rectangular block.",
            "S  Smooth the selected cells using neighboring values.",
            "These keys have no Ctrl or Alt modifier. Focus in a text field can consume "
                    + "typing instead, so return focus to the table before using them.",
            "",
            "Arrow keys    Move the selected cell.",
            "Shift+arrows  Extend the selection.",
            "Ctrl+A        Select all cells.",
            "Ctrl+Space    Toggle the current cell's selection.",
            "Tab / Shift+Tab  Move to the next / previous cell.",
            "Shift+click extends a selection; Ctrl+click toggles a cell.",
            "",
            "Up and Down toolbar buttons change selected values by the delta field "
                    + "(initially 0.5). The = button sets a value for the selection. These buttons "
                    + "have no dedicated shortcuts. Arrow keys select cells; they do not change values.",
            "Tune Undo and Redo are toolbar buttons without dedicated shortcuts.",
            "In curve tables, type to replace a cell value, Enter to accept, or Esc to cancel "
                    + "an edit. Curve tables have no dedicated increase/decrease shortcuts.",
            "",
            "COMMAND INPUT",
            "Ctrl+Enter  Send the command text (each line is sent separately).",
            "In a single-line command field, Enter also sends the command and Up/Down "
                    + "browse command history. In a multiline field, Enter inserts a new line.",
            "",
            "LUA EDITOR",
            "Ctrl+Z  Undo a script edit.",
            "Ctrl+Y  Redo a script edit.",
            "Ctrl+Space  Show completion suggestions.",
            "These undo/redo operations affect the script, not tuning tables.",
            "Text fields also support standard Ctrl+A/C/X/V for select all/copy/cut/paste "
                    + "(typically Command instead of Ctrl on macOS).",
            "",
            "CHARTS AND ENGINE SNIFFER",
            "Numpad + / -  Zoom in / out where chart zoom controls are available.",
            "Alt+X         Also zooms in.",
            "Left / Right  Previous / next captured chart page where paging is available.",
            "Focused controls such as tables or text fields can use these keys themselves.",
            "On panels with the corresponding buttons: Alt+P pauses, Alt+C clears, "
                    + "and Alt+S saves an image. These actions depend on the active panel.",
            "",
            "MENUS AND DIALOGS",
            "Alt+F  File menu; then L loads a tune, S saves a tune, X exits.",
            "Alt+A  Actions menu.",
            "Alt+B  Binary Logging menu.",
            "F1     Open this Shortcuts window from the main Console.",
            "Alt reveals underlined menu/button letters on platforms using Alt mnemonics.",
            "Tab / Shift+Tab moves between controls; Space activates a focused button.",
            "In firmware selection, rollback, update-blocked and emergency-wipe dialogs, "
                    + "Esc cancels or closes. Esc or Alt+C closes this Shortcuts window.",
            "At the connection screen, Enter activates the Connect button.",
            "",
            "UPDATER AT LAUNCH",
            "Turn Caps Lock on before launching the updater to skip the automatic update "
                    + "check and launch the installed Console.");

    ShortcutsDialog(JFrame owner) {
        super(owner, "Shortcuts", Dialog.ModalityType.MODELESS);
        setDefaultCloseOperation(DISPOSE_ON_CLOSE);

        JTextArea text = new JTextArea(LEGEND, 28, 86);
        text.setEditable(false);
        text.setLineWrap(true);
        text.setWrapStyleWord(true);
        text.setFont(new Font(Font.MONOSPACED, Font.PLAIN, text.getFont().getSize()));
        text.setMargin(new Insets(12, 12, 12, 12));
        text.setCaretPosition(0);
        text.getAccessibleContext().setAccessibleName("Keyboard shortcut reference");
        JScrollPane scroll = new JScrollPane(text, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
                JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
        add(scroll, BorderLayout.CENTER);

        JButton close = new JButton("Close");
        close.setMnemonic(KeyEvent.VK_C);
        close.setToolTipText("Close this window (Esc or Alt+C)");
        close.addActionListener(e -> dispose());
        JPanel buttons = new JPanel(new FlowLayout(FlowLayout.RIGHT));
        buttons.add(close);
        add(buttons, BorderLayout.SOUTH);
        getRootPane().registerKeyboardAction(e -> dispose(), KeyStroke.getKeyStroke(KeyEvent.VK_ESCAPE, 0),
                JComponent.WHEN_IN_FOCUSED_WINDOW);
        pack();
        setLocationRelativeTo(owner);
    }
}
