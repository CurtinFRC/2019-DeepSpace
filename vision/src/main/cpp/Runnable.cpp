#pragma once
class VisionRunner {
 public:
  virtual void Init() = 0;
  virtual void Periodic() = 0;
};
