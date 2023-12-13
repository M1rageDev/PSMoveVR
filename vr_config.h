#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <opencv2/opencv.hpp>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtx/string_cast.hpp"

namespace vr {
	bool fileExists(const char* name);

	// saving (CV)
	void beginSave(const char* path);

	void addNode(const char* name, cv::Mat mat);

	void addNode(const char* name, cv::Scalar mat);

	void addNode(const char* name, glm::mat4 mat);

	void endSave();

	// reading (CV)
	void beginRead(const char* path);

	cv::Mat readNode(const char* name);

	cv::Scalar readNodeScalar(const char* name);

	glm::mat4 readNodeMat4(const char* name);

	void endRead();
}
