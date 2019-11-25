#include "strategy/Strategy.h"
#include "strategy/StrategyBuilder.h"

#include <exception>

using namespace wml;

StrategyQueue::StrategyQueue() : Strategy() {
  SetCanBeReused(false);
  SetCanBeInterrupted(true);
}

std::string StrategyQueue::GetStrategyName() {
  if (!_custom_name.empty()) {
    return "StrategyQueue[" + _custom_name + ", n(queued): " + std::to_string(_queue.size()) + "]";
  } else {
    return "StrategyQueue[n(queued): " + std::to_string(_queue.size()) + "]";
  }
}

void StrategyQueue::OnStart() { }

void StrategyQueue::OnUpdate(double dt) {
  if (!_queue.empty()) {
    bool allFinished = true;
    for (auto strat : _queue.front()) {
      strat->Update(dt);
      if (!strat->IsFinished())
        allFinished = false;
    }

    if (allFinished) {
      _queue.pop_front();
      if (_queue.empty())
        SetDone();
    }
  } else {
    SetDone();
  }
}

void StrategyQueue::OnStop() {
  if (GetStrategyState() != StrategyState::DONE) {
    // Something is abnormal
    while (!_queue.empty()) {
      for (auto strat : _queue.front()) {
        strat->Stop(GetStrategyState());
      }
      _queue.pop_front();
    }
  }
}

StrategyQueue::node_queue_t::const_iterator StrategyQueue::begin() {
  return _queue.cbegin();
}

StrategyQueue::node_queue_t::const_iterator StrategyQueue::end() {
  return _queue.cend();
}

void StrategyQueue::InheritRequirements(std::shared_ptr<Strategy> strat) {
  if (!strat->CanBeInterrupted())
    SetCanBeInterrupted(false);
  
  for (auto it : strat->GetRequirements()) {
    Requires(it);
  }
}

StrategyBuilder *StrategyBuilder::Start() {
  if (_q != nullptr) {
    _q = nullptr;
  }

  _q = std::make_shared<StrategyQueue>();

  return this;
}

StrategyBuilder *StrategyBuilder::Add(std::shared_ptr<Strategy> strat) {
  if (_q == nullptr) {
    _q = std::make_shared<StrategyQueue>();
  }

  if (_q->_queue.size() == 0)
    _q->_queue.emplace_back();

  auto &parallels = _q->_queue.back();
  
  for (auto it : parallels) {
    for (auto resource : strat->GetRequirements()) {
      if (it->IsRequiring(resource)) {
        throw std::invalid_argument("Strategies in parallel may not use the same Systems!");
      }
    }
  }

  parallels.push_back(strat);
  _q->InheritRequirements(strat);

  return this;
}

StrategyBuilder *StrategyBuilder::Then() {
  if (_q == nullptr) {
    _q = std::make_shared<StrategyQueue>();
  }

  // If the back of the queue is empty, there's no point in adding another stage
  if (_q->_queue.size() == 0 || _q->_queue.back().size() > 0) {
    _q->_queue.emplace_back();
  }

  return this;
}

std::shared_ptr<StrategyQueue> StrategyBuilder::Build(std::string name) {
  if (!name.empty()) {
    _q->_custom_name = name;
  }
  std::shared_ptr<StrategyQueue> strat = _q;
  _q = nullptr;
  return strat;
}