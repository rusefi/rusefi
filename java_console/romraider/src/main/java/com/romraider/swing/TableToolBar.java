/*
 * RomRaider Open-Source Tuning, Logging and Reflashing
 * Copyright (C) 2006-2012 RomRaider.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

package com.romraider.swing;

import static javax.swing.BorderFactory.createLineBorder;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Image;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.awt.event.KeyEvent;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.beans.PropertyVetoException;
import java.math.BigDecimal;
import java.net.URL;
import java.text.DecimalFormat;
import java.text.ParseException;
import java.util.Vector;

import javax.naming.NameNotFoundException;
import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.BorderFactory;
import javax.swing.ImageIcon;
import javax.swing.InputMap;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JFormattedTextField;
import javax.swing.JInternalFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextArea;
import javax.swing.JToolBar;
import javax.swing.KeyStroke;
import javax.swing.border.Border;
import javax.swing.border.TitledBorder;

import org.apache.log4j.Logger;

//import com.ecm.graphics.Graph3dFrameManager;
//import com.ecm.graphics.data.GraphData;
//import com.ecm.graphics.data.GraphDataListener;
import com.romraider.Settings;
import com.romraider.editor.ecu.ECUEditorManager;
import com.romraider.maps.DataCell;
import com.romraider.maps.Scale;
import com.romraider.maps.Table;
import com.romraider.maps.Table1D;
import com.romraider.maps.Table3D;
import com.romraider.util.SettingsManager;

public class TableToolBar extends JToolBar implements MouseListener, ItemListener, ActionListener
//        , GraphDataListener
{

    private static final long serialVersionUID = 8697645329367637930L;
    private static final Logger LOGGER = Logger.getLogger(TableToolBar.class);
    private final JButton incrementFine = new JButton();
    private final JButton decrementFine = new JButton();
    private final JButton incrementCoarse = new JButton();
    private final JButton decrementCoarse = new JButton();
    private final JButton enable3d = new JButton();
    private final JButton colorCells = new JButton();
    private final JButton refreshCompare = new JButton();

    private final JButton setValue = new JButton("Set");
    private final JButton multiply = new JButton("Mul");

    private final JFormattedTextField incrementByFine = new JFormattedTextField(new DecimalFormat("#.####"));
    private final JFormattedTextField incrementByCoarse = new JFormattedTextField(new DecimalFormat("#.####"));
    private final JFormattedTextField setValueText = new JFormattedTextField(new DecimalFormat("#.####"));

    private final JComboBox scaleSelection = new JComboBox();

    private final JPanel liveDataPanel = new JPanel();
//    private final JCheckBox overlayLog = new JCheckBox("Overlay Log");
//    private final JButton clearOverlay = new JButton("Clear Overlay");
    private final JLabel liveDataValue = new JLabel();

    private final URL incrementFineImage = getClass().getResource( "/graphics/icon-incfine.png" );
    private final URL decrementFineImage = getClass().getResource( "/graphics/icon-decfine.png" );
    private final URL incrementCoarseImage = getClass().getResource( "/graphics/icon-inccoarse.png" );
    private final URL decrementCoarseImage = getClass().getResource( "/graphics/icon-deccoarse.png" );
    private final URL enable3dImage = getClass().getResource( "/graphics/3d_render.png" );
    private final URL colorCellImage = getClass().getResource( "/graphics/icon-palette.png" );
    private final URL refreshCompareImage = getClass().getResource( "/graphics/table_refresh.png" );

    private final TitledBorder toolbarBorder = BorderFactory.createTitledBorder(Settings.defaultTableToolBarName);

    private Table selectedTable = null;

    public TableToolBar() {
        super(Settings.defaultTableToolBarName);
        this.setFloatable(true);
        this.setRollover(true);
        FlowLayout toolBarLayout = new FlowLayout(FlowLayout.LEFT, 0, 0);
        this.setLayout(toolBarLayout);

        setBorder(toolbarBorder);

        this.updateIcons();

        JPanel finePanel = new JPanel();
        finePanel.add(incrementFine);
        finePanel.add(decrementFine);
        finePanel.add(incrementByFine);
        this.add(finePanel);
        finePanel.setBorder(BorderFactory.createCompoundBorder(BorderFactory.createEmptyBorder(3, 3, 3, 3),
                BorderFactory.createLineBorder(Color.black)));

        JPanel coarsePanel = new JPanel();
        coarsePanel.add(incrementCoarse);
        coarsePanel.add(decrementCoarse);
        coarsePanel.add(incrementByCoarse);
        this.add(coarsePanel);
        coarsePanel.setBorder(BorderFactory.createCompoundBorder(BorderFactory.createEmptyBorder(3, 3, 3, 3),
                BorderFactory.createLineBorder(Color.red)));

        JPanel setValuePanel = new JPanel();
        setValuePanel.add(setValueText);
        setValuePanel.add(setValue);
        setValuePanel.add(multiply);
        this.add(setValuePanel);
        setValuePanel.setBorder(BorderFactory.createCompoundBorder(BorderFactory.createEmptyBorder(3, 3, 3, 3),
                BorderFactory.createLineBorder(Color.green)));

        colorCells.setEnabled(false);
        refreshCompare.setEnabled(false);
        enable3d.setEnabled(false);

        JPanel otherPanel = new JPanel();
        otherPanel.add(colorCells);
        otherPanel.add(refreshCompare);
        otherPanel.add(enable3d);
        this.add(otherPanel);

        JPanel scaleSelectionPanel = new JPanel();
        scaleSelectionPanel.add(scaleSelection);
        this.add(scaleSelectionPanel);

        incrementFine.setBorder(createLineBorder(new Color(150, 150, 150), 1));
        decrementFine.setBorder(createLineBorder(new Color(150, 150, 150), 1));
        incrementCoarse.setBorder(createLineBorder(new Color(150, 150, 150), 1));
        decrementCoarse.setBorder(createLineBorder(new Color(150, 150, 150), 1));

        enable3d.setBorder(createLineBorder(new Color(150, 150, 150), 1));
        setValue.setPreferredSize(new Dimension(33, 23));
        setValue.setBorder(createLineBorder(new Color(150, 150, 150), 1));
        multiply.setPreferredSize(new Dimension(33, 23));
        multiply.setBorder(createLineBorder(new Color(150, 150, 150), 1));

        colorCells.setBorder(createLineBorder(new Color(150, 150, 150), 1));
        refreshCompare.setBorder(createLineBorder(new Color(150, 150, 150), 1));

        scaleSelection.setPreferredSize(new Dimension(80, 23));

//        clearOverlay.setPreferredSize(new Dimension(75, 23));
//        clearOverlay.setBorder(createLineBorder(new Color(150, 150, 150), 1));

        incrementByFine.setAlignmentX(JTextArea.CENTER_ALIGNMENT);
        incrementByFine.setAlignmentY(JTextArea.CENTER_ALIGNMENT);
        incrementByFine.setPreferredSize(new Dimension(45, 23));
        incrementByCoarse.setAlignmentX(JTextArea.CENTER_ALIGNMENT);
        incrementByCoarse.setAlignmentY(JTextArea.CENTER_ALIGNMENT);
        incrementByCoarse.setPreferredSize(new Dimension(45, 23));
        setValueText.setAlignmentX(JTextArea.CENTER_ALIGNMENT);
        setValueText.setAlignmentY(JTextArea.CENTER_ALIGNMENT);
        setValueText.setPreferredSize(new Dimension(45, 23));

        incrementFine.setToolTipText("Increment Value (Fine)");
        decrementFine.setToolTipText("Decrement Value (Fine)");
        incrementCoarse.setToolTipText("Increment Value (Coarse)");
        decrementCoarse.setToolTipText("Decrement Value (Coarse)");
        enable3d.setToolTipText("Render data in 3d");
        setValue.setToolTipText("Set Absolute Value");
        setValueText.setToolTipText("Set Absolute Value");
        incrementByFine.setToolTipText("Fine Value Adjustment");
        incrementByCoarse.setToolTipText("Coarse Value Adjustment");
        multiply.setToolTipText("Multiply Value");
//        overlayLog.setToolTipText("Enable Overlay Of Real Time Log Data");
//        clearOverlay.setToolTipText("Clear Log Data Overlay Highlights");
        colorCells.setToolTipText("Color Table Cells");
        refreshCompare.setToolTipText("Refresh Table Compare");

        incrementFine.addMouseListener(this);
        decrementFine.addMouseListener(this);
        incrementCoarse.addMouseListener(this);
        decrementCoarse.addMouseListener(this);
        enable3d.addMouseListener(this);
        setValue.addMouseListener(this);
        multiply.addMouseListener(this);
        scaleSelection.addItemListener(this);
//        overlayLog.addItemListener(this);
//        clearOverlay.addActionListener(this);
        colorCells.addMouseListener(this);
        refreshCompare.addMouseListener(this);

        // key binding actions
        Action enterAction = new AbstractAction() {
            private static final long serialVersionUID = -6008026264821746092L;

            @Override
            public void actionPerformed(ActionEvent e) {
                JInternalFrame selectedFrame = ECUEditorManager.getECUEditor().getRightPanel().getSelectedFrame();
                if(selectedFrame == null || !(selectedFrame instanceof TableFrame)) {
                    return;
                }
                TableFrame frame = (TableFrame)selectedFrame;
                frame.toFront();
                try {
                    frame.setSelected(true);
                } catch (PropertyVetoException ex) {
                }
                frame.requestFocusInWindow();
                setValue(frame.getTable());
            }
        };

        // set input mapping
        InputMap im = getInputMap(WHEN_IN_FOCUSED_WINDOW);

        KeyStroke enter = KeyStroke.getKeyStroke(KeyEvent.VK_ENTER, 0);

        im.put(enter, "enterAction");
        getActionMap().put(im.get(enter), enterAction);

//        liveDataPanel.add(overlayLog);
//        liveDataPanel.add(clearOverlay);
        //liveDataPanel.add(liveDataValue);
        this.add(liveDataPanel);
//        overlayLog.setEnabled(false);
//        clearOverlay.setEnabled(false);

        incrementFine.getInputMap().put(enter, "enterAction");
        decrementFine.getInputMap().put(enter, "enterAction");
        incrementCoarse.getInputMap().put(enter, "enterAction");
        decrementCoarse.getInputMap().put(enter, "enterAction");
        incrementByFine.getInputMap().put(enter, "enterAction");
        incrementByCoarse.getInputMap().put(enter, "enterAction");
        setValueText.getInputMap().put(enter, "enterAction");
        setValue.getInputMap().put(enter, "enterAction");

        this.setEnabled(true);
    }

    public void updateIcons() {
        Settings settings = SettingsManager.getSettings();
        incrementFine.setIcon(rescaleImageIcon(new ImageIcon(incrementFineImage), settings.getTableIconScale()));
        decrementFine.setIcon(rescaleImageIcon(new ImageIcon(decrementFineImage), settings.getTableIconScale()));
        incrementCoarse.setIcon(rescaleImageIcon(new ImageIcon(incrementCoarseImage), settings.getTableIconScale()));
        decrementCoarse.setIcon(rescaleImageIcon(new ImageIcon(decrementCoarseImage), settings.getTableIconScale()));
        enable3d.setIcon(rescaleImageIcon(new ImageIcon(enable3dImage), settings.getTableIconScale()));
        colorCells.setIcon(rescaleImageIcon(new ImageIcon(colorCellImage), settings.getTableIconScale()));
        refreshCompare.setIcon(rescaleImageIcon(new ImageIcon(refreshCompareImage), settings.getTableIconScale()));
    }

    private ImageIcon rescaleImageIcon(ImageIcon imageIcon, int percentOfOriginal) {
        int height = imageIcon.getImage().getHeight(this);
        int newHeight = (int) (height * (percentOfOriginal * .01));
        int width = imageIcon.getImage().getWidth(this);
        int newWidth = (int) (width * (percentOfOriginal * .01));
        if (newHeight == 0 || newWidth == 0)
            throw new IllegalStateException("rescale error: w " + width + "/" + newWidth + ", h " + height + "/" + newHeight);

        imageIcon.setImage(imageIcon.getImage().getScaledInstance(newWidth, newHeight, Image.SCALE_SMOOTH));
        return imageIcon;
    }

    public Table getTable() {
        JInternalFrame frame = ECUEditorManager.getECUEditor().getRightPanel().getSelectedFrame();
        if(null == frame) {
            return null;
        }

        if( !(frame instanceof TableFrame) ) {
            return null;
        }


        return ((TableFrame)frame).getTable();
    }

    @Override
    public void setBorder(Border border) {
        if(SettingsManager.getSettings().isShowTableToolbarBorder()) {
            super.setBorder(toolbarBorder);
        } else {
            super.setBorder(BorderFactory.createEmptyBorder());
        }
    }

    public void updateTableToolBar() {
        this.updateTableToolBar(getTable());
    }

    public void updateTableToolBar(Table selectedTable) {
        if(selectedTable == null  && this.selectedTable == null) {
            // Skip if the table is the same to avoid multiple updates
            return;
        } else if(selectedTable == null || this.selectedTable == null) {
            // Update the toolbar.
        } else if(this.selectedTable.equals(selectedTable)) {
            // Skip if the table is the same to avoid multiple updates
            return;
        }

        this.selectedTable = selectedTable;

        setBorder(toolbarBorder);

        if(null == selectedTable)
        {
            // disable the toolbar.
            toggleTableToolBar(selectedTable);
            return;
        }

        updateToolbarIncrementDecrementValues();

//        this.overlayLog.setSelected(selectedTable.getOverlayLog());
        this.enable3d.setEnabled(selectedTable.getType() == Settings.TABLE_3D);

        setScales(selectedTable.getScales());

        if(null == selectedTable.getCurrentScale())
        {
            this.scaleSelection.setSelectedItem("Default");
        } else {
            this.scaleSelection.setSelectedItem(selectedTable.getCurrentScale().getName());
        }

        toggleTableToolBar(selectedTable);
    }

    private void updateToolbarIncrementDecrementValues() {
        if(null == selectedTable) {
            return;
        }

        double fineIncrement = 0;
        double coarseIncrement = 0;

        try {
            // enable the toolbar.
            fineIncrement = Math.abs(selectedTable.getCurrentScale().getFineIncrement());
            coarseIncrement = Math.abs(selectedTable.getCurrentScale().getCoarseIncrement());
        } catch (Exception ex) {
            // scaling units haven't been added yet -- no problem
        }

        incrementByFine.setValue(fineIncrement);
        incrementByCoarse.setValue(coarseIncrement);
    }

    private void toggleTableToolBar(Table currentTable) {
        String newTitle = "";
        boolean enabled;

        if(null == currentTable) {
            enabled = false;
        } else {
            if(currentTable instanceof Table1D) {
                Table1D cur1DTable = (Table1D)currentTable;
                if(cur1DTable.isAxis()) {
                    newTitle = cur1DTable.getAxisParent().getName() + " ("+ cur1DTable.getName() +")";
                } else {
                    newTitle = currentTable.getName();
                }
            } else {
                newTitle = currentTable.getName();
            }

            enabled = true;
        }

        toolbarBorder.setTitle(newTitle);

        incrementFine.setEnabled(enabled);
        decrementFine.setEnabled(enabled);
        incrementCoarse.setEnabled(enabled);
        decrementCoarse.setEnabled(enabled);

        setValue.setEnabled(enabled);
        multiply.setEnabled(enabled);

        incrementByFine.setEnabled(enabled);
        incrementByCoarse.setEnabled(enabled);
        setValueText.setEnabled(enabled);

        scaleSelection.setEnabled(enabled);

        liveDataValue.setEnabled(enabled);

        colorCells.setEnabled(enabled);
        refreshCompare.setEnabled(enabled);

        //Only enable the 3d button if table includes 3d data
        if (null != currentTable && currentTable.getType() == Settings.TABLE_3D && enabled) {
            enable3d.setEnabled(true);
        }
        else{
            enable3d.setEnabled(false);
        }

        if(null != currentTable && null != currentTable.getCompareTable() && enabled) {
            refreshCompare.setEnabled(true);
        } else {
            refreshCompare.setEnabled(false);
        }

        if (null != currentTable && currentTable.isLiveDataSupported() && enabled) {
//            overlayLog.setEnabled(true);
//            clearOverlay.setEnabled(true);
        }
        else{
//            overlayLog.setEnabled(false);
//            clearOverlay.setEnabled(false);
        }

        if(null != currentTable && currentTable.isStaticDataTable()) {
            if(enabled) {
                scaleSelection.setEnabled(true);
            } else {
                scaleSelection.setEnabled(false);
            }

            // Disable everything that does not apply to static value tables.
            colorCells.setEnabled(false);
            refreshCompare.setEnabled(false);

            incrementFine.setEnabled(false);
            decrementFine.setEnabled(false);
            incrementCoarse.setEnabled(false);
            decrementCoarse.setEnabled(false);
            incrementByFine.setEnabled(false);
            incrementByCoarse.setEnabled(false);
            setValue.setEnabled(false);
            setValueText.setEnabled(false);
            multiply.setEnabled(false);
            enable3d.setEnabled(false);
        }


        repaint();
    }

    public void setScales(Vector<Scale> scales) {

        // remove item listener to avoid null pointer exception when populating
        scaleSelection.removeItemListener(this);
        scaleSelection.removeAllItems();

        for (Scale scale : scales) {
            scaleSelection.addItem(scale.getName());
        }

        // and put it back
        scaleSelection.addItemListener(this);
    }

    @Override
    public void mouseClicked(MouseEvent e) {
        Table curTable = getSelectedTable();
        if(null == curTable)
        {
            return;
        }

        if (e.getSource() == incrementCoarse) {
            incrementCoarse(curTable);
        } else if (e.getSource() == decrementCoarse) {
            decrementCoarse(curTable);
        } else if (e.getSource() == enable3d) {
            enable3d(curTable);
        } else if (e.getSource() == incrementFine) {
            incrementFine(curTable);
        } else if (e.getSource() == decrementFine) {
            decrementFine(curTable);
        } else if (e.getSource() == multiply) {
            multiply(curTable);
        } else if (e.getSource() == setValue) {
            setValue(curTable);
        } else if (e.getSource() == colorCells) {
            colorCells(curTable);
        } else if (e.getSource() == refreshCompare) {
            refreshCompare(curTable);
        }
    }

    public void setValue(Table currentTable) {
        currentTable.setRealValue(setValueText.getText());
    }

    public void multiply() {
        Table curTable = getSelectedTable();
        if(null == curTable) {
            return;
        }
        multiply(curTable);
    }

    public void multiply(Table currentTable) {
        try{
            currentTable.multiply(Double.parseDouble(setValueText.getText()));
        }catch(NumberFormatException nex) {
            // Do Nothing.  setValueText is null or not a valid double.
        }
    }

    public void incrementFine() {
        Table curTable = getSelectedTable();
        if(null == curTable) {
            return;
        }
        incrementFine(curTable);
    }

    public void incrementFine(Table currentTable) {
        currentTable.increment(Double.parseDouble(String.valueOf(incrementByFine.getValue())));
    }

    public void decrementFine() {
        Table curTable = getSelectedTable();
        if(null == curTable) {
            return;
        }
        decrementFine(curTable);
    }

    public void decrementFine(Table currentTable) {
        currentTable.increment(0 - Double.parseDouble(String.valueOf(incrementByFine.getValue())));
    }

    public void incrementCoarse() {
        Table curTable = getSelectedTable();
        if(null == curTable) {
            return;
        }
        incrementCoarse(curTable);
    }

    public void incrementCoarse(Table currentTable) {
        currentTable.increment(Double.parseDouble(String.valueOf(incrementByCoarse.getValue())));
    }

    public void decrementCoarse() {
        Table curTable = getSelectedTable();
        if(null == curTable) {
            return;
        }
        decrementCoarse(curTable);
    }

    public void decrementCoarse(Table currentTable) {
        currentTable.increment(0 - Double.parseDouble(String.valueOf(incrementByCoarse.getValue())));
    }

    /**
     * Method launches a 3d Frame.
     */
    public void enable3d(Table currentTable) {
        int rowCount = 0;
        int valueCount = 0;

        //Pull data into format 3d graph understands
        Vector<float[]> graphValues = new Vector<float[]>();
        graphValues.clear();

        if (currentTable.getType() == Settings.TABLE_3D) {
            Table3D table3d = (Table3D) currentTable;
            DataCell[][] tableData = table3d.get3dData();
            valueCount = tableData.length;
            DataCell[] dataRow = tableData[0];
            rowCount = dataRow.length;

            for (int j = (rowCount - 1); j >= 0; j--) {
                float[] rowValues = new float[valueCount];
                for (int i = 0; i < valueCount; i++) {
                    DataCell theCell = tableData[i][j];
                    rowValues[i] = (float) theCell.getRealValue();
                }
                graphValues.add(rowValues);
            }

            Table1D xAxisTable1D = table3d.getXAxis();
            Table1D yAxisTable1D = table3d.getYAxis();

            //Gather x axis values
            DataCell[] dataCells = xAxisTable1D.getData();
            int length = dataCells.length;
            double[] xValues = new double[length];

            for (int i = 0; i < length; i++) {
                xValues[i] = dataCells[i].getRealValue();
            }

            //Gather y/z axis values
            dataCells = yAxisTable1D.getData();
            length = dataCells.length;
            double[] yValues = new double[length];

            for (int i = 0; i < length; i++) {
                double theValue = dataCells[i].getRealValue();
                BigDecimal finalRoundedValue = new BigDecimal(theValue).setScale(2, BigDecimal.ROUND_HALF_UP);
                yValues[i] = finalRoundedValue.doubleValue();
            }

            //Define Labels for graph
            String xLabel = table3d.getXAxis().getName();
            String zLabel = table3d.getYAxis().getName();
            String yLabel = table3d.getCategory();

            //TODO Figure out mix between heavy weight and lightweight components
            //Below is initial work on making graph3d a JInternal Frame
            /*
               Graph3dJPanel graph3dJPanel = new Graph3dJPanel(graphValues, testX, testZ,xLabel, yLabel, zLabel);
               graph3dJPanel.addModifiedDataListener(this);
               JInternalFrame graphFrame = new JInternalFrame();
               graphFrame.add(graph3dJPanel);
               graphFrame.setSize(200, 200);

               graphFrame.setFrameIcon(null);
               graphFrame.setBorder(BorderFactory.createBevelBorder(0));
               graphFrame.setVisible(true);
               graphFrame.setDefaultCloseOperation(JInternalFrame.HIDE_ON_CLOSE);
               ECUEditorManager.getECUEditor().rightPanel.add(graphFrame);
             */

            // TODO: do we want to get the max/min allowed or the max/min current?
            table3d.calcCellRanges();
            double maxV = table3d.getMaxReal();
            double minV = table3d.getMinReal();
            //TODO Remove this when above is working
            //***********
            /*minV = 0.0;
            maxV = 13.01;*/
            LOGGER.debug("Scale: " + maxV + "," + minV);
            //***********

            throw new UnsupportedOperationException();
//            Graph3dFrameManager.openGraph3dFrame(graphValues, minV, maxV, xValues, yValues, xLabel, yLabel, zLabel, currentTable.getName());
//            GraphData.addGraphDataListener(this);
        }
    }

    public void colorCells(Table currentTable) {
        currentTable.colorCells();
    }

    public void refreshCompare(Table currentTable) {
        currentTable.populateCompareValues(currentTable.getCompareTable());
    }

    public void setCoarseValue(double input) {
        incrementByCoarse.setText(String.valueOf(input));
        try {
            incrementByCoarse.commitEdit();
        } catch (ParseException ex) {
        }
    }

    public void setFineValue(double input) {
        incrementByFine.setText(String.valueOf(input));
        try {
            incrementByFine.commitEdit();
        } catch (ParseException ex) {
        }
    }

    public void focusSetValue(char input) {
        setValueText.requestFocus();
        setValueText.setText(String.valueOf(input));
    }

    public void setInputMap(InputMap im) {
        incrementFine.setInputMap(WHEN_FOCUSED, im);
        decrementFine.setInputMap(WHEN_FOCUSED, im);
        incrementCoarse.setInputMap(WHEN_FOCUSED, im);
        decrementCoarse.setInputMap(WHEN_FOCUSED, im);
        setValue.setInputMap(WHEN_FOCUSED, im);
    }

    @Override
    public void mousePressed(MouseEvent e) {
    }

    @Override
    public void mouseReleased(MouseEvent e) {
    }

    @Override
    public void mouseEntered(MouseEvent e) {
    }

    @Override
    public void mouseExited(MouseEvent e) {
    }

    @Override
    public void itemStateChanged(ItemEvent e) {
        Table curTable = getSelectedTable();
        if(null == curTable) {
            return;
        }

        if (e.getSource() == scaleSelection) {
            // scale changed
            try {

                curTable.setScaleByName((String)scaleSelection.getSelectedItem());
                updateToolbarIncrementDecrementValues();
            } catch (NameNotFoundException e1) {
                e1.printStackTrace();
            }
//        } else if (e.getSource() == overlayLog) {
//            // enable/disable log overlay and live data display
//            curTable.setOverlayLog(overlayLog.isSelected());
        }
    }


    @Override
    public void actionPerformed(ActionEvent e) {
        Table curTable = getSelectedTable();
        if(null == curTable) {
            return;
        }

//        if (e.getSource() == clearOverlay) {
//            // clear log overlay
//            curTable.clearLiveDataTrace();
//        }
    }

    public void setLiveDataValue(String value) {
        liveDataValue.setText(value);
    }


    // ******************************************
    // Code for listening to graph3d data changes
    // ******************************************

    //@Override
    public void newGraphData(int x, int z, float value) {
        Table curTable = getSelectedTable();
        if(null == curTable) {
            return;
        }

        if(curTable.getType() == Settings.TABLE_3D) {
            Table3D table3d = (Table3D) curTable;
            table3d.selectCellAt(x, table3d.getSizeY() - z - 1);

            //Set the value
            table3d.setRealValue(String.valueOf(value));
        }
    }

    //@Override
    public void selectStateChange(int x, int z, boolean value) {
        Table curTable = getSelectedTable();
        if(null == curTable) {
            return;
        }

        if(curTable.getType() == Settings.TABLE_3D) {
            if (value) {
                Table3D table3d = (Table3D) curTable;
                table3d.selectCellAtWithoutClear(x, table3d.getSizeY() - z - 1);
            } else {
                Table3D table3d = (Table3D) curTable;
                table3d.deSelectCellAt(x, table3d.getSizeY() - z - 1);
            }
        }
    }

    public Table getSelectedTable() {
        return this.selectedTable;
    }
}