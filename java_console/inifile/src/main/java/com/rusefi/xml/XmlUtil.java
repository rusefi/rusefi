package com.rusefi.xml;

import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBException;
import javax.xml.bind.Marshaller;
import javax.xml.bind.Unmarshaller;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.StringWriter;

public class XmlUtil {
    static {
        XmlUtil.setParserImpl();
    }

    public static void writeXml(Object instance, Class<?> modelClass, String fileName) throws JAXBException, IOException {
        JAXBContext jaxbContext = JAXBContext.newInstance(modelClass);

        Marshaller marshaller = jaxbContext.createMarshaller();
        marshaller.setProperty(Marshaller.JAXB_FORMATTED_OUTPUT, true);

        StringWriter xmlWriter = new StringWriter();
        marshaller.marshal(instance, xmlWriter);
        System.out.println(xmlWriter.toString());

        System.out.println("Writing " + fileName);
        FileWriter writer = new FileWriter(fileName);
        marshaller.marshal(instance, writer);
        System.out.println("Marshalling finished " + fileName);
        writer.close();
    }

    public static <T> T readModel(Class<?> modelClass, String fileName) throws Exception {
        File xmlFile = new File(fileName);
        JAXBContext jaxbContext;
        jaxbContext = JAXBContext.newInstance(modelClass);
        Unmarshaller jaxbUnmarshaller = jaxbContext.createUnmarshaller();
        return (T) jaxbUnmarshaller.unmarshal(xmlFile);
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
