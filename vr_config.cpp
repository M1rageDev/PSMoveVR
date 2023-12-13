#include "vr_config.h"

namespace vr {
	namespace _vr_config_private {
		const char* FILENAME;
		cv::FileStorage FS;
	}
}


bool vr::fileExists(const char* name) {
	std::ifstream f(name);
	bool good = f.good();
	f.close();
	return good;
}

void vr::beginSave(const char* path) {
	vr::_vr_config_private::FILENAME = path;
	vr::_vr_config_private::FS = cv::FileStorage(path, cv::FileStorage::WRITE);
}

void vr::addNode(const char* name, cv::Mat mat) {
	vr::_vr_config_private::FS << name << mat;
}

void vr::addNode(const char* name, cv::Scalar mat) {
	vr::_vr_config_private::FS << name << mat;
}

void vr::addNode(const char* name, glm::mat4 mat) {
	cv::Mat cvMat(4, 4, CV_32F);
	memcpy(cvMat.data, glm::value_ptr(mat), 16 * sizeof(float));
	vr::_vr_config_private::FS << name << cvMat;
}

void vr::endSave() {
	vr::_vr_config_private::FILENAME = "";
	vr::_vr_config_private::FS.release();
}

void vr::beginRead(const char* path) {
	vr::_vr_config_private::FILENAME = path;
	vr::_vr_config_private::FS = cv::FileStorage(path, cv::FileStorage::READ);
}

cv::Mat vr::readNode(const char* name) {
	cv::Mat node;
	vr::_vr_config_private::FS[name] >> node;
	return node;
}

cv::Scalar vr::readNodeScalar(const char* name) {
	cv::Scalar node;
	vr::_vr_config_private::FS[name] >> node;
	return node;
}

glm::mat4 vr::readNodeMat4(const char* name) {
	glm::mat4 node;
	cv::Mat cvNode(4, 4, CV_32F);
	vr::_vr_config_private::FS[name] >> cvNode;
	memcpy(glm::value_ptr(node), cvNode.data, 16 * sizeof(float));

	return node;
}

void vr::endRead() {
	vr::_vr_config_private::FILENAME = "";
	vr::_vr_config_private::FS.release();
}
