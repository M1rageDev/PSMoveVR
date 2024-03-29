#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "opencv2/opencv.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/string_cast.hpp"
#include "psmoveapi/psmoveapi.h"

#include "vr_filter.h"

namespace vr {
	inline glm::quat Q90 = glm::quat(0.7071069f, 0.7071067f, 0.f, 0.f);

	struct VRButtonStructure {
		float trigger;
		bool square, triangle, cross, circle, move, select, start, playstation;
	};

	struct VROpticalController {
		cv::Scalar lowerColor, higherColor;
	};

	class VRController {
	public:
		std::string serial;
		bool connected;

		RGB color = { 0.f, 0.f, 0.f };
		float rumble = 0.f;

		VRButtonStructure buttons = VRButtonStructure();

		glm::vec3 accel = { 0.f, 0.f, 0.f };
		glm::vec3 gyro = { 0.f, 0.f, 0.f };
		glm::vec3 gyroOffsets = { 0.f, 0.f, 0.f };
		vr::VRMadgwick orientation = vr::VRMadgwick();

		vr::VROpticalController optical;

		float timestep = 0.f;
		float lastTime = 0.f;

		void connect(Controller* controller);
		void update(Controller* controller);
	};

	struct VRControllerHandler : public psmoveapi::Handler {
		VRControllerHandler(std::string leftSerial, std::string rightSerial);

		virtual void connect(Controller* controller);
		virtual void update(Controller* controller);
		virtual void disconnect(Controller* controller);

		VRController* getController(uint8_t index);

		bool isConnected(uint8_t index);
		glm::vec3 getGyro(uint8_t index);
		glm::vec3 getAccel(uint8_t index);

		VRController leftController;
		VRController rightController;
	};
}
