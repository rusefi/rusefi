package com.rusefi.maintenance;

public interface OpenbltCallbacks {
    OpenbltCallbacks CONSOLE = new OpenbltCallbacks() {
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

    void log(String line);

    void updateProgress(int percent);

    void error(String line);

    void setPhase(String title, boolean hasProgress);
}
