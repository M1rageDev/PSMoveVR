#pragma once
#include "vr_calibration.h"

#include "data_structure.h"

namespace psmovevr
{
	// Cameras
	void readCameras();
	cv::Mat getPicture(uint8_t index);

	// Controllers
	void tickControllers();
	void setControllerColor(uint8_t index, RGB color);

	// Calibration
	void calibrateImu(unsigned int samples);
}
