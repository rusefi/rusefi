package com.irnems;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * @author Andrey Belomutskiy
 *         8/5/13
 */
public class MedianFilter {
    private List<Double> values;
    private int size;

    public MedianFilter(List<Double> values, int size) {
        this.values = values;
        this.size = size;
    }

    public static List<Double> filter(List<Double> values, int size) {
        return new MedianFilter(values, size).filter();
    }

    private List<Double> filter() {

        List<Double> result = new ArrayList<>();

        for(int i=0;i<values.size();i++) {

            int fromIndex = Math.max(0, i - size);

            List<Double> copy = new ArrayList<>(values.subList(fromIndex, i + 1));

            Collections.sort(copy);

            result.add(copy.get(copy.size() / 2));

        }


        return result;
    }
}
