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

	// Create UDP connection
	vr::VRConnection connection = vr::VRConnection(49000);

	// Initialize
	psmovevr::loadCameras();
	psmovevr::loadControllers();

	// Calibrate controller IMUs
	psmovevr::calibrateImu(5000);

	// Set colors
	psmovevr::setControllerColor(0, { 0.f, 1.f, 1.f });
	psmovevr::setControllerColor(1, { 1.f, 0.f, 1.f });
	psmovevr::tickControllers();

	// Main loop
	for (;;) {
		// Tick
		clock.tick();

		// Tick cameras and controllers
		psmovevr::readCameras();
		psmovevr::tickControllers();

		// Debug info
		cv::putText(psmovevr::getPicture(0), std::to_string(1.f / clock.timestep), cv::Point(0.f, 16.f), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(255.f, 255.f, 255.f));

		// Send UDP signal to SteamVR
		//std::string stringBuffer = std::vformat(DATA_BUFFER, std::make_format_args());
		//const char* charBuffer = stringBuffer.c_str();
		connection.send("Mm bobbies");

		cv::imshow("Camera", psmovevr::getPicture(0));
		int key = cv::waitKey(1);
		if (key == 27) break;
	}

	// Stop the service
	connection.stop();
	cv::destroyAllWindows();
	psmovevr::stopCameras();
	return 0;
}
