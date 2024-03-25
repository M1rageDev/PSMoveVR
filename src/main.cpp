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
vr::VRControllerHandler controllers = vr::VRControllerHandler("00:06:f7:c9:a1:fb", "00:13:8a:9c:31:42");
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

	// Set pointers to the left/right controller
	leftController = &controllers.leftController;
	rightController = &controllers.rightController;

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

const char* formatSteamVr() {
	float rxcL = optical::world_cm_l.x;
	float rycL = optical::world_cm_l.y;
	float rzcL = optical::world_cm_l.z;
	float qwL = leftController->orientation.q.w;
	float qxL = leftController->orientation.q.x;
	float qyL = leftController->orientation.q.z;
	float qzL = -leftController->orientation.q.y;
	float trigL = leftController->buttons.trigger;

	float rxcR = optical::world_cm_r.x;
	float rycR = optical::world_cm_r.y;
	float rzcR = optical::world_cm_r.z;
	float qwR = rightController->orientation.q.w;
	float qxR = rightController->orientation.q.x;
	float qyR = rightController->orientation.q.z;
	float qzR = -rightController->orientation.q.y;
	float trigR = rightController->buttons.trigger;

	std::string stringBuffer = std::vformat(DATA_BUFFER_VR, std::make_format_args(rxcL, rycL, rzcL, qwL, qxL, qyL, qzL, rxcR, rycR, rzcR, qwR, qxR, qyR, qzR, trigL, trigR));
	const char* charBuffer = stringBuffer.c_str();

	return charBuffer;
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

	// Start the optical task, making it tick itself
	optical::start();

	// Main loop
	for (;;) {
		// Tick the clock and controllers
		ps_clock.tick();
		moveAPI.update();

		// Format and send UDP signal to SteamVR
		const char* buffer = formatSteamVr();
		connection.send(buffer);

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
