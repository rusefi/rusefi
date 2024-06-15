#include "pch.h"

extern float filterMapValue(float value);

TEST(average, exp) {
  EngineTestHelper eth(engine_type_e::TEST_ENGINE);
  ASSERT_EQ(filterMapValue(0), 0);

  engineConfiguration->mapExpAverageAlpha = 0.5;


  ASSERT_EQ(filterMapValue(3), 3);	// this would start collecting
  ASSERT_EQ(filterMapValue(1), 2);
  ASSERT_EQ(filterMapValue(3), 2.5);


}
