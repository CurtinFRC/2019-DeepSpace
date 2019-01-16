#pragma once

#include <frc/CounterBase.h>

namespace curtinfrc {
  namespace sensors {
    class Encoder {
     public:
      Encoder(int encoderTicksPerRotation) : _encoderTicksPerRotation(encoderTicksPerRotation) {};
      virtual int GetEncoderTicks() = 0;
      virtual void ResetEncoder() = 0;
      double GetEncoderRotations();

      int GetEncoderTicksPerRotation();

     private:
      int _encoderTicksPerRotation;
    };

    class EncoderTranslator : public Encoder {
     public:
      EncoderTranslator(int encoderTicksPerRotation, frc::CounterBase &counterBase) : Encoder(encoderTicksPerRotation), _counterBase(counterBase) {};
      int GetEncoderTicks() override;
      void ResetEncoder() override;
    
     private:
      frc::CounterBase &_counterBase;
    };
  } // ns sensors
} // ns curtinfrc
