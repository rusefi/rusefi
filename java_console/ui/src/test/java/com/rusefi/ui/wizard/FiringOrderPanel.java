package com.rusefi.ui.wizard;

import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.ui.UIContext;

import javax.swing.*;
import java.awt.*;
import java.util.*;
import java.util.stream.Collectors;

public class FiringOrderPanel {
    private final JPanel content = new JPanel(new BorderLayout());
    private static final Map<String, String> EXAMPLES = new HashMap<String, String>() {{
        put("1_3_4_2", "typical inline 4");
        put("1_3_2_4", "horizontally opposed engine");
        put("1_4_3_2", "VW aircooled boxer engine");
        put("1_5_3_6_2_4", "VAG VR6, Straight-6, Opel Omega A");
        put("1_6_5_4_3_2", "GM 3800 engine");
        put("1_4_5_2_3_6", "Chevrolet Corvair");
        put("1_4_2_5_3_6", "Mercedes-Benz M104 engine");
        put("1_2_3_4_5_6", "all Nissan v6, GM 60-Degree V6 engine");
        put("1_6_3_2_5_4", "Subaru EG33");
        put("1_4_3_6_2_5", "VAG v6 different from VAG VR6");
        put("1_6_2_4_3_5", "Some 911");
        put("1_8_4_3_6_5_7_2", "SBC, Dodge");
        put("1_8_7_2_6_5_4_3", "GM Gen. 3, 4, 5 LT1");
        put("1_5_4_2_6_3_7_8", "Ford Mustang");
        put("1_3_7_2_6_5_4_8", "Ford 5.0 HO and 351W");
        put("1_2_3_4_5_6_7_8", "linearly incrementing, for V8 testing");
        put("1_5_4_8_6_3_7_2", "Audi 4.2 40v V8");
        put("1_5_4_8_3_7_2_6", "Ford Voodoo");
        put("1_8_7_3_6_5_4_2", "VH41DE (Japanese Y32 Variant)");
        put("1_8_6_2_7_3_4_5", "Ferrari-Maserati F136");
        put("1_10_9_4_3_6_5_8_7_2", "dodge and viper ram v10");
        put("1_6_5_10_2_7_3_8_4_9", "BMW S85, also Audi 5.2 R8 and Huracan GDI. Ford Triton 6.8L V10");
        put("1_7_5_11_3_9_6_12_2_8_4_10", "BMW M70 & M73, Ferrari 456M GT V12");
        put("1_7_4_10_2_8_6_12_3_9_5_11", "Lamborghini Diablo VT, typical rusEfi use-case");
        put("1_12_5_8_3_10_6_7_2_11_4_9", "VAG W12, M120");
        put("1_2_3_4_5_6_7_8_9_10_11_12", "mostly for hardware testing purposes");
        put("1_14_9_4_7_12_15_6_13_8_3_16_11_2_5_10", "WR16");
    }};

    public FiringOrderPanel(UIContext uiContext, int cylindersCount) {

        JLabel title = new JLabel("Select Firing Order for " + cylindersCount + " cylinders");
        title.setHorizontalAlignment(SwingConstants.CENTER);
        scaleComponent(title, 2);
        content.add(title, BorderLayout.NORTH);

        JPanel ordersPanel = new JPanel();
        ordersPanel.setLayout(new BoxLayout(ordersPanel, BoxLayout.Y_AXIS));

        IniField firingOrderField = uiContext.iniFileState.getIniFileModel().findIniField("firingOrder").orElse(null);
        if (firingOrderField instanceof EnumIniField) {
            EnumIniField enumField = (EnumIniField) firingOrderField;
            Collection<String> allOrders = enumField.getEnums().values();

            // Filter firing orders by counting the number of digits/elements in the string
            // Firing orders are like "1-3-4-2"
            Collection<String> matchingOrders = allOrders.stream()
                .filter(order -> {
                    if (order == null || order.equalsIgnoreCase("INVALID") || order.isEmpty()) return false;
                    String[] parts = order.split("[-_ ]");
                    int count = 0;
                    for (String part : parts) {
                        if (part.matches("\\d+")) {
                            count++;
                        }
                    }
                    // Fallback for names like "One Cylinder" or "1-2"
                    if (count == 0 && order.equalsIgnoreCase("One Cylinder") && cylindersCount == 1) return true;

                    return count == cylindersCount;
                })
                .collect(Collectors.toList());

            for (String order : matchingOrders) {
                String label = order;
                String exampleKey = order.replace("-", "_");
                if (EXAMPLES.containsKey(exampleKey)) {
                    label += " (" + EXAMPLES.get(exampleKey) + ")";
                }
                JButton button = new JButton(label);
                scaleComponent(button, 2);
                button.setAlignmentX(Component.CENTER_ALIGNMENT);
                button.addActionListener(e -> {
                    System.out.println("Selected firing order: " + order);
                });
                ordersPanel.add(Box.createVerticalStrut(10));
                ordersPanel.add(button);
            }
        } else {
            ordersPanel.add(new JLabel("Firing order information not found in INI"));
        }

        content.add(new JScrollPane(ordersPanel), BorderLayout.CENTER);
    }


    private void scaleComponent(JComponent component, float factor) {
        Font font = component.getFont();
        component.setFont(font.deriveFont(font.getSize() * factor));
    }

    public JComponent getPanel() {
        return content;
    }
}
