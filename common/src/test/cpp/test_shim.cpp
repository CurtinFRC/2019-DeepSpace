#include <gtest/gtest.h>

#include "example_shim.h"

TEST(ExampleShim, WhoAmI) {
  example_shim shim;
  std::cout << shim.who_am_i() << std::endl;
  std::cout << shim.who_are_you() << std::endl;
}