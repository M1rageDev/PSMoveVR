// Standard
#include "connection.h"
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
#include "optical.h"

using namespace psmovevr;

// Constants
const std::string DATA_BUFFER = "{}|{}|{}|{}|{}|{}|{}|{}|{}|{}|{}|{}|{}|{}|0|0|0|0|0|0|0|0|{}|{}|0|0|0|0|0|0|0|0";

// Instances
Clock ps_clock;
Connection connection;
vr::VRCamera* cameras;
vr::VRControllerHandler controllers = vr::VRControllerHandler("00:06:f7:c9:a1:fb", "00:13:8a:9c:31:42");
psmoveapi::PSMoveAPI moveAPI(&controllers);

vr::VRController* leftController;
vr::VRController* rightController;

// Corrections/offsets
glm::vec4 headOffset = glm::vec4();

void init() {
	ps_clock = Clock();
	cameras = vr::initializeEveryCam(60, 640, 480, 20, 0, true);
	connection = Connection();
	connection.start(49152);

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

std::string formatSteamVr() {
	// Apply corrections/offsets
	glm::vec4 corrected_l = (optical::world_cm_l - headOffset) / 100.f;
	glm::vec4 corrected_r = (optical::world_cm_r - headOffset) / 100.f;
	glm::quat correctedRot_l = leftController->orientation.q;
	glm::quat correctedRot_r = rightController->orientation.q;

	// Stuff to make formatting easier to write/read
	float rxcL = corrected_l.x;
	float rycL = corrected_l.y;
	float rzcL = corrected_l.z;
	float qwL = correctedRot_l.w;
	float qxL = correctedRot_l.x;
	float qyL = correctedRot_l.z;
	float qzL = -correctedRot_l.y;
	float trigL = leftController->buttons.trigger;

	float rxcR = corrected_r.x;
	float rycR = corrected_r.y;
	float rzcR = corrected_r.z;
	float qwR = correctedRot_r.w;
	float qxR = correctedRot_r.x;
	float qyR = correctedRot_r.z;
	float qzR = -correctedRot_r.y;
	float trigR = rightController->buttons.trigger;

	// Format base string
	std::string stringBuffer = std::vformat(DATA_BUFFER, std::make_format_args(rxcL, rycL, rzcL, qwL, qxL, qyL, qzL, rxcR, rycR, rzcR, qwR, qxR, qyR, qzR, trigL, trigR));

	return stringBuffer;
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

	// Start the optical task, making it update itself
	optical::start();

	// Main loop
	for (;;) {
		// Tick the clock and controllers
		ps_clock.tick();
		moveAPI.update();

		// Read controller input and do actions like offsets
		if (rightController->buttons.triangle) headOffset = optical::world_cm_r;
		if (leftController->buttons.square) leftController->orientation.q = glm::quat(vr::Q90);
		if (rightController->buttons.square) rightController->orientation.q = glm::quat(vr::Q90);

		// Format and send UDP signal to SteamVR
		std::string buffer = formatSteamVr();
		const char* charBuffer = buffer.c_str();
		connection.send(charBuffer);

		Sleep(10);

		// Exit
		if (optical::running == false) break;
	}

	connection.send("D");

	// Stop the service
	//connection.stop();
	optical::stop();
	cv::destroyAllWindows();
	vr::stopEveryCam(cameras);
	return 0;
}
