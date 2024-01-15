#pragma once
#include <vector>
#include <iostream>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include "opencv2/opencv.hpp"
#include <glm/gtc/type_ptr.hpp>

#include "vr_controllers.h"

namespace vr {
	glm::vec3 calibrateGyroscope(unsigned int sampleCount, vr::VRController* controller, psmoveapi::PSMoveAPI* api);
	void calibrateControllers(unsigned int sampleCount, vr::VRControllerHandler* controllers, psmoveapi::PSMoveAPI* api);

	std::tuple<bool, glm::mat4, cv::Mat, cv::Mat> calibrateWorldMatrix(std::vector<cv::Point2f> samples, cv::Mat cameraMatrix, cv::Mat cameraDistortion);
}
