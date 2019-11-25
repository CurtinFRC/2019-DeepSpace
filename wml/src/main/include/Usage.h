#pragma once

#include <algorithm>
#include <functional>
#include <vector>
#include <map>

namespace wml {

/**
 * Usage Event classification for the lifecycle of a Usage'd object
 */
enum class UsageEvent { CREATE, DESTROY };

/**
 * Usage class - used to track the usage of a certain type of object. Typically used in simulation to detect when objects are
 * in use.
 * 
 * @param TARGET The type to track.
 */
template <typename TARGET>
class Usage {
 public:
  /**
   * Scoped RAII class, used to automatically report the usage of a class on allocation or deallocation.
   * 
   * Add an instance of this to the class you wish to track.
   */
  class Scoped {
   public:
    Scoped(TARGET *target) : _target(target) { INSTANCE()->Create(_target); }

    ~Scoped() { INSTANCE()->Destroy(_target); }

    TARGET *_target;
  };

  /**
   * Registry of usage types - used to automatically translate the usage of the class into the creation and 
   * deletion of another class. This is typically used in simulation to spawn a new window when the object
   * is created, and despawn the window when it is destructed.
   * 
   * @param REG_TYPE The registry type - the type to create / 'bind'
   */
  template <typename REG_TYPE>
  class Registry {
   public:
    using supplier_func = typename std::function<REG_TYPE *(TARGET *)>;

    void ListenFunc(UsageEvent event, TARGET *target) {
      if (event == UsageEvent::CREATE) {
        _map[target] = _supplier(target);
      } else {
        if (_map.find(target) != _map.end()) {
          auto it = _map.find(target);
          delete it->second;
          _map.erase(it);
        }
      }
    }
    
    Registry(supplier_func supplier) : _supplier(supplier) {
      INSTANCE()->Listen(std::bind(&Registry::ListenFunc, this, std::placeholders::_1, std::placeholders::_2));
    }

   private:
    std::map<TARGET *, REG_TYPE *> _map;
    supplier_func _supplier;
  };

  using Consumer = typename std::function<void(UsageEvent, TARGET *)>;

  /**
   * Get the Usage singleton
   */
  static Usage *INSTANCE() {
    static Usage _inst;
    return &_inst;
  }

  /**
   * Mark a target type as created (constructed)
   */
  void Create(TARGET *target) {
    _targets.push_back(target);
    std::for_each(_listeners.begin(), _listeners.end(), [&](Consumer f) { f(UsageEvent::CREATE, target); });
  }

  /**
   * Mark a target type as destroyed (destructed)
   */
  void Destroy(TARGET *target) {
    std::for_each(_listeners.begin(), _listeners.end(), [&](Consumer f) { f(UsageEvent::DESTROY, target); });
    _targets.erase(std::remove(_targets.begin(), _targets.end(), target), _targets.end());
  }

  /**
   * Listen for events for this target type
   */
  void Listen(Consumer func) { _listeners.push_back(func); }

  /**
   * Get all target type objects currently in use
   */
  std::vector<TARGET *> &all() { return _targets; }

 private:
  std::vector<Consumer> _listeners;
  std::vector<TARGET *> _targets;
};

}  // namespace wml