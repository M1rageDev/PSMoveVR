#pragma once
#include <stdio.h>
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "psmoveapi/psmoveapi.h"

namespace vr {
	struct VRButtonStructure {
		float trigger;
		bool square, triangle, cross, circle, move, select, start, playstation;
	};

	struct VRController {
		RGB color = { 0.f, 0.f, 0.f };
		float rumble = 0.f;

		VRButtonStructure buttons = VRButtonStructure();

		glm::vec3 accel = { 0.f, 0.f, 0.f };
		glm::vec3 gyro = { 0.f, 0.f, 0.f };

		float timestep = 0.f;
		float lastTime = 0.f;

		void update(Controller* controller, float mBeta);
	};

	struct VRControllerHandler : public psmoveapi::Handler {
		VRControllerHandler(const char* leftSerialNumber, const char* rightSerialNumber);
		VRControllerHandler(glm::quat initialRot, const char* leftSerialNumber, const char* rightSerialNumber, float madgwickBeta_);

		virtual void connect(Controller* controller);

		virtual void update(Controller* controller);

		virtual void disconnect(Controller* controller);

		glm::vec3 getGyro(bool leftFunction);

		glm::vec3 getAccel(bool leftFunction);

		float madgwickBeta = 0.035f;

		VRController left;
		const char* leftSerial;
		bool leftConnected;
		VRController right;
		const char* rightSerial;
		bool rightConnected;
	};
}
