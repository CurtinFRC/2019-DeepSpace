#pragma once

#include <frc/Encoder.h>

namespace curtinfrc {
namespace sensors {
  class Encoder {
   public:
    Encoder(int encoderTicksPerRotation) : _encoderTicksPerRotation(encoderTicksPerRotation){};
    virtual int  GetEncoderTicks() = 0;
    virtual void ResetEncoder()    = 0;

    double GetEncoderRotations();
    int    GetEncoderTicksPerRotation();

   private:
    int _encoderTicksPerRotation;
  };

  class DigitalEncoder : public Encoder {
   public:
    DigitalEncoder(int channelA, int channelB, int ticksPerRotation)
        : _channelA(channelA),
          _channelB(channelB),
          _nativeEncoder(channelA, channelB),
          Encoder(ticksPerRotation){};

    int GetEncoderTicks() override;
    void ResetEncoder() override;

    int GetChannelA();
    int GetChannelB();

   private:
    int          _channelA, _channelB;
    frc::Encoder _nativeEncoder;
  };
}  // namespace sensors
}  // namespace curtinfrc
