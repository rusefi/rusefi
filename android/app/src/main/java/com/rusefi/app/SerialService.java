package com.rusefi.app;

import android.app.Service;
import android.content.Intent;
import android.os.Binder;
import android.os.IBinder;

import androidx.annotation.Nullable;

public class SerialService extends Service {
    class SerialBinder extends Binder {
/*
        SerialService getService() {
            return SerialService.this;
        }
*/
    }

    private final IBinder binder = new SerialBinder();

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return binder;
    }
}
