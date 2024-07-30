//
// Created by kifir on 7/29/24.
//

#pragma once

#include <optional>

class ValueProvider2D {
public:
    virtual std::optional<float> getValue(float x) const = 0;
};

template<int TLength, typename TValue, typename TBin>
class Map2D : public ValueProvider2D {
public:
    Map2D(const char* const name);

    void initTable(const TValue (&values)[TLength], const TBin (&bins)[TLength]);

    virtual std::optional<float> getValue(const float x) const;
private:
    const char* const m_name;

    const TValue (*m_values)[TLength] = nullptr;
    const TBin (*m_bins)[TLength] = nullptr;
};

template<int TLength, typename TValue, typename TBin>
Map2D<TLength, TValue, TBin>::Map2D(const char* const name) : m_name(name) {
}

template<int TLength, typename TValue, typename TBin>
void Map2D<TLength, TValue, TBin>::initTable(const TValue (&values)[TLength], const TBin (&bins)[TLength]) {
    m_values = &values;
    m_bins = &bins;
}

template<int TLength, typename TValue, typename TBin>
std::optional<float> Map2D<TLength, TValue, TBin>::getValue(const float x) const {
    if (!m_bins) {
        criticalError("Access to uninitialized bins: %s", m_name);
        return {};
    } else if (!m_values) {
        criticalError("Access to uninitialized values: %s", m_name);
        return {};
    } else if ((1 < TLength) && ((*m_bins)[0] == 0.0f) && ((*m_bins)[1] == 0.0f)) {
        // bins are uninitialized, but it isn't critical - maybe old configuration
        return {};
    } else {
        return std::make_optional(interpolate2d(x, *m_bins, *m_values));
    }
}