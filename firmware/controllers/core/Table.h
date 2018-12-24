/**
 *  @file Table.h
 *  @brief Table (2d and 3d) look up and interpolation logic
 */
#pragma once

#include <cstdint>
#include <type_traits>

struct BinResult
{
    int8_t Idx;
    float Frac;
};

template<class TBin, uint8_t TSize>
class TableBins final
{
    // Enforce numeric only (int, float, uintx_t, etc)
    static_assert(std::is_fundamental<TBin>::value);
    // Enforce large enough
    static_assert(TSize > 1);

    TBin* m_bins;

public:
    TableBins(TBin bins[TSize]) : m_bins(bins) { }

    /**
     * @brief Finds the location of a value in the bin array.
     * 
     * @param value The value to find in the bins.
     * @return A result containing the index to the left of the value,
     * and how far from (idx) to (idx + 1) the value is located.
     */
    BinResult Get(float value) const;
};

/**
 * @brief Provides a single-input, single-output lookup table.
 * 
 * Computes a lookup table of the form f(x), that is a single numeric output
 * that maps on to a single numeric output.
 */
template <typename TBin, typename TValue, uint8_t TSize>
class Table2d final
{
    // Enforce numeric only (int, float, uintx_t, etc)
    static_assert(std::is_fundamental<TBin>::value);
    static_assert(std::is_fundamental<TValue>::value);
    // Enforce that we have two or more bins, so we actually
    // have something to do
    static_assert(TSize > 1);

    const char* m_name;
    TableBins<TBin, TSize> m_bins;
    TValue* m_values;

public:
    Table2d(const char* name, TBin bins[TSize], TValue values[TSize])
        : m_name(name)
        , m_bins(bins)
        , m_values(values)
    {
    }

    Table2d(TBin bins[TSize], TValue values[TSize])
        : Table2d("", bins, values)
    {
    }

    float Get(float x) const;
};
