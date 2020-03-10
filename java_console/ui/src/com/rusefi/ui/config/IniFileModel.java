package com.rusefi.ui.config;

import com.opensr5.io.IniFileReader;
import com.opensr5.io.RawIniFile;
import org.jetbrains.annotations.Nullable;

import java.io.*;
import java.util.*;

import static com.rusefi.Launcher.INI_FILE_PATH;

/**
 * (c) Andrey Belomutskiy
 * 12/23/2015.
 */
public class IniFileModel {
    public static final String RUSEFI_INI_PREFIX = "rusefi";
    public static final String RUSEFI_INI_SUFFIX = ".ini";

    private final static IniFileModel INSTANCE = new IniFileModel();
    private String dialogId;
    private String dialogUiName;
    private Map<String, DialogModel> dialogs = new TreeMap<>();
    private Map<String, DialogModel.Field> allFields = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
    // this is only used while reading model - TODO extract reader
    private List<DialogModel.Field> fieldsOfCurrentDialog = new ArrayList<>();

    public static void main(String[] args) {
        System.out.println(IniFileModel.INSTANCE.dialogs);
    }

    private IniFileModel() {
        readIniFile();
    }

    private void readIniFile() {
        String fileName = findMetaInfoFile();
        File input = null;
        if (fileName != null)
            input = new File(fileName);
        if (fileName == null || !input.exists()) {
            System.out.println("No such file: " + RUSEFI_INI_PREFIX + "*" + RUSEFI_INI_SUFFIX);
            return;
        }

        RawIniFile content = IniFileReader.read(input);


        for (RawIniFile.Line line : content.getLines()) {
            handleLine(line);
        }

        finishDialog();
    }

    private String findMetaInfoFile() {
        File dir = new File(INI_FILE_PATH);
        if (!dir.isDirectory())
            return null;
        for (String file : dir.list()) {
            if (file.startsWith(RUSEFI_INI_PREFIX) && file.endsWith(RUSEFI_INI_SUFFIX))
                return INI_FILE_PATH + File.separator + file;
        }
        return null;
    }

    private void finishDialog() {
        if (fieldsOfCurrentDialog.isEmpty())
            return;
        if (dialogUiName == null)
            dialogUiName = dialogId;
        dialogs.put(dialogUiName, new DialogModel(dialogId, dialogUiName, fieldsOfCurrentDialog));

        dialogId = null;
        fieldsOfCurrentDialog.clear();
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

        String uiFieldName = list.isEmpty() ? "" : list.removeFirst();

        String key = list.isEmpty() ? null : list.removeFirst();

        DialogModel.Field field = new DialogModel.Field(key, uiFieldName);
        if (key != null) {
            // UI labels do not have 'key'
            allFields.put(key, field);
        }
        fieldsOfCurrentDialog.add(field);
        System.out.println("IniFileModel: Field label=[" + uiFieldName + "] : key=[" + key + "]");
    }

    @Nullable
    public DialogModel.Field getField(String key) {
        DialogModel.Field field = allFields.get(key);
        return field;
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
