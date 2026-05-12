package com.rusefi.ts_plugin.knock;

import com.rusefi.core.ui.FrameHelper;

import javax.swing.*;
import java.awt.*;
import java.util.Random;

/**
 * Interactive sandbox to visually test {@link KnockCanvas} with simulated spectrogram data.
 * <p>
 * Run {@link #main} to open a window showing the canvas updating in real time.
 */
public class KnockCanvasSandbox {

    private static final int FREQUENCY_START_HZ = 1000;
    private static final float FREQUENCY_STEP_HZ = 200.0f;
    private static final int VALUE_COUNT = 64;
    private static final int PREFERRED_WIDTH = 600;
    private static final int PREFERRED_HEIGHT = 400;
    private static final int UPDATE_INTERVAL_MS = 5;

    public static void main(String[] args) {
        SwingUtilities.invokeLater(KnockCanvasSandbox::show);
    }

    private static void show() {
        KnockCanvas canvas = new KnockCanvas(1, 1, true);
        canvas.setFrequencyStart(FREQUENCY_START_HZ);
        canvas.setFrequencyStep(FREQUENCY_STEP_HZ);

        JComponent component = canvas.getComponent();
        component.setPreferredSize(new Dimension(PREFERRED_WIDTH, PREFERRED_HEIGHT));
        component.setMinimumSize(new Dimension(PREFERRED_WIDTH, PREFERRED_HEIGHT));
        component.setDoubleBuffered(true);
        component.setFocusable(true);

        FrameHelper frameHelper = new FrameHelper();
        frameHelper.getFrame().setSize(PREFERRED_WIDTH, PREFERRED_HEIGHT);
        frameHelper.getFrame().setLocationRelativeTo(null);
        frameHelper.showFrame(component, false);

        startFeedingData(canvas, component);
    }

    private static void startFeedingData(KnockCanvas canvas, JComponent component) {
        Random random = new Random();
        // 6-cylinder engine, ~85mm bore: first knock mode ≈ 6.5 kHz
        // With FREQUENCY_START_HZ=1000 and FREQUENCY_STEP_HZ=200, bin=(Hz-1000)/200
        // 6500 Hz → bin 27, 7000 Hz → bin 30 — keep peak within this realistic band
        final int KNOCK_PEAK_BIN_MIN = 25; // ~6.0 kHz
        final int KNOCK_PEAK_BIN_MAX = 30; // ~7.0 kHz
        int[] peakBin = {27};
        // Bad knock event: remaining ticks of elevated out-of-band broadband energy
        int[] badKnockTicksLeft = {0};

        Timer timer = new Timer(UPDATE_INTERVAL_MS, e -> {
            // ~1% chance per tick to trigger a bad knock burst (lasts ~30 ticks = 150ms)
            if (badKnockTicksLeft[0] == 0 && random.nextInt(100) == 0) {
                badKnockTicksLeft[0] = 30;
            }

            float[] values = generateSpectrum(random, peakBin[0], badKnockTicksLeft[0] > 0);
            canvas.processValues(values);

            if (badKnockTicksLeft[0] > 0) {
                badKnockTicksLeft[0]--;
            }

            // Slight jitter within the realistic knock band
            if (random.nextInt(15) == 0) {
                peakBin[0] += random.nextInt(3) - 1;
                if (peakBin[0] < KNOCK_PEAK_BIN_MIN) peakBin[0] = KNOCK_PEAK_BIN_MIN;
                if (peakBin[0] > KNOCK_PEAK_BIN_MAX) peakBin[0] = KNOCK_PEAK_BIN_MAX;
            }

            component.repaint();
        });
        timer.start();
    }

    /**
     * Generate a simulated frequency spectrum: Gaussian peak plus noise.
     * During a bad knock event, adds a strong broadband burst across a wide frequency range.
     */
    private static float[] generateSpectrum(Random random, int peakBin, boolean badKnock) {
        float[] values = new float[VALUE_COUNT];
        float peakAmplitude = 200 + random.nextFloat() * 55;
        float width = 3.0f;

        // Bad knock: strong broadband energy spanning bins 10–55 (~3–12 kHz)
        int badKnockCenter = badKnock ? (10 + random.nextInt(45)) : -1;
        float badKnockAmplitude = badKnock ? (600 + random.nextFloat() * 200) : 0;
        float badKnockWidth = 12.0f;

        for (int i = 0; i < VALUE_COUNT; i++) {
            float noise = random.nextFloat() * 10;
            float distance = i - peakBin;
            float gaussian = peakAmplitude * (float) Math.exp(-(distance * distance) / (2 * width * width));

            float badKnockGaussian = 0;
            if (badKnock) {
                float badDistance = i - badKnockCenter;
                badKnockGaussian = badKnockAmplitude * (float) Math.exp(-(badDistance * badDistance) / (2 * badKnockWidth * badKnockWidth));
            }

            values[i] = noise + gaussian + badKnockGaussian;
        }
        return values;
    }
}
