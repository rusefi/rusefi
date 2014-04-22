package com.rusefi.pcb;

import java.util.List;

/**
 * @author Andrey Belomutskiy
 *         2/7/14.
 */
public class LayerNode extends PcbNode {
    public final String name;

    public LayerNode(String nodeName, int closingIndex, List<Object> children) {
        super(nodeName, closingIndex, children);
        name = (String) children.get(0);
    }

    boolean isSikscreenLayer() {
        return name.equals("B.SilkS") || name.equals("F.SilkS");
    }
}
