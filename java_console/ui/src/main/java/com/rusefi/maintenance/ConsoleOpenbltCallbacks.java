package com.rusefi.maintenance;

public class ConsoleOpenbltCallbacks {
    public static final OpenbltJni.OpenbltCallbacks CONSOLE = new OpenbltJni.OpenbltCallbacks() {
        @Override
        public void log(String line) {
            System.out.println(line);
        }

        @Override
        public void updateProgress(int percent) {
            System.out.println("Progress: " + percent + "%");
            System.out.flush();
        }

        @Override
        public void error(String line) {
            System.out.println("Error: " + line);
        }

        @Override
        public void setPhase(String title, boolean hasProgress) {
            log("Begin phase: " + title);
        }
    };
}
