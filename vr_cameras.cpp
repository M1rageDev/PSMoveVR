#include "vr_cameras.h"

vr::VRCamera::VRCamera() {

}

vr::VRCamera::VRCamera(uint16_t fps, uint32_t width, uint32_t height, uint8_t exposure, uint8_t gain, bool flip) {
	std::vector<PS3EYECam::PS3EYERef> devices = PS3EYECam::getDevices();
	eye = devices.at(0);
	eye->init(width, height, 60);
	eye->setExposure(exposure);
	eye->setGain(gain);
	eye->setFlip(flip);
	eye->start();

	videoPixels = new unsigned char[width * height * 3];
	eye->getFrame(videoPixels);
	cvImg = cv::Mat(cv::Size(width, height), CV_8UC3, videoPixels);
}

vr::VRCamera::VRCamera(PS3EYECam::PS3EYERef ref, uint16_t fps, uint32_t width, uint32_t height, uint8_t exposure, uint8_t gain, bool flip) {
	eye = ref;
	eye->init(width, height, 60);
	eye->setExposure(exposure);
	eye->setGain(gain);
	eye->setFlip(flip);
	eye->start();

	videoPixels = new unsigned char[width * height * 3];
	eye->getFrame(videoPixels);
	cvImg = cv::Mat(cv::Size(width, height), CV_8UC3, videoPixels);
}

void vr::VRCamera::read() {
	eye->getFrame(videoPixels);
	cvImg.data = videoPixels;
}

void vr::VRCamera::stop() {
	eye->stop();
}

vr::VRCamera* vr::initializeEveryCam(uint16_t fps, uint32_t width, uint32_t height, uint8_t exposure, uint8_t gain, bool flip) {
	std::vector<PS3EYECam::PS3EYERef> devices = PS3EYECam::getDevices();
	vr::VRCamera* cams = new vr::VRCamera[devices.size()];
	for (int i = 0; i < devices.size(); i++) {
		cams[i] = vr::VRCamera(devices.at(i), fps, width, height, exposure, gain, flip);
	}

	return cams;
}

void vr::stopEveryCam(vr::VRCamera* devices) {
	std::vector<PS3EYECam::PS3EYERef> devicesCount = PS3EYECam::getDevices();
	for (int i = 0; i < devicesCount.size(); i++) {
		devices[i].stop();
	}
}
