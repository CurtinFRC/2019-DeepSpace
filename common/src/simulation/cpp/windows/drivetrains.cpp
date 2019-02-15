#include "simulation/windows/drivetrains.h"

#include "simulation/loader.h"

#include <frc/RobotController.h>
#include <algorithm>

using namespace simulation;
using namespace curtinfrc;

static std::string _field_name = "2019-field.jpg";
static ui::point   _field_tl{217, 40}, _field_br{1372, 615}; // Top left and bottom right of the field image, in pixels
static ui::point   _field_size{54 * 0.3048, 27 * 0.3048};    // Field size, in metres.

static cv::Mat    _field_img_mat;
static ui::point  _field_scale{0, 0};
static ui::point  _field_offset{0, 0};

static ui::point  _default_xy{1.6, 4.12};

void drivetrain_window::init() {
  static Usage<DrivetrainConfig>::Registry<drivetrain_window> registry(&ui::init_window<drivetrain_window, DrivetrainConfig *>);

  try {
    _field_img_mat = resource_loader::load_img_resource(_field_name);

    ui::point field_size = ui::point{(double)_field_img_mat.cols, (double)_field_img_mat.rows};
    _field_scale = ((_field_br - _field_tl) / _field_size) / field_size;
    _field_offset = _field_tl / field_size;

    std::cout << "[SIM] Field image loaded" << std::endl;
  } catch (std::runtime_error &e) {
    std::cout << "[SIM] Could not load field images for simulation." << std::endl;
    std::cout << e.what() << std::endl;
    abort();
  }
}

drivetrain_window::drivetrain_window(DrivetrainConfig *config) : ui::window("Drivetrain", _field_img_mat.cols, _field_img_mat.rows), _config(config), physics_aware() {
  _enc_sim_left = components::create_encoder(config->leftDrive.encoder);
  _enc_sim_right = components::create_encoder(config->rightDrive.encoder);
  _gyro_sim = components::create_gyro(config->gyro);

  register_button(resetPos);
  register_button(resetAngle);
  register_button(switchAlliance);

  resetPos.set_can_activate(false);
  resetPos.on_click([&](bool, ui::button&) {
    _x = _default_xy.x;
    _y = _default_xy.y;
  });

  resetAngle.set_can_activate(false);
  resetAngle.on_click([&](bool, ui::button&) {
    _heading = 0;
  });

  switchAlliance.set_can_activate(false);
  switchAlliance.on_click([&](bool, ui::button&) {
    _blue_alliance = !_blue_alliance;
  });

  _x = _default_xy.x;
  _y = _default_xy.y;
}

double drivetrain_window::get_motor_val(bool left) {
  auto &trans = left ? _config->leftDrive.transmission : _config->rightDrive.transmission;
  return trans->Get() * (trans->GetInverted() ? -1 : 1);
}

void drivetrain_window::update_encoder(bool left, double pos, double vel) {
  // double C = 2 * 3.14159265 * _config->wheelRadius;
  double twopi = 2 * 3.14159265;
  double rots = pos / twopi;
  double rotspersec = vel / twopi;
  
  auto sim_encoder = left ? _enc_sim_left : _enc_sim_right;
  auto encoder = left ? _config->leftDrive.encoder : _config->rightDrive.encoder;

  if (encoder != nullptr) {
    sim_encoder->set_counts(static_cast<int>(encoder->GetEncoderTicks() + rots * encoder->GetEncoderTicksPerRotation()));
    sim_encoder->set_counts_per_sec(static_cast<int>(rotspersec * encoder->GetEncoderTicksPerRotation()));
  }
}

void drivetrain_window::update_physics(double time_delta) {
  for (bool left : {true, false}) {
    wheel_state &state = left ? _wheel_left : _wheel_right;
    double speed = get_motor_val(left) * (left ? -1 : 1);
    double voltage = speed * frc::RobotController::GetInputVoltage();

    physics::DcMotor motor = left ? _config->leftDrive.motor : _config->rightDrive.motor;
    motor = motor.reduce(left ? _config->leftDrive.reduction : _config->rightDrive.reduction);

    double current = motor.current(voltage, state.angular_vel);
    double torque = motor.torque(current);
    double accel = torque / (_config->mass / 2.0 * _config->wheelRadius);

    state.angular_vel += (accel * time_delta) / _config->wheelRadius;
    state.angular_position += state.angular_vel * time_delta;

    update_encoder(left, state.angular_vel * time_delta, state.angular_vel);
  }

  double robot_angular_vel = (_wheel_right.angular_vel - _wheel_left.angular_vel) * _config->wheelRadius / _config->trackwidth;
  _linear_vel = (_wheel_left.angular_vel + _wheel_right.angular_vel) * _config->wheelRadius / 2.0;
  _heading += robot_angular_vel * time_delta;

  _x += _linear_vel * time_delta * std::cos(_heading);
  _y += _linear_vel * time_delta * std::sin(_heading);

  if (_gyro_sim != nullptr) {
    // Unfortunately WPILib convention is backwards - clockwise is +ve according to frc::Gyro.
    _gyro_sim->add_angle(-robot_angular_vel * time_delta * 180 / 3.1415);
    _gyro_sim->set_rate(-robot_angular_vel * 180 / 3.1415);
  }
}

void drivetrain_window::on_mouse(int event, ui::point pt) {
  _mouse = pt;
}

void drivetrain_window::render_field(cv::Mat &img) {
  if (_blue_alliance)
    _field_img_mat.copyTo(img);
  else
    cv::flip(_field_img_mat, img, 1);
}

void drivetrain_window::render(cv::Mat &img) {
  render_field(img);

  double xscale = _field_scale.x;
  double yscale = _field_scale.y;

  for (double x = 0; x <= 1 / xscale; x += 1) {
    ui::line{ui::point{_field_offset.x + x * xscale, 0}, ui::point{_field_offset.x + x * xscale, 1}}.draw(img, ui::colour::gray() * 0.5, 1);
  }

  for (double y = 0; y <= 1 / yscale; y += 1) {
    ui::line{ui::point{0, _field_offset.y + y * yscale}, ui::point{1, _field_offset.y + y * yscale}}.draw(img, ui::colour::gray() * 0.5, 1);
  }

  double xval = 0.02;
  double yval = 0.10;

  ui::point{xval, yval}.textl(img, ("X: " + ui::utils::fmt_precision(_x, 2) + "m").c_str(), 0.5, ui::colour::black(), 2);
  ui::point{xval, yval += 0.04}.textl(img, ("Y: " + ui::utils::fmt_precision(_y, 2) + "m").c_str(), 0.5, ui::colour::black(), 2);

  if (_enc_sim_left != nullptr && _enc_sim_right != nullptr) {
    yval += 0.1;
    ui::point{xval, yval}.textl(img, ("Enc(L): " + std::to_string(_config->leftDrive.encoder->GetEncoderTicks())).c_str(), 0.5, ui::colour::black(), 2);
    ui::point{xval, yval += 0.04}.textl(img, ("Enc(R): " + std::to_string(_config->rightDrive.encoder->GetEncoderTicks())).c_str(), 0.5, ui::colour::black(), 2);
  }

  if (_gyro_sim != nullptr) {
    yval += 0.1;
    ui::point{xval, yval}.textl(img, ("Gyro: " + ui::utils::fmt_precision(_config->gyro->GetAngle(), 2) + " deg").c_str(), 0.5, ui::colour::black(), 2);
    ui::point{xval, yval += 0.04}.textl(img, (ui::utils::fmt_precision(_config->gyro->GetRate(), 2) + " deg/s").c_str(), 0.5, ui::colour::black(), 2);
  }

  ui::point mouse_real = (_mouse - _field_offset) / _field_scale;
  ui::point{xval, 0.95}.textl(img, ("Mouse: " + ui::utils::fmt_precision(mouse_real.x, 2) + "m, " + ui::utils::fmt_precision(mouse_real.y, 2) + "m").c_str(), 0.5, ui::colour::black(), 2);

  draw_robot(img);
}

void drivetrain_window::draw_robot(cv::Mat &img) {
  double width = _config->trackwidth;
  double depth = _config->trackdepth;

  double rot = -3.14159265 / 2 - _heading;

  ui::point robot{_x, _y};

  ui::point fl = robot + ui::point{-width / 2, depth / 2}.rotate(rot);
  ui::point fr = robot + ui::point{width / 2, depth / 2}.rotate(rot);
  ui::point bl = robot + ui::point{-width / 2, -depth / 2}.rotate(rot);
  ui::point br = robot + ui::point{width / 2, -depth / 2}.rotate(rot);

  ui::line left   = _field_offset + ui::line{ fl * _field_scale, bl * _field_scale };
  ui::line right  = _field_offset + ui::line{ fr * _field_scale, br * _field_scale };
  ui::line front  = _field_offset + ui::line{ fl * _field_scale, fr * _field_scale };
  ui::line back   = _field_offset + ui::line{ bl * _field_scale, br * _field_scale };

  left.draw(img, _left_colour * (0.5 + std::abs(get_motor_val(true)) / 2), 2);
  right.draw(img, _right_colour * (0.5 + std::abs(get_motor_val(true)) / 2), 2);
  front.draw(img, _front_colour, 2);
  back.draw(img, ui::colour::black(), 2);
}