/*
 * @file flex_transient.h
 *
 * Flex-fuel transient fueling compensation: CLT x Ethanol% multiplier tables
 * applied to acceleration enrichment and wall wetting (tau/beta).
 *
 * Gasoline and ethanol behave very differently during transient throttle, especially
 * when cold, so these tables allow extra enrichment when ethanol content is high and
 * the engine is cold, while staying neutral (1.0) when warm or on gasoline.
 */

#pragma once

/**
 * Look up a flex-fuel transient compensation multiplier from a CLT (X) x Ethanol% (Y) table.
 *
 * Returns 1.0 (neutral) when there is no flex fuel sensor, or when the table is left
 * un-initialized (all zero), so existing tunes and gasoline operation are unchanged.
 */
template <typename TTable>
float getFlexTransientMult(const TTable& table) {
	if (!engineConfiguration->flexFuelTransientComp || !Sensor::hasSensor(SensorType::FuelEthanolPercent)) {
		return 1.0f;
	}

	// Default to normal operating temperature in case of CLT sensor failure
	float clt = Sensor::get(SensorType::Clt).value_or(90);
	// If failed flex sensor, default to 50% E (matches cranking behavior)
	float ethanol = Sensor::get(SensorType::FuelEthanolPercent).value_or(50);

	float mult = interpolate3d(table,
		config->flexTransientEthanolBins, ethanol,
		config->flexTransientCltBins, clt);

	// An un-initialized (all-zero) table means "no compensation"
	return mult > 0.01f ? mult : 1.0f;
}
