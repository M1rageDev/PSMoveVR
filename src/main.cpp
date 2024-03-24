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
#include "orientation.h"

using namespace psmovevr;

// CONSTANTS
const std::string DATA_BUFFER = "{}{}{}{}{}{}{}";

// INSTANCES
Clock ps_clock;
Connection connection;
vr::VRCamera* cameras;
vr::VRControllerHandler controllers = vr::VRControllerHandler();
psmoveapi::PSMoveAPI moveAPI(&controllers);
vr::VRController* leftController;
vr::VRController* rightController;

void init() {
	ps_clock = Clock();
	cameras = vr::initializeEveryCam(60, 640, 480, 20, 0, true);
	connection = Connection();
	connection.start(49000);

	// Create controller handlers
	const char* leftSrl = "00:06:f7:c9:a1:fb";
	const char* rightSrl = "00:13:8a:9c:31:42";

	// Allocate the space for the left/right controller in the controller vector
	controllers.allocateControllerSpace(leftSrl);
	controllers.allocateControllerSpace(rightSrl);

	// Set pointers to the left/right controller
	leftController = &controllers.controllers[0];
	rightController = &controllers.controllers[1];

	// Set controller optical colors
	leftController->optical.lowerColor = { 29, 0, 75 };
	leftController->optical.higherColor = { 106, 255, 255 };
	rightController->optical.lowerColor = { 95, 0, 92 };
	rightController->optical.higherColor = { 169, 169, 255 };

	// Let there be light!
	leftController->color = { 0.f, 1.f, 1.f };
	rightController->color = { 1.f, 0.f, 1.f };
	moveAPI.update();
}

int main()
{
	// Error handling
	if (!vr::checkCamerasConnected()) {
		// Exit if we have no cameras
		printf("Error: No cameras connected.");
		exit(1);
	}

	// Initialize
	init();
	optical::init(&controllers, cameras);
	orientation::init(&controllers, cameras);

	// Main loop
	for (;;) {
		// Tick the clock and controllers
		ps_clock.tick();
		moveAPI.update();

		// Loop the optical task
		// TODO: thread the optical task, making the orientation/UDP tasks run faster on the main thread
		optical::loop();

		// Loop the orientation task
		orientation::loop();

		// Debug info
		cv::putText(cameras[0].cvImg, std::to_string(1.f / ps_clock.timestep) + "hz", {0, 16}, cv::FONT_HERSHEY_PLAIN, 1, {255.f, 255.f, 255.f});

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
