#include "gtest/gtest.h"

#include "controllers/Controller.h"

using namespace curtinfrc::controllers;


TEST (Controller, Constructor) {
  Controller cont(new curtinfrc::controllers::GenericHID(0));

  ASSERT_EQ(cont.GetPort(), 0);
}
