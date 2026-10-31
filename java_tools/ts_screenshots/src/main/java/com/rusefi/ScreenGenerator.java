package com.rusefi;

import com.opensr5.ini.IniFileModel;
import com.rusefi.ini.reader.IniFileReaderUtil;
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
import java.util.Base64;
import java.nio.charset.StandardCharsets;

import static com.rusefi.core.FileUtil.RUSEFI_SETTINGS_FOLDER;

public class ScreenGenerator {
    private static final String PNG = "png";
    private static final ArrayList<AbstractButton> topLevelButtons = new ArrayList<>();

    private static final int MENU_CLICK_DELAY = 50;
    private static final int TOP_MENU_CLICK_DELAY = 200;
    private static final int WAITING_FOR_FRAME_PERIOD = 1000;

    private static final String IMG_DESTINATION = RUSEFI_SETTINGS_FOLDER + "images" + File.separator;
    static final String XML_DUMP = IMG_DESTINATION + "ScreenGeneratorTool.xml";

    static ContentModel contentModel = new ContentModel();
    static IniFileModel iniFileModel = null;

    static Map<String, com.opensr5.ini.DialogModel.Field> byCleanUiName = new TreeMap<>();

    public static void main(String[] args) throws Exception {
        if (args.length != 1) {
            System.out.println("One parameter expected: path to directory containing rusefi_XXX.ini file");
            System.exit(-1);
        }
        String iniFileName = args[0];

        iniFileModel = IniFileReaderUtil.readIniFile(iniFileName);

        for (Map.Entry<String, com.opensr5.ini.DialogModel.Field> a : iniFileModel.getFieldsInUiOrder().entrySet()) {
            String cleanUiName = cleanName(a.getValue().getUiName());
            byCleanUiName.put(cleanUiName, a.getValue());
        }

        if (byCleanUiName.isEmpty())
            throw new IllegalStateException("Something not right with input file.");


        System.out.println("mkdirs " + IMG_DESTINATION);
        new File(IMG_DESTINATION).mkdirs();

        System.out.println("Launching TunerStudioIntegration");
        Frame mainFrame = TunerStudioIntegration.findMainFrame();

        waitForMainFrame(mainFrame);

        System.out.println("Done discovering buttons, " + topLevelButtons.size());

        Thread.sleep(2 * WAITING_FOR_FRAME_PERIOD); // we have this sleep to avoid an artifact on first screenshot

        handleTopLevelButtons(mainFrame, topLevelButtons);

        XmlUtil.writeXml(contentModel, ContentModel.class, XML_DUMP);
        System.out.println(ScreenGenerator.class.getSimpleName() + ": DONE!");
        System.exit(0);
    }

    private static void waitForMainFrame(Frame mainFrame) throws InterruptedException {
        while (topLevelButtons.isEmpty()) {
            UiUtils.visitComponents(mainFrame, "", (parent, component) -> {
                if (component instanceof AbstractButton) {
                    AbstractButton ab = (AbstractButton) component;
                    if (TunerStudioIntegration.DISCOVERY_MODE) {
                        // search for 'topLevelButton Setup'
                        // search for 'topLevelButton Sensors'
                        System.out.println("topLevelButton " + ab.getText() + "/class " + ab.getClass().getName());
                    }

                    if (TunerStudioIntegration.isTopLevelMenuButton(component)) {
                        topLevelButtons.add(ab);
                    }
                }
                return true;
            });
            Thread.sleep(WAITING_FOR_FRAME_PERIOD);
        }
        System.out.println("top level buttons discovered: " + topLevelButtons);
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

        new File(IMG_DESTINATION).mkdirs();
        String pathname = IMG_DESTINATION + imageName;
        System.out.println("ImageIO Write " + pathname);
        ImageIO.write(
            UiUtils.getScreenShot(frame),
            PNG,
            new File(pathname));

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
            System.out.println("handleMenuItem: dialog Not found for " + menuItem);
            return;
        }

        String dialogTitle = dialog.getTitle();
        String dialogName = iniFileModel.getDialogKeyByTitle(dialogTitle);
        com.opensr5.ini.DialogModel iniDialog = iniFileModel.getDialogs().get(dialogName);

        // we need to parse this HTML to b64 to correctly store on the
        String helpText = iniFileModel.getContextHelp().get(iniDialog.getTopicHelp()).toHtml();
        if (helpText != null) {
            helpText = Base64.getEncoder().encodeToString(helpText.getBytes(StandardCharsets.UTF_8));
        }

        String imageName = "dialog_" + cleanName(dialogTitle) + "." + PNG;
        DialogModel dialogModel = new DialogModel(dialogTitle, imageName, helpText);
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
                    new File(IMG_DESTINATION + imageName));
                dialog.setVisible(false);
                dialog.dispose();
            } catch (Exception e) {
                throw new IllegalStateException(e);
            }
        });
    }

    private static void saveSlices(String dialogTitle, Map<Integer, String> yCoordinates, Integer xCoordinate, Map<Integer, Integer> heights, BufferedImage dialogScreenShot, DialogModel dialogModel) {
        System.out.println("Label Y coordinates: " + yCoordinates);
        yCoordinates.put(0, "top");
        // not sure about this
        heights.put(0, 0);
        yCoordinates.put(dialogScreenShot.getHeight(), "bottom");
        heights.put(dialogScreenShot.getHeight(), 0);

        List<Integer> sorted = new ArrayList<>(yCoordinates.keySet());

        for (int i = 0; i < sorted.size() - 1; i++) {
            int fromY = sorted.get(i);

            String sectionNameWithSpecialCharacters = yCoordinates.get(sorted.get(i));
            String sectionName = cleanName(stripUnits(sectionNameWithSpecialCharacters));
            int sectionHeight = heights.get(sorted.get(i));

            BufferedImage slice;
            try {
                slice = dialogScreenShot.getSubimage(0, fromY, dialogScreenShot.getWidth(), sectionHeight);
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
            String tooltip = iniFileModel.getTooltips().get(fieldName);


            dialogModel.fields.add(new FieldModel(sectionNameWithSpecialCharacters, fieldName, fileName, tooltip, xCoordinate, fromY));

            File output = new File(IMG_DESTINATION + fileName);
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
            public boolean onComponent(Component parent, Component component) {
                if (component instanceof JPanel &&
                    ((JPanel) component).getLayout() instanceof BoxLayout &&
                    ((BoxLayout) ((JPanel) component).getLayout()).getAxis() != BoxLayout.X_AXIS) {
                    JPanel panel = (JPanel) component;
                    handleBox(dialog, panel, dialogModel);
                    return false;
                }
                return true;
            }
        });
    }

    private static void handleBox(JDialog dialog, JPanel panel, DialogModel dialogModel) {
        String dialogTitle = dialog.getTitle();

        BoxLayout layout = (BoxLayout) panel.getLayout();
        if (layout.getAxis() == BoxLayout.X_AXIS)
            return;

        BufferedImage panelImage = UiUtils.getScreenShot(panel);
        if (panelImage == null) {
            System.out.println("Skipping empty panel");
            return;
        }

        Map<Integer, String> yCoordinates = new TreeMap<>();
        Map<Integer, Integer> heights = new TreeMap<>();
        int relativeY = panel.getLocationOnScreen().y;
        int xCoordinate = panel.getLocationOnScreen().x - dialog.getLocationOnScreen().x;

        UiUtils.visitComponents(panel, "Looking inside the box", new Callback() {
                @Override
                public boolean onComponent(Component parent, Component component) {
                    if (component instanceof JLabel) {
                        JLabel label = (JLabel) component;
                        if (!label.isVisible() || label.getSize().width == 0)
                            return false;
                        String labelText = label.getText();
                        if (labelText.length() > 0) {
                            System.out.println("Looking at " + label);
                            try {
                                heights.put(label.getLocationOnScreen().y - relativeY, label.getSize().height);
                                yCoordinates.put(label.getLocationOnScreen().y - relativeY, labelText);
                            } catch (IllegalComponentStateException e) {
                                System.out.printf("Did not go well for " + label);
                            }
                        }
                    }
                    return true;
                }
            });

        saveSlices(dialogTitle, yCoordinates, xCoordinate, heights, panelImage, dialogModel);
    }

    private static String stripUnits(String title) {
        // Units are added without a space.
        // This removes all trailing parentheticals without a leading space.
        // If you want a parenthetical, be sure to separate it with a space.
        // e.g. `Cam mode (intake)`
        String reUnit = "(?<! )\\([^()]+\\)$";
        return title.replaceFirst(reUnit, "");
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
