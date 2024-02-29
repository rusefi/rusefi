#include "pch.h"

BigBufferUser getBigBufferCurrentUser();

TEST(BigBuffer, CppMagic) {
  BigBufferHandle h = getBigBuffer(BigBufferUser::ToothLogger);
  ASSERT_EQ(getBigBufferCurrentUser(), BigBufferUser::ToothLogger);
  h = {};
  ASSERT_EQ(getBigBufferCurrentUser(), BigBufferUser::None);
}
