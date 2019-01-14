#include "sensors/Encoder.h"

int curtinfrc::sensors::EncoderTranslator::GetEncoderTicks() {
    return _counterBase.Get();
}
