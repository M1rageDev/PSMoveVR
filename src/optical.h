#pragma once

#include <thread>

#include "vr_cameras.h"
#include "vr_controllers.h"
#include "vr_tracking.h"
#include "vr_math.h"

namespace psmovevr::optical
{
	inline std::thread task;
	inline bool running = false;

	inline vr::VRControllerHandler* controllers;
	inline vr::VRCamera* cameras;

	inline cv::Mat buffer;
	inline cv::Mat mask;
	inline cv::Mat frame;

	inline glm::vec4 last_cm_l;
	inline glm::vec4 last_cm_r;

	inline glm::vec4 world_cm_l;
	inline glm::vec4 world_cm_r;

	// PROGRAM MODULE FUNCTIONS
	void init(vr::VRControllerHandler* controllers_, vr::VRCamera* cameras_);
	void calibrate();
	void start();

	void loop();

	void stop();

	// OTHER FUNCTIONS
	inline glm::vec4 axisAngle(glm::quat q) {
		if (q.w > 1.f) q = glm::normalize(q);

		float x, y, z;

		float angle = 2.f * glm::acos(q.w);
		float s = glm::sqrt(1.f - q.w * q.w);
		if (s < 0.001) {
			x = q.x; y = q.y; z = q.z;
		}
		else {
			x = q.x / s; y = q.y / s; z = q.z / s;
		}

		return glm::vec4(x, y, z, angle);
	}
}
