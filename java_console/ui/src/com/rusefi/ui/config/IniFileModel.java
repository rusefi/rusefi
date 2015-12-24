package com.rusefi.ui.config;

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

    public static void main(String[] args) {
        System.out.println(IniFileModel.INSTANCE);
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
                LinkedList<String> list = new LinkedList<>(Arrays.asList(line.split("[\t =,]")));

                trim(list);

                if (list.isEmpty())
                    continue;
                String first = list.getFirst();

                if ("dialog".equals(first)) {
                    list.removeFirst();
                    state = State.DIALOG;
                    trim(list);
                    String keyword = list.removeFirst();
                    trim(list);
                    String name = list.isEmpty() ? null : list.removeFirst();

                    System.out.println("Dialog " + keyword + ": " + name);

                }


            }
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }
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
            if (c == '\"' || c == ' ' && !inQuote) {
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

}
