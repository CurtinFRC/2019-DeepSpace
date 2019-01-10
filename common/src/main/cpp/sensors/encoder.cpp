#include "sensors/Encoder.h"

double curtinfrc::sensors::EncoderTranslator::getEncoderTicks() {
    return _counterBase.Get();
}
