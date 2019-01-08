#pragma once

#include <frc/CounterBase.h>

namespace curtinfrc {
  namespace sensors {
    class encoder {
     public:
      virtual double get_encoder_ticks() = 0;
    };

    class encoder_translator : public encoder {
     public:
      encoder_translator(frc::CounterBase &counter_base) : _counter_base(counter_base) {};
      double get_encoder_ticks() override;
    
     private:
      frc::CounterBase &_counter_base;
    };
  } // ns sensors
} // ns curtinfrc
