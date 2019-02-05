#include "gtest/gtest.h"

#include "Toggle.h"

using namespace curtinfrc;

TEST (Toggle, DefConstructor) {
  Toggle toggleDef;

  EXPECT_TRUE(toggleDef.Update(true));
}

TEST (Toggle, Falling) {
  Toggle toggle(ToggleEvent::ONFALL);

  for (int i = 0; i < 2; i++) {
    ASSERT_FALSE(toggle.Update(true));
    ASSERT_FALSE(toggle.Update(true));
    ASSERT_TRUE(toggle.Update(false));
    ASSERT_FALSE(toggle.Update(false));
    ASSERT_FALSE(toggle.Update(false));
  }
}

TEST (Toggle, Rising) {
  Toggle toggle(ToggleEvent::ONRISE);

  for (int i = 0; i < 2; i++) {
    ASSERT_FALSE(toggle.Update(false));
    ASSERT_FALSE(toggle.Update(false));
    ASSERT_TRUE(toggle.Update(true));
    ASSERT_FALSE(toggle.Update(true));
    ASSERT_FALSE(toggle.Update(true));
  }
}

TEST (Toggle, Changing) {
  Toggle toggle(ToggleEvent::ONCHANGE);

  for (int i = 0; i < 2; i++) {
    ASSERT_TRUE(toggle.Update(true));
    ASSERT_FALSE(toggle.Update(true));
    ASSERT_FALSE(toggle.Update(true));
    ASSERT_TRUE(toggle.Update(false));
    ASSERT_FALSE(toggle.Update(false));
    ASSERT_FALSE(toggle.Update(false));
  }
}
