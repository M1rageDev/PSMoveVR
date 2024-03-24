#pragma once
#include "ps3eye.h"
#include "opencv2/opencv.hpp"
#include "vr_config.h"

using namespace ps3eye;

namespace vr {
	inline const char* camerasPath = "Cameras";

	class VRCamera {
	public:
		PS3EYECam::PS3EYERef eye;

		glm::mat4 transformMatrix;
		float f_px = 0.f;
		cv::Mat matrix, distortion;
		cv::Mat cvImg;
		unsigned char* videoPixels;

		VRCamera();
		VRCamera(uint8_t index, uint16_t fps, uint32_t width, uint32_t height, uint8_t exposure, uint8_t gain, bool flip);
		VRCamera(uint8_t index, PS3EYECam::PS3EYERef ref, uint16_t fps, uint32_t width, uint32_t height, uint8_t exposure, uint8_t gain, bool flip);

		void saveCalibration();

		void read();

		void stop();
	private:
		uint8_t index;
		bool tryReadCalibration();
	};

	uint8_t getCameraCount();
	bool checkCamerasConnected();
	VRCamera* initializeEveryCam(uint16_t fps, uint32_t width, uint32_t height, uint8_t exposure, uint8_t gain, bool flip);
	void stopEveryCam(vr::VRCamera* devices);
}
