package com.rusefi.app;

import android.Manifest;
import android.app.Activity;
import android.content.pm.PackageManager;
import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;
import android.media.audiofx.AcousticEchoCanceler;
import android.media.audiofx.NoiseSuppressor;

import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

public class SoundBroadcast {
    private static final int sampleRate = 16000; // 44100 for music
    private static final int channelConfig = AudioFormat.CHANNEL_CONFIGURATION_MONO;
    private static final int audioFormat = AudioFormat.ENCODING_PCM_16BIT;
    private static final int minBufSize = AudioRecord.getMinBufferSize(sampleRate, channelConfig, audioFormat) + 2048;
    private byte[] buffer = new byte[minBufSize];

    static void checkOrRequestPermission(Activity context) {
        if (ContextCompat.checkSelfPermission(context, Manifest.permission.RECORD_AUDIO)
                != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(context,
                    new String[]{Manifest.permission.RECORD_AUDIO},
                    1234);
        }
    }

    public void start() {
        Thread streamThread = new Thread(new Runnable() {
            @Override
            public void run() {
/*
                AudioRecord recorder = new AudioRecord(MediaRecorder.AudioSource.MIC, sampleRate, channelConfig, audioFormat, minBufSize * 10);
                if (NoiseSuppressor.isAvailable()) {
                    NoiseSuppressor.create(recorder.getAudioSessionId()).setEnabled(true);
                }
                if (AcousticEchoCanceler.isAvailable()) {
                    AcousticEchoCanceler.create(recorder.getAudioSessionId()).setEnabled(true);
                }

                recorder.startRecording();

                while (true) {

                    int bytes = recorder.read(buffer, 0, buffer.length);

                }
*/
            }
        });
        streamThread.start();
    }

}
