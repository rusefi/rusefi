package com.rusefi.pcb;

import java.util.List;

/**
 * (c) Andrey Belomutskiy
 * 2/11/14.
 */
public class ZoneNode extends PcbNode {
    private final LayerNode layerNode;

    public ZoneNode(String nodeName, int i, List<Object> children) {
        super(nodeName, i, children);
        layerNode = (LayerNode) find("layer");
    }

    public LayerNode getLayerNode() {
        return layerNode;
    }
}
