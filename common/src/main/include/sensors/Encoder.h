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
      EncoderTranslator(frc::CounterBase &counterBase) : _counterBase(counterBase) {};
      double GetEncoderTicks() override;
    
     private:
      frc::CounterBase &_counterBase;
    };
  } // ns sensors
} // ns curtinfrc
