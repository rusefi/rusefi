#pragma once

enum class SyncEdge : uint8_t {
	Rise,		// Sync on rising edges, use rise+fall for phase info
	Fall,		// Sync on falling edges, use rise+fall for phase info
	RiseOnly,	// Completely ignore all falling edges (this used to be useOnlyRisingEdgeForTrigger=true etc)
	Both		// Sync on both edges, use both for phase
};
