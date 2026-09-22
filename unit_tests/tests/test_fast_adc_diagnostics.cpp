#include <gtest/gtest.h>
#include "../../firmware/hw_layer/adc/fast_adc_diagnostics.h"

TEST(FastAdcDiagnostics, CompletedDmaWaitingForCallback) {
	FastAdcDiagnostics diagnostics;
	diagnostics.record(true, 0, true, false, false);
	EXPECT_EQ(FastAdcDiagnostics::CompletionPending, diagnostics.takePending());
}

TEST(FastAdcDiagnostics, ConversionOverlapsNextDeadline) {
	FastAdcDiagnostics diagnostics;
	diagnostics.record(true, 3, false, true, false);
	EXPECT_EQ(FastAdcDiagnostics::ConversionBusy, diagnostics.takePending());
}

TEST(FastAdcDiagnostics, CompletionBetweenRegisterReads) {
	FastAdcDiagnostics diagnostics;
	diagnostics.record(true, 1, true, false, false);
	EXPECT_EQ(FastAdcDiagnostics::CompletionPending, diagnostics.takePending());
}

TEST(FastAdcDiagnostics, UnknownAndHardwareErrorStatesAreNotMisclassified) {
	FastAdcDiagnostics diagnostics;
	// Counter reached zero but TC is not visible yet.
	diagnostics.record(true, 0, false, true, false);
	EXPECT_EQ(FastAdcDiagnostics::Other, diagnostics.takePending());
	// DMA was stopped with transfers outstanding.
	diagnostics.record(true, 3, false, false, false);
	EXPECT_EQ(FastAdcDiagnostics::Other, diagnostics.takePending());
	// Driver is not active; even a stale TC flag cannot establish our cases.
	diagnostics.record(false, 0, true, false, false);
	EXPECT_EQ(FastAdcDiagnostics::Other, diagnostics.takePending());
	// A pending peripheral/DMA fault takes precedence over either condition.
	diagnostics.record(true, 0, true, false, true);
	EXPECT_EQ(FastAdcDiagnostics::Other, diagnostics.takePending());
	diagnostics.record(true, 3, false, true, true);
	EXPECT_EQ(FastAdcDiagnostics::Other, diagnostics.takePending());
}

TEST(FastAdcDiagnostics, CoalescesRepeatsWithoutLosingDistinctReasons) {
	FastAdcDiagnostics diagnostics;
	EXPECT_EQ(0, diagnostics.takePending());
	diagnostics.record(true, 3, false, true, false);
	diagnostics.record(true, 1, false, true, false);
	diagnostics.record(true, 0, true, false, false);
	EXPECT_EQ(FastAdcDiagnostics::ConversionBusy | FastAdcDiagnostics::CompletionPending,
		diagnostics.takePending());
	EXPECT_EQ(0, diagnostics.takePending());
	// An event after a drain must survive until the next report.
	diagnostics.record(true, 0, true, false, false);
	EXPECT_EQ(FastAdcDiagnostics::CompletionPending, diagnostics.takePending());
}
