package com.rusefi.pcb;

import java.util.List;

/**
 * @author Andrey Belomutskiy
 *         1/21/14
 */
public class ModuleNode extends PcbNode {
    final List<PadNode> pads;
    public final PointNode at;
    private final String reference;

    public ModuleNode(String nodeName, int i, List<Object> children) {
        super(nodeName, i, children);
        Object o = iterate("pad");
        pads = (List<PadNode>) o;
        at = (PointNode) find("at");

        reference = iterate("fp_text").get(0).getChild(1);
    }

    public String getReference() {
        return reference;
    }

    @Override
    public String toString() {
        return "ModuleNode{" +
                "reference=" + reference +
                ", pads.size=" + pads.size() +
                '}';
    }

    @Override
    public boolean isConnected(PointNode point) {
        PointNode offsetPoint = at.translate(point);

        for (PadNode pad : pads) {
            if (pad.isConnected(offsetPoint))
                return true;
        }
        return false;
    }
}
