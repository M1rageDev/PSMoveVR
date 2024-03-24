#pragma once
#include "opencv2/opencv.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "vr_cameras.h"

namespace vr {
	const float REAL_BALL_RADIUS = 2.25f;

	std::tuple<bool, glm::vec3> detectCircle(vr::VRCamera camera, cv::Mat frame, cv::Scalar low, cv::Scalar high);

	glm::vec3 estimatePosition(glm::vec3 circle, float f_px, uint16_t imgW, uint16_t imgH);

	glm::vec3 estimatePosition(glm::vec3 circle1, glm::vec3 circle2, vr::VRCamera camera1, vr::VRCamera camera2);
}
