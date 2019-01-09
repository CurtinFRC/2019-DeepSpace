#pragma once

#include <frc/CounterBase.h>

namespace curtinfrc {
  namespace sensors {
    class Encoder {
     public:
      virtual double GetEncoderTicks() = 0;
    };

    class EncoderTranslator : public Encoder {
     public:
      EncoderTranslator(frc::CounterBase &counter_base) : _counter_base(counter_base) {};
      double GetEncoderTicks() override;
    
     private:
      frc::CounterBase &_counter_base;
    };
  } // ns sensors
} // ns curtinfrc
