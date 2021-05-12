package com.romraider.maps;

import com.rusefi.ConsoleUI;

import javax.swing.*;
import java.awt.*;
import java.awt.datatransfer.StringSelection;

class CopyTableWorker extends SwingWorker<Void, Void> {
    Table table;

    public CopyTableWorker(Table table) {
        this.table = table;
    }

    @Override
    protected Void doInBackground() throws Exception {
        String tableHeader = table.getSettings().getTableHeader();
        StringBuffer output = new StringBuffer(tableHeader);
        output.append(table.getTableAsString());
        Toolkit.getDefaultToolkit().getSystemClipboard().setContents(new StringSelection(String.valueOf(output)), null);
        return null;
    }

    @Override
    public void done() {
        Window ancestorWindow = SwingUtilities.getWindowAncestor(table);
        if(null != ancestorWindow) {
            ancestorWindow.setCursor(null);
        }
        table.setCursor(null);
        ConsoleUI.getFrame().setCursor(null);
    }
}