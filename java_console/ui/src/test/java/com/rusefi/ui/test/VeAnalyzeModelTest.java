package com.rusefi.ui.test;

import com.rusefi.ui.VeAnalyzeModel;
import org.junit.Assert;
import org.junit.jupiter.api.Test;

/**
 * Unit tests for the pure VE autotune model. No ECU / Swing / ini dependencies.
 */
public class VeAnalyzeModelTest {

    private VeAnalyzeModel sampleModel() {
        double[] rpm = {1000, 2000, 3000};
        double[] load = {30, 60, 100};
        double[][] ve = {
            {50, 55, 60},
            {70, 75, 80},
            {90, 95, 100},
        };
        return new VeAnalyzeModel(rpm, load, ve);
    }

    @Test
    public void testIndexing() {
        VeAnalyzeModel m = sampleModel();
        Assert.assertEquals(0, m.rpmIndex(1000));
        Assert.assertEquals(2, m.rpmIndex(3000));
        Assert.assertEquals(-1, m.rpmIndex(999));
        Assert.assertEquals(1, m.loadIndex(60));
        Assert.assertEquals(-1, m.loadIndex(101));
    }

    @Test
    public void testRejectsOutOfClosedLoop() {
        VeAnalyzeModel m = sampleModel();
        Assert.assertNull(m.addSample(1500, 40, 1.0, 1.0, false));
        Assert.assertEquals(0, m.getCellsWithData());
        Assert.assertEquals(0.0, m.getCoverage(), 0.0001);
    }

    @Test
    public void testRejectsBadLambda() {
        VeAnalyzeModel m = sampleModel();
        Assert.assertNull(m.addSample(1500, 40, 1.0, 0.0, true));
        Assert.assertNull(m.addSample(1500, 40, 0.0, 1.0, true));
        Assert.assertEquals(0, m.getCellsWithData());
    }

    @Test
    public void testRichSampleRichensCell() {
        VeAnalyzeModel m = sampleModel();
        // actual 1.1 / target 1.0 => engine runs lean => VE must increase
        m.addSample(1000, 30, 1.1, 1.0, true);
        double[][] suggested = m.getSuggestedVe();
        Assert.assertEquals(50.0 * (1.0 + 0.5 * 0.1), suggested[0][0], 0.0001);
    }

    @Test
    public void testLeanSampleLeansCell() {
        VeAnalyzeModel m = sampleModel();
        // actual 0.9 / target 1.0 => engine runs rich => VE must decrease
        m.addSample(3000, 100, 0.9, 1.0, true);
        double[][] suggested = m.getSuggestedVe();
        Assert.assertEquals(100.0 * (1.0 + 0.5 * (-0.1)), suggested[2][2], 0.0001);
    }

    @Test
    public void testUntouchedCellStaysSame() {
        VeAnalyzeModel m = sampleModel();
        m.addSample(1000, 30, 1.1, 1.0, true);
        double[][] suggested = m.getSuggestedVe();
        Assert.assertEquals(80.0, suggested[1][2], 0.0001);
        Assert.assertEquals(1, m.getCellsWithData());
    }

    @Test
    public void testAveragingAcrossSamples() {
        VeAnalyzeModel m = sampleModel();
        m.addSample(1000, 30, 1.1, 1.0, true);
        m.addSample(1000, 30, 0.9, 1.0, true);
        // mean ratio = 1.0 -> no change
        double[][] suggested = m.getSuggestedVe();
        Assert.assertEquals(50.0, suggested[0][0], 0.0001);
        Assert.assertEquals(2, m.getSampleCounts()[0][0]);
    }

    @Test
    public void testConfidenceLevels() {
        VeAnalyzeModel m = sampleModel();
        Assert.assertEquals(VeAnalyzeModel.Confidence.NONE, m.getConfidence(0, 0));

        for (int i = 0; i < 4; i++) {
            m.addSample(1000, 30, 1.0, 1.0, true);
        }
        Assert.assertEquals(VeAnalyzeModel.Confidence.LOW, m.getConfidence(0, 0));

        while (m.getSampleCounts()[0][0] < 20) {
            m.addSample(1000, 30, 1.0, 1.0, true);
        }
        Assert.assertEquals(VeAnalyzeModel.Confidence.MEDIUM, m.getConfidence(0, 0));

        while (m.getSampleCounts()[0][0] <= 20) {
            m.addSample(1000, 30, 1.0, 1.0, true);
        }
        Assert.assertEquals(VeAnalyzeModel.Confidence.HIGH, m.getConfidence(0, 0));
    }

    @Test
    public void testReset() {
        VeAnalyzeModel m = sampleModel();
        m.addSample(1000, 30, 1.1, 1.0, true);
        m.reset();
        Assert.assertEquals(0, m.getCellsWithData());
        Assert.assertEquals(50.0, m.getSuggestedVe()[0][0], 0.0001);
    }

    @Test
    public void testInterpolate() {
        double[] x = {0, 10};
        double[] y = {0, 10};
        double[][] z = {
            {0, 10},
            {10, 20},
        };
        Assert.assertEquals(0.0, VeAnalyzeModel.interpolate(0, 0, x, y, z), 0.0001);
        Assert.assertEquals(20.0, VeAnalyzeModel.interpolate(10, 10, x, y, z), 0.0001);
        Assert.assertEquals(10.0, VeAnalyzeModel.interpolate(5, 5, x, y, z), 0.0001);
    }
}
