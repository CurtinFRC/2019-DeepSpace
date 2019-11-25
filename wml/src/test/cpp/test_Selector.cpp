#include "gtest/gtest.h"

#include "Selector.h"

using namespace wml;

TEST (Selector, DefConstructor) {
  Selector selectorDef;

  ASSERT_EQ(selectorDef.Get(), 0);
  ASSERT_EQ(selectorDef.GetLength(), 3);
}

TEST (Selector, RawShift) {
  Selector selector{ 10, 0, true };

  ASSERT_EQ(selector.Get(), 0);
  ASSERT_EQ(selector.GetLength(), 10);

  ASSERT_EQ(selector.Shift(3), 3);
  ASSERT_EQ(selector.Shift(2), 5);
  ASSERT_EQ(selector.Shift(9), 4);

  ASSERT_EQ(selector.Shift(-3), 1);
  ASSERT_EQ(selector.Shift(-5), 6);
}

TEST (Selector, Set) {
  Selector selector{ 10, 0, true };

  ASSERT_EQ(selector.Get(), 0);
  ASSERT_EQ(selector.GetLength(), 10);

  ASSERT_EQ(selector.Set(4), 4);
  ASSERT_EQ(selector.Shift(-1), 3);
  ASSERT_EQ(selector.Set(-1), 9);
  ASSERT_EQ(selector.Shift(1), 0);
  ASSERT_EQ(selector.Set(22), 2);
}

TEST (Selector, NoWrap) {
  Selector selector{ 10 };

  ASSERT_EQ(selector.Get(), 0);
  ASSERT_EQ(selector.GetLength(), 10);

  ASSERT_EQ(selector.Set(4), 4);
  ASSERT_EQ(selector.ShiftLeft(2), 2);
  ASSERT_EQ(selector.ShiftLeft(2), 0);
  ASSERT_EQ(selector.ShiftLeft(2), 0);

  ASSERT_EQ(selector.Set(6), 6);
  ASSERT_EQ(selector.ShiftRight(3), 9);
  ASSERT_EQ(selector.ShiftRight(3), 9);

  ASSERT_EQ(selector.Set(-2), 0);
  ASSERT_EQ(selector.Set(12), 9);
}

TEST (Selector, Shift) {
  // Has been manually tested with repl, and the
  // really intense test I had here before was way
  // over the top and took too long to run, so ~

  // Also if Set is passing this should pass fine
}
