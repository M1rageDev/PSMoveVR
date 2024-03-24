#include "vr_cameras.h"

vr::VRCamera::VRCamera() {

}

vr::VRCamera::VRCamera(uint8_t index, uint16_t fps, uint32_t width, uint32_t height, uint8_t exposure, uint8_t gain, bool flip) {
	std::vector<PS3EYECam::PS3EYERef> devices = PS3EYECam::getDevices();
	eye = devices.at(index);
	eye->init(width, height, 60);
	eye->setExposure(exposure);
	eye->setGain(gain);
	eye->setFlip(flip);
	eye->start();

	this->index = index;
	videoPixels = new unsigned char[width * height * 3];
	eye->getFrame(videoPixels);
	cvImg = cv::Mat(cv::Size(width, height), CV_8UC3, videoPixels);
}

vr::VRCamera::VRCamera(uint8_t index, PS3EYECam::PS3EYERef ref, uint16_t fps, uint32_t width, uint32_t height, uint8_t exposure, uint8_t gain, bool flip) {
	eye = ref;
	eye->init(width, height, 60);
	eye->setExposure(exposure);
	eye->setGain(gain);
	eye->setFlip(flip);
	eye->start();

	this->index = index;
	videoPixels = new unsigned char[width * height * 3];
	eye->getFrame(videoPixels);
	cvImg = cv::Mat(cv::Size(width, height), CV_8UC3, videoPixels);
	tryReadCalibration();
}

bool vr::VRCamera::tryReadCalibration() {
	// Create path
	std::string path = camerasPath;
	path += "/";
	path += "camera";
	path += std::to_string(index);
	path += ".cfg";

	// Check if file exists, and create empty arrays
	if (!(vr::fileExists(path.c_str()))) {
		printf_s("WARNING: Camera %i failed to read calibration.\n", index);
		matrix = cv::Mat();
		distortion = cv::Mat();
		return false;
	}

	vr::beginRead(path.c_str());
	matrix = vr::readNode("matrix");
	distortion = vr::readNode("distortion");
	vr::endRead();
	f_px = matrix.at<double>(0);

	return true;
}

void vr::VRCamera::saveCalibration() {
	// Create path
	std::string path = camerasPath;
	path += "/";
	path += "camera";
	path += std::to_string(index);
	path += ".cfg";

	// Check if the camera is actually calibrated
	if (matrix.empty()) {
		printf_s("WARNING: Camera %i not calibrated.\n", index);
		return;
	}

	vr::beginSave(path.c_str());
	vr::addNode("matrix", matrix);
	vr::addNode("distortion", distortion);
	vr::endSave();
}

void vr::VRCamera::read() {
	eye->getFrame(videoPixels);
	cvImg.data = videoPixels;
}

void vr::VRCamera::stop() {
	eye->stop();
	saveCalibration();
}

uint8_t vr::getCameraCount() {
	std::vector<PS3EYECam::PS3EYERef> devices = PS3EYECam::getDevices();
	return devices.size();
}

bool vr::checkCamerasConnected() {
	std::vector<PS3EYECam::PS3EYERef> devices = PS3EYECam::getDevices();
	return devices.size() != 0;
}

vr::VRCamera* vr::initializeEveryCam(uint16_t fps, uint32_t width, uint32_t height, uint8_t exposure, uint8_t gain, bool flip) {
	// Iterate over every device and initialize
	std::vector<PS3EYECam::PS3EYERef> devices = PS3EYECam::getDevices();

	vr::VRCamera* cams = new vr::VRCamera[devices.size()];
	for (int i = 0; i < devices.size(); i++) {
		cams[i] = vr::VRCamera(i, devices.at(i), fps, width, height, exposure, gain, flip);
	}

	return cams;
}

void vr::stopEveryCam(vr::VRCamera* devices) {
	// Iterate over every device and stop
	std::vector<PS3EYECam::PS3EYERef> devicesCount = PS3EYECam::getDevices();
	for (int i = 0; i < devicesCount.size(); i++) {
		devices[i].stop();
	}
}
