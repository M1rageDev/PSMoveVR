#include <iostream>
#include "opencv2/opencv.hpp"
#include <opencv2/core/utils/logger.hpp>

#include "vr_lib/vr_cameras.h"
#include "vr_lib/vr_controllers.h"
#include "vr_lib/vr_filter.h"
#include "vr_lib/vr_calibration.h"
#include "vr_lib/vr_tracking.h"
#include "vr_lib/vr_config.h" 
#include "vr_lib/vr_connection.h"

float lastTime = 0.f;
float timestep = 0.f;

const std::string DATA_BUFFER = "{}|{}|{}|{}|{}|{}|{}|{}|{}|{}|{}|{}|{}|{}|0|0|0|0|0|0|0|0|{}|{}|0|0|0|0|0|0|0|0";

int main()
{
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_ERROR);

	if (!vr::checkCamerasConnected()) {
		// Exit if we have no cameras
		printf("Error: No cameras connected.");
		exit(1);
	}

	// Create UDP connection
	vr::VRConnection connection = vr::VRConnection(49000);

	// Initialize
	vr::VRCamera* cameras = vr::initializeEveryCam(60, 640, 480, 150, 100, false);

	// Create controller handlers
	vr::VRControllerHandler controllers = vr::VRControllerHandler("00:06:f7:c9:a1:fb","00:13:8a:9c:31:42");
	psmoveapi::PSMoveAPI moveAPI(&controllers);

	// Calibrate controller IMUs
	vr::calibrateControllers(5000, &controllers, &moveAPI);

	// Set colors
	controllers.left.color = { 0.f, 1.f, 1.f };
	controllers.right.color = { 1.f, 0.f, 1.f };
	moveAPI.update();

	// Main loop
	for (;;) {
		// Check timestep
		float curTime = clock();
		timestep = (curTime - lastTime) / 1000.f;
		lastTime = curTime;

		// Tick cameras and controllers
		cameras[0].read();
		moveAPI.update();

		// Send UDP signal to SteamVR
		std::string stringBuffer = std::vformat(DATA_BUFFER, std::make_format_args());
		const char* charBuffer = stringBuffer.c_str();
		connection.send(charBuffer);

		cv::imshow("egg", cameras[0].cvImg);
		int key = cv::waitKey(1);
		if (key == 27) break;
	}

	connection.stop();
	cv::destroyAllWindows();
	vr::stopEveryCam(cameras);
	return 0;
}
