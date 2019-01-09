#include "sensors/Encoder.h"

double curtinfrc::sensors::EncoderTranslator::GetEncoderTicks() {
    return _counter_base.Get();
}
