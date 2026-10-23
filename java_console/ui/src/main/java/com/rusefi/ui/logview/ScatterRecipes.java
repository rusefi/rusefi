package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

/**
 * Ready-made scatter plots for common questions, each offered when the log has its signals: where knock happens,
 * how well the mixture follows its target, whether oil pressure holds up, injector headroom, heat soak, and where
 * the engine ran. A recipe sets X, Y and Z (as the user's choice), the colouring, a fade level and guide lines.
 */
final class ScatterRecipes {
    /** A knock retard or count above this is drawn strong; lower points pale. */
    static final double KNOCK_FADE = 0.01;
    static final double INJECTOR_DUTY_LIMIT = 85;

    static final class Recipe {
        final String title;
        final String description;
        final String[] x;
        final String[] y;
        final String[] z;
        /** Not picked for Y even when its name matches, e.g. a target where the measurement is wanted. */
        final String yExcluded;
        final ScatterPlot.ColourBy colourBy;
        /** Points under this Z (logged unit) are drawn pale, when Z is one of {@link #fadeSignals}; NaN for none. */
        final double fadeBelow;
        final List<String> fadeSignals;
        final List<ScatterPlot.Reference> references;

        Recipe(String title, String description, String[] x, String[] y, String yExcluded, String[] z,
               ScatterPlot.ColourBy colourBy, double fadeBelow, List<String> fadeSignals,
               List<ScatterPlot.Reference> references) {
            this.title = title;
            this.description = description;
            this.x = x;
            this.y = y;
            this.yExcluded = yExcluded;
            this.z = z;
            this.colourBy = colourBy;
            this.fadeBelow = fadeBelow;
            this.fadeSignals = fadeSignals;
            this.references = references;
        }

        /**
         * {x, y, z} fields for this log, or null when one is missing or never changes: a knock retard that stays 0
         * says nothing, so the next Z candidate (e.g. the knock level) is taken.
         */
        int[] fields(CsvLogReader log) {
            int xField = varying(log, null, x);
            int yField = varying(log, yExcluded, y);
            int zField = varying(log, null, z);
            if (xField < 0 || yField < 0 || zField < 0 || xField == yField) {
                return null;
            }
            return new int[]{xField, yField, zField};
        }

        /** The fade level for a Z signal: this recipe's, for its fade signals; NaN otherwise. */
        double fadeFor(CsvLogReader log, int zField) {
            return fadeSignals.contains(log.name(zField)) ? fadeBelow : Double.NaN;
        }

        /** The first candidate (each by exact name, then as part of a name) that the log has and that changes. */
        private static int varying(CsvLogReader log, String excluded, String[] names) {
            for (String name : names) {
                int field = HistogramSignals.findExcept(log, excluded, name);
                if (field >= 0 && !log.isConstant(field)) {
                    return field;
                }
            }
            return -1;
        }
    }

    static final List<Recipe> ALL = Collections.unmodifiableList(recipes());

    private ScatterRecipes() {
    }

    private static List<Recipe> recipes() {
        String[] rpm = {"RPM", "Engine Speed"};
        String[] load = {"Ignition: load", "MAP", "Engine load"};
        List<Recipe> recipes = new ArrayList<>();
        recipes.add(new Recipe("Where the engine ran",
                "RPM by MAP, coloured by the time spent at each spot", rpm, HistogramSignals.Y_NAMES, null,
                HistogramSignals.Z_NAMES, ScatterPlot.ColourBy.DENSITY, Double.NaN, Collections.emptyList(), Collections.emptyList()));
        recipes.add(new Recipe("Knock map",
                "Ignition load by RPM, coloured by knock retard: knock in strong colour on top, the rest pale",
                rpm, load, null, new String[]{"Knock: Retard", "Knock: Count", "Knock: Current level"},
                ScatterPlot.ColourBy.Z, KNOCK_FADE, Arrays.asList("Knock: Retard", "Knock: Count"), Collections.emptyList()));
        recipes.add(new Recipe("Lambda vs target",
                "Measured lambda against its target, coloured by RPM: above the line is lean, below rich",
                new String[]{"Fuel: target lambda"}, new String[]{"Lambda 1", "Front Lambda", "Lambda", "Lambda 2"},
                HistogramSignals.Z_EXCLUDED, rpm, ScatterPlot.ColourBy.Z, Double.NaN, Collections.emptyList(),
                Collections.singletonList(new ScatterPlot.Reference(ScatterPlot.Reference.Kind.DIAGONAL, 0,
                        "on target"))));
        recipes.add(new Recipe("Oil pressure vs RPM",
                "Oil pressure by RPM, coloured by oil temperature (or coolant): a weak pump or hot oil drops away",
                rpm, new String[]{"Oil Pressure"}, null, new String[]{"Oil Temperature", "CLT"},
                ScatterPlot.ColourBy.Z, Double.NaN, Collections.emptyList(), Collections.emptyList()));
        recipes.add(new Recipe("Injector duty vs RPM",
                "Injector duty by RPM, coloured by MAP, with the usual 85 % limit",
                rpm, new String[]{"Fuel: injector duty cycle"}, null, new String[]{"MAP"},
                ScatterPlot.ColourBy.Z, Double.NaN, Collections.emptyList(),
                Collections.singletonList(new ScatterPlot.Reference(ScatterPlot.Reference.Kind.HORIZONTAL,
                        INJECTOR_DUTY_LIMIT, "85 % duty"))));
        recipes.add(new Recipe("Heat soak",
                "Intake air temperature by vehicle speed, coloured by coolant: hot at low speed is heat soak",
                new String[]{"Vehicle Speed"}, new String[]{"Intake Air IAT", "IAT"}, null, new String[]{"CLT"},
                ScatterPlot.ColourBy.Z, Double.NaN, Collections.emptyList(), Collections.emptyList()));
        return recipes;
    }

    static Recipe byTitle(String title) {
        for (Recipe recipe : ALL) {
            if (recipe.title.equals(title)) {
                return recipe;
            }
        }
        return null;
    }
}
