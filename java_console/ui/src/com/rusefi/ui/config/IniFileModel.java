package com.rusefi.ui.config;

import com.opensr5.io.IniFileReader;
import com.opensr5.io.RawIniFile;

import java.io.*;
import java.util.*;

/**
 * (c) Andrey Belomutskiy
 * 12/23/2015.
 */
public class IniFileModel {
    // todo: https://sourceforge.net/p/rusefi/tickets/243/
    private static final String FILENAME = "rusefi.ini";

    private final static IniFileModel INSTANCE = new IniFileModel();
    private String dialogId;
    private String dialogUiName;
    private List<DialogModel.Field> fields = new ArrayList<>();
    private Map<String, DialogModel> dialogs = new TreeMap<>();

    public static void main(String[] args) {
        System.out.println(IniFileModel.INSTANCE.dialogs);
    }

    private IniFileModel() {
        readIniFile();
    }

    private void readIniFile() {
        File input = new File(FILENAME);
        if (!input.exists()) {
            System.out.println("No such file: " + FILENAME);
            return;
        }

        RawIniFile content = IniFileReader.read(input);


        for (RawIniFile.Line line : content.getLines()) {
            handleLine(line);
        }

        finishDialog();
    }

    private void finishDialog() {
        if (fields.isEmpty())
            return;
        if (dialogUiName == null)
            dialogUiName = dialogId;
        dialogs.put(dialogUiName, new DialogModel(dialogId, dialogUiName, fields));

        dialogId = null;
        fields.clear();
    }

    private void handleLine(RawIniFile.Line line) {
        try {
            LinkedList<String> list = new LinkedList<>(Arrays.asList(line.getTokens()));

            trim(list);

            if (list.isEmpty())
                return;
            String first = list.getFirst();

            if ("dialog".equals(first)) {
                handleDialog(list);

            } else if ("field".equals(first)) {
                handleField(list);
            }
        } catch (RuntimeException e) {
            throw new IllegalStateException("While [" + line.getRawText() + "]", e);
        }
    }

    private void handleField(LinkedList<String> list) {
        list.removeFirst(); // "field"

        String uiLabel = list.isEmpty() ? "" : list.removeFirst();

        String key = list.isEmpty() ? null : list.removeFirst();

        fields.add(new DialogModel.Field(key, uiLabel));
        System.out.println("IniFileModel: Field label=[" + uiLabel + "] : key=[" + key + "]");
    }

    private void handleDialog(LinkedList<String> list) {
        finishDialog();
        State state;
        list.removeFirst(); // "dialog"
        state = State.DIALOG;
//                    trim(list);
        String keyword = list.removeFirst();
//                    trim(list);
        String name = list.isEmpty() ? null : list.removeFirst();

        dialogId = keyword;
        dialogUiName = name;
        System.out.println("IniFileModel: Dialog key=" + keyword + ": name=[" + name + "]");
    }

    private void trim(LinkedList<String> list) {
        while (!list.isEmpty() && list.getFirst().isEmpty())
            list.removeFirst();
    }

    enum State {
        SKIPPING,
        DIALOG
    }


    public static IniFileModel getInstance() {
        return INSTANCE;
    }

    public Map<String, DialogModel> getDialogs() {
        return dialogs;
    }
}
