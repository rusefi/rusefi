/*
 * RomRaider Open-Source Tuning, Logging and Reflashing
 * Copyright (C) 2006-2014 RomRaider.com
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

package com.romraider.editor.ecu;

//import static com.romraider.Version.ECU_DEFS_URL;
import static com.romraider.Version.PRODUCT_NAME;
import static com.romraider.Version.VERSION;
import static javax.swing.JOptionPane.DEFAULT_OPTION;
import static javax.swing.JOptionPane.ERROR_MESSAGE;
import static javax.swing.JOptionPane.INFORMATION_MESSAGE;
import static javax.swing.JOptionPane.WARNING_MESSAGE;
import static javax.swing.JOptionPane.showMessageDialog;
import static javax.swing.JOptionPane.showOptionDialog;
import static javax.swing.ScrollPaneConstants.HORIZONTAL_SCROLLBAR_AS_NEEDED;
import static javax.swing.ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER;
import static javax.swing.ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS;
import static javax.swing.ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Cursor;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyVetoException;
import java.io.BufferedReader;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.IOException;
import java.util.Vector;

import javax.swing.ImageIcon;
import javax.swing.JCheckBox;
import javax.swing.JInternalFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTextArea;
import javax.swing.SwingWorker;
import javax.swing.tree.TreePath;

import org.w3c.dom.Document;
import org.xml.sax.InputSource;
import org.xml.sax.SAXParseException;

import com.romraider.net.BrowserControl;
import com.romraider.Settings;
import com.romraider.maps.Rom;
import com.romraider.net.URL;
import com.romraider.swing.AbstractFrame;
import com.romraider.swing.CustomToolbarLayout;
import com.romraider.swing.ECUEditorMenuBar;
import com.romraider.swing.ECUEditorToolBar;
import com.romraider.swing.JProgressPane;
import com.romraider.swing.MDIDesktopPane;
import com.romraider.swing.RomTree;
import com.romraider.swing.RomTreeRootNode;
import com.romraider.swing.TableFrame;
import com.romraider.swing.TableToolBar;
import com.romraider.util.SettingsManager;
import com.romraider.xml.DOMRomUnmarshaller;
import com.romraider.xml.RomNotFoundException;
import com.sun.org.apache.xerces.internal.parsers.DOMParser;

public class ECUEditor extends AbstractFrame {
    private static final long serialVersionUID = -7826850987392016292L;

    private final String titleText = PRODUCT_NAME + " v" + VERSION + " | ECU Editor";

    private final RomTreeRootNode imageRoot = new RomTreeRootNode("Open Images");
    private final RomTree imageList = new RomTree(imageRoot);
    public MDIDesktopPane rightPanel = new MDIDesktopPane();
    public JProgressPane statusPanel = new JProgressPane();
    private JSplitPane splitPane = new JSplitPane();
    private Rom lastSelectedRom = null;
    private ECUEditorToolBar toolBar;
    private ECUEditorMenuBar menuBar;
    private TableToolBar tableToolBar;
    private final JPanel toolBarPanel = new JPanel();
    private OpenImageWorker openImageWorker;
    private CloseImageWorker closeImageWorker;
    private SetUserLevelWorker setUserLevelWorker;
    private LaunchLoggerWorker launchLoggerWorker;
    private final ImageIcon editorIcon = new ImageIcon(getClass().getResource("/graphics/romraider-ico.gif"), "RomRaider ECU Editor");

    public ECUEditor() {
        Settings settings = SettingsManager.getSettings();
        if (!settings.getRecentVersion().equalsIgnoreCase(VERSION)) {
            showReleaseNotes();
        }

        setSize(settings.getWindowSize());
        setLocation(settings.getWindowLocation());
        if (settings.isWindowMaximized()) {
            setExtendedState(MAXIMIZED_BOTH);
        }

        JScrollPane rightScrollPane = new JScrollPane(rightPanel,
                VERTICAL_SCROLLBAR_AS_NEEDED, HORIZONTAL_SCROLLBAR_AS_NEEDED);
        JScrollPane leftScrollPane = new JScrollPane(imageList,
                VERTICAL_SCROLLBAR_AS_NEEDED, HORIZONTAL_SCROLLBAR_AS_NEEDED);

        splitPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, leftScrollPane, rightScrollPane);
        splitPane.setDividerSize(3);
        splitPane.setDividerLocation(settings.getSplitPaneLocation());
        splitPane.addPropertyChangeListener(this);
        splitPane.setContinuousLayout(true);
        getContentPane().add(splitPane);

        rightPanel.setBackground(Color.BLACK);
        imageList.setScrollsOnExpand(true);

        this.add(statusPanel, BorderLayout.SOUTH);

        //set remaining window properties
        setIconImage(editorIcon.getImage());

        setDefaultCloseOperation(EXIT_ON_CLOSE);
        addWindowListener(this);
        setTitle(titleText);
        setVisible(true);
        toFront();
    }

    public void initializeEditorUI() {
        //create menubar
        menuBar = new ECUEditorMenuBar();
        this.setJMenuBar(menuBar);

        // create toolbars
        toolBar = new ECUEditorToolBar("Editor Tools");

        tableToolBar = new TableToolBar();
        tableToolBar.updateTableToolBar();

        CustomToolbarLayout toolBarLayout = new CustomToolbarLayout(FlowLayout.LEFT, 0, 0);

        toolBarPanel.setLayout(toolBarLayout);
        toolBarPanel.add(toolBar);
        toolBarPanel.add(tableToolBar);
        toolBarPanel.setVisible(true);

        this.add(toolBarPanel, BorderLayout.NORTH);
        validate();
    }

    public void checkDefinitions() {
        if (SettingsManager.getSettings().getEcuDefinitionFiles().size() <= 0) {
            // no ECU definitions configured - let user choose to get latest or configure later
            Object[] options = {"Yes", "No"};
            int answer = showOptionDialog(null,
                    "ECU definitions not configured.\nGo online to download the latest definition files?",
                    "Editor Configuration",
                    DEFAULT_OPTION,
                    WARNING_MESSAGE,
                    null,
                    options,
                    options[0]);
            if (answer == 0) {
//                BrowserControl.displayURL(ECU_DEFS_URL);
            } else {
                showMessageDialog(this,
                        "ECU definition files need to be configured before ROM images can be opened.\nMenu: ECU Definitions > ECU Definition Manager...",
                        "Editor Configuration",
                        INFORMATION_MESSAGE);
            }
        }
    }

    private void showReleaseNotes() {
        try {
            BufferedReader br = new BufferedReader(new FileReader(SettingsManager.getSettings().getReleaseNotes()));
            try {
                // new version being used, display release notes
                JTextArea releaseNotes = new JTextArea();
                releaseNotes.setEditable(false);
                releaseNotes.setWrapStyleWord(true);
                releaseNotes.setLineWrap(true);
                releaseNotes.setFont(new Font("Tahoma", Font.PLAIN, 12));

                StringBuffer sb = new StringBuffer();
                while (br.ready()) {
                    sb.append(br.readLine()).append(Settings.NEW_LINE);
                }
                releaseNotes.setText(sb.toString());
                releaseNotes.setCaretPosition(0);

                JScrollPane scroller = new JScrollPane(releaseNotes, VERTICAL_SCROLLBAR_ALWAYS, HORIZONTAL_SCROLLBAR_NEVER);
                scroller.setPreferredSize(new Dimension(600, 500));

                showMessageDialog(this, scroller,
                        PRODUCT_NAME + VERSION + " Release Notes", INFORMATION_MESSAGE);
            } finally {
                br.close();
            }
        } catch (Exception e) {
            /* Ignore */
        }
    }

    public void handleExit() {
        Settings settings = SettingsManager.getSettings();
        settings.setSplitPaneLocation(splitPane.getDividerLocation());
        settings.setWindowMaximized(getExtendedState() == MAXIMIZED_BOTH);
        settings.setWindowSize(getSize());
        settings.setWindowLocation(getLocation());

        // Save when exit to save file settings.
        SettingsManager.save(settings, statusPanel);
        statusPanel.update("Ready...", 0);
        repaint();
    }

    @Override
    public void windowClosing(WindowEvent e) {
        handleExit();
    }

    @Override
    public void windowOpened(WindowEvent e) {
    }

    @Override
    public void windowClosed(WindowEvent e) {
    }

    @Override
    public void windowIconified(WindowEvent e) {
    }

    @Override
    public void windowDeiconified(WindowEvent e) {
    }

    @Override
    public void windowActivated(WindowEvent e) {
    }

    @Override
    public void windowDeactivated(WindowEvent e) {
    }

    public String getVersion() {
        return VERSION;
    }

    public void addRom(Rom input) {
        Settings settings = SettingsManager.getSettings();
        input.refreshDisplayedTables();

        // add to ecu image list pane
        getImageRoot().add(input);

        getImageList().setVisible(true);
        getImageList().expandPath(new TreePath(getImageRoot()));

        getImageList().expandPath(new TreePath(input.getPath()));

        if(!settings.isOpenExpanded()) {
            imageList.collapsePath(new TreePath(input.getPath()));
        }

        getImageList().setRootVisible(false);
        getImageList().repaint();

        // Only set if no other rom has been selected.
        if(null == getLastSelectedRom()) {
            setLastSelectedRom(input);
        }

        if (input.getRomID().isObsolete() && settings.isObsoleteWarning()) {
            JPanel infoPanel = new JPanel();
            infoPanel.setLayout(new GridLayout(3, 1));
            infoPanel.add(new JLabel("A newer version of this ECU revision exists. " +
                    "Please visit the following link to download the latest revision:"));
//            infoPanel.add(new URL(settings.getRomRevisionURL()));

            JCheckBox check = new JCheckBox("Always display this message", true);
            check.setHorizontalAlignment(JCheckBox.RIGHT);

            check.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    SettingsManager.getSettings().setObsoleteWarning(((JCheckBox) e.getSource()).isSelected());
                }
            });

            infoPanel.add(check);
            showMessageDialog(this, infoPanel, "ECU Revision is Obsolete", INFORMATION_MESSAGE);
        }
    }

    public void displayTable(TableFrame frame) {
        try {
            // check if frame has been added.
            for(JInternalFrame curFrame : getRightPanel().getAllFrames()) {
                if(curFrame.equals(frame)) {
                    // table is already open.
                    if(1 == SettingsManager.getSettings().getTableClickBehavior()) { // open/focus frame
                        // table is already open, so set focus on the frame.
                        boolean selected = true;
                        frame.toFront();
                        try {
                            frame.setSelected(true);
                        } catch (PropertyVetoException e) {
                            frame.toBack();
                            selected = false;
                        }
                        if(selected) {
                            frame.requestFocusInWindow();
                        }
                    } else { // default to open/close frame
                        // table is already open, so close the frame.
                        rightPanel.remove(frame);
                        frame.setVisible(false);
                        try {
                            frame.setClosed(true);
                        } catch (PropertyVetoException e) {
                            ; // Do nothing.
                        }
                        frame.dispose();
                    }
                    frame.pack();
                    rightPanel.repaint();
                    return;
                }
            }

            // frame not added.  Draw table and add the frame.
            frame.getTable().drawTable();
            rightPanel.add(frame);
        } catch (IllegalArgumentException ex) {
            ;// Do nothing.
        }
        frame.pack();
        rightPanel.repaint();
    }

    public void removeDisplayTable(TableFrame frame) {
        frame.setVisible(false);
        this.getTableToolBar().updateTableToolBar();
        rightPanel.remove(frame);
        rightPanel.validate();
        refreshUI();
    }

    public void closeImage() {
        setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
        closeImageWorker = new CloseImageWorker(getLastSelectedRom());
        closeImageWorker.addPropertyChangeListener(getStatusPanel());
        closeImageWorker.execute();
    }

    public void closeAllImages() {
        while (imageRoot.getChildCount() > 0) {
            closeImage();
        }
    }

    public Rom getLastSelectedRom() {
        return lastSelectedRom;
    }

    public String getLastSelectedRomFileName() {
        Rom lastSelRom = getLastSelectedRom();
        return lastSelRom == null ? "" : lastSelRom.getFileName() + " ";
    }

    public void setLastSelectedRom(Rom lastSelectedRom) {
        this.lastSelectedRom = lastSelectedRom;
        if (lastSelectedRom == null) {
            setTitle(titleText);
        } else {
            setTitle(titleText + " - " + lastSelectedRom.getFileName());
        }
    }

    public ECUEditorToolBar getToolBar() {
        return toolBar;
    }

    public void setToolBar(ECUEditorToolBar toolBar) {
        this.toolBar = toolBar;
    }

    public ECUEditorMenuBar getEditorMenuBar() {
        return menuBar;
    }

    public TableToolBar getTableToolBar() {
        return tableToolBar;
    }

    public void redrawVisableTables(Settings settings) {

    }

    public void setUserLevel(int userLevel) {
        setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
        SettingsManager.getSettings().setUserLevel(userLevel);
        setUserLevelWorker = new SetUserLevelWorker();
        setUserLevelWorker.addPropertyChangeListener(getStatusPanel());
        setUserLevelWorker.execute();
    }

    public Vector<Rom> getImages() {
        Vector<Rom> images = new Vector<Rom>();
        for (int i = 0; i < imageRoot.getChildCount(); i++) {
            if(imageRoot.getChildAt(i) instanceof Rom) {
                Rom rom = (Rom) imageRoot.getChildAt(i);
                if(null != rom) {
                    images.add(rom);
                }
            }
        }
        return images;
    }

    @Override
    public void propertyChange(PropertyChangeEvent evt) {
        imageList.updateUI();
        imageList.repaint();
        rightPanel.updateUI();
        rightPanel.repaint();
    }

    public void refreshUI()
    {
        getToolBar().updateButtons();
        getEditorMenuBar().updateMenu();
        getTableToolBar().updateTableToolBar();
        imageList.updateUI();
        imageList.repaint();
        rightPanel.updateUI();
        rightPanel.repaint();
    }

    public void refreshTableCompareMenus() {
        for (int i = 0; i < imageRoot.getChildCount(); i++) {
            if(imageRoot.getChildAt(i) instanceof Rom) {
                Rom rom = (Rom) imageRoot.getChildAt(i);
                if(null != rom) {
                    rom.refreshTableCompareMenus();
                }
            }
        }
    }

    public void openImage(File inputFile) throws Exception {
        setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
        openImageWorker = new OpenImageWorker(inputFile);
        openImageWorker.addPropertyChangeListener(getStatusPanel());
        openImageWorker.execute();
    }

    public void openImages(File[] inputFiles) throws Exception {
        if(inputFiles.length < 1) {
            showMessageDialog(this, "Image Not Found", "Error Loading Image(s)", ERROR_MESSAGE);
            return;
        }
        for(int j = 0; j < inputFiles.length; j++) {
            openImage(inputFiles[j]);
        }
    }

    public byte[] readFile(File inputFile) throws IOException {
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        FileInputStream fis = new FileInputStream(inputFile);
        try {
            byte[] buf = new byte[8192];
            int bytesRead;
            while ((bytesRead = fis.read(buf)) != -1) {
                baos.write(buf, 0, bytesRead);
            }
        } finally {
            fis.close();
        }
        return baos.toByteArray();
    }

    public void launchLogger() {
        setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
//        launchLoggerWorker = new LaunchLoggerWorker();
//        launchLoggerWorker.addPropertyChangeListener(getStatusPanel());
//        launchLoggerWorker.execute();
    }

    public RomTreeRootNode getImageRoot() {
        return imageRoot;
    }

    public RomTree getImageList() {
        return imageList;
    }

    public JProgressPane getStatusPanel() {
        return this.statusPanel;
    }

    public MDIDesktopPane getRightPanel() {
        return this.rightPanel;
    }
}

class LaunchLoggerWorker extends SwingWorker<Void, Void> {
    public LaunchLoggerWorker() {
    }

    @Override
    protected Void doInBackground() throws Exception {
        ECUEditor editor = ECUEditorManager.getECUEditor();
        editor.getStatusPanel().setStatus("Launching Logger...");
        setProgress(10);
//        EcuLogger.startLogger(javax.swing.WindowConstants.DISPOSE_ON_CLOSE, editor);
        return null;
    }

    public void propertyChange(PropertyChangeEvent evnt)
    {
        SwingWorker source = (SwingWorker) evnt.getSource();
        if (null != source && "state".equals( evnt.getPropertyName() )
                && (source.isDone() || source.isCancelled() ) )
        {
            source.removePropertyChangeListener(ECUEditorManager.getECUEditor().getStatusPanel());
        }
    }

    @Override
    public void done() {
        ECUEditor editor = ECUEditorManager.getECUEditor();
        editor.getStatusPanel().setStatus("Ready...");
        setProgress(0);
        editor.setCursor(null);
        editor.refreshUI();
    }
}

class SetUserLevelWorker extends SwingWorker<Void, Void> {
    public SetUserLevelWorker() {
    }

    @Override
    protected Void doInBackground() throws Exception {
        for(Rom rom : ECUEditorManager.getECUEditor().getImages()) {
            rom.refreshDisplayedTables();
        }
        return null;
    }

    public void propertyChange(PropertyChangeEvent evnt)
    {
        SwingWorker source = (SwingWorker) evnt.getSource();
        if (null != source && "state".equals( evnt.getPropertyName() )
                && (source.isDone() || source.isCancelled() ) )
        {
            source.removePropertyChangeListener(ECUEditorManager.getECUEditor().getStatusPanel());
        }
    }

    @Override
    public void done() {
        ECUEditor editor = ECUEditorManager.getECUEditor();
        editor.getStatusPanel().setStatus("Ready...");
        setProgress(0);
        editor.setCursor(null);
        editor.refreshUI();
    }
}

class CloseImageWorker extends SwingWorker<Void, Void> {
    Rom rom;

    public CloseImageWorker(Rom romToRemove) {
        this.rom = romToRemove;
    }

    @Override
    protected Void doInBackground() throws Exception {
        ECUEditor editor = ECUEditorManager.getECUEditor();
        RomTreeRootNode imageRoot = editor.getImageRoot();

        rom.clearData();
        rom.removeFromParent();
        rom = null;

        if (imageRoot.getChildCount() > 0) {
            editor.setLastSelectedRom((Rom) imageRoot.getChildAt(0));
        } else {
            // no other images open
            editor.setLastSelectedRom(null);
        }

        editor.refreshTableCompareMenus();

        return null;
    }

    @Override
    public void done() {
        ECUEditor editor = ECUEditorManager.getECUEditor();
        editor.getStatusPanel().setStatus("Ready...");
        setProgress(0);
        editor.setCursor(null);
        editor.refreshUI();
        System.gc();
    }
}

class OpenImageWorker extends SwingWorker<Void, Void> {
    private final File inputFile;

    public OpenImageWorker(File inputFile) {
        this.inputFile = inputFile;
    }

    @Override
    protected Void doInBackground() throws Exception {
        ECUEditor editor = ECUEditorManager.getECUEditor();
        Settings settings = SettingsManager.getSettings();

        DOMParser parser = new DOMParser();
        Document doc;
        FileInputStream fileStream;

        try {
            editor.getStatusPanel().setStatus("Parsing ECU definitions...");
            setProgress(0);

            byte[] input = editor.readFile(inputFile);

            editor.getStatusPanel().setStatus("Finding ECU definition...");
            setProgress(10);

            // parse ecu definition files until result found
            for (int i = 0; i < settings.getEcuDefinitionFiles().size(); i++) {
                fileStream = new FileInputStream(settings.getEcuDefinitionFiles().get(i));
                InputSource src = new InputSource(fileStream);

                parser.parse(src);
                doc = parser.getDocument();

                Rom rom;

                try {
                    rom = new DOMRomUnmarshaller().unmarshallXMLDefinition(doc.getDocumentElement(), input, editor.getStatusPanel());
                } catch (RomNotFoundException rex) {
                    // rom was not found in current file, skip to next
                    continue;
                } catch (Exception ex) {
                    ex.printStackTrace();
                    showMessageDialog(editor, "Error Loading.  Unknown Exception.", "Error Loading " + inputFile.getName(), ERROR_MESSAGE);
                    return null;
                } finally {
                    // Release mem after unmarshall.
                    parser.reset();
                    doc.removeChild(doc.getDocumentElement());
                    doc = null;
                    fileStream.close();
                    System.gc();
                }

                editor.getStatusPanel().setStatus("Populating tables...");
                setProgress(50);

                rom.setFullFileName(inputFile);
                rom.populateTables(input, editor.getStatusPanel());

                editor.getStatusPanel().setStatus("Finalizing...");
                setProgress(90);

                editor.addRom(rom);
                editor.refreshTableCompareMenus();

                editor.getStatusPanel().setStatus("Done loading image...");
                setProgress(100);
                return null;
            }

            // if code executes to this point, no ROM was found, report to user
            showMessageDialog(editor, "ECU Definition Not Found", "Error Loading " + inputFile.getName(), ERROR_MESSAGE);

        } catch (SAXParseException spe) {
            // catch general parsing exception - enough people don't unzip the defs that a better error message is in order
            showMessageDialog(editor, "Unable to read XML definitions.  Please make sure the definition file is correct.  If it is in a ZIP archive, unzip the file and try again.", "Error Loading " + inputFile.getName(), ERROR_MESSAGE);

        } catch (StackOverflowError ex) {
            // handles looped inheritance, which will use up all available memory
            showMessageDialog(editor, "Looped \"base\" attribute in XML definitions.", "Error Loading " + inputFile.getName(), ERROR_MESSAGE);

        } catch (OutOfMemoryError ome) {
            // handles Java heap space issues when loading multiple Roms.
            showMessageDialog(editor, "Error loading Image. Out of memeory.", "Error Loading " + inputFile.getName(), ERROR_MESSAGE);

        }
        return null;
    }

    public void propertyChange(PropertyChangeEvent evnt)
    {
        SwingWorker source = (SwingWorker) evnt.getSource();
        if (null != source && "state".equals( evnt.getPropertyName() )
                && (source.isDone() || source.isCancelled() ) )
        {
            source.removePropertyChangeListener(ECUEditorManager.getECUEditor().getStatusPanel());
        }
    }

    @Override
    public void done() {
        ECUEditor editor = ECUEditorManager.getECUEditor();
        editor.getStatusPanel().setStatus("Ready...");
        setProgress(0);
        editor.setCursor(null);
        editor.refreshUI();
        System.gc();
    }
}
