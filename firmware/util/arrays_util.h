#pragma once

#include <array>

template<typename ValueType, size_t source_size, size_t target_size>
constexpr std::array<ValueType, target_size> initArrayFromAnotherArray(
    const std::array<ValueType, source_size>& source,
    const ValueType defaultValue
) {
    std::array<ValueType, target_size> result {};
    const size_t elemsToCopyCount = std::min(source_size, target_size);
    for (size_t i = 0; i < elemsToCopyCount; i++) {
        result[i] = source[i];
    }
    for (size_t i = elemsToCopyCount; i < target_size; i++) {
        result[i] = defaultValue;
    }
    return result;
}

