package com.rusefi.ts_plugin;

import javax.sound.sampled.*;
import java.io.IOException;
import java.io.InputStream;
import java.net.ServerSocket;
import java.net.Socket;

public class AudioPlayback {
    private static final int sampleRate = 16000;

    private static final AudioFormat format = new AudioFormat(sampleRate, 16, 1, true, false);

    public AudioPlayback() throws IOException {
        ServerSocket serverSocket = new ServerSocket(16000);

        while (true) {

            Socket clientSocket = serverSocket.accept();
            new Thread(() -> {
                try {
                    playSound(clientSocket.getInputStream());
                } catch (IOException e) {
                    throw new IllegalStateException(e);
                }
            }).start();
        }
    }

    public static void start() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    new AudioPlayback();
                } catch (IOException e) {
                    throw new IllegalStateException(e);
                }
            }
        }).start();
    }


    private static void playSound(InputStream stream) throws IOException {
        while (true) {
            byte[] buffer = new byte[30000];
            int got = stream.read(buffer);

            byte copy[] = new byte[got];
            System.arraycopy(buffer, 0, copy, 0, got);
            new Thread(new Runnable() {
                @Override
                public void run() {
                    toSpeaker(copy);
                }
            }).start();
        }
    }

    public static void toSpeaker(byte soundbytes[]) {
        try {
            DataLine.Info dataLineInfo = new DataLine.Info(SourceDataLine.class, format);
            SourceDataLine sourceDataLine = (SourceDataLine) AudioSystem.getLine(dataLineInfo);

            sourceDataLine.open(format);

            FloatControl volumeControl = (FloatControl) sourceDataLine.getControl(FloatControl.Type.MASTER_GAIN);
            volumeControl.setValue(100.0f);

            sourceDataLine.start();
            sourceDataLine.open(format);

            sourceDataLine.start();

            System.out.println("format? :" + sourceDataLine.getFormat());

            sourceDataLine.write(soundbytes, 0, soundbytes.length);
            System.out.println("Playing " + soundbytes.toString());
            sourceDataLine.drain();
            sourceDataLine.close();
        } catch (Exception e) {
            System.out.println("Not working in speakers...");
            e.printStackTrace();
        }
    }
}
