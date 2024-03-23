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
	for (auto vrc : controllers->controllers) {
		//std::cout << vrc.serial << std::endl;
		std::cout << vrc.connected << std::endl;
		//if (!vrc.connected) continue;

		auto [ret, ball] = vr::detectCircle(cameras[0].cvImg, { 95, 0, 92 }, { 169, 169, 255 });  // TODO: color calibration
		cv::circle(cameras[0].cvImg, { (int)ball.x, (int)ball.y }, (int)ball.z, { 255, 255, 255 }, 1);
	}
}

// OTHER FUNCTIONS
