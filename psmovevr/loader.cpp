#include "loader.h"

void psmovevr::loadCameras() {
	vr::VRCamera* cameras = vr::initializeEveryCam(60, 640, 480, 20, 0, true);

	for (int i = 0; i < vr::getCameraCount(); i++) {
		// Initialize camera structure
		psmovevr::camera_structure cam;
		cam.eye = cameras[i];
		cam.distortion = cam.eye.distortion;
		cam.matrix = cam.eye.matrix;
		cam.id = i;
		cam.name = std::to_string(cam.id);

		// Add camera to list
		psmovevr::cameras.push_back(cam);
	}
}

void psmovevr::loadControllers() {
	psmovevr::controllerHandler = vr::VRControllerHandler();
	psmovevr::moveApi = new psmoveapi::PSMoveAPI(&psmovevr::controllerHandler);
}

void psmovevr::stopCameras() {
	for (auto cam : psmovevr::cameras) {
		cam.eye.stop();
	}

	psmovevr::cameras.clear();
}
