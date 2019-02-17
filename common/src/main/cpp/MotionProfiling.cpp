#include "MotionProfiling.h"

#include <networktables/NetworkTableInstance.h>

using namespace curtinfrc;

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

PathfinderController::PathfinderController(PathfinderGains gains) : _gains(gains) {
  Reset();
}

void PathfinderController::Load(std::string project, std::string pathname) {
  Reset();
  CurtinPathfinder::LoadDeployedFile(project, pathname + ".left", _segmentsL);
  _trajLen = CurtinPathfinder::LoadDeployedFile(project, pathname + ".right", _segmentsR);
}

void PathfinderController::Reset() {
  _cfg = FollowerConfig{ _gains.GetkP(), _gains.GetkI(), _gains.GetkD(), _gains.GetkV(), _gains.GetkA() };
  _followerL = DistanceFollower{0, 0, 0, 0, false};
  _followerR = DistanceFollower{0, 0, 0, 0, false};
}

std::pair<double, double> PathfinderController::Calculate(double distL, double distR, double gyroAngle) {
  double l = pathfinder_follow_distance(_cfg, &_followerL, _segmentsL, _trajLen, distL);
  double r = pathfinder_follow_distance(_cfg, &_followerR, _segmentsR, _trajLen, distR);

  double desiredAngle = -r2d(_followerL.heading);
  double turnCoeff = _gains.GetkG() * (desiredAngle - gyroAngle);

  return std::pair<double, double>{l + turnCoeff, r - turnCoeff};
}