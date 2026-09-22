#pragma once

// adcIsBufferComplete() only distinguishes half/full callbacks for circular
// conversions. A linear conversion has one completion callback. The callable
// is deliberately evaluated only for circular groups.
template <typename CompletionCheck>
bool adcCallbackShouldProcess(bool circular, CompletionCheck&& isComplete) {
	return !circular || isComplete();
}
