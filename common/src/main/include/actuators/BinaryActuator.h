#pragma once

namespace curtinfrc {
  namespace actuators {
    enum BinaryActuatorState { kForward, kReverse };

    /**
     * @brief BinaryActuator
     * 
     * BinaryActuator baseclass for actuators with two states.
     * 
     */
    class BinaryActuator { // might implement curtinfrc::StateDevice<BinaryActuatorState> in future?
     public:
      /**
       * @brief Construct a new BinaryActuator
       * 
       * @param initialState The initial state of the BinaryActuator. Defaults to kReverse.
       */
      BinaryActuator(BinaryActuatorState initialState = kReverse) : _state(initialState) {};
      using ActuatorState = BinaryActuatorState;

      /**
       * @brief Set the current state
       * 
       * Set the state of a BinaryActuator.
       * 
       * @param state The state to set (kForward or kReverse).
       */
      void SetTarget(BinaryActuatorState state) { SetState(state); Init(); };

      /**
       * @brief Update the BinaryActuator
       * 
       * Provides a virtual method for periodic updates. Should be called every 'tick' (or as fast/frequently as possible).
       * 
       * @param dt Time (in secs) since the function was last called.
       */
      virtual void Update(double dt) = 0;

      /**
       * @brief Stop the BinaryActuator
       * 
       * Provides a virtual method to 'stop' (or cleanup) the BinaryActuator after reaching the set state.
       * 
       */
      virtual void Stop() = 0;

      /**
       * @brief Query completion of BinaryActuator
       * 
       * A virtual method that queries whether the BinaryActuator has reached the currently set state.
       * 
       * @return true The BinaryActuator has finished actuating to the currently set state.
       * @return false The BinaryActuator is still actuating to the currently set state.
       */
      virtual bool IsDone() = 0;

      /**
       * @brief Get the current state
       * 
       * @return BinaryActuatorState The currently set state.
       */
      BinaryActuatorState Get() { return _state; };

     protected:
      /**
       * @brief Set the current state
       * 
       * @param state 
       */
      void SetState(BinaryActuatorState state) { _state = state; };
      BinaryActuatorState _state;

      /**
       * @brief Initialise the BinaryActuator
       * 
       * A virtual method called once when the set state changes.
       * 
       */
      virtual void Init() {};
    };
  } // ns actuators
} // ns curtinfrc
