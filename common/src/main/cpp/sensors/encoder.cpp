#include "sensors/encoder.h"

double curtinfrc::sensors::encoder_translator::get_encoder_ticks() {
    return _counter_base.Get();
}
