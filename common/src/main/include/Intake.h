#pragma once

namespace curtinfrc {
  class Intake {
   public:
    Intake() {};
    enum IntakeState { kIn, kOut, kOff };

    void SetState(IntakeState state) { _state = state; };

    virtual void Update(int dTime) = 0;

    private:
    IntakeState _state;
  };
} // ns curtinfrc
