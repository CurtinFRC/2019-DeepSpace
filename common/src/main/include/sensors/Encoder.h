#pragma once

#include <frc/CounterBase.h>

namespace curtinfrc {
  namespace sensors {
    class Encoder {
     public:
      virtual int GetEncoderTicks() = 0;
    };

    class EncoderTranslator : public Encoder {
     public:
      EncoderTranslator(frc::CounterBase &counterBase) : _counterBase(counterBase) {};
      int GetEncoderTicks() override;
    
     private:
      frc::CounterBase &_counterBase;
    };
  } // ns sensors
} // ns curtinfrc
