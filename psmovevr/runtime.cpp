#include "runtime.h"

void psmovevr::readCameras() {
	for (auto cam : psmovevr::cameras) cam.eye.read();
}

cv::Mat psmovevr::getPicture(uint8_t index) {
	return psmovevr::cameras[index].eye.cvImg;
}

void psmovevr::tickControllers() {
	psmovevr::moveApi->update();
}

void psmovevr::setControllerColor(uint8_t index, RGB color) {
	psmovevr::controllerHandler.controllers[index].color = color;
}

void psmovevr::calibrateImu(unsigned int samples) {
	vr::calibrateControllers(samples, &psmovevr::controllerHandler, psmovevr::moveApi);
}
