#pragma once

enum class SyncEdge : uint8_t {
	Rise,		// Sync on rising edges, use rise+fall for phase info
	Fall,		// Sync on falling edges, use rise+fall for phase info
	// VR or lazy Hall
	RiseOnly,	// Completely ignore all falling edges (this used to be useOnlyRisingEdgeForTrigger=true etc)
	// nicer Hall
	Both		// Sync on both edges, use both for phase
};
