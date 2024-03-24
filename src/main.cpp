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
	// TODO: color calibration stored in controller CFG
	leftController->optical.lowerColor = { 29, 0, 75 };
	leftController->optical.higherColor = { 106, 255, 255 };
	rightController->optical.lowerColor = { 95, 0, 92 };
	rightController->optical.higherColor = { 169, 169, 255 };

	// Let there be light!
	leftController->color = { 0.f, 1.f, 1.f };
	rightController->color = { 1.f, 0.f, 1.f };
	moveAPI.update();
}

int main(int argc, char** argv)
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

	// Calibrate if needed
	optical::calibrate();
	vr::calibrateControllers(5000, &controllers, &moveAPI);

	// Start the optical task
	optical::start();

	// Main loop
	for (;;) {
		// Tick the clock and controllers
		ps_clock.tick();
		moveAPI.update();

		// Debug the hz
		optical::debugInfo = std::to_string(1.f / ps_clock.timestep) + " hz";

		// Send UDP signal to SteamVR
		//std::string stringBuffer = std::vformat(DATA_BUFFER, std::make_format_args());
		//const char* charBuffer = stringBuffer.c_str();
		//connection.send("Mm bobbies");

		// Exit
		if (optical::running == false) break;
	}

	// Stop the service
	connection.stop();
	optical::stop();
	cv::destroyAllWindows();
	vr::stopEveryCam(cameras);
	return 0;
}
