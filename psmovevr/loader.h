#pragma once
#include "opencv2/opencv.hpp"
#include <opencv2/core/utils/logger.hpp>

#include "vr_cameras.h"
#include "vr_controllers.h"

#include "errors.h"
#include "data_structure.h"

namespace psmovevr
{
	void loadCameras();
	void loadControllers();

	void stopCameras();
}
