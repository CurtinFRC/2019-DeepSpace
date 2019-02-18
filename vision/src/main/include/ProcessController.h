#pragma once
#include "threading/Runnable.h"
#include "Display.h"
#include "Process.h"
#include "TapeProcessing.h"
#include "HatchProcessing.h"
#include <string.h>
#include <iostream>
#include "networktables/NetworkTableInstance.h"

class Processing : public Runnable, public Displayable {
 public:
	void ProcessPick();
	nt::NetworkTableEntry TapeCamSet;
	std::string GetProcessType();
	Processing(Capture &capture, TapeProcessing &tape, HatchProcessing &hatch) : _capture(capture), _tape(tape), _hatch(hatch) {} // need to _capture = capture later
	virtual void Init() override;
	virtual void Periodic() override;

	virtual void GetDisplayMat(cv::Mat &displayMat) override;
	virtual cv::Size GetDisplaySize() override;

 protected:
  std::mutex _classMutex;
  cs::VideoMode _videoMode;

  cv::Mat _imgProcessedTrack;
  cv::Mat _imgProcessedThresh;
  cv::Mat _imgProcessing;

 private:
	Capture &_capture;
	TapeProcessing &_tape;
	HatchProcessing &_hatch;
	std::string processType;
	bool _tapeSet;
	bool _lastTapeSet;
};
