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
            "Ctrl+S  Start or stop recording a data log (Start Data Logging / Stop Logging in the status bar).",
            "        Starting records at once into the rusEFI logs folder and requires a",
            "        connected ECU; Logging > Start As… chooses the file instead.",
            "        If Save tune is checked, recording starts after the tune is saved.",
            "        Press Ctrl+S again to stop recording or cancel a pending tune capture.",
            "        This shortcut controls logging; use File > Save Tune to save a tune.",
            "",
            "PINOUT",
            "Enter   In the search box: select the first matching pin on the connector shown.",
            "",
            "LOGGING PROFILES (Logging > Profile > Edit profiles…)",
            "Delete  In the profile's signals: stop recording the selected signal.",
            "Enter   In the signals to add: record the selected signal. In the search box: record the",
            "        selected signal, or the only match.",
            "Esc     Close the window. Changes are saved as you make them.",
            "",
            "TUNING TABLES",
            "Focus the table with Tab or click a cell. Editing shortcuts operate on the selected data cells "
                    + "in an editable table, not on the axis column or a read-only comparison.",
            "H  Interpolate horizontally between the selected endpoints in each row.",
            "V  Interpolate vertically between the selected endpoints in each column.",
            "/  Interpolate the selection: horizontal for one row, vertical for one column,",
            "   or between the four corners for a rectangular block.",
            "S  Smooth the selected cells using neighboring values.",
            "These keys have no Ctrl or Alt modifier. Focus in a text field can consume "
                    + "typing instead, so return focus to the table before using them.",
            "",
            "Arrow keys    Move the selected cell; stop at the value-grid edges.",
            "Home / End    First / last value column in the current row.",
            "Ctrl+Home / Ctrl+End  First / last row, keeping the current column.",
            "+ / -         Increase / decrease selected data values by 1, regardless of precision.",
            "              Main keyboard or numpad, with table focus and no Ctrl or Alt modifier.",
            "              While editing a number, these keys enter signs as text.",
            "Mouse wheel over the grid  Adjust selected values by their displayed precision.",
            "Shift+wheel   Adjust by three steps. Ctrl+wheel moves up/down instead.",
            "              Wheel actions are ignored while editing.",
            "Shift+arrows  Extend the selection.",
            "Ctrl+A        Select all cells.",
            "Ctrl+Space    Toggle the current cell's selection.",
            "Digits        Start replacing the selected cell; changes stay pending until accepted.",
            "F2 / double-click  Edit the existing number with its text selected.",
            "Backspace / Delete  Start an empty pending edit. Space does not start editing.",
            "Enter / Shift+Enter  Accept an edit and keep the same cell selected.",
            "Esc           Cancel a pending cell edit.",
            "After typing directly: arrows accept the edit and move to the neighboring cell.",
            "After F2: left/right move the text cursor; up/down accept and move between rows.",
            "Tab / Shift+Tab  Accept an edit and move to the next / previous value cell,",
            "                 wrapping between rows and skipping the axis column.",
            "Invalid numbers block Enter and cell navigation. Leaving focus accepts valid",
            "numbers and cancels invalid or empty input.",
            "Ctrl+Tab / Ctrl+Shift+Tab  Finish the edit and move focus out of the table.",
            "With multiple cells selected, digits immediately set the entire selection.",
            "Backspace removes the last typed digit; Esc ends entry without undoing changes.",
            "Period (.) increments by one precision step outside numeric entry; within a",
            "number it enters a decimal point. Bulk entry always skips axis values.",
            "The active value cell's X and Y scale labels are softly highlighted.",
            "Shift+click extends a selection; Ctrl+click toggles a cell.",
            "",
            "Up and Down toolbar buttons change selected values by the delta field "
                    + "(initially 0.5). The = button sets a value for the selection. These buttons "
                    + "have no dedicated shortcuts. Arrow keys select cells; they do not change values.",
            "Tune Undo and Redo are toolbar buttons without dedicated shortcuts.",
            "In curve tables, type to replace a cell value, Enter to accept, or Esc to cancel "
                    + "an edit. Curve tables have no dedicated increase/decrease shortcuts.",
            "",
            "LOG VIEWER",
            "Ctrl+O  Open a data log (.mlg or .msl) while the Log Viewer tab has focus.",
            "        File > Open Data Log… and Binary Logging > View Last Log also open logs;",
            "        so does dropping a log file onto the tab.",
            "In the signal list: Space adds the selected signal to the active plot or removes it,",
            "Shift+Space adds it to a new plot. Click and Shift+click do the same.",
            "Mouse on the plots: wheel zooms around the pointer, drag selects a time range to zoom "
                    + "into, right-drag or Shift+wheel pans, double-click shows the whole log.",
            "Hover a legend item or a Stats row to highlight its trace with markers at its visible min "
                    + "and max (a legend item also shows a card with max, min and average); click it to keep "
                    + "the highlight, click again to release it. With the pointer off the plots the legend "
                    + "shows each signal's visible min–max. Stats (toolbar) lists every plotted signal.",
            "Measure (toolbar): click a plot to place cursor A, then B; later clicks move the nearer one "
                    + "and dragging a flag moves it. The pill above the time axis shows both times and the time "
                    + "between them, the legend each signal's change (Δ), and statistics cover A–B. "
                    + "Esc (with the plots focused) or the plot ⋯ menu clears A and B.",
            "Keys on the plots (click them first to give them focus):",
            "Left / Right       Step the cursor one sample (Shift: 10 samples); the view follows it.",
            "Ctrl+Left / Right  Pan by half the visible time.",
            "+ / -              Zoom in / out around the cursor (main keyboard or numpad).",
            "Home / End         Cursor to the first / last sample.",
            "0                  Whole log.",
            "A / B              Put measurement cursor A or B at the cursor (turns Measure on).",
            "Esc                Clear A and B; pressed again, release the highlighted trace.",
            "The cursor set by keys stays until the mouse moves over the plots.",
            "Drag a signal or legend item onto a plot or onto + Add plot; drag a legend item back "
                    + "onto the list to remove it. Drag the gap between plots to resize them; "
                    + "double-click the gap to make all plots the same height.",
            "",
            "LOG VIEWER: HISTOGRAM",
            "Timeline / Histogram (toolbar) switches views. In the Histogram view a click on a signal in the "
                    + "list puts it on the highlighted axis (X, Y or Z); dragging it onto an axis works too.",
            "Keys on the table (click it first to give it focus):",
            "Arrow keys         Move the selected cell; Up is the next higher Y bin.",
            "Shift+arrows       Extend the selection to a block; Shift+click does the same.",
            "Enter              Show the selected cell on the timeline (as does a double-click):",
            "                   its stays are shaded and Previous / Next step through them.",
            "Ctrl+C             Copy the selected cells (or the whole table), tab-separated, for a",
            "                   TunerStudio table; Ctrl+Shift+C copies them with their axis values.",
            "Ctrl+A             Select all cells.",
            "Esc                Clear the selection.",
            "Right-click the table for the same actions; click the legend to change the colours.",
            "",
            "LOG VIEWER: SCATTER",
            "Scatter (toolbar) shows every record as a point by X and Y, coloured by time spent, by Z or by when it "
                    + "was logged. Signals go on the axes as in the Histogram view; Recipes set up common plots "
                    + "(knock, lambda vs target). Layout: Side by side shows two plots together: the toolbar and the signal list act "
                    + "on the one last clicked (outlined).",
            "Mouse on the plot:",
            "Hover              The point's values in the status strip.",
            "Click / drag       Pick a point / select the points in a box.",
            "Double-click       A point: show it on the timeline. An empty spot: the whole plot.",
            "Wheel              Zoom around the pointer (Shift: X only, Ctrl: Y only); right-drag pans.",
            "Right-click        Show on timeline, zoom to the box, hide its points or keep only them.",
            "Keys on the plot (click it first to give it focus):",
            "Enter              Show the picked point or the box's points on the timeline.",
            "Esc                Clear the selection.",
            "0                  The whole plot.",
            "",
            "COMMAND INPUT",
            "Ctrl+Enter  Send the command text (each line is sent separately).",
            "In a single-line command field, Enter also sends the command and Up/Down "
                    + "browse command history. In a multiline field, Enter inserts a new line.",
            "",
            "LLM TAB",
            "Ctrl+Enter  Send the typed message while the message field is focused.",
            "            Available when sending is enabled (signed in, model selected,",
            "            no request in progress). Enter inserts a new line.",
            "",
            "LUA EDITOR",
            "Ctrl+Z  Undo a script edit.",
            "Ctrl+Y  Redo a script edit.",
            "Ctrl+Space  Show completion suggestions.",
            "These undo/redo operations affect the script, not tuning tables.",
            "Text fields also support standard Ctrl+A/C/X/V for select all/copy/cut/paste "
                    + "(typically Command instead of Ctrl on macOS).",
            "",
            "GAUGES: EDIT LAYOUT (FREE LAYOUT)",
            "Available after Edit Layout in Free mode; click a gauge to select it.",
            "Arrow keys    Move the selected gauge by 1% of the gauge area.",
            "Shift+arrows  Move the selected gauge by 5%.",
            "Delete / Backspace  Remove the selected gauge.",
            "Esc           Finish editing (same as the Done button).",
            "Mouse: drag to move, drag the blue corner to resize, click the red cross to remove, "
                    + "right-click to bring to front or send to back.",
            "",
            "CHARTS AND ENGINE SNIFFER",
            "Numpad + / -  Zoom in / out where chart zoom controls are available.",
            "Alt+X         Also zooms in.",
            "Left / Right  Previous / next captured chart page where paging is available.",
            "Focused controls such as tables or text fields can use these keys themselves.",
            "Alt+P pauses Engine Sniffer or message panels.",
            "On panels with the corresponding buttons: Alt+C clears "
                    + "and Alt+S saves an image. These actions depend on the active panel.",
            "",
            "MENUS AND DIALOGS",
            "Alt+F  File menu; then L loads a tune, S saves a tune, X exits.",
            "Alt+A  Actions menu.",
            "Alt+B  Binary Logging menu.",
            "Alt+T  Tools menu; then M opens Messages, E opens Engine Sniffer.",
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
