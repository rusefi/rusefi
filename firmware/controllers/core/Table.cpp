/**
 *  @file Table.cpp
 *  @brief Table (2d and 3d) look up and interpolation logic
 *
 *  @date Dec 24, 2018
 *  @author Matthew Kennedy, (c) 2018
 */
#include "Table.h"

#include "efilib.h"

template<class TBin, uint8_t TSize>
BinResult TableBins<TBin, TSize>::Get(float value) const
{
    // Handle off-scale low
    if (value <= m_bins[0])
    {
        return { 0, 0.0f };
    }

    // Handle off-scale high
    if (value >= m_bins[TSize - 1])
    {
        return { TSize - 2, 1.0f };
    }

    int8_t idx = 0;

    // Find the last index less than the searched value
    // Linear search for now, maybe binary search in future
    // after collecting real perf data
    for (idx = 0; idx < TSize - 1; idx++)
    {
        if (m_bins[idx + 1] > value)
        {
            break;
        }
    }

    float low = m_bins[idx];
    float high = m_bins[idx + 1];

    // Compute how far along the bin we are
    // (0.0f = left side, 1.0f = right side)
    float fraction = (value - low) / (high - low);
    
    return { idx, fraction };
}

template <typename TBin, uint8_t TSize>
bool TableBins<TBin, TSize>::IsAscending()
{
    for(int8_t i = 0; i < TSize - 1; i++)
    {
        if(m_bins[i] >= m_bins[i + 1])
        {
            return false;
        }
    }

    return true;
}

struct TableHelper
{
    static float Linterp(float low, float high, float frac)
    {
        return high * frac + low * (1 - frac);
    }
};

template <typename TBin, typename TValue, uint8_t TSize>
float Table2d<TBin, TValue, TSize>::Get(float x) const
{
    if (isnan(x))
    {
        firmwareError(CUSTOM_INTERPOLATE_NAN, "NaN in Table2d::Get named %s", m_name);
        return NAN;
    }

    BinResult b = m_bins.Get(x);

    float low = static_cast<float>(m_values[b.Idx]);
    float high = static_cast<float>(m_values[b.Idx + 1]);

    float frac = b.Frac;

    return TableHelper::Linterp(low, high, frac);
};

// Explicitly instantiate types we use
template class Table2d<uint8_t, float, 8>;
template class Table2d<float, float, 8>;

// Explicit instantiation isn't necessary for bins, as
// appropriate bin templates get instantiated by getting
// used in a table


// These exist for unit tests (linker will remove in real firmware)
template class Table2d<float, float, 2>;
template class Table2d<float, float, 3>;
