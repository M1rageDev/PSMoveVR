#include "vr_tracking.h"

std::tuple<bool, glm::vec3> vr::detectCircle(cv::Mat frame, cv::Scalar low, cv::Scalar high) {
	// TODO: improve performance, currently its shit af (performance went from 60 to 30-45fps with 2 controllers tracked)

	// Mask out the color
	cv::Mat hsvFrame;
	cv::cvtColor(frame, hsvFrame, cv::COLOR_BGR2HSV);

	cv::Mat mask;
	cv::inRange(hsvFrame, low, high, mask);

	// Find contours
	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(mask, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

	if (contours.empty()) return { false, glm::vec3()};

	// Find largest contour
	std::vector<cv::Point> maxContour = contours[0];
	double maxArea = cv::contourArea(maxContour);
	for (auto cont : contours) {
		double area = cv::contourArea(cont);
		if (area > maxArea) {
			maxArea = area;
			maxContour = cont;
		}
	}

	// Fit circle to the contour
	cv::Point2f center;
	float radius;
	cv::minEnclosingCircle(maxContour, center, radius);

	return { true, glm::vec3(center.x, center.y, radius) };
}

glm::vec3 vr::estimatePosition(glm::vec3 ball, float f_px, uint16_t imgW, uint16_t imgH) {
	float X_px = ball.x;
	float Y_px = ball.y;
	float A_px = ball.z;

	X_px -= imgW / 2;
	Y_px = imgH / 2 - Y_px;

	float L_px = sqrtf(X_px * X_px + Y_px * Y_px);

	float k = L_px / f_px;
	float j = (L_px + A_px) / f_px;
	float l = (j - k) / (1 + j * k);

	float D_cm = vr::REAL_BALL_RADIUS * sqrtf(1 + l * l) / l;

	float fl = f_px / L_px;
	float Z_cm = D_cm * fl / sqrtf(1 + fl * fl);

	float L_cm = Z_cm * k;

	float X_cm = L_cm * X_px / L_px;
	float Y_cm = L_cm * Y_px / L_px;

	return glm::vec3(X_cm, Y_cm, Z_cm);
}
