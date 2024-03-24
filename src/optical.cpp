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
	frame = cameras[0].cvImg;

	// Process each controller
	for (int i = 0; i < controllers->controllers.size(); i++) {
		vr::VRController vrc = controllers->controllers[i];

		// Skip allocated and unconnected controllers to save performance
		if (!controllers->connectedControllers[i]) continue;

		// Detect ball
		auto [ret, ball] = vr::detectCircle(cameras[0], frame, vrc.optical.lowerColor, vrc.optical.higherColor);  // TODO: color calibration

		// Track in 3D space
		camera_cm = vr::estimatePosition(ball, cameras[0].f_px, frame.cols, frame.rows);

		// Transform coordinate space
		// TODO: implement camera transform calibration
		world_cm = glm::vec4(camera_cm, 1.f) * cameras[0].transformMatrix;
		
		// Annotate
		cv::circle(frame, { (int)ball.x, (int)ball.y }, (int)ball.z, { 255, 255, 255 }, 1);
		cv::putText(frame, glm::to_string(camera_cm), { (int)ball.x, (int)ball.y }, cv::FONT_HERSHEY_PLAIN, 1, { 255.f, 255.f, 255.f });
	}
}

// OTHER FUNCTIONS
