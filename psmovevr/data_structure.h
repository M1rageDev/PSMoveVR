#pragma once
#include <iostream>
#include "psmoveapi/psmoveapi.h"

#include "vr_cameras.h"
#include "vr_controllers.h"

#include "opencv2/opencv.hpp"
#include <glm/glm.hpp>

#ifndef PSMOVEVR_DATA_STRUCT
#define PSMOVEVR_DATA_STRUCT
namespace psmovevr
{
	struct controller_structure {
		// General
		uint8_t id;

		// Optical
		cv::Scalar lowerColor;
		cv::Scalar upperColor;
	};

	struct camera_structure {
		// General
		vr::VRCamera eye;
		std::string name;
		uint8_t id;

		// Optical
		cv::Mat matrix, distortion;

		// Pose
		glm::mat4 tracker;
	};

	// Paths
	inline std::string masterPath;
	inline std::string controllersFolder;
	inline std::string camerasFolder;
	
	// Global
	inline vr::VRControllerHandler controllerHandler = vr::VRControllerHandler();
	inline psmoveapi::PSMoveAPI moveApi(&psmovevr::controllerHandler);

	inline std::vector<controller_structure> controllers;
	inline std::vector<camera_structure> cameras;
}
#endif
