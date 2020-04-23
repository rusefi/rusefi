
import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.awt.image.RasterFormatException;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.util.*;
import java.util.List;
import java.util.concurrent.atomic.AtomicReference;

public class ScreenGenerator {
    public static final String TS_DIALOG = "com.efiAnalytics.ui.dg";
    public static final String PNG = "png";
    private static final int MENU_CLICK_DELAY = 300;
    private static final int TOP_MENU_CLICK_DELAY = 1000;
    private static ArrayList<AbstractButton> topLevelButtons = new ArrayList<>();

    private static final String DESTINATION = "images" + File.separator;

    public static void main(String[] args) throws InterruptedException, InvocationTargetException, IOException, AWTException {
        new File(DESTINATION).mkdirs();

        Thread t = new Thread(() -> TunerStudio.main(args));

        t.setDaemon(false);
        t.start();


        Frame mainFrame = findMainFrame();

        while (topLevelButtons.isEmpty()) {
            visitComponents(mainFrame, "", (parent, component) -> {
                if (component instanceof AbstractButton) {
                    AbstractButton ab = (AbstractButton) component;
                    System.out.println("topLevelButton " + ab.getText());

                    if (isTopLevelMenuButton(component)) {
                        topLevelButtons.add(ab);
                    }
                }
            });
            Thread.sleep(1000);
        }

        System.out.println("Done discovering buttons, " + topLevelButtons.size());

        doJob(mainFrame);
    }

    private static Frame findMainFrame() throws InterruptedException {
        while (true) {
            Frame[] all = JFrame.getFrames();

            for (Frame frame : all) {
                System.out.println("I see " + frame.getTitle());

                if (frame.getTitle().contains("TunerStudio")) {
                    return frame;
                }
            }
            Thread.sleep(1000);
        }
    }

    interface Callback {
        void onComponent(Component parent, Component component);
    }

    public static void visitComponents(Component cmp, String prefix, Callback callback) {
        visitComponents(cmp, null, prefix, callback);
    }

    public static void visitComponents(Component component, Component parent, String prefix, Callback callback) {
        if (component == null)
            throw new NullPointerException("component");
        if (component instanceof AbstractButton) {
            AbstractButton ab = (AbstractButton) component;
            System.out.println("[button " + ab.getText() + "]");
        } else if (component instanceof JLabel) {
            JLabel ab = (JLabel) component;
            System.out.println("[label " + ab.getText() + "]");
        } else if (component instanceof JComboBox) {
            JComboBox ab = (JComboBox) component;
            System.out.println("[combo " + ab.getSelectedItem() + "]");
        }


        System.out.println(prefix + " I see " + component.getClass());
        callback.onComponent(parent, component);
        Container container = (Container) component;
        if (container == null) {
            // Not a container, return
            return;
        }
        // Go visit and add all children
        for (Component subComponent : container.getComponents()) {
            if (subComponent == null)
                continue;
            visitComponents(subComponent, component, prefix + " " + subComponent.getClass().getSimpleName(), callback);
        }
    }

    private static boolean isTopLevelMenuButton(Component component) {
        return component instanceof bi.b;
    }

    private static void doJob(Frame frame) throws InterruptedException, InvocationTargetException, IOException, AWTException {
        printAllDialogs("Dialogs before clicking ", JDialog.getWindows());


        for (AbstractButton topLevel : topLevelButtons) {
            handleTopLevelButton(frame, topLevel);
        }
    }

    private static void handleTopLevelButton(Frame frame, AbstractButton topLevel) throws InterruptedException, InvocationTargetException, IOException {
        SwingUtilities.invokeAndWait(topLevel::doClick);
        Thread.sleep(TOP_MENU_CLICK_DELAY);

        ImageIO.write(
                getScreenShot(frame),
                "png",
                new File(DESTINATION + cleanName(topLevel.getText()) + ".png"));

        List<JMenuItem> menuItems = findMenuItems(frame);

        for (JMenuItem menuItem : menuItems) {
            handleMenuItem(menuItem);
        }
    }

    private static void handleMenuItem(JMenuItem menuItem) throws InterruptedException, InvocationTargetException {
        SwingUtilities.invokeAndWait(new Runnable() {
            @Override
            public void run() {
                menuItem.doClick();
            }
        });

        Thread.sleep(MENU_CLICK_DELAY);


        AtomicReference<JDialog> ref = new AtomicReference<>();
        SwingUtilities.invokeAndWait(() -> {
            ref.set(findDynamicDialog());
        });
        // let's give it time to appear on the screen
        Thread.sleep(MENU_CLICK_DELAY);
        JDialog dialog = ref.get();
        if (dialog == null) {
            System.out.println("Not found for " + menuItem);
            return;
        }

        SwingUtilities.invokeAndWait(() -> {
            try {
                Map<Integer, String> yCoordinates = new TreeMap<>();

                findSlices(dialog, yCoordinates);

                if (dialog == null) {
                    // this happens for example for disabled menu items
                    return;
                }


                System.out.println("Label Y coordinates: " + yCoordinates);
                int topY = dialog.getLocationOnScreen().y;
                yCoordinates.put(topY, "top");
                yCoordinates.put(topY + dialog.getSize().height, "bottom");

                List<Integer> sorted = new ArrayList<>(yCoordinates.keySet());

                BufferedImage dialogScreenShot = getScreenShot(dialog);

                for (int i = 0; i < sorted.size() - 1; i++) {
                    Integer fromYScreen = sorted.get(i);
                    int fromY = fromYScreen - topY;
                    int toY = sorted.get(i + 1) - topY;

                    String sectionNameWithSpecifalCharacters = yCoordinates.get(fromYScreen);
                    String sectionName = cleanName(sectionNameWithSpecifalCharacters);

                    BufferedImage slice;
                    try {
                        slice = dialogScreenShot.getSubimage(0, fromY, dialogScreenShot.getWidth(), toY - fromY);
                    } catch (RasterFormatException e) {
                        System.out.printf("Dialog does not fit screen? " + sectionNameWithSpecifalCharacters);
                        continue;
                    }

                    if (slice == null) {
                        System.out.println("Weird");
                        continue;
                    }
                    String fileName = cleanName(dialog.getTitle()) + "_slice_" + fromY + "_" + sectionName + ".png";
                    File output = new File(DESTINATION + fileName);
                    if (output == null) {
                        System.out.println(sectionName + " in " + fileName + " was not a success");
                        continue;
                    }
                    try {
                        ImageIO.write(slice, PNG, output);
                    } catch (NullPointerException | FileNotFoundException e) {
                        System.out.println(sectionName + " in " + fileName + " was not a success?");
                        continue;
                    }
                }


//                            Robot robot = new Robot();
//                            Rectangle captureRect = new Rectangle(Toolkit.getDefaultToolkit().getScreenSize());
//                            BufferedImage screenFullImage = robot.createScreenCapture(captureRect);
//                            ImageIO.write(screenFullImage, PNG, new File(DESTINATION + "full_" + d.getTitle() + ".png"));

                ImageIO.write(
                        dialogScreenShot,
                        PNG,
                        new File(DESTINATION + cleanName(dialog.getTitle()) + ".png"));
                dialog.setVisible(false);
                dialog.dispose();
            } catch (Exception e) {
                throw new IllegalStateException(e);
            }
        });
    }

    private static void findSlices(JDialog dialog, Map<Integer, String> yCoordinates) {
        visitComponents(dialog, "Dynamic dialog", new Callback() {
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
                            yCoordinates.put(label.getLocationOnScreen().y, labelText);
                        } catch (IllegalComponentStateException e) {
                            System.out.printf("Did not go well for " + label);
                        }
                    }
                }

            }
        });
    }

    private static List<JMenuItem> findMenuItems(Frame frame) {
        List<JMenuItem> menuItems = new ArrayList<>();
        visitComponents(frame, "Just clicked ", (parent, component) -> {
            if (component instanceof JMenuItem && component.getClass().getName().endsWith("aH.gc")) {
                JMenuItem menuItem = (JMenuItem) component;
                System.out.println("Menu item " + menuItem.getText());
                menuItems.add(menuItem);
            }
        });
        return menuItems;
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

    private static JDialog findDynamicDialog() {
        for (Window d : Dialog.getWindows()) {
            if (d.getClass().getName().equals(TS_DIALOG) && d.isVisible()) {
                return (JDialog) d;
            }
        }
        return null;
    }

    private static void printAllDialogs(String message, Window[] windows) {
        System.out.println(message + windows.length);
        for (Window window : windows)
            System.out.println("type " + window.getClass());
    }

    public static BufferedImage getScreenShot(Component component) {

        BufferedImage image = new BufferedImage(
                component.getWidth(),
                component.getHeight(),
                BufferedImage.TYPE_INT_RGB
        );
        // call the Component's paint method, using
        // the Graphics object of the image.
        component.paint(image.getGraphics()); // alternately use .printAll(..)
        return image;
    }
}
