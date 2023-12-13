#pragma once
#include "opencv2/opencv.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "vr_cameras.h"

namespace vr {
	class VRPositionSolver {
	public:
		vr::VRCamera* camera;
		glm::mat4 proj, inverse;
		glm::vec4 viewport;

		VRPositionSolver(vr::VRCamera* camera);

		glm::vec4 solve(float x, float y, float r);
	};

	std::tuple<bool, glm::vec3> detectCircle(cv::Mat frame, cv::Scalar low, cv::Scalar high);
}
