#include "optical.h"

// PROGRAM MODULE FUNCTIONS
void psmovevr::optical::init(vr::VRControllerHandler* controllers_, vr::VRCamera* cameras_) {
	controllers = controllers_;
	cameras = cameras_;
}

void psmovevr::optical::calibrate() {

}

void psmovevr::optical::start() {
	running = true;
	task = std::thread(psmovevr::optical::loop);
}

void psmovevr::optical::loop() {
	while (running) {
		// Read frame
		cameras[0].read();
		frame = cameras[0].cvImg;

		// Process each controller
		for (int i = 0; i < controllers->controllers.size(); i++) {
			vr::VRController vrc = controllers->controllers[i];

			// Skip allocated and unconnected controllers to save performance
			if (!controllers->connectedControllers[i]) continue;

			// Detect ball
			auto [ret, ball] = vr::detectCircle(cameras[0], frame, vrc.optical.lowerColor, vrc.optical.higherColor);
			if (!ret) continue;

			// Track in 3D space
			camera_cm = vr::estimatePosition(ball, cameras[0].f_px, frame.cols, frame.rows);

			// Transform coordinate space
			// TODO: implement camera transform calibration
			world_cm = glm::vec4(camera_cm, 1.f) * cameras[0].transformMatrix;

			// Annotate
			cv::circle(frame, { (int)ball.x, (int)ball.y }, (int)ball.z, { 255, 255, 255 }, 1);
			cv::putText(frame, glm::to_string(camera_cm), { (int)ball.x, (int)ball.y }, cv::FONT_HERSHEY_PLAIN, 1, { 255.f, 255.f, 255.f });
		}

		// Debug info
		cv::putText(frame, debugInfo, { 0, 16 }, cv::FONT_HERSHEY_PLAIN, 1, { 255.f, 255.f, 255.f });

		// CV window
		cv::imshow("Camera", frame);
		int key = cv::waitKey(1);
		if (key == 27) running = false;
	}
}

void psmovevr::optical::stop() {
	cv::destroyWindow("Camera");
	running = false;
	task.join();
}

// OTHER FUNCTIONS
