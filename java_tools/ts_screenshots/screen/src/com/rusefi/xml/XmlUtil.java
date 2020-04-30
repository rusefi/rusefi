package com.rusefi.xml;

import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBException;
import javax.xml.bind.Marshaller;
import java.io.FileWriter;
import java.io.IOException;
import java.io.StringWriter;

public class XmlUtil {
    public static void writeXml(Content content) throws JAXBException, IOException {
        JAXBContext jaxbContext = JAXBContext.newInstance(Content.class);

        Marshaller marshaller = jaxbContext.createMarshaller();

        StringWriter xmlWriter = new StringWriter();
        marshaller.marshal(content, xmlWriter);
        System.out.println(xmlWriter.toString());

        marshaller.marshal(content, new FileWriter("output.xml"));
    }
}
