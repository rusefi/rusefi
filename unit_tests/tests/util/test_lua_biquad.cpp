#include "pch.h"
#include "lua_biquad.h"


TEST(util, luaBiQuad100_1) {
  EngineTestHelper eth(engine_type_e::TEST_ENGINE); // LOL engineConfiguration->verboseQuad
  LuaBiQuad b;

  b.configureLowpass(100, 1);

  ASSERT_NEAR(b.filter(25), 25, EPS2D);

  ASSERT_NEAR(b.filter(20), 25, EPS2D);
  ASSERT_NEAR(b.filter(20), 24.98, EPS2D);
  ASSERT_NEAR(b.filter(20), 24.94, EPS2D);
}


TEST(util, luaBiQuad100_10) {
  EngineTestHelper eth(engine_type_e::TEST_ENGINE); // LOL engineConfiguration->verboseQuad
  LuaBiQuad b;

  b.configureLowpass(100, 10);

  ASSERT_NEAR(b.filter(25), 25, EPS2D);

  ASSERT_NEAR(b.filter(20), 24.69, EPS2D);
  ASSERT_NEAR(b.filter(20), 23.74, EPS2D);
}

TEST(util, luaBiQuad1000_01) {
  EngineTestHelper eth(engine_type_e::TEST_ENGINE); // LOL engineConfiguration->verboseQuad
  LuaBiQuad b;

  b.configureLowpass(1000, 0.1);

  ASSERT_NEAR(b.filter(25), 23.64, EPS2D);
}
