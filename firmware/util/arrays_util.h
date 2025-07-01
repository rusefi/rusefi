#pragma once

#include <array>

template <
    typename ValueType,
    size_t source_col_count,
    size_t source_row_count,
    size_t target_col_count,
    size_t target_row_count
> constexpr std::array<std::array<ValueType, target_col_count>, target_row_count> initTableFromAnotherTable(
    const std::array<std::array<ValueType, source_col_count>, source_row_count>& source,
    const ValueType defaultValue
) {
    std::array<std::array<ValueType, target_col_count>, target_row_count> result {};
    const size_t colCountToCopy = std::min(source_col_count, target_col_count);
    const size_t rowCountToCopy = std::min(source_row_count, target_row_count);
    for (size_t row = 0; row < rowCountToCopy; ++row) {
        for (size_t col = 0; col < colCountToCopy; ++col) {
            result[row][col] = source[row][col];
        }
        for (size_t col = colCountToCopy; col < target_col_count; ++col) {
            result[row][col] = defaultValue;
        }
    }
    for (size_t row = rowCountToCopy; row < target_row_count; ++row) {
        for (size_t col = 0; col < target_col_count; ++col) {
            result[row][col] = defaultValue;
        }
    }
    return result;
}