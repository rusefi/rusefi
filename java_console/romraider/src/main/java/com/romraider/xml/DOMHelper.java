/*
 * RomRaider Open-Source Tuning, Logging and Reflashing
 * Copyright (C) 2006-2012 RomRaider.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

package com.romraider.xml;

import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

public final class DOMHelper {

    private DOMHelper() {
    }

    public static String unmarshallText(Node textNode) {
        StringBuffer buf = new StringBuffer();

        Node n;
        NodeList nodes = textNode.getChildNodes();

        for (int i = 0; i < nodes.getLength(); i++) {
            n = nodes.item(i);

            if (n.getNodeType() == Node.TEXT_NODE) {
                buf.append(n.getNodeValue());
            } else {
                // expected a text-only node (skip)
            }
        }
        return buf.toString();
    }

    public static String unmarshallAttribute(Node node, String name, String defaultValue) {
        Node n = node.getAttributes().getNamedItem(name);
        return (n != null) ? (n.getNodeValue()) : (defaultValue);
    }

    public static Double unmarshallAttribute(Node node, String name, double defaultValue) {
        return Double.parseDouble(unmarshallAttribute(node, name, String.valueOf(defaultValue)));
    }

    public static int unmarshallAttribute(Node node, String name, int defaultValue) {
        return Integer.parseInt(unmarshallAttribute(node, name, String.valueOf(defaultValue)));
    }

    public static boolean unmarshallAttribute(Node node, String name, boolean defaultValue) {
        return Boolean.parseBoolean(unmarshallAttribute(node, name, String.valueOf(defaultValue)));
    }

}
