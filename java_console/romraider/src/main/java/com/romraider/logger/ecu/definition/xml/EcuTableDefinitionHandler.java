/*
 * RomRaider Open-Source Tuning, Logging and Reflashing
 * Copyright (C) 2006-2013 RomRaider.com
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

package com.romraider.logger.ecu.definition.xml;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathConstants;
import javax.xml.xpath.XPathExpressionException;
import javax.xml.xpath.XPathFactory;

import org.w3c.dom.Document;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;

/**
 *  Parse an ECU Definition XML file and return the attributes of the
 *  table name supplied. 
 */
public final class EcuTableDefinitionHandler {

    private EcuTableDefinitionHandler() {
    }

    /**
     *  Parse a given ECU Definition XML file and return the attributes of the
     *  table name supplied.
     *  @param document - W3C DOM Document of an ECU definition XML file.
     *  @param inheritanceNodeList - a List of Nodes of the 'rom' parents
     *  to the CAL ID supplied.
     *  @param tableName - the name of the table to find.
     *  @return a Map of table and scaling name/value attributes.  
     */
    public static final Map<String, String> getTableDefinition(
            Document document,
            List<Node> inheritanceNodeList,
            String tableName) {

        final Map<String, String> table =
                new HashMap<String, String>();
        for (Node inheritIdNode : inheritanceNodeList) {
            get2dTableMap(inheritIdNode, tableName, table);
        }
        return table;
    }

    /**
     * For the given Node and Table Name populate a Map with the table's
     * attributes and the scaling attributes.
     * @param xpath - the XPath evaluation environment. 
     * @param node - the 'rom' Node to root the search within.
     * @param tableName - the Table Name to locate.
     * @param tableMap2d - a Map to populate with attribute names and values.
     */
    private static final void get2dTableMap(
            Node node, String tableName,
            Map<String, String> tableMap2d) {

        final XPathFactory xpf = XPathFactory.newInstance();
        final XPath xpath = xpf.newXPath();
        try {
            final Node tableNode = (Node) xpath.evaluate(
                    "table[@name='" + tableName + "']",
                    node, XPathConstants.NODE);
            addAttributesAsMap(tableNode, tableMap2d);

            final Node scalingNode = (Node) xpath.evaluate(
                    "scaling",
                    tableNode, XPathConstants.NODE);
            addAttributesAsMap(scalingNode, tableMap2d);
        }
        catch (NullPointerException e) {
        }
        catch (XPathExpressionException e) {
            e.printStackTrace();
        }
    }

    /**
     * Convert XML attributes to a Map.
     * @param node - the Node containing the attributes.
     * @param tableMap -  the Map to populate with attribute names and values.
     */
    private static final void addAttributesAsMap(
            Node node, Map<String, String> tableMap) {

        final NamedNodeMap attrs = node.getAttributes();
        for (int i = 0; i < attrs.getLength(); i++) {
            tableMap.put(
                    attrs.item(i).getNodeName(),
                    attrs.item(i).getNodeValue());
        }
    }
}
