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

//ECU version definition

package com.romraider.maps;

import java.io.Serializable;

public class RomID implements Serializable {

    private static final long serialVersionUID = 7272741253665400643L;
    private String xmlid;               //ID stored in XML
    private int    internalIdAddress;   //address of ECU version in image
    private String internalIdString;    //ID stored in image
    private String caseId;              //ECU hardware version
    private String ecuId;
    private String make;                //manufacturer
    private String market;
    private String model;
    private String subModel;            //trim, ie WRX
    private String transmission;
    private String year = "Unknown";
    private String flashMethod;         //flash method string used for ecuflash
    private String memModel;            //model used for reflashing with ecuflash
    private String editStamp;           //YYYY-MM-DD and v, the save count for this ROM
    private int    fileSize;
    private int    ramOffset;
    private boolean obsolete;           // whether a more recent revision exists

    public String toString() {
        return String.format(
                "%n   ---- RomID %s ----" +
                "%n   Internal ID Address: %s" +
                "%n   Internal ID String: %s" +
                "%n   Case ID: %s" +
                "%n   ECU ID: %s" +
                "%n   Make: %s" +
                "%n   Market: %s" +
                "%n   Model: %s" +
                "%n   Submodel: %s" +
                "%n   Transmission: %s" +
                "%n   Year: %s" +
                "%n   Flash Method: %s" +
                "%n   Memory Model: %s" +
                "%n   ---- End RomID %s ----",
                xmlid,
                internalIdAddress,
                internalIdString,
                caseId,
                ecuId,
                make,
                market,
                model,
                subModel,
                transmission,
                year,
                flashMethod,
                memModel,
                xmlid);
    }

    public RomID() {
        this.internalIdString = "";
        this.caseId = "";
    }

    public String getXmlid() {
        return xmlid;
    }

    public void setXmlid(String xmlid) {
        this.xmlid = xmlid;
    }

    public int getInternalIdAddress() {
        return internalIdAddress;
    }

    public void setInternalIdAddress(int internalIdAddress) {
        this.internalIdAddress = internalIdAddress;
    }

    public String getInternalIdString() {
        return internalIdString;
    }

    public void setInternalIdString(String internalIdString) {
        this.internalIdString = internalIdString;
    }

    public String getCaseId() {
        return caseId;
    }

    public void setCaseId(String caseId) {
        this.caseId = caseId;
    }

    public String getEcuId() {
        return ecuId;
    }

    public void setEcuId(String ecuId) {
        this.ecuId = ecuId;
    }

    public String getMake() {
        return make;
    }

    public void setMake(String make) {
        this.make = make;
    }

    public String getMarket() {
        return market;
    }

    public void setMarket(String market) {
        this.market = market;
    }

    public String getModel() {
        return model;
    }

    public void setModel(String model) {
        this.model = model;
    }

    public String getSubModel() {
        return subModel;
    }

    public void setSubModel(String subModel) {
        this.subModel = subModel;
    }

    public String getTransmission() {
        return transmission;
    }

    public void setTransmission(String transmission) {
        this.transmission = transmission;
    }

    public String getYear() {
        return year;
    }

    public void setYear(String year) {
        this.year = year;
    }

    public String getFlashMethod() {
        return flashMethod;
    }

    public void setFlashMethod(String flashMethod) {
        this.flashMethod = flashMethod;
    }

    public String getMemModel() {
        return memModel;
    }

    public void setMemModel(String memModel) {
        this.memModel = memModel;
    }

    public int getRamOffset() {
        return ramOffset;
    }

    public void setRamOffset(int ramOffset) {
        this.ramOffset = ramOffset;
    }

    public int getFileSize() {
        return fileSize;
    }

    public void setFileSize(int fileSize) {
        this.fileSize = fileSize;
    }

    public boolean isObsolete() {
        return obsolete;
    }

    public void setObsolete(boolean obsolete) {
        this.obsolete = obsolete;
    }

    public String getEditStamp() {
        return editStamp;
    }

    public void setEditStamp(String editStamp) {
        this.editStamp = editStamp;
    }
}