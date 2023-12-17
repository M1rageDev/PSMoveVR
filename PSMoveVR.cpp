#include <iostream>
#include "opencv2/opencv.hpp"
#include <opencv2/core/utils/logger.hpp>

#include "glm/gtx/string_cast.hpp"

#include "vr_cameras.h"
#include "vr_controllers.h"
#include "vr_filter.h"
#include "vr_calibration.h"
#include "vr_tracking.h"
#include "vr_config.h"

float lastTime = 0.f;
float timestep = 0.f;

int main()
{
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_ERROR);

	vr::VRCamera* cameras = vr::initializeEveryCam(60, 640, 480, 20, 0, true);

	vr::VRControllerHandler controllers = vr::VRControllerHandler("00:06:f7:c9:a1:fb","00:13:8a:9c:31:42");
	psmoveapi::PSMoveAPI moveAPI(&controllers);

	controllers.left.color = { 0.f, 1.f, 1.f };
	controllers.right.color = { 1.f, 0.f, 1.f };
	moveAPI.update();

	while (true) {
		float curTime = clock();
		timestep = (curTime - lastTime) / 1000.f;
		lastTime = curTime;

		cameras[0].read();
		moveAPI.update();
	}

	vr::stopEveryCam(cameras);
	return 0;
}
