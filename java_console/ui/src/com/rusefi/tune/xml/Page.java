package com.rusefi.tune.xml;

import com.rusefi.config.generated.Fields;

import javax.xml.bind.annotation.XmlAttribute;
import java.util.ArrayList;
import java.util.List;

public class Page {
    public final List<Constant> constants = new ArrayList<>();

    public Page() {
        constants.add(new Constant("name1", "units1"));
        constants.add(new Constant("name2", "units2"));
    }

    @XmlAttribute
    public int getNumber() {
        return 0;
    }

    @XmlAttribute
    public int getSize() {
        return Fields.TOTAL_CONFIG_SIZE;
    }
}
