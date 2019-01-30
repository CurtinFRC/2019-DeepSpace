#include "gtest/gtest.h"

#include "physics/Intersection.h"

using namespace curtinfrc::physics;

TEST (Intersection, DefClaim) {
  Intersection col;
  IntersectionUser entity{ 1, 3 };

  EXPECT_EQ(col.GetOwnerID(), -1);
  EXPECT_EQ(col.GetOwnerPriority(), 0);

  ASSERT_TRUE(col.Claim(entity));
  ASSERT_EQ(col.GetOwnerID(), 1);
  ASSERT_EQ(col.GetOwnerPriority(), 3);
}

TEST (Intersection, BasicClaim) {
  Intersection col;
  IntersectionUser entity1{ 0, 1 }, entity2{ 1, 2 };

  EXPECT_EQ(col.GetOwnerID(), -1);
  EXPECT_EQ(col.GetOwnerPriority(), 0);

  ASSERT_TRUE(col.Claim(entity1));
  ASSERT_EQ(col.GetOwnerID(), 0);
  ASSERT_EQ(col.GetOwnerPriority(), 1);

  ASSERT_TRUE(col.Claim(entity2));
  ASSERT_EQ(col.GetOwnerID(), 1);
  ASSERT_EQ(col.GetOwnerPriority(), 2);

  ASSERT_FALSE(col.Claim(entity1));
  ASSERT_EQ(col.GetOwnerID(), 1);
  ASSERT_EQ(col.GetOwnerPriority(), 2);
}
