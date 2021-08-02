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

import com.romraider.logger.ecu.definition.EcuDefinition;
import com.romraider.logger.ecu.definition.EcuDefinitionImpl;
import static com.romraider.util.ParamChecker.isNullOrEmpty;

import org.apache.log4j.Logger;
import org.xml.sax.Attributes;
import org.xml.sax.helpers.DefaultHandler;

import java.io.File;
import java.util.HashMap;
import java.util.Map;

/*
<rom base="16BITBASE">
  <romid>
    <xmlid>CAL ID:A4TC300K</xmlid>
    <internalidaddress>200</internalidaddress>
    <internalidstring>A4TC300K</internalidstring>
    <year>03</year>
    <transmission>AT</transmission>
    <ecuid>3614446205</ecuid>
  </romid>
</rom>
*/
public final class EcuDefinitionHandler extends DefaultHandler {
    private static final Logger LOGGER = Logger.getLogger(EcuDefinitionHandler.class);
    private static final String TAG_ROM    	 = "rom";
    private static final String TAG_ROMID    = "romid";
    private static final String TAG_CALID    = "internalidstring";
    private static final String TAG_ECUID    = "ecuid";
    private static final String TAG_CASEID   = "caseid";
    private static final String TAG_ADDRESS  = "internalidaddress";
    private static final String TAG_YEAR     = "year";
    private static final String TAG_MARKET   = "market";
    private static final String TAG_MAKE     = "make";
    private static final String TAG_MODEL    = "model";
    private static final String TAG_SUBMODEL = "submodel";
    private static final String TAG_TRANS    = "transmission";
    private static final String TAG_MEMMODEL = "memmodel";
    private static final String TAG_FLASH    = "flashmethod";
    private static final String TAG_SIZE     = "filesize";
    private static final String TAG_OBSOLETE = "obsolete";
    private static final String ATTR_BASE    = "base";
    private Map<String, EcuDefinition> ecuDefinitionMap = new HashMap<String, EcuDefinition>();
    private String calId;
    private String ecuId;
    private String caseId;
    private String address;
    private String year;
    private String market;
    private String make;
    private String model;
    private String submodel;
    private String transmission;
    private String memmodel;
    private String flashmethod;
    private String filesize;
    private String obsolete;
    private String inherit;
    private String carString;
    private StringBuilder charBuffer;
    private File ecuDefsFile;

    public EcuDefinitionHandler(File ecuDefsFile) {
        this.ecuDefsFile = ecuDefsFile;
    }

    public void startDocument() {
        ecuDefinitionMap = new HashMap<String, EcuDefinition>();
    }

    public void startElement(String uri, String localName, String qName, Attributes attributes) {
        if (TAG_ROM.equals(qName)) {
            inherit = attributes.getValue(ATTR_BASE);
        }
        else if (TAG_ROMID.equals(qName)) {
            calId        = "";
            ecuId        = "";
            caseId       = "";
            address      = "";
            year         = "";
            market       = "";
            make         = "";
            model        = "";
            submodel     = "";
            transmission = "";
            memmodel     = "";
            flashmethod  = "";
            filesize     = "";
            obsolete     = "0";
            carString    = "";
        }
        charBuffer = new StringBuilder();
    }

    public void characters(char[] ch, int start, int length) {
        charBuffer.append(ch, start, length);
    }

    public void endElement(String uri, String localName, String qName) {
        if (TAG_ROM.equals(qName)) {
            inherit = null;
        }
        else if (TAG_ROMID.equals(qName)) {
            if (!isNullOrEmpty(ecuId)    && 
                !isNullOrEmpty(calId)    &&
                !isNullOrEmpty(year)     &&
                !isNullOrEmpty(market)   &&
                !isNullOrEmpty(make)     &&
                !isNullOrEmpty(model)    &&
                !isNullOrEmpty(submodel) &&
                !isNullOrEmpty(transmission)
                ) {
                carString = String.format("%s %s %s %s %s %s",
                        year, market, make, model, submodel, transmission);
                ecuDefinitionMap.put(ecuId,
                        new EcuDefinitionImpl(
                                ecuId, calId, carString, inherit, ecuDefsFile));
            }
            if (!isNullOrEmpty(ecuId)    && 
                !isNullOrEmpty(calId)    &&
                !isNullOrEmpty(address)  &&
                !isNullOrEmpty(year)     &&
                !isNullOrEmpty(market)   &&
                !isNullOrEmpty(make)     &&
                !isNullOrEmpty(model)    &&
                !isNullOrEmpty(submodel) &&
                !isNullOrEmpty(transmission) &&
                !isNullOrEmpty(memmodel) &&
                !isNullOrEmpty(flashmethod) &&
                !isNullOrEmpty(obsolete)
                ) {
                LOGGER.debug(romDetail());
            }
        }
        else if (TAG_CALID.equals(qName)) {
            calId = charBuffer.toString();
        }
        else if (TAG_ECUID.equals(qName)) {
            ecuId = charBuffer.toString();
        }
        else if (TAG_CASEID.equals(qName)) {
            caseId = charBuffer.toString();
        }
        else if (TAG_ADDRESS.equals(qName)) {
            address = charBuffer.toString();
        }
        else if (TAG_YEAR.equals(qName)) {
            year = charBuffer.toString();
            if (!year.isEmpty()) {
                try {
                    if (Integer.parseInt(year) < 90) {
                        year = "20" + year;
                    }
                   }
                catch (NumberFormatException e) {
                    if ((year.contains("/") || year.contains("-")) &&
                            year.length() < 6 )
                        year = "20" + year;                
                }
            }
            else {
                year = "20xx";
            }
        }
        else if (TAG_MARKET.equals(qName)) {
            market = charBuffer.toString();
        }
        else if (TAG_MAKE.equals(qName)) {
            make = charBuffer.toString();
        }
        else if (TAG_MODEL.equals(qName)) {
            model = charBuffer.toString();
        }
        else if (TAG_SUBMODEL.equals(qName)) {
            submodel = charBuffer.toString();
        }
        else if (TAG_TRANS.equals(qName)) {
            transmission = charBuffer.toString();
        }
        else if (TAG_MEMMODEL.equals(qName)) {
            memmodel = charBuffer.toString();
        }
        else if (TAG_FLASH.equals(qName)) {
            flashmethod = charBuffer.toString();
        }
        else if (TAG_SIZE.equals(qName)) {
            filesize = charBuffer.toString();
        }
        else if (TAG_OBSOLETE.equals(qName)) {
            obsolete = charBuffer.toString();
        }
    }

    public Map<String, EcuDefinition> getEcuDefinitionMap() {
        return ecuDefinitionMap;
    }

    public String romDetail() {
        return String.format(
            "calid='%s'," +
            "address='%s'," +
            "string='%s'," +
            "caseid='%s'," +
            "year='%s'," +
            "market='%s'," +
            "make='%s'," +
            "model='%s'," +
            "submodel='%s'," +
            "transmission='%s'," +
            "memmodel='%s'," +
            "flash='%s'," +
            "filesize='%s'," +
            "obsolete='%s'," +
            "inherit='%s'" +
            "file='%s'",
            calId,
            address,
            calId,
            caseId,
            year,
            market,
            make,
            model,
            submodel,
            transmission,
            memmodel,
            flashmethod,
            filesize ,
            obsolete,
            inherit,
            ecuDefsFile.getName()
        );
    }
}
