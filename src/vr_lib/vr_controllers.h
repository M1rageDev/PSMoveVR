#pragma once
#include <iostream>
#include <vector>
#include <string>

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "psmoveapi/psmoveapi.h"

#include "vr_filter.h"

namespace vr {
	struct VRButtonStructure {
		float trigger;
		bool square, triangle, cross, circle, move, select, start, playstation;
	};

	struct VRController {
		std::string serial;

		RGB color = { 0.f, 0.f, 0.f };
		float rumble = 0.f;

		VRButtonStructure buttons = VRButtonStructure();

		glm::vec3 accel = { 0.f, 0.f, 0.f };
		glm::vec3 gyro = { 0.f, 0.f, 0.f };
		glm::vec3 gyroOffsets = { 0.f, 0.f, 0.f };
		vr::VRMadgwick orientation = vr::VRMadgwick();

		float timestep = 0.f;
		float lastTime = 0.f;

		void connect(Controller* controller);
		void update(Controller* controller);
	};

	struct VRControllerHandler : public psmoveapi::Handler {
		VRControllerHandler();

		virtual void connect(Controller* controller);

		virtual void update(Controller* controller);

		virtual void disconnect(Controller* controller);

		void allocateControllerSpace(const char* serial);

		glm::vec3 getGyro(uint8_t index);

		glm::vec3 getAccel(uint8_t index);

		std::vector<VRController> controllers;
	};
}
