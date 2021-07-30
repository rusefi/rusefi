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

import java.io.FileInputStream;
import java.io.IOException;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Document;
import org.xml.sax.SAXException;

import com.romraider.logger.ecu.definition.EcuDefinition;

/**
 *  Parse a given XML definition file into a DOM.
 */
public class EcuDefinitionDocumentLoader {

    private EcuDefinitionDocumentLoader() {
    }

    /**
     *  Parse a given XML definition file into a DOM.
     *  @param ecuDef - an ECU Definition containing a File to parse.
     *  @return a DOM.
     */
    public static final Document getDocument(EcuDefinition ecuDef) {
        final DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
        DocumentBuilder builder = null;
        try {
            builder = dbf.newDocumentBuilder();
        }
        catch (ParserConfigurationException e) {
            e.printStackTrace();  
        }

        Document document = null;
        try {
            document = builder.parse(
                    new FileInputStream(ecuDef.getEcuDefFile()));
        }
        catch (SAXException e) {
            e.printStackTrace();
        }
        catch (IOException e) {
            e.printStackTrace();
        }
        return document;
    }
}
