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

    public static void writeXml(Object instance, Class<?> modelClass, String fileName) throws JAXBException, IOException {
        JAXBContext jaxbContext = JAXBContext.newInstance(modelClass);

        Marshaller marshaller = jaxbContext.createMarshaller();

        StringWriter xmlWriter = new StringWriter();
        marshaller.marshal(instance, xmlWriter);
        System.out.println(xmlWriter.toString());

        System.out.println("Writing " + fileName);
        marshaller.marshal(instance, new FileWriter(fileName));
        System.out.println("Done " + fileName);
    }

    public static <T> T readModel(Class<?> modelClass, String fileName) throws Exception {
        File xmlFile = new File(fileName);
        JAXBContext jaxbContext;
        jaxbContext = JAXBContext.newInstance(modelClass);
        Unmarshaller jaxbUnmarshaller = jaxbContext.createUnmarshaller();
        return (T) jaxbUnmarshaller.unmarshal(xmlFile);
    }
}
