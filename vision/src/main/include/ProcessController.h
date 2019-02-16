#pragma once
#include "threading/Runnable.h"
#include "Display.h"
#include "TapeProcessing.h"
#include "HatchProcessing.h"

class Processing : public Runnable, public Displayable {
 public:
 private:
	Capture _capture{"HatchSide", 0};
	TapeProcessing _tape{_capture};
	HatchProcessing _hatch{_capture};
}