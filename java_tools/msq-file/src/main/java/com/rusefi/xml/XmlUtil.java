package com.rusefi.xml;

import com.devexperts.logging.Logging;

import jakarta.xml.bind.JAXBContext;
import jakarta.xml.bind.JAXBException;
import jakarta.xml.bind.Marshaller;
import jakarta.xml.bind.Unmarshaller;
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
    }
}

