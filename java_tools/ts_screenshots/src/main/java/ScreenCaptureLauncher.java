import com.rusefi.ScreenGenerator;

public class ScreenCaptureLauncher {
    public static void main(String[] args) throws Exception {
        startTunerStudio(args);
        ScreenGenerator.main(args);
    }

    private static void startTunerStudio(String[] args) {
        Thread t = new Thread(() -> {
            try {
               TunerStudio.main(args);
            } catch (Exception e) {
                e.printStackTrace();
                throw new IllegalStateException(e);
            }
        });

        t.setDaemon(false);
        t.start();
    }

}
