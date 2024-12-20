template<class ConverterType>
FunctionalSensorImpl<ConverterType>::FunctionalSensorImpl(const SensorType type, const efidur_t timeoutPeriod)
	: FunctionalSensorBase(type, timeoutPeriod) {
}

template<class ConverterType>
void FunctionalSensorImpl<ConverterType>::setFunction(ConverterType& func) {
	m_function = &func;
}

template<class ConverterType>
ConverterType* FunctionalSensorImpl<ConverterType>::getFunction() const {
	return m_function;
}

template<class ConverterType>
float FunctionalSensorImpl<ConverterType>::getRaw() const {
	return m_rawValue;
}

template<class ConverterType>
void FunctionalSensorImpl<ConverterType>::postRawValue(const float inputValue, const efitick_t timestamp) {
	// If no function is set, this sensor isn't valid.
	if (!m_function) {
		invalidate(UnexpectedCode::Configuration);
		return;
	}

	m_rawValue = inputValue;

	auto r = m_function->convert(inputValue);

	// This has to happen so that we set the valid bit after
	// the value is stored, to prevent the data race of reading
	// an old invalid value
	if (r.Valid) {
		setValidValue(r.Value, timestamp);
	} else {
		invalidate(r.Code);
	}
}
