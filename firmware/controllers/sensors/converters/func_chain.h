#pragma once

#include "sensor_converter_func.h"

#include <type_traits>
#include <utility>

namespace priv {
template <class... _Types>
class FuncChain;

template <>
class FuncChain<> {
protected:
	SensorResult convert(float input) const {
		// Base case is the identity function
		return {true, input};
	}
};

template <typename TFirst, typename... TRest>
class FuncChain<TFirst, TRest...> : private FuncChain<TRest...> {
	static_assert(std::is_base_of_v<SensorConverter, TFirst>, "Template parameters must inherit from SensorConverter");

private:
	using TBase = FuncChain<TRest...>;

public:
	SensorResult convert(float input) const {
		// Convert the current step
		SensorResult currentStep = m_f.convert(input);

		// if it was valid, pass this result to the chain of (n-1) functions that remain
		if (currentStep.Valid) {
			return TBase::convert(currentStep.Value);
		} else {
			return {false, 0};
		}
	}

	// Get the element in the current level
	template <class TGet>
	std::enable_if_t<std::is_same_v<TGet, TFirst>, TGet &> get() {
		return m_f;
	}

	// We don't have it - check level (n - 1)
	template <class TGet>
	std::enable_if_t<!std::is_same_v<TGet, TFirst>, TGet &> get() {
		return TBase::template get<TGet>();
	}

private:
	TFirst m_f;
};
} // namespace priv

template <typename... TFuncs>
class FuncChain : public SensorConverter {
public:
	SensorResult convert(float input) const override {
		return m_fs.convert(input);
	}

	template <typename TGet>
	TGet &get() {
		return m_fs.template get<TGet>();
	}

private:
	priv::FuncChain<TFuncs...> m_fs;
};
