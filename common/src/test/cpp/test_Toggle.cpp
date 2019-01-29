#include "gtest/gtest.h"

#include "Toggle.h"

using namespace curtinfrc;

TEST (Toggle, DefConstructor) {
  Toggle toggleDef;

  EXPECT_TRUE(toggleDef.tick(true));
}

TEST (Toggle, Falling) {
  Toggle toggle(ToggleEvent::ONFALL);

  for (int i = 0; i < 2; i++) {
    ASSERT_FALSE(toggle.tick(true));
    ASSERT_FALSE(toggle.tick(true));
    ASSERT_TRUE(toggle.tick(false));
    ASSERT_FALSE(toggle.tick(false));
    ASSERT_FALSE(toggle.tick(false));
  }
}

TEST (Toggle, Rising) {
  Toggle toggle(ToggleEvent::ONRISE);

  for (int i = 0; i < 2; i++) {
    ASSERT_FALSE(toggle.tick(false));
    ASSERT_FALSE(toggle.tick(false));
    ASSERT_TRUE(toggle.tick(true));
    ASSERT_FALSE(toggle.tick(true));
    ASSERT_FALSE(toggle.tick(true));
  }
}

TEST (Toggle, Changing) {
  Toggle toggle(ToggleEvent::ONCHANGE);

  for (int i = 0; i < 2; i++) {
    ASSERT_TRUE(toggle.tick(true));
    ASSERT_FALSE(toggle.tick(true));
    ASSERT_FALSE(toggle.tick(true));
    ASSERT_TRUE(toggle.tick(false));
    ASSERT_FALSE(toggle.tick(false));
    ASSERT_FALSE(toggle.tick(false));
  }
}
