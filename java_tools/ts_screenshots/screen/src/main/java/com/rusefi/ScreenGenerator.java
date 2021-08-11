package com.rusefi;

import com.opensr5.ini.IniFileModel;
import com.rusefi.xml.*;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.awt.image.RasterFormatException;
import java.io.*;
import java.lang.reflect.InvocationTargetException;
import java.util.*;
import java.util.List;
import java.util.concurrent.atomic.AtomicReference;

public class ScreenGenerator {
    public static final String FILE_NAME = "output.xml";
    private static final String PNG = "png";
    private static ArrayList<AbstractButton> topLevelButtons = new ArrayList<>();

    private static final int MENU_CLICK_DELAY = 50;
    private static final int TOP_MENU_CLICK_DELAY = 200;
    private static final int WAITING_FOR_FRAME_PERIOD = 1000;


    private static final String DESTINATION = "images" + File.separator;

    static ContentModel contentModel = new ContentModel();
    static IniFileModel iniFileModel = new IniFileModel();

    static Map<String, com.opensr5.ini.DialogModel.Field> byCleanUiName = new TreeMap<>();

    public static void main(String[] args) throws Exception {
        if (args.length != 1) {
            System.out.println("One parameter expected: path to directory containing rusefi.ini file");
            System.exit(-1);
        }

        iniFileModel.readIniFile(".");

        for (Map.Entry<String, com.opensr5.ini.DialogModel.Field> a : iniFileModel.getAllFields().entrySet()) {
            String cleanUiName = cleanName(a.getValue().getUiName());
            byCleanUiName.put(cleanUiName, a.getValue());
        }

        if (byCleanUiName.isEmpty())
            throw new IllegalStateException("Something not right with input file. April 29 version is needed");


        System.out.println("mkdirs " + DESTINATION);
        new File(DESTINATION).mkdirs();

        System.out.println("Launching TunerStudioIntegration");
        Frame mainFrame = TunerStudioIntegration.findMainFrame();

        waitForMainFrame(mainFrame);

        System.out.println("Done discovering buttons, " + topLevelButtons.size());

        Thread.sleep(2 * WAITING_FOR_FRAME_PERIOD); // we have this sleep to avoid an artifact on first screenshot

        handleTopLevelButtons(mainFrame, topLevelButtons);

        XmlUtil.writeXml(contentModel, ContentModel.class, FILE_NAME);
    }

    private static void waitForMainFrame(Frame mainFrame) throws InterruptedException {
        while (topLevelButtons.isEmpty()) {
            UiUtils.visitComponents(mainFrame, "", (parent, component) -> {
                if (component instanceof AbstractButton) {
                    AbstractButton ab = (AbstractButton) component;
                    System.out.println("topLevelButton " + ab.getText());

                    if (TunerStudioIntegration.isTopLevelMenuButton(component)) {
                        topLevelButtons.add(ab);
                    }
                }
            });
            Thread.sleep(WAITING_FOR_FRAME_PERIOD);
        }
    }

    private static void handleTopLevelButtons(Frame frame, ArrayList<AbstractButton> topLevelButtons) throws Exception {
        for (AbstractButton topLevel : topLevelButtons) {
            handleTopLevelButton(frame, topLevel);
        }
    }

    private static void handleTopLevelButton(Frame frame, AbstractButton topLevel) throws Exception {
        SwingUtilities.invokeAndWait(topLevel::doClick);
        Thread.sleep(TOP_MENU_CLICK_DELAY);


        String imageName = "top_level_" + cleanName(topLevel.getText()) + "." + PNG;

        TopLevelMenuModel topLevelMenuModel = new TopLevelMenuModel(topLevel.getText(), imageName);
        ScreenGenerator.contentModel.getTopLevelMenus().add(topLevelMenuModel);

        ImageIO.write(
                UiUtils.getScreenShot(frame),
                PNG,
                new File(DESTINATION +  imageName));

        List<JMenuItem> menuItems = TunerStudioIntegration.findMenuItems(frame);

        for (JMenuItem menuItem : menuItems) {
            handleMenuItem(menuItem, topLevelMenuModel);
        }
    }

    private static void handleMenuItem(JMenuItem menuItem, TopLevelMenuModel topLevelMenuModel) throws InterruptedException, InvocationTargetException {
        SwingUtilities.invokeAndWait(menuItem::doClick);

        Thread.sleep(MENU_CLICK_DELAY);

        AtomicReference<JDialog> ref = new AtomicReference<>();
        SwingUtilities.invokeAndWait(() -> ref.set(TunerStudioIntegration.findDynamicDialog()));
        // let's give it time to appear on the screen
        Thread.sleep(MENU_CLICK_DELAY);
        JDialog dialog = ref.get();
        if (dialog == null) {
            System.out.println("Not found for " + menuItem);
            return;
        }

        String dialogTitle = dialog.getTitle();

        String imageName = "dialog_" + cleanName(dialogTitle) + "." + PNG;
        DialogModel dialogModel = new DialogModel(dialogTitle, imageName);
        topLevelMenuModel.getDialogs().add(dialogModel);

        SwingUtilities.invokeAndWait(() -> {
            try {
                findSlices(dialog, dialogModel);

                if (dialog == null) {
                    // this happens for example for disabled menu items
                    return;
                }

//                            Robot robot = new Robot();
//                            Rectangle captureRect = new Rectangle(Toolkit.getDefaultToolkit().getScreenSize());
//                            BufferedImage screenFullImage = robot.createScreenCapture(captureRect);
//                            ImageIO.write(screenFullImage, PNG, new File(DESTINATION + "full_" + d.getTitle() + ".png"));

                BufferedImage dialogScreenShot = UiUtils.getScreenShot(dialog);
                ImageIO.write(
                        dialogScreenShot,
                        PNG,
                        new File(DESTINATION + imageName));
                dialog.setVisible(false);
                dialog.dispose();
            } catch (Exception e) {
                throw new IllegalStateException(e);
            }
        });
    }

    private static void saveSlices(String dialogTitle, Map<Integer, String> yCoordinates, BufferedImage dialogScreenShot, DialogModel dialogModel) {
        System.out.println("Label Y coordinates: " + yCoordinates);
        yCoordinates.put(0, "top");
        yCoordinates.put(dialogScreenShot.getHeight(), "bottom");

        List<Integer> sorted = new ArrayList<>(yCoordinates.keySet());

        for (int i = 0; i < sorted.size() - 1; i++) {
            int fromY = sorted.get(i);
            int toY = sorted.get(i + 1);

            String sectionNameWithSpecialCharacters = yCoordinates.get(sorted.get(i));
            String sectionName = cleanName(sectionNameWithSpecialCharacters);

            BufferedImage slice;
            try {
                slice = dialogScreenShot.getSubimage(0, fromY, dialogScreenShot.getWidth(), toY - fromY);
            } catch (RasterFormatException e) {
                System.out.println("Dialog does not fit screen? " + sectionNameWithSpecialCharacters);
                continue;
            }

            if (slice == null) {
                System.out.println("Weird");
                continue;
            }
            String fileName = cleanName(dialogTitle) + "_slice_" + fromY + "_" + sectionName + "." + PNG;

            com.opensr5.ini.DialogModel.Field f = byCleanUiName.get(sectionName);
            if (f == null)
                continue;

            String fieldName = f.getKey();
            String tooltip = iniFileModel.tooltips.get(fieldName);

            dialogModel.fields.add(new FieldModel(sectionNameWithSpecialCharacters, fieldName, fileName, tooltip));

            File output = new File(DESTINATION + fileName);
            if (output == null) {
                System.out.println(sectionName + " in " + fileName + " was not a success");
                continue;
            }
            try {
                ImageIO.write(slice, PNG, output);
            } catch (Exception e) {
                System.out.println(sectionName + " in " + fileName + " was not a success?");
                continue;
            }
        }
    }

    private static void findSlices(JDialog dialog, DialogModel dialogModel) {
        UiUtils.visitComponents(dialog, "Dynamic dialog", new Callback() {
            @Override
            public void onComponent(Component parent, Component component) {
                if (component instanceof JPanel) {
                    JPanel panel = (JPanel) component;
                    handleBox(dialog.getTitle(), panel, dialogModel);
                }
            }
        });
    }

    private static void handleBox(String dialogTitle, JPanel panel, DialogModel dialogModel) {
        if (panel.getLayout() instanceof BoxLayout) {
            BoxLayout layout = (BoxLayout) panel.getLayout();
            if (layout.getAxis() == BoxLayout.X_AXIS)
                return;

            BufferedImage panelImage = UiUtils.getScreenShot(panel);

            Map<Integer, String> yCoordinates = new TreeMap<>();
            int relativeY = panel.getLocationOnScreen().y;

            UiUtils.visitComponents(panel, "Looking inside the box", new Callback() {
                @Override
                public void onComponent(Component parent, Component component) {
                    if (component instanceof JLabel) {
                        JLabel label = (JLabel) component;
                        if (!label.isVisible() || label.getSize().width == 0)
                            return;
                        String labelText = label.getText();
                        if (labelText.length() > 0) {
                            System.out.println("Looking at " + label);
                            try {
                                yCoordinates.put(label.getLocationOnScreen().y - relativeY, labelText);
                            } catch (IllegalComponentStateException e) {
                                System.out.printf("Did not go well for " + label);
                            }
                        }
                    }
                }
            });

            saveSlices(dialogTitle, yCoordinates, panelImage, dialogModel);
        }
    }

    private static String cleanName(String title) {
        title = title.replace(' ', '_');
        title = title.replace(")", "");
        title = title.replace("%", "");
        title = title.replace(">", "");
        title = title.replace("<", "");
        title = title.replace("?", "");
        title = title.replace("(", "");
        title = title.replace('/', '_');
        title = title.replace('\\', '_');
        title = title.replace("  ", " ");
        return title;
    }
}
