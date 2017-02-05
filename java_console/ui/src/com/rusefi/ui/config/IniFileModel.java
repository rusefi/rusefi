package com.rusefi.ui.config;

import com.rusefi.config.Field;

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
        BufferedReader d = null;
        try {
            d = new BufferedReader(new InputStreamReader(new FileInputStream(input)));
        } catch (FileNotFoundException e) {
            System.out.println("No such file: " + FILENAME);
            return;
        }


        try {
            String line;

            State state = State.SKIPPING;

            while ((line = d.readLine()) != null) {
                handleLine(line);


            }
            finishDialog();
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }
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

    private void handleLine(String line) {
        try {
            LinkedList<String> list = new LinkedList<>(Arrays.asList(split(line)));

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
            throw new IllegalStateException("While [" + line + "]", e);
        }
    }

    private void handleField(LinkedList<String> list) {
        list.removeFirst(); // "field"

        String uiLabel = list.isEmpty() ? "" : list.removeFirst();

        String key = list.isEmpty() ? null : list.removeFirst();

        fields.add(new DialogModel.Field(key, uiLabel));
        System.out.println("Field label=[" + uiLabel + "] : key=[" + key + "]");
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
        System.out.println("Dialog key=" + keyword + ": name=[" + name + "]");
    }

    private void trim(LinkedList<String> list) {
        while (!list.isEmpty() && list.getFirst().isEmpty())
            list.removeFirst();
    }

    enum State {
        SKIPPING,
        DIALOG
    }

    public static String[] split(String str) {
        ArrayList<String> strings = new ArrayList<>();
        boolean inQuote = false;
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < str.length(); i++) {
            char c = str.charAt(i);
            if (c == '\"' || isWhitespace(c) && !inQuote) {
                if (c == '\"')
                    inQuote = !inQuote;
                if (!inQuote && sb.length() > 0) {
                    strings.add(sb.toString());
                    sb.delete(0, sb.length());
                }
            } else
                sb.append(c);
        }
        if (sb.length() > 0)
            strings.add(sb.toString());

        return strings.toArray(new String[strings.size()]);
    }

    private static boolean isWhitespace(int c) {
        return c == ' ' || c == '\t' || c == '=' || c == ',';
    }

    public static IniFileModel getinstance() {
        return INSTANCE;
    }

    public Map<String, DialogModel> getDialogs() {
        return dialogs;
    }
}
