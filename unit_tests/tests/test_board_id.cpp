/**
 * @file test_board_id.cpp
 *
 * getBoardId() and its custom_board_getBoardId override hook.
 */

#include "pch.h"
#include "board_id.h"
#include "board_overrides.h"

namespace {
class BoardIdTest : public ::testing::Test {
protected:
	void TearDown() override {
		custom_board_getBoardId = std::nullopt;
	}
};
} // namespace

TEST_F(BoardIdTest, defaultWithoutOverride) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	// host build: neither HW_HELLEN nor STATIC_BOARD_ID
	EXPECT_EQ(0, getBoardId());
}

TEST_F(BoardIdTest, overrideReplacesBoardId) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	custom_board_getBoardId = []() -> board_id_t { return 0x1234; };
	EXPECT_EQ(0x1234, getBoardId());

	custom_board_getBoardId = std::nullopt;
	EXPECT_EQ(0, getBoardId());
}
