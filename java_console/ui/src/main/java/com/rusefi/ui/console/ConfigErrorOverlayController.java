package com.rusefi.ui.console;

import java.util.Objects;
import java.util.function.BooleanSupplier;
import java.util.function.Consumer;

/** EDT-owned notification state. Closing an overlay acknowledges that message until it changes. */
final class ConfigErrorOverlayController {
    private final Consumer<String> show;
    private final Runnable hide;
    /**
     * True when an overlay we previously showed is no longer actually on screen because some other
     * owner replaced it. Our overlay lives in the frame glass pane, which {@link TabbedPanel} also
     * owns for its loading/updating status overlay; TabbedPanel installs its glass pane via a
     * delayed {@code invokeLater} during start-up and can silently replace ours right after we show
     * it. Without this check we would keep believing the (invisible) overlay is up and never
     * re-assert it - the root cause of issue #10219 appearing "not fixed".
     */
    private final BooleanSupplier displaced;
    private Object session;
    private String message;
    private boolean shown;

    ConfigErrorOverlayController(Consumer<String> show, Runnable hide) {
        this(show, hide, () -> false);
    }

    ConfigErrorOverlayController(Consumer<String> show, Runnable hide, BooleanSupplier displaced) {
        this.show = show;
        this.hide = hide;
        this.displaced = displaced;
    }

    void update(Object session, String message, boolean canShow) {
        // A displaced overlay is not on screen anymore, so forget we showed it and re-assert below.
        if (shown && displaced.getAsBoolean()) {
            shown = false;
        }
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
