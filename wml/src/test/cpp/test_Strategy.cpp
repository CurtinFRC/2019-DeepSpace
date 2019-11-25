#include "gtest/gtest.h"

#include "strategy/Strategy.h"

using namespace wml;

class MockStrategy : public Strategy {
 public:
  int started = 0, stopped = 0, periodic = 0;

  std::string GetStrategyName() override {
    return "MockStrategy";
  }

  void OnStart() override {
    started++;
  }

  void OnUpdate(double dt) override {
    periodic++;
  }

  void OnStop() override {
    stopped++;
  } 
};

class MockSystem : public StrategySystem { };

class StrategyTest : public testing::Test {
 public:
  StrategyTest() {
    strat = std::make_shared<MockStrategy>();
  }

  std::shared_ptr<MockStrategy> makeStrat() {
    return std::make_shared<MockStrategy>();
  }

  MockSystem sysA, sysB;
  StrategyController controller;
  std::shared_ptr<MockStrategy> strat;
};

TEST_F(StrategyTest, ThrowIfSystemNotRegistered) {
  strat->Requires(&sysA);

  ASSERT_THROW(controller.Schedule(strat), std::invalid_argument); // Sys not registered
}

TEST_F(StrategyTest, DefaultStrategyThrowsIfNotInterruptable) {
  controller.Register(&sysA);
  strat->Requires(&sysA);

  ASSERT_THROW(sysA.SetDefault(strat), std::invalid_argument);
}

TEST_F(StrategyTest, DefaultStrategy) {
  controller.Register(&sysA);
  strat->Requires(&sysA);

  strat->SetCanBeInterrupted(true);
  sysA.SetDefault(strat);

  ASSERT_EQ(sysA.GetDefaultStrategy(), strat);

  controller.Update();

  ASSERT_EQ(sysA.GetActiveStrategy(), sysA.GetDefaultStrategy());
  ASSERT_EQ(strat->GetStrategyState(), StrategyState::RUNNING);

  // Replaced with new Strategy

  std::shared_ptr<MockStrategy> strat2 = makeStrat();
  strat2->Requires(&sysA);
  
  ASSERT_TRUE(controller.Schedule(strat2));
  ASSERT_EQ(strat->GetStrategyState(), StrategyState::INTERRUPTED);
  ASSERT_EQ(sysA.GetActiveStrategy(), strat2);

  ASSERT_EQ(strat2->GetStrategyState(), StrategyState::RUNNING);

  // New Strategy Done

  strat2->SetDone();
  controller.Update();

  ASSERT_EQ(strat2->GetStrategyState(), StrategyState::DONE);
  ASSERT_EQ(strat->GetStrategyState(), StrategyState::RUNNING);

  ASSERT_EQ(sysA.GetActiveStrategy(), sysA.GetDefaultStrategy());

  // The default strategy should have been started twice and stopped once
  ASSERT_EQ(strat->started, 2);
  ASSERT_EQ(strat->stopped, 1);
}

// When a non-interruptable strategy is scheduled, it cannot be overridden without force
TEST_F(StrategyTest, RejectReplacement) {
  controller.Register(&sysA);
  strat->Requires(&sysA);

  std::shared_ptr<MockStrategy> strat2 = makeStrat();
  strat2->Requires(&sysA);

  ASSERT_TRUE(controller.Schedule(strat));
  ASSERT_FALSE(controller.Schedule(strat2));
  ASSERT_EQ(strat2->GetStrategyState(), StrategyState::CANCELLED);

  ASSERT_TRUE(controller.Schedule(strat2, true));
}

TEST_F(StrategyTest, AcceptReplacementInterrupt) {
  controller.Register(&sysA);
  strat->Requires(&sysA);
  strat->SetCanBeInterrupted(true);

  std::shared_ptr<MockStrategy> strat2 = makeStrat();
  strat2->Requires(&sysA);

  ASSERT_TRUE(controller.Schedule(strat));
  ASSERT_TRUE(controller.Schedule(strat2));
  
  ASSERT_EQ(strat->GetStrategyState(), StrategyState::INTERRUPTED);
}

TEST_F(StrategyTest, SystemStartsWithNoStrategies) {
  ASSERT_EQ(sysA.GetActiveStrategy(), nullptr);
  ASSERT_EQ(sysA.GetDefaultStrategy(), nullptr);
}

TEST_F(StrategyTest, StrategyStartsInitialized) {
  ASSERT_EQ(strat->GetStrategyState(), StrategyState::INITIALIZED);
}

TEST_F(StrategyTest, ScheduleFull) {
  controller.Register(&sysA);
  strat->Requires(&sysA);

  ASSERT_TRUE(controller.Schedule(strat));
  // sysA.StrategyUpdate(0);

  ASSERT_EQ(strat->GetStrategyState(), StrategyState::RUNNING);
  ASSERT_EQ(strat->started, 1);
  ASSERT_EQ(strat->stopped, 0);

  strat->SetDone();

  ASSERT_EQ(strat->stopped, 1);
  ASSERT_TRUE(strat->IsFinished());
  ASSERT_EQ(strat->GetStrategyState(), StrategyState::DONE);
}

TEST_F(StrategyTest, MultiRequirements) {
  controller.Register(&sysA);
  controller.Register(&sysB);

  strat->Requires(&sysA);
  strat->Requires(&sysB);

  ASSERT_TRUE(controller.Schedule(strat));

  ASSERT_EQ(sysA.GetActiveStrategy(), strat);
  ASSERT_EQ(sysB.GetActiveStrategy(), strat);
}

// Checks a reject if any system is preoccupied
TEST_F(StrategyTest, RejectMultiRequirements) {
  controller.Register(&sysA);
  controller.Register(&sysB);

  strat->Requires(&sysA);

  std::shared_ptr<MockStrategy> strat2 = makeStrat();

  strat2->Requires(&sysA);
  strat2->Requires(&sysB);

  ASSERT_TRUE(controller.Schedule(strat));

  ASSERT_EQ(sysA.GetActiveStrategy(), strat);
  ASSERT_EQ(sysB.GetActiveStrategy(), nullptr);

  ASSERT_FALSE(controller.Schedule(strat2));
}

TEST_F(StrategyTest, AcceptMultiRequirementsInterrupt) {
  controller.Register(&sysA);
  controller.Register(&sysB);

  strat->Requires(&sysA);
  strat->SetCanBeInterrupted(true);

  std::shared_ptr<MockStrategy> strat2 = makeStrat();

  strat2->Requires(&sysA);
  strat2->Requires(&sysB);

  ASSERT_TRUE(controller.Schedule(strat));

  ASSERT_EQ(sysA.GetActiveStrategy(), strat);
  ASSERT_EQ(sysB.GetActiveStrategy(), nullptr);

  ASSERT_TRUE(controller.Schedule(strat2));

  ASSERT_EQ(sysA.GetActiveStrategy(), strat2);
  ASSERT_EQ(sysB.GetActiveStrategy(), strat2);
}

class MockRecStrategy : public Strategy {
 public: 
  MockRecStrategy(std::shared_ptr<MockStrategy> strat, StrategyController *controller) : cont(controller), ms(strat) {}

  std::string GetStrategyName() override {
    return "MockRecStrategy";
  }

  void OnUpdate(double dt) override {
    SetDone();
  }

  void OnStop() {
    cont->Schedule(ms);
  }

  StrategyController *cont;
  std::shared_ptr<MockStrategy> ms;
};

TEST_F(StrategyTest, ScheduleInStrategy) {
  std::shared_ptr<MockRecStrategy> mrs = std::make_shared<MockRecStrategy>(strat, &controller);
  controller.Register(&sysA);
  mrs->Requires(&sysA);
  strat->Requires(&sysA);

  mrs->SetCanBeInterrupted(true);

  ASSERT_TRUE(controller.Schedule(mrs));
  ASSERT_EQ(sysA.GetActiveStrategy(), mrs);

  controller.Update();

  ASSERT_TRUE(mrs->IsFinished());
  ASSERT_EQ(sysA.GetActiveStrategy(), strat);
}

TEST_F(StrategyTest, TimedOut) {
  auto strat = makeStrat();
  strat->SetTimeout(0.5);

  controller.Register(&sysA);
  strat->Requires(&sysA);

  controller.Schedule(strat);
  controller.Update(1.0);

  ASSERT_EQ(strat->GetStrategyState(), StrategyState::TIMED_OUT);
}

TEST_F(StrategyTest, ThrowsIfReuseFalse) {
  auto strat = makeStrat();

  controller.Register(&sysA);
  strat->Requires(&sysA);

  ASSERT_TRUE(controller.Schedule(strat));
  ASSERT_THROW(controller.Schedule(strat), std::invalid_argument);
}

TEST_F(StrategyTest, AllowReuseIfTrue) {
  auto strat = makeStrat();
  strat->SetCanBeReused(true);

  controller.Register(&sysA);
  strat->Requires(&sysA);

  ASSERT_TRUE(controller.Schedule(strat));
  ASSERT_NO_THROW(controller.Schedule(strat));
}