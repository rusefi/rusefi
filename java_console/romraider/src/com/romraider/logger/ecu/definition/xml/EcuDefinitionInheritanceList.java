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

import java.util.ArrayList;
import java.util.List;

import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathConstants;
import javax.xml.xpath.XPathExpressionException;
import javax.xml.xpath.XPathFactory;

import org.w3c.dom.Document;
import org.w3c.dom.Node;

import com.romraider.logger.ecu.definition.EcuDefinition;

/**
 *  Get a List of ROM inheritance Nodes from the given DOM and ECU definition.
 */
public class EcuDefinitionInheritanceList {

    private EcuDefinitionInheritanceList() {
    }

    /**
     *  Get a List of ROM inheritance Nodes
     *  @param document - the DOM document containing the ECU definition and
     *                    its parents.
     *  @param ecuDef - ECU definition to start with.
     *  @return a List of Node starting with the given ECU listing back to the
     *  base definition. 
     */
    public static final List<Node> getInheritanceList(
            Document document,
            EcuDefinition ecuDef) {

        final XPathFactory xpf = XPathFactory.newInstance();
        final XPath xpath = xpf.newXPath();
        final List<String> inheritance = new ArrayList<String>();
        inheritance.add(ecuDef.getCalId());
        inheritance.add(ecuDef.getInherits());
        String base = ecuDef.getInherits();
        while (!base.contains("BASE")) {
            base = getBaseRomId(xpath, document, base);
            inheritance.add(base);
        }
        final List<Node> inheritanceNodes = new ArrayList<Node>();
        for (String inheritId : inheritance) {
            Node baseNode = getBaseRomIdNode(xpath, document, inheritId);
            inheritanceNodes.add(baseNode);
        }
        return inheritanceNodes;
    }

    /**
     * Get the CAL ID string of the base inheritance definition.
     * @param xpath - the XPath evaluation environment. 
     * @param document - W3C DOM Document of an ECU definition XML file.
     * @param calId - the known CAL ID to find the inheritance for.
     * @return the CAL ID from the attribute 'base' in the 'rom' element . 
     */
    private static final String getBaseRomId(
            XPath xpath, Document document, String calId) {

        String romBase = null;
        try {
            final Node calidNode = (Node) xpath.evaluate(
                    "/roms/rom/romid[xmlid='" + calId + "']",
                    document, XPathConstants.NODE);
            romBase = calidNode.getParentNode().getAttributes().item(0).
                    getNodeValue();
        }
        catch (XPathExpressionException e) {
            e.printStackTrace();
        }
        return romBase;
    }

    /**
     * Get the 'rom' Node which contains the CAL ID string.
     * @param xpath - the XPath evaluation environment. 
     * @param document - W3C DOM Document of an ECU definition XM file.
     * @param calId - the known CAL ID to find the inheritance for.
     * @return the Node of the 'rom' parent element to the CAL ID. 
     */
    private static final Node getBaseRomIdNode(
            XPath xpath, Document document, String calId) {

        Node romIdNode = null;
        try {
            romIdNode = (Node) xpath.evaluate(
                    "/roms/rom/romid[xmlid='" + calId + "']",
                    document, XPathConstants.NODE);
            romIdNode = romIdNode.getParentNode();
        }
        catch (XPathExpressionException e) {
            e.printStackTrace();
        }
        return romIdNode;
    }
}
