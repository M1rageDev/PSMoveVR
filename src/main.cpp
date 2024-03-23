// Standard
#include <iostream>

// VR lib
#include "vr_cameras.h"
#include "vr_controllers.h"
#include "vr_filter.h"
#include "vr_calibration.h"
#include "vr_tracking.h"
#include "vr_config.h" 

// Program modules
#include "clock.h"
#include "connection.h"
#include "optical.h"

const std::string DATA_BUFFER = "{}{}{}{}{}{}{}";

using namespace psmovevr;

int main()
{
	// Error handling
	if (!vr::checkCamerasConnected()) {
		// Exit if we have no cameras
		printf("Error: No cameras connected.");
		exit(1);
	}

	// Create clock
	Clock clock = Clock();

	// Initialize the cameras
	vr::VRCamera* cameras = vr::initializeEveryCam(60, 640, 480, 20, 0, true);

	// Create UDP connection
	Connection connection = Connection(49000);

	// Create controller handlers
	const char* leftSrl = "00:06:f7:c9:a1:fb";
	const char* rightSrl = "00:13:8a:9c:31:42";
	vr::VRControllerHandler controllers = vr::VRControllerHandler();
	psmoveapi::PSMoveAPI moveAPI(&controllers);

	// Allocate the space for the left/right controller in the controller vector
	controllers.allocateControllerSpace(leftSrl);
	controllers.allocateControllerSpace(rightSrl);

	// Set pointers to the left/right controller
	vr::VRController* leftController = &controllers.controllers[0];
	vr::VRController* rightController = &controllers.controllers[1];

	// Calibrate controller IMUs
	vr::calibrateControllers(5000, &controllers, &moveAPI);

	// Set colors
	leftController->color = { 0.f, 1.f, 1.f };
	rightController->color = { 1.f, 0.f, 1.f };
	moveAPI.update();

	// Main loop
	for (;;) {
		// Tick the clock
		clock.tick();

		// Read cameras and controllers
		cameras[0].read();
		moveAPI.update();

		// Debug info
		cv::putText(cameras[0].cvImg, std::to_string(1.f / clock.timestep), cv::Point(0.f, 16.f), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(255.f, 255.f, 255.f));

		// Send UDP signal to SteamVR
		//std::string stringBuffer = std::vformat(DATA_BUFFER, std::make_format_args());
		//const char* charBuffer = stringBuffer.c_str();
		//connection.send("Mm bobbies");

		// Show picture and exit program if ESC pressed
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
