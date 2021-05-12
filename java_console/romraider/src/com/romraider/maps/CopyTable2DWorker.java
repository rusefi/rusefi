package com.romraider.maps;

import com.rusefi.ConsoleUI;

import javax.swing.*;
import java.awt.*;
import java.awt.datatransfer.StringSelection;

class CopyTable2DWorker extends SwingWorker<Void, Void> {
    Table2D table;

    public CopyTable2DWorker(Table2D table)
    {
        this.table = table;
    }

    @Override
    protected Void doInBackground() throws Exception {
        String tableHeader = table.getSettings().getTable2DHeader();
        StringBuffer output = new StringBuffer(tableHeader);
        output.append(table.getTableAsString());

        //copy to clipboard
        Toolkit.getDefaultToolkit().getSystemClipboard().setContents(new StringSelection(output.toString()), null);
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