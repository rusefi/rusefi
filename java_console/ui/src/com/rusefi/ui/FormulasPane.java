package com.rusefi.ui;

import com.rusefi.ConfigurationImage;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.Fields;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.ui.config.ConfigField;
import com.rusefi.ui.util.UiUtils;
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
 * (c) Andrey Belomutskiy 2013-2016
 */
public class FormulasPane {
    private final JPanel content = new JPanel(new BorderLayout());
    private final JPanel centerProxy = new JPanel(new BorderLayout());
    private final String newLine = "\r\n \\\\ ";
    private boolean isPaused;

    public FormulasPane() {
        content.add(centerProxy, BorderLayout.CENTER);

        centerProxy.add(new JLabel("Waiting for data..."), BorderLayout.CENTER);

        final JButton pauseButton = UiUtils.createPauseButton();
        pauseButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                isPaused = !isPaused;
                UiUtils.setPauseButtonText(pauseButton, isPaused);
            }
        });
        JPanel top = new JPanel(new FlowLayout());
        top.add(pauseButton);
        content.add(top, BorderLayout.NORTH);

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
        BinaryProtocol bp = BinaryProtocol.instance;
        if (bp == null)
            return;
        ConfigurationImage ci = bp.getController();
        if (ci == null)
            return;

        int algorithm = ConfigField.getIntValue(ci, Fields.ALGORITHM);
        engine_load_mode_e[] values = engine_load_mode_e.values();
        if (algorithm >= values.length)
            throw new IllegalStateException("Unexpected "+ algorithm);
        engine_load_mode_e algo = values[algorithm];

        String acceleration = getAccelerationVariables(ci);

        String page;
        if (algo == engine_load_mode_e.LM_SPEED_DENSITY) {
            page = getSpeedDensity(ci, acceleration);
        } else if (algo == engine_load_mode_e.LM_PLAIN_MAF) {
            double rpm = SensorCentral.getInstance().getValue(Sensor.RPM);
            double maf = SensorCentral.getInstance().getValue(Sensor.MAF);

                    String baseFuelStr = oneDecimal(Sensor.FUEL_BASE);
            String baseFuel = "$Base_Fuel (ms) = lookup (" +
                    "(RPM = " + rpm + ", " +
                    "MAF = " + maf + ") = " +
                    baseFuelStr + "ms";

            String actualLastInjection = oneDecimal(Sensor.actualLastInjection);
            String injTime =  "$Fuel (ms) = " + baseFuel + getInjecctorLag() +
                    " = " + actualLastInjection + "ms_per_injection$";

            page = acceleration + injTime;
        } else {
            page = acceleration + "todo";
        }

        TeXFormula formula = new TeXFormula("\r\n" +
                algo.title + newLine + newLine + newLine + page + "");
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
        String tpsDelta = oneDecimal(Sensor.deltaTps);
        String elDelta = oneDecimal(Sensor.engineLoadAccelDelta);

        int tpsEnrichLength = ConfigField.getIntValue(ci, Fields.TPSACCELLENGTH);
        int elEnrichLength = ConfigField.getIntValue(ci, Fields.ENGINELOADACCELLENGTH);

        String tpsEnrichDelta = "$deltaTps = max(currentTps - previousTps, length = " + tpsEnrichLength +
                ") = " + tpsDelta +"$";

        double tpsAccelThreshold = ConfigField.getFloatValue(ci, Fields.TPSACCELENRICHMENTTHRESHOLD);
        double tpsAccelMult = ConfigField.getFloatValue(ci, Fields.TPSACCELENRICHMENTMULTIPLIER);
        String tpsAccelValue = oneDecimal(Sensor.tpsAccelFuel);

        double tpsDecelThreshold = ConfigField.getFloatValue(ci, Fields.TPSDECELENLEANMENTTHRESHOLD);
        double tpsDecelMult = ConfigField.getFloatValue(ci, Fields.TPSDECELENLEANMENTMULTIPLIER);

        String tpsEnrich = "$tpsAccelEnrich = if (" +
                "(tpsDelta = " + tpsDelta + ") > (tpsThreshold = " + tpsAccelThreshold +"), tpsDelta, 0) * " +
                "(tpsAccelMultiplier = " + tpsAccelMult +  ") = " + tpsAccelValue +  "$";

        String loadEnrichDelta = "$deltaLoad = max(currentLoad - previousLoad, length = " + elEnrichLength +
                ") = " + elDelta + "$";

        return tpsEnrichDelta + newLine +
                tpsEnrich + newLine +
                loadEnrichDelta + newLine;
    }

    @NotNull
    private String getSpeedDensity(ConfigurationImage ci, String acceleration) {
        String IAT = oneDecimal(Sensor.IAT);
        String MAP = oneDecimal(Sensor.MAP);
        String T_CHARGE = oneDecimal(Sensor.T_CHARGE);

        double rpm = SensorCentral.getInstance().getValue(Sensor.RPM);
        String RPM = "" + (int) rpm;
        String VE = oneDecimal(Sensor.CURRENT_VE);
        String TARGET_AFR = oneDecimal(Sensor.TARGET_AFR);
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

        String baseFuelStr = oneDecimal(Sensor.FUEL_BASE);
        String baseFuel = "$Base_Fuel (ms) = \\frac{" +
                "($Airmass = " + chargeAirMass + ")" +
                "}{" +
                "(TargetAFR (" + rpm_map + ") = " + TARGET_AFR + ")" +
                " * (injectorFlow = " + injectorFlow + " cc/min)" +
                "} = " + baseFuelStr + "ms$";

        String IATcorr = oneDecimal(Sensor.iatCorrection);
        String CLTcorr = oneDecimal(Sensor.cltCorrection);
        String tpsAccel = oneDecimal(Sensor.tpsAccelFuel);

        String tempCorrections = " * cltCorr(" + CLTcorr + ") * iatCorr(" + IATcorr + ")";

        String actualLastInjection = oneDecimal(Sensor.actualLastInjection);
        String injTime = "$Fuel (ms) = " +
                "(Base_Fuel (" + baseFuelStr + "ms) + Tps_Accel_Corr = (" + tpsAccel + "ms))" +
                tempCorrections + getInjecctorLag() +
                " = " + actualLastInjection + "ms_per_injection$";

        return acceleration +
                tCharge + newLine +
                mCharge + newLine +
                baseFuel + newLine +
                injTime + newLine;
    }

    @NotNull
    private String getInjecctorLag() {
        String vBatt = oneDecimal(Sensor.VBATT);
        return "+ ( injectorLag(VBatt = " + vBatt + ") = " + oneDecimal(Sensor.injectorLagMs) + ")";
    }

    private String oneDecimal(Sensor sensor) {
        return oneDecimal(SensorCentral.getInstance().getValue(sensor));
    }

    private String oneDecimal(double ve) {
        return String.format("%.1f", ve);
    }

    public JPanel getContent() {
        return content;
    }
}