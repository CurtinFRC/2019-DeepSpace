#pragma once
#include "threading/Runnable.h"
#include "Display.h"
#include "TapeProcessing.h"
#include "process.h"
#include "HatchProcessing.h"
#include <string.h>
#include <iostream>
#include "networktables/NetworkTableInstance.h"

class Processing : public Runnable, public Displayable {
 public:
	void ProcessPick();
	nt::NetworkTableEntry TapeCamSet;
	std::string GetProcessType();
	Processing(Capture &capture); // need to _capture = capture later
	virtual void Init() override;
  	virtual void Periodic() override;

	virtual void GetDisplayMat(cv::Mat &displayMat) override {};
	virtual cv::Size GetDisplaySize() override {};

 protected:
  std::mutex _classMutex;
  cs::VideoMode _videoMode;

  cv::Mat _imgProcessedTrack;
  cv::Mat _imgProcessedThresh;
  cv::Mat _imgProcessing;

 private:
	Capture _capture{"HatchSide", 0};
	TapeProcessing _tape{_capture};
	HatchProcessing _hatch{_capture};
	std::string processType;
};