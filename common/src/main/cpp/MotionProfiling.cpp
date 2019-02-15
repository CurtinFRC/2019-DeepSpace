#include "MotionProfiling.h"

using namespace curtinfrc;

// Implement encoder follower here so we can adjust if necessary.
double pf_follow_enc(EncoderConfig c, EncoderFollower *follower, Segment *segs, int trajectory_length, int encoder_tick) {
  int segment = follower->segment;
  if (segment >= trajectory_length) {
    follower->finished = 1;
    follower->output = 0.0;
    Segment last = segs[trajectory_length - 1];
    follower->heading = last.heading;
    return 0.0;
  }
  
  double distance_covered = ((double)encoder_tick - (double)c.initial_position) / ((double)c.ticks_per_revolution);
  distance_covered = distance_covered * c.wheel_circumference;

  Segment s = segs[segment];
  if (follower->segment < trajectory_length) {
    follower->finished = 0;
    double error = s.position - distance_covered;
    double calculated_value = c.kp * error +
      c.kd * ((error - follower->last_error) / s.dt) +
      (c.kv * s.velocity + c.ka * s.acceleration);

    follower->last_error = error;
    follower->heading = s.heading;
    follower->output = calculated_value;
    follower->segment = follower->segment + 1;
    return calculated_value;
  } else {
    follower->finished = 1;
    return 0.0;
  }
}

// Pathfinder MP

void PathfinderMPMode::init() {
  _ecfg = {
    _enc->GetEncoderTicks(),
    _enc->GetEncoderTicksPerRotation(), _cfg.wheel_diameter * 0.0254 * PI, _cfg.kp, _cfg.ki, _cfg.kd, _cfg.kv, _cfg.ka
  };
  _follow = { 0, 0, 0, 0, 0 };
}

double PathfinderMPMode::calculate() {
  double ret = pf_follow_enc(_ecfg, &_follow, _segments, _seg_length, _enc->GetEncoderRotations());
  done = _follow.finished;
  return ret;
}

// void TalonMPMode::init() {
//   _talon->reset_mp();
//   _talon->SetSelectedSensorPosition(0, 0, 0);
//   _talon->configure_mp_update_rate(_segments[0].dt * 1000);
//   _talon->configure_encoder_edges_per_rev(_cfg.enc_ticks_per_rev);
//   _talon->configure_wheel_diameter(_cfg.wheel_diameter);
//   _talon->configure_pidf(
//     _cfg.kp, _cfg.ki, _cfg.kd,
//     1023.0 / _cfg.kv 
//   );
//   _talon->load_pathfinder(_segments, _seg_length);
//   _talon->Set(ctre::phoenix::motion::SetValueMotionProfile::Disable);
// }

// // Talon FP

// double TalonMPMode::calculate() {
//   _talon->load_pathfinder(_segments, _seg_length);
//   auto status = _talon->process_mp();

//   done = status.isLast;
//   return status.isLast ? ctre::phoenix::motion::SetValueMotionProfile::Hold : ctre::phoenix::motion::SetValueMotionProfile::Enable;
// }
