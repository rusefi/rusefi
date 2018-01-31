package com.rusefi.ui;

import com.opensr5.ConfigurationImage;
import com.rusefi.FileLog;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.BinaryProtocolHolder;
import com.rusefi.config.Fields;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.ui.config.ConfigField;
import com.rusefi.ui.util.UiUtils;
import com.rusefi.ui.widgets.IntGaugeLabel;
import org.jetbrains.annotations.NotNull;
import org.scilab.forge.jlatexmath.TeXConstants;
import org.scilab.forge.jlatexmath.TeXFormula;
import org.scilab.forge.jlatexmath.TeXIcon;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;

/**
 * (c) Andrey Belomutskiy 2013-2017
 */
public class FormulasPane {
    private static final String NL = "\r\n \\\\ ";
    private static final String NL2 = NL + "\\  " + NL; // two new lines
    private static final String NL3 = NL2 + "\\  " + NL; // two new lines

    private final JPanel content = new JPanel(new BorderLayout());
    private final JPanel centerProxy = new JPanel(new BorderLayout());
    private boolean isPaused;

    public FormulasPane() {
        content.add(centerProxy, BorderLayout.CENTER);

        centerProxy.add(new JLabel("Waiting for data..."), BorderLayout.CENTER);

        JButton saveImage = UiUtils.createSaveImageButton();
        saveImage.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String fileName = FileLog.getDate() + "_formulas.png";

                UiUtils.saveImageWithPrompt(fileName, centerProxy, centerProxy);
            }
        });

        final JButton pauseButton = UiUtils.createPauseButton();
        pauseButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                isPaused = !isPaused;
                UiUtils.setPauseButtonText(pauseButton, isPaused);
            }
        });
        JPanel top = new JPanel(new FlowLayout());
        top.add(saveImage);
        top.add(pauseButton);
        content.add(top, BorderLayout.NORTH);

        JPanel bottomPanel = new JPanel(new FlowLayout());
        bottomPanel.add(new IntGaugeLabel("count", Sensor.errorCodeCounter));
        bottomPanel.add(new IntGaugeLabel("error", Sensor.lastErrorCode));

        content.add(bottomPanel, BorderLayout.SOUTH);

        updateFormula();
        new Timer(200, new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (!isPaused)
                    updateFormula();
            }
        }).start();
    }

    enum engine_load_mode_e {
        LM_PLAIN_MAF("Plain MAF"),
        LM_ALPHA_N("Alpha-N/TPS"),
        LM_MAP("Plain MAP"),
        LM_SPEED_DENSITY("Speed Density/MAP"),
        LM_REAL_MAF("MAF");

        private final String title;

        engine_load_mode_e(String title) {
            this.title = title;
        }
    }

    private void updateFormula() {
        BinaryProtocol bp = BinaryProtocolHolder.getInstance().get();
        if (bp == null)
            return;
        ConfigurationImage ci = bp.getController();
        if (ci == null)
            return;

        int algorithm = ConfigField.getIntValue(ci, Fields.FUELALGORITHM);
        engine_load_mode_e[] values = engine_load_mode_e.values();
        if (algorithm >= values.length) {
            FileLog.MAIN.logLine("Invalid algorithm" + algorithm);
            algorithm = 0; // we are here for example in case of an invalid/incompatible configuration
        }
        engine_load_mode_e algo = values[algorithm];

        String acceleration = getAccelerationVariables(ci) + NL2;

        String page;
        if (algo == engine_load_mode_e.LM_SPEED_DENSITY) {
            page = getSpeedDensity(ci, acceleration);
        } else if (algo == engine_load_mode_e.LM_PLAIN_MAF) {
            double rpm = SensorCentral.getInstance().getValue(Sensor.RPM);
            double maf = SensorCentral.getInstance().getValue(Sensor.MAF);

            String baseFuelStr = twoDecimals(Sensor.FUEL_BASE);
            String baseFuel = "$Table_Fuel (ms) = lookup (" +
                    "(RPM = " + rpm + ", " +
                    "MAF = " + maf + ") = " +
                    baseFuelStr + "ms";

            String actualLastInjection = twoDecimals(Sensor.actualLastInjection);
            String injTime =  "$Fuel (ms) = " + baseFuel + getInjecctorLag() +
                    " = " + actualLastInjection + "ms_per_injection$";

            page = acceleration + injTime;
        } else {
            page = acceleration + "todo";
        }

        TeXFormula formula = new TeXFormula("\r\n" +
                algo.title + NL3 + page + "");
        TeXIcon icon = formula.createTeXIcon(TeXConstants.STYLE_DISPLAY, 20);

        BufferedImage image = new BufferedImage(icon.getIconWidth(), icon.getIconHeight(), BufferedImage.TYPE_INT_ARGB);
        Graphics2D g2 = image.createGraphics();
        g2.setColor(Color.white);
        g2.fillRect(0, 0, icon.getIconWidth(), icon.getIconHeight());
        JLabel label = new JLabel(icon);

        centerProxy.removeAll();
        centerProxy.add(label, BorderLayout.CENTER);
    }

    @NotNull
    private String getAccelerationVariables(ConfigurationImage ci) {
        String tpsDelta = twoDecimals(Sensor.deltaTps);
        String elDelta = twoDecimals(Sensor.engineLoadAccelDelta);

        int tpsEnrichLength = ConfigField.getIntValue(ci, Fields.TPSACCELLENGTH);
        int elEnrichLength = ConfigField.getIntValue(ci, Fields.ENGINELOADACCELLENGTH);

        String tpsEnrichDelta = "$deltaTps = max(currentTps - previousTps, length = " + tpsEnrichLength +
                ") = " + tpsDelta +"$";

        double tpsAccelThreshold = ConfigField.getFloatValue(ci, Fields.TPSACCELENRICHMENTTHRESHOLD);
        String tpsAccelMult = "fixme";//ConfigField.getFloatValue(ci, Fields.TPSACCELENRICHMENTMULTIPLIER);
        String tpsAccelValue = twoDecimals(Sensor.tpsAccelFuel);

        double tpsDecelThreshold = ConfigField.getFloatValue(ci, Fields.TPSDECELENLEANMENTTHRESHOLD);
        double tpsDecelMult = ConfigField.getFloatValue(ci, Fields.TPSDECELENLEANMENTMULTIPLIER);

        String tpsEnrich = "$tpsAccelEnrich = if (" +
                "(tpsDelta = " + tpsDelta + ") > (tpsThreshold = " + tpsAccelThreshold +"), tpsDelta, 0) * " +
                "(tpsAccelMultiplier = " + tpsAccelMult +  ") = " + tpsAccelValue +  "$";

        String loadEnrichDelta = "$deltaLoad = max(currentLoad - previousLoad, length = " + elEnrichLength +
                ") = " + elDelta + "$";

        return tpsEnrichDelta + NL +
                tpsEnrich + NL +
                loadEnrichDelta;
    }

    @NotNull
    private String getSpeedDensity(ConfigurationImage ci, String acceleration) {
        String IAT = oneDecimal(Sensor.IAT);
        String MAP = oneDecimal(Sensor.MAP);
        String T_CHARGE = oneDecimal(Sensor.T_CHARGE);

        double rpm = SensorCentral.getInstance().getValue(Sensor.RPM);
        String RPM = "" + (int) rpm;
        String VE = twoDecimals(Sensor.CURRENT_VE);
        String TARGET_AFR = twoDecimals(Sensor.TARGET_AFR);
        String tpsStr = oneDecimal(Sensor.TPS);
        String chargeAirMass = String.format("%.3fgm", SensorCentral.getInstance().getValue(Sensor.CHARGE_AIR_MASS));

        double displacement = ConfigField.getFloatValue(ci, Fields.DISPLACEMENT);
        int cylinderCount = ConfigField.getIntValue(ci, Fields.CYLINDERSCOUNT);
        String cylinderDisplacement = oneDecimal(displacement / cylinderCount);
        String injectorFlow = oneDecimal((float) ConfigField.getValue(ci, Fields.INJECTOR_FLOW));

        String tCharge = "$Tcharge=f(CLT=" + oneDecimal(Sensor.CLT) + "C,IAT=" + IAT
                + "C,TPS=" + tpsStr + "\\%, RPM = " + RPM + ")=" + T_CHARGE + "C$";

        String rpm_map = "RPM=" + RPM + ",MAP=" + MAP + "kPa";
        String mCharge = "$Airmass (g/cyl) =\\frac{(V_Cylinder = " + cylinderDisplacement + "L) " +
                "* (VE(" + rpm_map + ")  = " + VE + "\\%) " +
                "* (MAP = " + MAP + "Kpa)" +
                "}{" +
                "(GAS_R = 0.28705) * ((Tcharge = " + T_CHARGE + "C) + 273.15)} = " +
                chargeAirMass +
                "$";

        String baseFuelStr = twoDecimals(Sensor.FUEL_BASE);
        String baseFuel = "$SD_Fuel (ms) = \\frac{" +
                "($Airmass = " + chargeAirMass + ")" +
                "}{" +
                "(TargetAFR (" + rpm_map + ") = " + TARGET_AFR + ")" +
                " * (injectorFlow = " + injectorFlow + " cc/min)" +
                "} = " + baseFuelStr + "ms$";

        String IATcorr = twoDecimals(Sensor.iatCorrection);
        String CLTcorr = twoDecimals(Sensor.cltCorrection);
        String tpsAccel = twoDecimals(Sensor.tpsAccelFuel);

        String tempCorrections = " * cltCorr(" + CLTcorr + ") * iatCorr(" + IATcorr + ")";

        String actualLastInjection = twoDecimals(Sensor.actualLastInjection);
        String injTime = "$Fuel (ms) = " +
                "(SD_Fuel (" + baseFuelStr + "ms) + Tps_Accel_Corr = (" + tpsAccel + "ms))" +
                tempCorrections + NL + getInjecctorLag() +
                " = " + actualLastInjection + "ms_per_injection$";

        return acceleration +
                tCharge + NL3 +
                mCharge + NL2 +
                baseFuel + NL2 +
                injTime + NL;
    }

    @NotNull
    private String getInjecctorLag() {
        String vBatt = twoDecimals(Sensor.VBATT);
        return "+ ( injectorLag(VBatt = " + vBatt + ") = " + twoDecimals(Sensor.injectorLagMs) + ")";
    }

    private String oneDecimal(Sensor sensor) {
        return oneDecimal(SensorCentral.getInstance().getValue(sensor));
    }

    private String twoDecimals(Sensor sensor) {
        return twoDecimals(SensorCentral.getInstance().getValue(sensor));
    }

    private String oneDecimal(double ve) {
        return String.format("%.1f", ve);
    }

    private String twoDecimals(double ve) {
        return String.format("%.2f", ve);
    }

    public JPanel getContent() {
        return content;
    }
}