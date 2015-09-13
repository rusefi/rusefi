package com.rusefi;

public class BinarySearch {
    public static int binarySearch(double target, double[] angles) {
        System.out.println("Testing " + target);

        int left = 0;
        int right = angles.length - 1;

        while (left <= right) {
            int mid = (left + right) / 2;
            double midVal = angles[mid];

            if (midVal < target)
                left = mid + 1;  // Neither val is NaN, thisVal is smaller
            else if (midVal > target)
                right = mid - 1; // Neither val is NaN, thisVal is larger
            else {
                if (midVal == target)     // Values are equal
                    return mid;             // Key found
                else if (midVal < target) // (-0.0, 0.0) or (!NaN, NaN)
                    left = mid + 1;
                else                        // (0.0, -0.0) or (NaN, !NaN)
                    right = mid - 1;
            }
        }
        return left - 1;
    }
}