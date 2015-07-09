package com.irnems;

import java.util.ArrayList;
import java.util.List;

/**
 * @author Andrey Belomutskiy
 *         8/5/13
 */
public class SimpleFilter {
    public static List<Double> filter(List<Double> medianValues, int K) {
        List<Double> result = new ArrayList<Double>();

        Double firstValue = medianValues.get(0);

        double Dacc = firstValue * K;
        double Dout = firstValue;


//        for (int i = 0; i < 3 * K; i++) {
//            Dacc = Dacc + firstValue - Dout;
//            Dout = Dacc / K;
//        }

        for (double Din : medianValues) {
            Dacc = Dacc + Din - Dout;
            Dout = Dacc / K;
            result.add(Dout);
        }

        return result;
    }
}
