#pragma once
#include "ps3eye.h"
#include "opencv2/opencv.hpp"

using namespace ps3eye;

namespace vr {
	class VRCamera {
	public:
		cv::Mat cvImg;
		PS3EYECam::PS3EYERef eye;
		unsigned char* videoPixels;

		VRCamera();
		VRCamera(uint16_t fps, uint32_t width, uint32_t height, uint8_t exposure, uint8_t gain, bool flip);
		VRCamera(PS3EYECam::PS3EYERef ref, uint16_t fps, uint32_t width, uint32_t height, uint8_t exposure, uint8_t gain, bool flip);

		void read();

		void stop();
	};

	VRCamera* initializeEveryCam(uint16_t fps, uint32_t width, uint32_t height, uint8_t exposure, uint8_t gain, bool flip);
	void stopEveryCam(vr::VRCamera* devices);
}
