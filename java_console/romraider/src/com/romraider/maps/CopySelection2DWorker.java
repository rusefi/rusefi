package com.romraider.maps;

import com.rusefi.ConsoleUI;

import javax.swing.*;
import java.awt.*;

class CopySelection2DWorker extends SwingWorker<Void, Void> {
    Table2D table;
    Table extendedTable;

    public CopySelection2DWorker(Table2D table)
    {
        this.table = table;
    }

    @Override
    protected Void doInBackground() throws Exception {
        table.getAxis().copySelection();
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

