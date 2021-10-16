/**
 * @author Matthew Kennedy, (c) 2019
 * 
 * This lets us compose multiple functions in to a single function. If we have
 * conversion functions F(x), G(x), and H(x), we can define a new function
 * FuncChain<F, G, H> that will compute H(G(F(X))).  F first, then G, then H.
 */

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
		return input;
	}

	void showInfo(float testInputValue) const {
		// base case does nothing
		(void)testInputValue;
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
			return unexpected;
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

	void showInfo(float testInputValue) const {
		// Print info about this level
		m_f.showInfo(testInputValue);

		// If valid, recurse down
		auto res = m_f.convert(testInputValue);
		if (res.Valid) {
			TBase::showInfo(res.Value);
		}
	}

private:
	TFirst m_f;
};
} // namespace priv

template <typename... TFuncs>
class FuncChain : public SensorConverter {
public:
	// Perform chained conversion of all functions in TFuncs
	SensorResult convert(float input) const override {
		return m_fs.convert(input);
	}

	// Access the sub-function of type TGet
	template <typename TGet>
	TGet &get() {
		return m_fs.template get<TGet>();
	}

	void showInfo(float testInputValue) const override {
		m_fs.showInfo(testInputValue);
	}

private:
	priv::FuncChain<TFuncs...> m_fs;
};
