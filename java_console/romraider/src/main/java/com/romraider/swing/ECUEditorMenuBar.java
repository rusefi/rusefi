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

package com.romraider.swing;

import com.romraider.Settings;
import com.romraider.editor.ecu.ECUEditor;
import com.romraider.editor.ecu.ECUEditorManager;
import com.romraider.maps.Rom;
import com.romraider.maps.Table;
import com.romraider.util.SettingsManager;
import com.rusefi.ConsoleUI;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileWriter;

import static com.romraider.Version.*;
import static javax.swing.JOptionPane.*;

//import static com.romraider.Version.ECU_DEFS_URL;

@SuppressWarnings("FieldCanBeLocal")
public class ECUEditorMenuBar extends JMenuBar implements ActionListener {

    private static final long serialVersionUID = -4777040428837855236L;
//    private final JMenu fileMenu = new JMenu("File");
//    private final JMenuItem openImage = new JMenuItem("Open Image...");
//    private final JMenuItem openImages = new JMenuItem("Open Image(s)...");
//    private final JMenuItem saveImage = new JMenuItem("Save Image As...");
//    private final JMenuItem saveAsRepository = new JMenuItem("Save Image As Repository...");
    private final JMenuItem refreshImage = new JMenuItem("Refresh Image");
//    private final JMenuItem closeImage = new JMenuItem("Close Image");
//    private final JMenuItem closeAll = new JMenuItem("Close All Images");
//    private final JMenuItem exit = new JMenuItem("Exit");

//    private final JMenu definitionMenu = new JMenu("ECU Definitions");
//    private final JMenuItem defManager = new JMenuItem("ECU Definition Manager...");
//    private final JMenuItem editDefinition = new JMenuItem("Edit ECU Definitions...");
//    private final JMenuItem updateDefinition = new JMenuItem("Get ECU Definitions...");

    private final JMenu editMenu = new JMenu("Edit");
    private final JMenuItem settings = new JMenuItem(PRODUCT_NAME + " Settings...");
    private final JMenuItem compareImages = new JMenuItem("Compare Images...");
    private final JMenu convertRom = new JMenu("Convert Image");
    private final JMenuItem convertIncrease = new JMenuItem("160KB --> 192KB...");
    private final JMenuItem convertDecrease = new JMenuItem("192KB --> 160KB...");
    private final ButtonGroup convertGroup = new ButtonGroup();

    private final JMenu viewMenu = new JMenu("View");
    private final JMenuItem romProperties = new JMenuItem("ECU Image Properties");
    private final ButtonGroup levelGroup = new ButtonGroup();
    private final JMenu levelMenu = new JMenu("User Level");
    private final JRadioButtonMenuItem level1 = new JRadioButtonMenuItem("1 Beginner");
    private final JRadioButtonMenuItem level2 = new JRadioButtonMenuItem("2 Intermediate");
    private final JRadioButtonMenuItem level3 = new JRadioButtonMenuItem("3 Advanced");
    private final JRadioButtonMenuItem level4 = new JRadioButtonMenuItem("4 Highest");
    private final JRadioButtonMenuItem level5 = new JRadioButtonMenuItem("5 Debug Mode");

//    private final JMenu loggerMenu = new JMenu("Logger");
//    private final JMenuItem openLogger = new JMenuItem("Launch Logger...");

//    private final JMenu ramTuneMenu = new JMenu("SSM");
    private final JMenuItem launchRamTuneTestApp = new JMenuItem("Launch Test App...");

    private final JMenu helpMenu = new JMenu("Help");
    private final JMenuItem about = new JMenuItem("About " + PRODUCT_NAME);

    public ECUEditorMenuBar() {
        // file menu items
//        add(fileMenu);
//        fileMenu.setMnemonic('F');
//
//        fileMenu.add(openImage);
//        openImage.addActionListener(this);
//        openImage.setMnemonic('O');

        //fileMenu.add(openImages);
        //openImages.addActionListener(this);
        //openImages.setMnemonic('I');

//        fileMenu.add(saveImage);
//        saveImage.addActionListener(this);
//        saveImage.setMnemonic('S');
//
//        fileMenu.add(saveAsRepository);
//        saveAsRepository.setMnemonic('D');
//        saveAsRepository.addActionListener(this);
//
//        fileMenu.add(refreshImage);
//        refreshImage.addActionListener(this);
//        refreshImage.setMnemonic('R');
//
//        fileMenu.add(new JSeparator());
//
//        fileMenu.add(closeImage);
//        closeImage.addActionListener(this);
//        closeImage.setMnemonic('C');

        //fileMenu.add(closeAll);
        //closeAll.addActionListener(this);
        //closeAll.setMnemonic('A');

//        fileMenu.add(new JSeparator());
//
//        fileMenu.add(exit);
//        exit.addActionListener(this);
//        exit.setMnemonic('X');

        // edit menu items
        add(editMenu);
        editMenu.setMnemonic('E');

        editMenu.add(settings);
        settings.addActionListener(this);
        settings.setMnemonic('S');

        editMenu.add(compareImages);
        compareImages.addActionListener(this);
        compareImages.setMnemonic('C');

        editMenu.add(convertRom);
        convertRom.setMnemonic('O');

        convertRom.add(convertIncrease);
        convertIncrease.addActionListener(this);
        convertIncrease.setMnemonic('I');

        convertRom.add(convertDecrease);
        convertDecrease.addActionListener(this);
        convertDecrease.setMnemonic('D');

        convertGroup.add(convertIncrease);
        convertGroup.add(convertDecrease);

//        // ecu def menu items
//        add(definitionMenu);
//        definitionMenu.setMnemonic('D');
//
//        definitionMenu.add(defManager);
//        defManager.addActionListener(this);
//        defManager.setMnemonic('D');
//
//        definitionMenu.add(updateDefinition);
//        updateDefinition.addActionListener(this);
//        updateDefinition.setMnemonic('U');

        //definitionMenu.add(editDefinition);
        //editDefinition.setMnemonic('E');
        //editDefinition.addActionListener(this);

        // view menu items
        add(viewMenu);
        viewMenu.setMnemonic('V');

        viewMenu.add(romProperties);
        romProperties.addActionListener(this);
        romProperties.setMnemonic('P');

        viewMenu.add(levelMenu);
        levelMenu.setMnemonic('U');

        levelMenu.add(level1);
        level1.addActionListener(this);
        level1.setMnemonic('1');

        levelMenu.add(level2);
        level2.addActionListener(this);
        level2.setMnemonic('2');

        levelMenu.add(level3);
        level3.addActionListener(this);
        level3.setMnemonic('3');

        levelMenu.add(level4);
        level4.addActionListener(this);
        level4.setMnemonic('4');

        levelMenu.add(level5);
        level5.addActionListener(this);
        level5.setMnemonic('5');

        levelGroup.add(level1);
        levelGroup.add(level2);
        levelGroup.add(level3);
        levelGroup.add(level4);
        levelGroup.add(level5);

        // select correct userlevel button
        if (getSettings().getUserLevel() == 1) {
            level1.setSelected(true);
        } else if (getSettings().getUserLevel() == 2) {
            level2.setSelected(true);
        } else if (getSettings().getUserLevel() == 3) {
            level3.setSelected(true);
        } else if (getSettings().getUserLevel() == 4) {
            level4.setSelected(true);
        } else if (getSettings().getUserLevel() == 5) {
            level5.setSelected(true);
        }

        // logger menu items
//        add(loggerMenu);
//        loggerMenu.setMnemonic('L');
//
//        loggerMenu.add(openLogger);
//        openLogger.addActionListener(this);
//        openLogger.setMnemonic('O');

        // ramtune menu items
//        add(ramTuneMenu);
//        ramTuneMenu.setMnemonic('R');
//
//        ramTuneMenu.add(launchRamTuneTestApp);
        launchRamTuneTestApp.addActionListener(this);
        launchRamTuneTestApp.setMnemonic('L');

        // help menu items
        add(helpMenu);
        helpMenu.setMnemonic('H');

        helpMenu.add(about);
        about.addActionListener(this);
        about.setMnemonic('A');

        updateMenu();
    }

    public void updateMenu() {
        String file = getLastSelectedRomFileName();
        if ("".equals(file)) {
//            saveImage.setEnabled(false);
//            saveAsRepository.setEnabled(false);
//            closeImage.setEnabled(false);
            //closeAll.setEnabled(false);
            romProperties.setEnabled(false);
//            saveImage.setText("Save As...");
//            saveAsRepository.setText("Save As Repository...");
            compareImages.setEnabled(false);
            convertRom.setEnabled(false);
        } else {
//            saveImage.setEnabled(true);
//            saveAsRepository.setEnabled(true);
//            closeImage.setEnabled(true);
            //closeAll.setEnabled(true);
            romProperties.setEnabled(true);
//            saveImage.setText("Save " + file + " As...");
//            saveAsRepository.setText("Save "+ file +" As Repository...");
            compareImages.setEnabled(true);
            convertRom.setEnabled(true);
        }
        refreshImage.setText("Refresh " + file);
    //    closeImage.setText("Close " + file);
        romProperties.setText(file + "Properties");

        int lastSelectedRomSize = 0;
        Rom lastSelectedRom = ECUEditorManager.getECUEditor().getLastSelectedRom();
        if(null != lastSelectedRom) {
            lastSelectedRomSize = lastSelectedRom.getRealFileSize();
        }

        if(Settings.SIXTEENBIT_SMALL_SIZE == lastSelectedRomSize) {
            convertIncrease.setEnabled(true);
            convertDecrease.setEnabled(false);
        } else if (Settings.SIXTEENBIT_LARGE_SIZE == lastSelectedRomSize) {
            convertIncrease.setEnabled(false);
            convertDecrease.setEnabled(true);
        } else {
            convertIncrease.setEnabled(false);
            convertDecrease.setEnabled(false);
        }

        //openImages.setEnabled(false);
//        editDefinition.setEnabled(false);
        revalidate();
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        ECUEditor parent = ECUEditorManager.getECUEditor();
//        if (e.getSource() == openImage) {
//            try {
//                openImageDialog();
//            } catch (Exception ex) {
//                showMessageDialog(parent,
//                        new DebugPanel(ex, getSettings().getSupportURL()), "Exception", ERROR_MESSAGE);
//            }
//
//        } else if (e.getSource() == openImages) {
//            try {
//                openImagesDialog();
//            } catch (Exception ex) {
//                showMessageDialog(parent,
//                        new DebugPanel(ex, getSettings().getSupportURL()), "Exception", ERROR_MESSAGE);
//            }
//
//        } else if (e.getSource() == saveImage) {
//            try {
//                this.saveImage();
//            } catch (Exception ex) {
//                showMessageDialog(parent,
//                        new DebugPanel(ex, getSettings().getSupportURL()), "Exception", ERROR_MESSAGE);
//            }
//        } else if (e.getSource() == saveAsRepository) {
//            try {
//                this.saveAsRepository();
//            } catch(Exception ex) {
//                showMessageDialog(parent,
//                        new DebugPanel(ex, getSettings().getSupportURL()), "Exception", ERROR_MESSAGE);
//            }
//        } else if (e.getSource() == closeImage) {
//            parent.closeImage();
//
//        } else if (e.getSource() == closeAll) {
//            parent.closeAllImages();
//
//        } else if (e.getSource() == exit) {
//            parent.handleExit();
//            System.exit(0);

        //} else
            if (e.getSource() == romProperties) {
            showMessageDialog(ConsoleUI.getFrame(), new RomPropertyPanel(parent.getLastSelectedRom()),
                    parent.getLastSelectedRom().getRomIDString() + " Properties", INFORMATION_MESSAGE);

        } else if (e.getSource() == settings) {
            SettingsForm form = new SettingsForm();
            form.setLocationRelativeTo(ConsoleUI.getFrame());
            form.setVisible(true);

        } else if (e.getSource() == compareImages){
            CompareImagesForm form = new CompareImagesForm(parent.getImages(), ConsoleUI.getFrame().getIconImage());
            form.setLocationRelativeTo(ConsoleUI.getFrame());
            form.setVisible(true);

        } else if (e.getSource() == level1) {
            parent.setUserLevel(1);

        } else if (e.getSource() == level2) {
            parent.setUserLevel(2);

        } else if (e.getSource() == level3) {
            parent.setUserLevel(3);

        } else if (e.getSource() == level4) {
            parent.setUserLevel(4);

        } else if (e.getSource() == level5) {
            parent.setUserLevel(5);

//        } else if (e.getSource() == openLogger) {
//            parent.launchLogger();
//        } else if (e.getSource() == updateDefinition) {
//            BrowserControl.displayURL(ECU_DEFS_URL);

//        } else if (e.getSource() == launchRamTuneTestApp) {
//            RamTuneTestApp.startTestApp(DISPOSE_ON_CLOSE);

        } else if (e.getSource() == about) {
            //TODO:  change this to use com.romraider.swing.menubar.action.AboutAction
            String message = PRODUCT_NAME + " - ECU Editor\n"
                    + "Version: " + VERSION + "\n"
                    + "Build #: " + BUILDNUMBER + "\n"
                    + SUPPORT_URL;
            String title = "About " + PRODUCT_NAME;
            showMessageDialog(ConsoleUI.getFrame(), message, title, INFORMATION_MESSAGE, ABOUT_ICON);
        }
    }

    public void saveImage() throws Exception {
        Rom lastSelectedRom = ECUEditorManager.getECUEditor().getLastSelectedRom();
        if (lastSelectedRom != null) {
            File selectedFile = getImageOutputFile();
            if(null != selectedFile){
                byte[] output = lastSelectedRom.saveFile();
                this.writeImage(output, selectedFile);
            }
        }
    }

    private File getImageOutputFile() throws Exception {
        ECUEditor parent = ECUEditorManager.getECUEditor();
        JFileChooser fc = new JFileChooser(SettingsManager.getSettings().getLastImageDir());
        fc.setFileFilter(new ECUImageFilter());
        if (fc.showSaveDialog(ConsoleUI.getFrame()) == JFileChooser.APPROVE_OPTION) {
            File selectedFile = fc.getSelectedFile();
            if (selectedFile.exists()) {
                int option = showConfirmDialog(ConsoleUI.getFrame(), selectedFile.getName() + " already exists! Overwrite?");

                // option: 0 = Cancel, 1 = No
                if (option == CANCEL_OPTION || option == 1) {
                    return null;
                }
            }
            return selectedFile;
        }
        return null;
    }

    private void writeImage(byte[] output, File selectedFile) throws Exception {
        ECUEditor parent = ECUEditorManager.getECUEditor();
        FileOutputStream fos = new FileOutputStream(selectedFile);
        try {
            fos.write(output);
        } finally {
            fos.close();
        }
        parent.getLastSelectedRom().setFullFileName(selectedFile.getAbsoluteFile().getName());
        parent.setLastSelectedRom(parent.getLastSelectedRom());
        SettingsManager.getSettings().setLastImageDir(selectedFile.getParentFile());
    }

    private File getRepositoryOutputDir() {
        JFileChooser fc = new JFileChooser();
        fc.setCurrentDirectory(getSettings().getLastRepositoryDir());
        fc.setDialogTitle("Select Repository Directory");
        fc.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);

        // disable the "All files" option
        fc.setAcceptAllFileFilterUsed(false);

        if (fc.showSaveDialog(ConsoleUI.getFrame()) == JFileChooser.APPROVE_OPTION) {
            File selectedDir = fc.getSelectedFile();
            if (selectedDir.exists()) {
                int option = showConfirmDialog(ConsoleUI.getFrame(), selectedDir.getName() + " already exists! Overwrite?");

                // option: 0 = Cancel, 1 = No
                if (option == CANCEL_OPTION || option == 1) {
                    return null;
                }
            }
            return selectedDir;
        }
        return null;
    }

    private void saveAsRepository() throws Exception {
        File selectedDir = getRepositoryOutputDir();
        String separator = System.getProperty("file.separator");

        if(null != selectedDir) {
            for(TableTreeNode treeNode : ECUEditorManager.getECUEditor().getLastSelectedRom().getTableNodes())
            {
                Table table = treeNode.getTable();
                String category = table.getCategory();
                String tableName = table.getName();
                String tableDirString = selectedDir.getAbsolutePath() + separator + category;
                File tableDir = new File(tableDirString.replace('/', '-'));
                tableDir.mkdirs();
                String tableFileString = tableDir.getAbsolutePath() + separator + tableName+".txt";
                File tableFile = new File(tableFileString.replace('/', '-'));

                if(tableFile.exists())
                {
                    tableFile.delete();
                }

                tableFile.createNewFile();
                StringBuffer tableData = table.getTableAsString();
                BufferedWriter out = new BufferedWriter(new FileWriter(tableFile));
                try {
                    out.write(tableData.toString());
                } finally {
                    try {
                        out.close();
                    } catch(Exception ex) {
                        ;// Do Nothing.
                    }
                }
            }
            getSettings().setLastRepositoryDir(selectedDir);
        }
    }

    private void increaseRomSize() throws Exception{
        Rom lastSelectedRom = ECUEditorManager.getECUEditor().getLastSelectedRom();
        if (lastSelectedRom != null) {
            File selectedFile = getImageOutputFile();
            if(null != selectedFile){
                if(lastSelectedRom.getRealFileSize() != Settings.SIXTEENBIT_SMALL_SIZE)
                {
                    showMessageDialog(ConsoleUI.getFrame(), "Error converting image.  Image size is invalid.");
                } else {
                    byte[] output = lastSelectedRom.saveFile();
                    byte[] incOutput = new byte[Settings.SIXTEENBIT_LARGE_SIZE];
                    System.arraycopy(output, 0, incOutput, 0, Settings.SIXTEENBIT_START_ADDRESS);
                    System.arraycopy(output, Settings.SIXTEENBIT_START_ADDRESS, incOutput, Settings.SIXTEENBIT_END_ADDRESS, Settings.SIXTEENBIT_SEGMENT_SIZE);
                    for(int i = Settings.SIXTEENBIT_START_ADDRESS; i < Settings.SIXTEENBIT_END_ADDRESS; i++) {
                        // Fill space.
                        incOutput[i] = Settings.SIXTEENBIT_SEGMENT_VALUE;
                    }
                    this.writeImage(incOutput, selectedFile);
                }
            }
        }
    }

    private void decreaseRomSize() throws Exception {
        Rom lastSelectedRom = ECUEditorManager.getECUEditor().getLastSelectedRom();
        if (lastSelectedRom != null) {
            File selectedFile = getImageOutputFile();
            if(null != selectedFile){
                if(lastSelectedRom.getRealFileSize() != Settings.SIXTEENBIT_LARGE_SIZE)
                {
                    showMessageDialog(ConsoleUI.getFrame(), "Error converting image.  Image size is invalid.");
                } else {
                    byte[] output =lastSelectedRom.saveFile();
                    byte[] decOutput = new byte[Settings.SIXTEENBIT_SMALL_SIZE];
                    System.arraycopy(output, 0, decOutput, 0, Settings.SIXTEENBIT_START_ADDRESS);
                    System.arraycopy(output, Settings.SIXTEENBIT_END_ADDRESS, decOutput, Settings.SIXTEENBIT_START_ADDRESS, Settings.SIXTEENBIT_SEGMENT_SIZE);
                    this.writeImage(decOutput, selectedFile);
                }
            }
        }
    }

    private String getLastSelectedRomFileName() {
        Rom lastSelectedRom = ECUEditorManager.getECUEditor().getLastSelectedRom();
        return lastSelectedRom == null ? "" : lastSelectedRom.getFileName() + " ";
    }

    private Settings getSettings() {
        return SettingsManager.getSettings();
    }
}