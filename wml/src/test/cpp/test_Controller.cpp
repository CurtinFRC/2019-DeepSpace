#include "gtest/gtest.h"

#include "controllers/Controller.h"

using namespace wml::controllers;


TEST (Controller, Constructor) {
  Controller cont(new wml::controllers::GenericHID(0));

  ASSERT_EQ(cont.GetPort(), 0);
}
