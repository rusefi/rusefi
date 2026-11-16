package com.rusefi.io;

import com.devexperts.logging.Logging;
import com.rusefi.ui.StatusConsumer;

public interface UpdateOperationCallbacks extends StatusConsumer {
    Logging log = Logging.getLogging(UpdateOperationCallbacks.class);

    enum FirmwareUpdateStage {
        DOWNLOAD(0, 15, "downloading firmware"),
        CALIBRATION_BACKUP(15, 25, "backing up tune"),
        ERASE(25, 45, "erasing"),
        PROGRAM(45, 95, "flashing"),
        CALIBRATION_FINALIZE(95, 99, "finalizing update"),
        CALIBRATION_RESTORE(95, 99, "migrating tune");

        private final int start;
        private final int end;
        private final String description;

        FirmwareUpdateStage(int start, int end, String description) {
            this.start = start;
            this.end = end;
            this.description = description;
        }

        int map(int percent) {
            int bounded = Math.max(0, Math.min(100, percent));
            return start + (end - start) * bounded / 100;
        }

        public String getDescription() {
            return description;
        }
    }

    void log(String message, boolean breakLineOnTextArea, boolean sendToLogger);

    @Override
    default void logLine(final String message) {
        log(message, true, true);
    }

    /** Whether maintenance may ask the user through Swing dialogs. */
    default boolean isInteractive() {
        return this != DUMMY && this != LOGGER;
    }

    void done();
    void warning();
    void error();

    /** Reports a pre-handoff rejection. UI clients may prominently display the reason. */
    default void firmwareUpdateBlocked(String reason) {
        logLine(reason);
    }

    /** Runs after eligibility passes and before an automatic firmware job takes over the connection. */
    default void firmwareHandoffStarted() {
    }

    /** Reveals operation details after a job is accepted but before its first progress callback. */
    default void revealActivity() {
    }

    void clear();

    UpdateOperationCallbacks DUMMY = new UpdateOperationCallbacks() {
        @Override
        public void log(final String message, final boolean breakLineOnTextArea, boolean sendToLogger) {
        }

        @Override
        public void done() {
        }

        @Override
        public void error() {
        }

        @Override
        public void warning() {

        }

        @Override
        public void clear() {
        }

        @Override
        public String toString() {
            return "DUMMY";
        }
    };

    UpdateOperationCallbacks LOGGER = new UpdateOperationCallbacks() {
        @Override
        public void log(final String message, final boolean breakLineOnTextArea, boolean sendToLogger) {
            log.info(message);
        }

        @Override
        public void done() {
            log.info("[DONE]");
        }

        @Override
        public void warning() {
            log.info("[warning]");
        }

        @Override
        public void error() {
            log.info("[error]");
        }

        @Override
        public void clear() {
        }

        @Override
        public String toString() {
            return "LOGGER";
        }
    };

    UpdateOperationCallbacks CONSOLE = new UpdateOperationCallbacks() {
        @Override
        public void log(final String message, final boolean breakLineOnTextArea, boolean sendToLogger) {
            if (breakLineOnTextArea) {
                System.out.println(message);
            } else {
                System.out.print(message);
            }
            if (sendToLogger) {
                LOGGER.log(message, breakLineOnTextArea, true);
            }
        }

        @Override
        public void done() {
            log.info("[DONE]");
            LOGGER.done();
        }

        @Override
        public void warning() {
            log.info("[warning]");
            LOGGER.warning();
        }

        @Override
        public void error() {
            log.info("[error]");
            LOGGER.error();
        }

        @Override
        public void clear() {
        }
    };

    default void updateProgress(int percent) {
        logLine("Progress: " + percent + "%");
    }

    /** Reports a firmware phase without changing the overall progress value. */
    default void updateFirmwarePhase(FirmwareUpdateStage stage) {
    }

    /** Reports progress within a known firmware-update stage without changing ordinary job progress semantics. */
    default void updateFirmwareProgress(FirmwareUpdateStage stage, int percent) {
        updateFirmwarePhase(stage);
        updateProgress(stage.map(percent));
    }
}
