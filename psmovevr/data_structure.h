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
	std::string masterPath;
	std::string controllersFolder;
	std::string camerasFolder;
	
	// Global
	vr::VRControllerHandler controllerHandler;
	psmoveapi::PSMoveAPI* moveApi;
	std::vector<controller_structure> controllers;
	std::vector<camera_structure> cameras;
}
#endif
