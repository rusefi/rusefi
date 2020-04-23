
import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.util.ArrayList;

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
            visitComponent(mainFrame, null, "", (parent, component) -> {
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

    public static void visitComponent(Component cmp, Component parent, String prefix, Callback callback) {
        System.out.println(prefix + " I see " + cmp.getClass());
        callback.onComponent(parent, cmp);
        Container container = (Container) cmp;
        if (container == null) {
            // Not a container, return
            return;
        }
        // Go visit and add all children
        for (Component subComponent : container.getComponents()) {
            visitComponent(subComponent, cmp, prefix + " " + subComponent.getClass().getSimpleName(), callback);
        }
    }

    private static boolean isTopLevelMenuButton(Component cmp) {
        return cmp instanceof bi.b;
    }

    private static void doJob(Frame frame) throws InterruptedException, InvocationTargetException, IOException, AWTException {
        printAllDialogs("Dialogs before clicking ", JDialog.getWindows());


        for (AbstractButton topLevel : topLevelButtons) {
            SwingUtilities.invokeAndWait(topLevel::doClick);
            Thread.sleep(TOP_MENU_CLICK_DELAY);

            ImageIO.write(
                    getScreenShot(frame),
                    "png",
                    new File(DESTINATION + cleanName(topLevel.getText()) + ".png"));

            printAllDialogs("dialogs after clicking ", JDialog.getWindows());
            java.util.List<JMenuItem> menuItems = new ArrayList<>();
            visitComponent(frame, null, "just clicked ", (parent, component) -> {
                if (component instanceof JMenuItem && component.getClass().getName().endsWith("aH.gc")) {
                    JMenuItem menuItem = (JMenuItem) component;
                    System.out.println("Menu item " + menuItem.getText());
                    menuItems.add(menuItem);
                }
            });


            for (JMenuItem m : menuItems) {

                //d.getLocation()

                SwingUtilities.invokeAndWait(new Runnable() {
                    @Override
                    public void run() {
                        try {

                            m.doClick();
                            Thread.sleep(MENU_CLICK_DELAY);

                            JDialog d = findDialog();
                            if (d == null) {
                                // this happens for example for disabled menu items
                                return;
                            }

//                            Robot robot = new Robot();
//                            Rectangle captureRect = new Rectangle(Toolkit.getDefaultToolkit().getScreenSize());
//                            BufferedImage screenFullImage = robot.createScreenCapture(captureRect);
//                            ImageIO.write(screenFullImage, PNG, new File(DESTINATION + "full_" + d.getTitle() + ".png"));

                            ImageIO.write(
                                    getScreenShot(d),
                                    PNG,
                                    new File(DESTINATION + cleanName(d.getTitle()) + ".png"));
                            d.setVisible(false);
                            d.dispose();
                        } catch (Exception e) {
                            throw new IllegalStateException(e);
                        }
                    }
                });

            }

            printAllDialogs("total frames now = ", JFrame.getFrames());

            Thread.sleep(1000);
        }
    }

    private static String cleanName(String title) {
        title = title.replace(' ', '_');
        title = title.replace(")", "");
        title = title.replace("(", "");
        title = title.replace('/', '_');
        title = title.replace('\\', '_');
        title = title.replace("  ", " ");
        return title;
    }

    private static JDialog findDialog() {
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
