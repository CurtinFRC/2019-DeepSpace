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
	Processing(Capture &capture, TapeProcessing &tape, HatchProcessing &hatch) : _capture(capture), _tape(tape), _hatch(hatch) {} // need to _capture = capture later
	virtual void Init() override;
	virtual void Periodic() override;

	virtual void GetDisplayMat(cv::Mat &displayMat) override;
	virtual cv::Size GetDisplaySize() override;
	
 private:
	Capture &_capture;

	TapeProcessing &_tape;
	HatchProcessing &_hatch;

	nt::NetworkTableEntry _usingTapeEntry;
	bool _useTape;
	bool _lastUseTape;
};
