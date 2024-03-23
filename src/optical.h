#pragma once

#include "vr_cameras.h"
#include "vr_controllers.h"
#include "vr_tracking.h"

namespace psmovevr::optical
{
	inline vr::VRControllerHandler* controllers;

	inline vr::VRCamera* cameras;

	// PROGRAM MODULE FUNCTIONS
	void init(vr::VRControllerHandler* controllers_, vr::VRCamera* cameras_);

	void calibrate();

	void loop();

	// OTHER FUNCTIONS
}
