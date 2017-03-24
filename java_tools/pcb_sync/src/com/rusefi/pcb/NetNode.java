package com.rusefi.pcb;

import java.util.List;

/**
 * @author Andrey Belomutskiy
 *         1/21/14
 */
public class NetNode extends PcbNode {
    public final String id;
    final String name;
    public static int GND_NET_ID;

    public NetNode(String nodeName, int i, List<Object> children) {
        super(nodeName, i, children);
        id = getChild(0);
        name = children.size() > 1 ? getChild(1) : null;
        if (name != null)
            System.out.println("NetNode(" + name + " network: " + id + ")");

        if ("GND".equalsIgnoreCase(name))
            GND_NET_ID = Integer.parseInt(id);
    }

    @Override
    public String toString() {
        return "NetNode{" +
                "id='" + id + '\'' +
                '}';
    }
}
