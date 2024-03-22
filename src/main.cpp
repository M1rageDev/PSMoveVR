// Standard
#include <iostream>

// VR lib
#include "vr_cameras.h"
#include "vr_controllers.h"
#include "vr_filter.h"
#include "vr_calibration.h"
#include "vr_tracking.h"
#include "vr_config.h" 
#include "vr_connection.h"

// Program modules
#include "clock.h"

const std::string DATA_BUFFER = "{}{}{}{}{}{}{}";

int main()
{
	// Create clock
	psmovevr::Clock clock = psmovevr::Clock();

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
	const char* leftSrl = "00:06:f7:c9:a1:fb";
	const char* rightSrl = "00:06:f7:c9:a1:fb";
	vr::VRControllerHandler controllers = vr::VRControllerHandler();
	psmoveapi::PSMoveAPI moveAPI(&controllers);

	// Set pointers to the left/right controller
	vr::VRController* leftController = nullptr;
	vr::VRController* rightController = nullptr;
	for (int i = 0; i < controllers.controllers.size(); i++) {
		if (controllers.controllers[i].serial == leftSrl) leftController = &controllers.controllers[i];
		else if (controllers.controllers[i].serial == rightSrl) rightController = &controllers.controllers[i];
	}

	// Calibrate controller IMUs
	vr::calibrateControllers(5000, &controllers, &moveAPI);

	// Set colors
	leftController->color = { 0.f, 1.f, 1.f };
	rightController->color = { 1.f, 0.f, 1.f };
	moveAPI.update();

	// Main loop
	for (;;) {
		// Tick
		clock.tick();

		// Tick cameras and controllers
		cameras[0].read();
		moveAPI.update();

		// Debug info
		//cv::putText(psmovevr::getPicture(0), std::to_string(1.f / clock.timestep), cv::Point(0.f, 16.f), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(255.f, 255.f, 255.f));

		// Send UDP signal to SteamVR
		//std::string stringBuffer = std::vformat(DATA_BUFFER, std::make_format_args());
		//const char* charBuffer = stringBuffer.c_str();
		//connection.send("Mm bobbies");

		cv::imshow("Camera", cameras[0].cvImg);
		int key = cv::waitKey(1);
		if (key == 27) break;
	}

	// Stop the service
	connection.stop();
	cv::destroyAllWindows();
	vr::stopEveryCam(cameras);
	return 0;
}
