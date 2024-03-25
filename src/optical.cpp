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
		for (int i = 0; i < 2; i++) {
			vr::VRController vrc = *controllers->getController(i);

			// Skip allocated and unconnected controllers to save performance
			if (!controllers->isConnected(i)) continue;

			// Detect ball
			auto [ret, ball] = vr::detectCircle(cameras[0], frame, buffer, mask, vrc.optical.lowerColor, vrc.optical.higherColor);
			if (!ret) continue;

			// Track in 3D space and transform the coordinate space
			// TODO: implement camera transform calibration
			camera_cm = vr::estimatePosition(ball, cameras[0].f_px, frame.cols, frame.rows);
			world_cm = glm::vec4(camera_cm, 1.f) * cameras[0].transformMatrix;

			// Frame axes
			glm::vec4 rot = optical::axisAngle(glm::quat(vrc.orientation.q.x, vrc.orientation.q.z, vrc.orientation.q.y, vrc.orientation.q.w));
			cv::Mat rvec = cv::Mat({rot.z * rot.w, rot.y * rot.w, -rot.x * rot.w});

			// Annotate
			cv::rectangle(frame, { (int)ball.x - (int)ball.z, (int)ball.y - (int)ball.z }, { (int)ball.x + (int)ball.z, (int)ball.y + (int)ball.z }, { 255, 255, 255 });
			cv::drawMarker(frame, { (int)ball.x, (int)ball.y }, { 255, 255, 255 });
			cv::drawFrameAxes(frame, cameras[0].matrix, cameras[0].distortion, rvec, cv::Mat({ camera_cm.x, -camera_cm.y, camera_cm.z }), vr::REAL_BALL_RADIUS);
		}

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
