package com.rusefi.app.util;

import android.app.Activity;
import android.os.Build;
import android.view.Window;
import android.view.WindowManager;

public class AndroidUtil {
    private AndroidUtil() {
    }

    public static void turnScreenOn(Activity rusEFI) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O_MR1) {
            rusEFI.setTurnScreenOn(true);
        } else {
            Window window = rusEFI.getWindow();
            window.addFlags(WindowManager.LayoutParams.FLAG_TURN_SCREEN_ON);
        }
    }
}
