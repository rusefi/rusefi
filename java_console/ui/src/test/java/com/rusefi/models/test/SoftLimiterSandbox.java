package com.rusefi.models.test;

import java.util.ArrayList;
import java.util.Random;

public class SoftLimiterSandbox {
    private Random r = new Random();

    public static void main(String[] args) {
        new SoftLimiterSandbox().testSoftLimit();
    }

    public void testSoftLimit() {
        double targetMissRatio = 0.3;

        int windowSize = 50;
        State state = new State(windowSize);

        System.out.println("Target miss ratio: " + targetMissRatio);
        System.out.println("Window length: " + windowSize);
        System.out.println("eventIndex,state,currentWindowDeepRatio");

        for (int i = 0; i < 250 * windowSize; i++) {
            state.add(targetMissRatio);
            System.out.println(i + "," + state.getLatest() + "," + state.getCurrentRatio());
        }
    }

    class State {
        private final int windowSize;

        ArrayList<Boolean> states = new ArrayList<>();


        public State(int windowSize) {
            this.windowSize = windowSize;
            states.add(false);
        }

        boolean getLatest() {
            return states.get(states.size() - 1);
        }

        double getCurrentRatio() {
            int from = Math.max(0, states.size() - windowSize);
            int count = 0;
            for (int i = from; i < states.size(); i++) {
                if (states.get(i))
                    count++;
            }
            return count * 1.0 / windowSize;


        }

        public void add(double targetMissRatio) {
            if (!getLatest()) {
                // never skip two in a row
                states.add(true);
            } else {
                double currentRatio = getCurrentRatio();
                //double thisEventProbability =

                /**
                 * Due to "never miss two events in a row" requirement we want more or less double the target in comparison
                 * if current ratio is above target we want to lower probability for next event to fire
                 */

                boolean newState = r.nextDouble() > 2 * targetMissRatio;
                states.add(newState);

            }
        }
    }
}
