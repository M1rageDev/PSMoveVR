#include "optical.h"

// PROGRAM MODULE FUNCTIONS
void psmovevr::optical::init(vr::VRControllerHandler* controllers_, vr::VRCamera* cameras_) {
	controllers = controllers_;
	cameras = cameras_;
}

void psmovevr::optical::calibrate() {

}

void psmovevr::optical::loop() {
	// Read frame
	cameras[0].read();

	// Process each controller
	for (int i = 0; i < controllers->controllers.size(); i++) {
		vr::VRController vrc = controllers->controllers[i];

		auto [ret, ball] = vr::detectCircle(cameras[0].cvImg, { 95, 0, 92 }, { 169, 169, 255 });  // TODO: color calibration
		cv::circle(cameras[0].cvImg, { (int)ball.x, (int)ball.y }, (int)ball.z, { 255, 255, 255 }, 1);
	}
}

// OTHER FUNCTIONS
