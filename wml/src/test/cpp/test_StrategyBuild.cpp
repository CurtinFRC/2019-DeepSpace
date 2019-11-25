#include "gtest/gtest.h"

#include "strategy/Strategy.h"
#include "strategy/StrategyBuilder.h"

using namespace wml;

class MockCountingStrategy : public Strategy {
 public:
  MockCountingStrategy(int *start_count, int *periodic_count, int *stop_count)
              : start_count(start_count), periodic_count(periodic_count), stop_count(stop_count) {}

  std::string GetStrategyName() override {
    return "MockStrategy";
  }

  void OnStart() override {
    if (start_count) (*start_count)++;
  }

  void OnUpdate(double dt) override {
    if (periodic_count) (*periodic_count)++;
    if (stopAfterSingle)
      SetDone();
  }

  void OnStop() override {
    if (stop_count) (*stop_count)++;
  }

  int *start_count, *periodic_count, *stop_count;
  bool stopAfterSingle = true;
};

class MockSystem : public StrategySystem { };

class StrategyBuilderTest : public testing::Test {
 public:
  StrategyBuilderTest() {
    controller.Register(&sysA);
    controller.Register(&sysB);
  }

  std::shared_ptr<MockCountingStrategy> makeStrat() {
    std::shared_ptr<MockCountingStrategy> strat = std::make_shared<MockCountingStrategy>(&num_start, &num_periodic, &num_stop);
    strat->Requires(&sysA);
    return strat;
  }

  std::shared_ptr<MockCountingStrategy> makeStratB() {
    std::shared_ptr<MockCountingStrategy> strat = std::make_shared<MockCountingStrategy>(&num_start, &num_periodic, &num_stop);
    strat->Requires(&sysB);
    return strat;
  }

  MockSystem sysA, sysB;
  StrategyController controller;

  int num_start = 0, num_stop = 0, num_periodic = 0;
};

TEST_F(StrategyBuilderTest, SequentialOnly) {
  auto it = StrategyBuilder{}.Start()
              ->Then()->Add(makeStrat())
              ->Then()->Add(makeStrat())
              ->Then()->Add(makeStrat())
              ->Build();
              
  ASSERT_EQ(std::distance(it->begin(), it->end()), 3);
  
  controller.Schedule(it);

  for (int i = 0; i < 3; i++) {
    controller.Update();
    ASSERT_EQ(num_start, i+1);
    ASSERT_EQ(num_periodic, i+1);
    ASSERT_EQ(num_stop, i+1);
  }

  ASSERT_TRUE(it->IsFinished());
  ASSERT_EQ(sysA.GetActiveStrategy(), nullptr);
}

TEST_F(StrategyBuilderTest, ParallelOnly) {
  auto it = StrategyBuilder{}.Start()
              ->Add(makeStrat())
              ->Add(makeStratB())
              ->Build();

  ASSERT_EQ(std::distance(it->begin(), it->end()), 1);
  ASSERT_EQ(it->begin()->size(), 2);

  controller.Schedule(it);

  controller.Update();

  ASSERT_EQ(num_start, 2);
  ASSERT_EQ(num_periodic, 2);
  ASSERT_EQ(num_stop, 2);

  ASSERT_TRUE(it->IsFinished());
  ASSERT_EQ(sysA.GetActiveStrategy(), nullptr);
}

TEST_F(StrategyBuilderTest, AnyTimeout) {
  auto normalStrat = makeStrat();
  normalStrat->stopAfterSingle = false;

  auto it = StrategyBuilder{}.Start()->Add(normalStrat)->Build();
  it->SetTimeout(0.5);

  controller.Schedule(it);
  controller.Update(1.0);

  ASSERT_EQ(it->GetStrategyState(), StrategyState::TIMED_OUT);
  ASSERT_EQ(normalStrat->GetStrategyState(), StrategyState::TIMED_OUT);
}

TEST_F(StrategyBuilderTest, MultiSystemResource) {
  ASSERT_THROW(StrategyBuilder{}.Start()->Add(makeStrat())->Add(makeStrat())->Build(), std::invalid_argument);
  ASSERT_NO_THROW(StrategyBuilder{}.Start()->Add(makeStrat())->Add(makeStratB())->Build());
}