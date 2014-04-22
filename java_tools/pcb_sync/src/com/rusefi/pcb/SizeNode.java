package com.rusefi.pcb;

import java.util.List;

/**
 * @author Andrey Belomutskiy
 *         1/21/14
 */
public class SizeNode extends PcbNode {
    public final double w;
    public final double h;

    public SizeNode(String nodeName, int i, List<Object> children) {
        super(nodeName, i, children);
        if (children.size() == 1) {
            w = h = Double.parseDouble((String) children.get(0));
            return;
        }

        if (children.size() != 2)
            throw new IllegalStateException("Size: " + children.size());
        w = Double.parseDouble((String) children.get(0));
        h = Double.parseDouble((String) children.get(1));
    }

    @Override
    public String toString() {
        return "SizeNode{" +
                "w=" + w +
                ", h=" + h +
                '}';
    }
}
