package com.rusefi.xml;

import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBException;
import javax.xml.bind.Marshaller;
import java.io.FileWriter;
import java.io.IOException;
import java.io.StringWriter;

public class XmlUtil {

    public static final String FILE_NAME = "output.xml";

    public static void writeXml(ContentModel contentModel) throws JAXBException, IOException {
        JAXBContext jaxbContext = JAXBContext.newInstance(ContentModel.class);

        Marshaller marshaller = jaxbContext.createMarshaller();

        StringWriter xmlWriter = new StringWriter();
        marshaller.marshal(contentModel, xmlWriter);
        System.out.println(xmlWriter.toString());

        System.out.println("Writing " + FILE_NAME);
        marshaller.marshal(contentModel, new FileWriter(FILE_NAME));
        System.out.println("Done " + FILE_NAME);
    }
}
