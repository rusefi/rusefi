/**
 * @author Matthew Kennedy, (c) 2021
 * 
 * A function to convert input voltage output value based on a 2d table.
 */

#pragma once

#include "sensor_converter_func.h"
#include "interpolation.h"
#include "efi_ratio.h"

template <class TBin, class TValue, int TSize, typename TOutputScale = efi::ratio<1>>
class TableFunc final : public SensorConverter {
public:
	TableFunc(TBin (&bins)[TSize], TValue (&values)[TSize])
		: m_bins(bins)
		, m_values(values)
	{
	}

	SensorResult convert(float inputValue) const override {
		return interpolate2d(inputValue, m_bins, m_values) * TOutputScale::asFloat();
	}

	void showInfo(float /*testInputValue*/) const override { }

private:
	TBin (&m_bins)[TSize];
	TValue (&m_values)[TSize];
};
