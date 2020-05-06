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

    public static ContentModel readModel() throws Exception {
        File xmlFile = new File(FILE_NAME);
        JAXBContext jaxbContext;
        jaxbContext = JAXBContext.newInstance(ContentModel.class);
        Unmarshaller jaxbUnmarshaller = jaxbContext.createUnmarshaller();
        return (ContentModel) jaxbUnmarshaller.unmarshal(xmlFile);
    }
}
