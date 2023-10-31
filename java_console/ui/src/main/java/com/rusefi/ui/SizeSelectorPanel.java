package com.rusefi.ui;

import com.rusefi.ui.util.UiUtils;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

public class SizeSelectorPanel extends JPanel {
    public static final int WIDTH = 5;
    public static final int HEIGHT = 3;

    private int selectedRow = 1;
    private int selectedColumn = 1;

    public SizeSelectorPanel(final SizeSelectorListener sizeSelectorListener) {
        super(new GridLayout(HEIGHT, WIDTH));

        MouseListener listener = new MouseAdapter() {
            @Override
            public void mouseEntered(MouseEvent e) {
                Element selected = (Element) e.getSource();
                selectedColumn = selected.column;
                selectedRow = selected.row;
                UiUtils.trueLayout(SizeSelectorPanel.this);
                UiUtils.trueRepaint(SizeSelectorPanel.this);
                System.out.println(selectedColumn + " r=" + selectedRow);
            }

            @Override
            public void mouseClicked(MouseEvent e) {
                // close the menu
                MenuSelectionManager.defaultManager().clearSelectedPath();
                Element selected = (Element) e.getSource();
                sizeSelectorListener.onSelected(selected.row, selected.column);
            }
        };

        for (int r = 1; r <= HEIGHT; r++) {
            for (int c = 1; c <= WIDTH; c++) {
                Element e = new Element(r, c);
                e.addMouseListener(listener);
                add(e);
            }
        }
    }

    class Element extends JPanel {
        private final int row;
        private final int column;

        public Element(int row, int column) {
            this.row = row;
            this.column = column;
        }

        @Override
        public Dimension getPreferredSize() {
            return new Dimension(25, 25);
        }

        @Override
        public void paint(Graphics g) {
            super.paint(g);
            boolean isSelected = row <= selectedRow && column <= selectedColumn;
            g.setColor(isSelected ? Color.black : Color.white);
            g.drawOval(5, 5, 15, 15);
        }
    }

    interface SizeSelectorListener {
        void onSelected(int row, int column);
    }
}