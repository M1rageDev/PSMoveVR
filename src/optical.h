#pragma once

#include "vr_cameras.h"
#include "vr_controllers.h"
#include "vr_tracking.h"

namespace psmovevr::optical
{
	inline vr::VRControllerHandler* controllers;

	inline vr::VRCamera* cameras;

	inline cv::Mat frame;
	inline glm::vec3 camera_cm;
	inline glm::vec4 world_cm;

	// PROGRAM MODULE FUNCTIONS
	void init(vr::VRControllerHandler* controllers_, vr::VRCamera* cameras_);

	void calibrate();

	void loop();

	// OTHER FUNCTIONS
}
