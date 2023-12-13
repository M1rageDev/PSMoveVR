#include "vr_tracking.h"

vr::VRPositionSolver::VRPositionSolver(vr::VRCamera* camera) {
	this->camera = camera;

	proj = glm::perspectiveFov<float>(glm::radians(75.f), camera->eye->getWidth(), camera->eye->getHeight(), 0.001f, 10000.f);
	inverse = glm::inverse(proj);
	viewport = glm::vec4(0.f, 0.f, camera->eye->getWidth(), camera->eye->getHeight());
}

glm::vec4 vr::VRPositionSolver::solve(float x, float y, float r) {
    // Code kindly taken from https://github.com/thp/psmoveapi/blob/master/src/tracker/psmove_fusion.cpp#L105
    float wx = 2.f * (x - viewport[0]) / viewport[2] - 1.f;
    float wy = 2.f * (1.f - (y - viewport[1]) / viewport[3]) - 1.f;

    float xx = (proj[0][0] * viewport[2]) / (4.f * r);

    float zc = wx * inverse[0][2] +
        wy * inverse[1][2] +
        inverse[3][2];
    float zf = inverse[2][2];
    float wc = wx * inverse[0][3] +
        wy * inverse[1][3] +
        inverse[3][3];
    float wf = inverse[2][3];

    float wz = (xx * wc - zc) / (zf - xx * wf);

   return (inverse * glm::vec4(wx, wy, wz, 1.f));
}

std::tuple<bool, glm::vec3> vr::detectCircle(cv::Mat frame, cv::Scalar low, cv::Scalar high) {
	cv::Mat hsvFrame;
	cv::cvtColor(frame, hsvFrame, cv::COLOR_BGR2HSV);

	cv::Mat mask;
	cv::inRange(hsvFrame, low, high, mask);

	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(mask, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

	if (contours.empty()) return { false, glm::vec3() };

    auto maxContour = std::max_element(
        contours.begin(), contours.end(),
        [](const auto& a, const auto& b) {
            return cv::contourArea(a) < cv::contourArea(b);
        }
    );

	cv::Point2f center;
	float radius;
	cv::minEnclosingCircle(*maxContour, center, radius);

	return { true, glm::vec3(center.x, center.y, radius) };
}
