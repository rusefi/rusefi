package com.rusefi.xml;

import com.devexperts.logging.Logging;

import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBException;
import javax.xml.bind.Marshaller;
import javax.xml.bind.Unmarshaller;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.StringWriter;

import static com.devexperts.logging.Logging.getLogging;

public class XmlUtil {
    private static final Logging log = getLogging(XmlUtil.class);

    static {
        XmlUtil.setParserImpl();
    }

    public static void writeXml(Object instance, Class<?> modelClass, String fileName) throws JAXBException, IOException {
        JAXBContext jaxbContext = JAXBContext.newInstance(modelClass);

        Marshaller marshaller = jaxbContext.createMarshaller();
        marshaller.setProperty(Marshaller.JAXB_FORMATTED_OUTPUT, true);

        StringWriter xmlWriter = new StringWriter();
        marshaller.marshal(instance, xmlWriter);

        log.info("Writing " + fileName);
        try (FileWriter writer = new FileWriter(fileName)) {
            marshaller.marshal(instance, writer);
        } finally {
            log.info("Marshalling finished " + fileName);
        }
    }

    public static <T> T readModel(Class<?> modelClass, File xmlFile) throws JAXBException {
        log.info("Reading " + xmlFile.getAbsolutePath());
        JAXBContext jaxbContext;
        jaxbContext = JAXBContext.newInstance(modelClass);
        Unmarshaller jaxbUnmarshaller = jaxbContext.createUnmarshaller();
        try {
            return (T) jaxbUnmarshaller.unmarshal(xmlFile);
        } catch (Throwable e) {
            throw new IllegalStateException("While reading " + xmlFile.getAbsolutePath(), e);
        }
    }

    /**
     * See https://stackoverflow.com/questions/25644023/error-unmarshalling-xml-in-java-8-secure-processing-org-xml-sax-saxnotrecognize
     */
    private static void setParserImpl() {
        System.setProperty("org.xml.sax.driver", "com.sun.org.apache.xerces.internal.parsers.SAXParser");
        System.setProperty("javax.xml.parsers.DocumentBuilderFactory","com.sun.org.apache.xerces.internal.jaxp.DocumentBuilderFactoryImpl");
        System.setProperty("javax.xml.parsers.SAXParserFactory","com.sun.org.apache.xerces.internal.jaxp.SAXParserFactoryImpl");
    }
}
