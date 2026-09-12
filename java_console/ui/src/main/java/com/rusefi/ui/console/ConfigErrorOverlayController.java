package com.rusefi.ui.console;

import java.util.Objects;
import java.util.function.Consumer;

/** EDT-owned notification state. Closing an overlay acknowledges that message until it changes. */
final class ConfigErrorOverlayController {
    private final Consumer<String> show;
    private final Runnable hide;
    private Object session;
    private String message;
    private boolean shown;

    ConfigErrorOverlayController(Consumer<String> show, Runnable hide) {
        this.show = show;
        this.hide = hide;
    }

    void update(Object session, String message, boolean canShow) {
        if (this.session != session || !Objects.equals(this.message, message)) {
            hide.run();
            this.session = session;
            this.message = message;
            shown = false;
        }
        if (session != null && message != null && !message.isEmpty() && !shown && canShow) {
            shown = true;
            show.accept(message);
        }
    }
}
