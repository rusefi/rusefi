package com.rusefi.ui;

import com.rusefi.ConfigurationImage;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.Fields;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.ui.config.ConfigField;
import org.scilab.forge.jlatexmath.TeXConstants;
import org.scilab.forge.jlatexmath.TeXFormula;
import org.scilab.forge.jlatexmath.TeXIcon;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;

public class FormulasPane {
    private final JPanel content = new JPanel(new BorderLayout());
    private final JPanel centerProxy = new JPanel(new BorderLayout());

    public FormulasPane() {
        content.add(centerProxy, BorderLayout.CENTER);

        updateFormula();
        new Timer(200, new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                updateFormula();
            }
        }).start();
    }

    private void updateFormula() {

        BinaryProtocol bp = BinaryProtocol.instance;
        if (bp == null)
            return;
        ConfigurationImage ci = bp.getController();
        if (ci == null)
            return;



        String IAT = oneDecimal(Sensor.IAT);
        String MAP = oneDecimal(Sensor.MAP);
        String T_CHARGE = oneDecimal(Sensor.T_CHARGE);

        double rpm = SensorCentral.getInstance().getValue(Sensor.RPM);
        String RPM = "" + (int) rpm;
        String VE = oneDecimal(Sensor.CURRENT_VE);
        String TARGET_AFR = oneDecimal(Sensor.TARGET_AFR);
        String tpsStr = oneDecimal(Sensor.TPS);
        String chargeAirMass = String.format("%.3fgm", SensorCentral.getInstance().getValue(Sensor.CHARGE_AIR_MASS));

        float displacement = (Float) ConfigField.getValue(ci, Fields.DISPLACEMENT);
        int cylinderCount = (int) ConfigField.getValue(ci, Fields.CYLINDERSCOUNT);
        String cylinderDisplacement = oneDecimal(displacement / cylinderCount);
        String injectorFlow = oneDecimal((float)ConfigField.getValue(ci, Fields.INJECTOR_FLOW));

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

        String injTime = "$Injection_Time (ms) = \\frac{" +
                "($Airmass = " + chargeAirMass + ")" +
                "}{" +
                "(TargetAFR (" + rpm_map + ") = " + TARGET_AFR + ")" +
                " * (injectorFlow = " + injectorFlow + " cc/min)" +
                "} = " + "yyy" + "ms$";

        String newLine = "\r\n \\\\ ";
        String page = tCharge + newLine + newLine + newLine +
                mCharge + newLine + newLine + newLine +
                injTime + newLine;

        TeXFormula formula = new TeXFormula(page);
        TeXIcon icon = formula.createTeXIcon(TeXConstants.STYLE_DISPLAY, 20);

        BufferedImage image = new BufferedImage(icon.getIconWidth(), icon.getIconHeight(), BufferedImage.TYPE_INT_ARGB);
        Graphics2D g2 = image.createGraphics();
        g2.setColor(Color.white);
        g2.fillRect(0, 0, icon.getIconWidth(), icon.getIconHeight());
        JLabel label = new JLabel(icon);

        centerProxy.removeAll();
        centerProxy.add(label, BorderLayout.CENTER);
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