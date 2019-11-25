#include "MotionProfiling.h"

#include <networktables/NetworkTableInstance.h>

using namespace wml;

PathfinderGains::PathfinderGains(std::string name, double kP, double kI, double kD, double kV, double kA, double kG) : _name(name), _kP(kP), _kI(kI), _kD(kD), _kV(kV), _kA(kA), _kG(kG) {
  _table = nt::NetworkTableInstance::GetDefault().GetTable("PathfinderGains[" + name + "]");

  _ntbounds.emplace_back(_table, "kP", &_kP);
  _ntbounds.emplace_back(_table, "kI", &_kI);
  _ntbounds.emplace_back(_table, "kD", &_kD);
  _ntbounds.emplace_back(_table, "kV", &_kV);
  _ntbounds.emplace_back(_table, "kA", &_kA);
  _ntbounds.emplace_back(_table, "kG", &_kG);
}

double PathfinderGains::GetkP() const {
  return _kP;
}

double PathfinderGains::GetkI() const {
  return _kI;
}

double PathfinderGains::GetkD() const {
  return _kD;
}

double PathfinderGains::GetkV() const {
  return _kV;
}

double PathfinderGains::GetkA() const {
  return _kA;
}

double PathfinderGains::GetkG() const {
  return _kG;
}

double PathfinderGains::GetMaximumVelocity() const {
  return 12.0 / GetkV();
}

double PathfinderGains::GetMaximumAcceleration() const {
  return 12.0 / GetkA();
}

PathfinderController::PathfinderController(PathfinderGains gains) : _gains(gains) {
  Reset();
}

void PathfinderController::Load(std::string project, std::string pathname) {
  Reset();
  // PathWeaver has a bug where the left is actually the right, and the right is actually the left.
  CurtinPathfinder::LoadDeployedFile(project, pathname + ".left", _segmentsR);
  _trajLen = CurtinPathfinder::LoadDeployedFile(project, pathname + ".right", _segmentsL);
}

void PathfinderController::Reset() {
  _cfg = FollowerConfig{ _gains.GetkP(), _gains.GetkI(), _gains.GetkD(), _gains.GetkV(), _gains.GetkA() };
  _followerL = DistanceFollower{0, 0, 0, 0, false};
  _followerR = DistanceFollower{0, 0, 0, 0, false};
}

void PathfinderController::SetOffset(double distanceL, double distanceR) {
  _offsetL = distanceL;
  _offsetR = distanceR;
} 

std::pair<double, double> PathfinderController::Calculate(double distL, double distR, double gyroAngle) {
  double l = pathfinder_follow_distance(_cfg, &_followerL, _segmentsL, _trajLen, distL - _offsetL);
  double r = pathfinder_follow_distance(_cfg, &_followerR, _segmentsR, _trajLen, distR - _offsetR);

  double desiredAngle = r2d(_followerL.heading);
  double angleDiff = desiredAngle - gyroAngle;
  angleDiff = std::fmod(angleDiff, 360.0);
  if (std::abs(angleDiff) > 180.0) {
    angleDiff = (angleDiff > 0) ? angleDiff - 360 : angleDiff + 360;
  } 

  double turnCoeff = _gains.GetkG() * angleDiff;

  return std::pair<double, double>{l + turnCoeff, r - turnCoeff};
}

bool PathfinderController::IsDone() {
  return _followerL.finished || _followerR.finished;
}