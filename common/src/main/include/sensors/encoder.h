#pragma once

#include <frc/CounterBase.h>

namespace curtinfrc {
  namespace sensors {
    class Encoder {
     public:
      virtual double getEncoderTicks() = 0;
    };

    class EncoderTranslator : public Encoder {
     public:
      EncoderTranslator(frc::CounterBase &counterBase) : _counterBase(counterBase) {};
      double getEncoderTicks() override;
    
     private:
      frc::CounterBase &_counterBase;
    };
  } // ns sensors
} // ns curtinfrc
