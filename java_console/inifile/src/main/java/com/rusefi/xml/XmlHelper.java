package com.rusefi.xml;

import com.devexperts.logging.Logging;
import com.rusefi.tune.xml.*;
import org.w3c.dom.*;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import java.io.File;

import static com.devexperts.logging.Logging.getLogging;

public enum XmlHelper {
    INSTANCE;

    private static final Logging log = getLogging(XmlHelper.class);

    private final DocumentBuilderFactory documentBuilderFactory = DocumentBuilderFactory.newInstance();
    private final TransformerFactory transformerFactory = TransformerFactory.newInstance();

    static final String MSQ_ELEMENT_NAME = "msq";
    static final String PAGE_ELEMENT_NAME = "page";
    static final String VERSION_INFO_ELEMENT_NAME = "versionInfo";
    static final String BIBLIOGRAPHY_ELEMENT_NAME = "bibliography";
    static final String SETTINGS_ELEMENT_NAME = "settings";
    static final String USER_COMMENTS_ELEMENT_NAME = "userComments";
    static final String FIRMWARE_INFO_ATTRIBUTE_NAME = "firmwareInfo";
    static final String N_PAGES_ATTRIBUTE_NAME = "nPages";
    static final String SIGNATURE_ATTRIBUTE_NAME = "signature";
    static final String VERSION_ATTRIBUTE_NAME = "version";
    static final String AUTHOR_ATTRIBUTE_NAME = "author";
    static final String TUNE_COMMENT_ATTRIBUTE_NAME = "tuneComment";
    static final String WRITE_DATE_ATTRIBUTE_NAME = "writeDate";

    public Msq readFromFile(final File xmlFile) throws Exception {
        final DocumentBuilder builder = documentBuilderFactory.newDocumentBuilder();
        final Document doc = builder.parse(xmlFile);
        //doc.getDocumentElement().normalize();
        return readMsq(doc);
    }

    public void writeToFile(final Msq msq, final File xmlFile) throws Exception {
        final DocumentBuilder builder = documentBuilderFactory.newDocumentBuilder();
        final Document doc = builder.newDocument();
        doc.setXmlVersion("1.0");
        doc.setXmlStandalone(true);

        writeMsq(msq, doc);

        final DOMSource dom = new DOMSource(doc);
        final Transformer transformer = transformerFactory.newTransformer();
        final StreamResult streamResult = new StreamResult(xmlFile);
        transformer.transform(dom, streamResult);
    }

    private Msq readMsq(final Document doc) {
        final Msq result = new Msq();
        final Element msqElement = doc.getDocumentElement();
        if (!MSQ_ELEMENT_NAME.equals(msqElement.getTagName())) {
            throw new RuntimeException("XML document root element is not `msq`");
        }
        readMsq(msqElement, result);
        return result;
    }

    private void readMsq(final Element msqElement, final Msq targetMsq) {
        final NodeList childNodes = msqElement.getChildNodes();
        for (int i = 0; i < childNodes.getLength(); i ++) {
            final Node childNode = childNodes.item(i);
            if (childNode.getNodeType() == Node.ELEMENT_NODE) {
                final Element childElement = (Element) childNode;
                switch (childElement.getTagName()) {
                    case PAGE_ELEMENT_NAME: {
                        targetMsq.page.add(readPage(childElement));
                        break;
                    }
                    case VERSION_INFO_ELEMENT_NAME: {
                        readVersionInfo(childElement, targetMsq.getVersionInfo());
                        break;
                    }
                    case BIBLIOGRAPHY_ELEMENT_NAME: {
                        readBibliography(childElement, targetMsq.bibliography);
                        break;
                    }
                    case SETTINGS_ELEMENT_NAME: {
                        readSettings(childElement, targetMsq.getSettings());
                        break;
                    }
                    case USER_COMMENTS_ELEMENT_NAME: {
                        readUserComments(childElement, targetMsq.getUserComments());
                        break;
                    }
                    default: {
                        break; // ignore
                    }
                }
            }
        }
    }

    private Page readPage(final Element pageElement) {
        final Page result = new Page();
        return result;
    }

    private void readVersionInfo(final Element versionInfoElement, final VersionInfo targetVersionInfo) {
        final Attr firmwareInfoAttribute = versionInfoElement.getAttributeNode(FIRMWARE_INFO_ATTRIBUTE_NAME);
        if (firmwareInfoAttribute != null) {
            targetVersionInfo.setFirmwareInfo(firmwareInfoAttribute.getValue());
        }
        final Attr signatureAttribute = versionInfoElement.getAttributeNode(SIGNATURE_ATTRIBUTE_NAME);
        if (signatureAttribute != null) {
            targetVersionInfo.setSignature(signatureAttribute.getValue());
        }
    }

    private void readBibliography(final Element bibliographyElement, final Bibliography targetBibliography) {
        final Attr tuneCommentAttribute = bibliographyElement.getAttributeNode(TUNE_COMMENT_ATTRIBUTE_NAME);
        if (tuneCommentAttribute != null) {
            targetBibliography.setTuneComment(tuneCommentAttribute.getValue());
        }
    }

    private void readSettings(final Element seetingsElement, final Settings settings) {
    }

    private void readUserComments(final Element userCommentsElement, final UserComments userComments) {
    }

    private void writeMsq(final Msq msq, final Document doc) {
        final Element msqElement = doc.createElementNS("http://www.msefi.com/:msq", MSQ_ELEMENT_NAME);
        msq.page.forEach(page -> writePage(msqElement, page));
        writeVersionInfo(msqElement, msq.getVersionInfo());
        writeBibliography(msqElement, msq.bibliography);
        writeSettings(msqElement, msq.getSettings());
        writeUserComments(msqElement, msq.getUserComments());
    }

    private void writePage(final Element msqElement, final Page page) {
        final Element pageElement = createChildElement(msqElement, PAGE_ELEMENT_NAME);
        //TODO: implement
    }

    private void writeVersionInfo(final Element msqElement, final VersionInfo versionInfo) {
        final Element versionInfoElement = createChildElement(msqElement, VERSION_INFO_ELEMENT_NAME);
        versionInfoElement.setAttribute(FIRMWARE_INFO_ATTRIBUTE_NAME, versionInfo.getFirmwareInfo());
        versionInfoElement.setAttribute(N_PAGES_ATTRIBUTE_NAME, versionInfo.getNPages());
        versionInfoElement.setAttribute(SIGNATURE_ATTRIBUTE_NAME, versionInfo.getSignature());
        versionInfoElement.setAttribute(VERSION_ATTRIBUTE_NAME, versionInfo.getVersion());
    }

    private void writeBibliography(final Element msqElement, final Bibliography bibliography) {
        final Element bibliographyElement = createChildElement(msqElement, BIBLIOGRAPHY_ELEMENT_NAME);
        bibliographyElement.setAttribute(AUTHOR_ATTRIBUTE_NAME, bibliography.getAuthor());
        final String tuneComment = bibliography.getTuneComment();
        if (tuneComment != null) {
            bibliographyElement.setAttribute(TUNE_COMMENT_ATTRIBUTE_NAME, tuneComment);
        }
        bibliographyElement.setAttribute(WRITE_DATE_ATTRIBUTE_NAME, bibliography.getWriteDate());
    }

    private void writeSettings(final Element msqElement, final Settings settings) {
        final Element settingsElement = createChildElement(msqElement, SETTINGS_ELEMENT_NAME);
    }

    private void writeUserComments(final Element msqElement, final UserComments userComments) {
        final Element userCommentsElement = createChildElement(msqElement, USER_COMMENTS_ELEMENT_NAME);
    }

    private Element createChildElement(final Element parentElement, final String tagName) {
        final Element childElement = parentElement.getOwnerDocument().createElement(tagName);
        parentElement.appendChild(childElement);
        return childElement;
    }
}
