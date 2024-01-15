#include "vr_calibration.h"

glm::vec3 vr::calibrateGyroscope(unsigned int sampleCount, vr::VRController* controller, psmoveapi::PSMoveAPI* api) {
	std::vector<glm::vec3> imuArray;
	glm::vec3 gyroOffsets{ 0.0, 0.0, 0.0 };
	while (imuArray.size() < sampleCount + 1) {
		api->update();
		auto gyro_data = controller->gyro;
		imuArray.push_back(gyro_data);
	}
	int imuArraySize = imuArray.size();

	glm::vec3 sum{ 0.0, 0.0, 0.0 };
	for (int i = 0; i < imuArraySize; i++) {
		sum += imuArray[i];
	}
	gyroOffsets = sum / (float)imuArraySize;
	return gyroOffsets;
}

void vr::calibrateControllers(unsigned int sampleCount, vr::VRControllerHandler* controllers, psmoveapi::PSMoveAPI* api) {
	controllers->right.gyroOffsets = vr::calibrateGyroscope(sampleCount, &controllers->right, api);
	controllers->left.gyroOffsets = vr::calibrateGyroscope(sampleCount, &controllers->left, api);
}

std::tuple<bool, glm::mat4, cv::Mat, cv::Mat> vr::calibrateWorldMatrix(std::vector<cv::Point2f> samples, cv::Mat cameraMatrix, cv::Mat cameraDistortion) {
	/*
	CALIBRATION_MAT_OBJECT = {
		cv::Point3f(-10.5f, 0.f, -14.85f),
		cv::Point3f(10.5f, 0.f, 14.85f),
		cv::Point3f(-10.5f, 0.f, 14.85f),
		cv::Point3f(10.5f, 0.f, -14.85f)
	};*/
	std::vector<cv::Point3f> CALIBRATION_MAT_OBJECT = {
		cv::Point3f(-23.75f, 0.f, -26.7f),
		cv::Point3f(23.75f, 0.f, 26.7f),
		cv::Point3f(-23.75f, 0.f, 26.7f),
		cv::Point3f(23.75f, 0.f, -26.7f)
	};

	cv::Mat rvecs, tvecs;
	bool ret = cv::solvePnP(CALIBRATION_MAT_OBJECT, samples, cameraMatrix, cameraDistortion, rvecs, tvecs);

	if (ret) {
		std::cout << "Computing inverse camera pose..." << std::endl;
		cv::Mat R;
		cv::Rodrigues(rvecs, R);

		float rotMat[9];
		for (int i = 0; i < 9; i++)
		{
			rotMat[i] = static_cast<float>(R.at<double>(i));
		}

		cv::Mat R_inv = R.t();
		cv::Mat tvecInv = -R_inv * tvecs; // translation of the inverse R|t transform
		float tv[3];
		for (int i = 0; i < 3; i++)
		{
			tv[i] = static_cast<float>(tvecInv.at<double>(i));
		}

		float RTMat[] = {
			rotMat[0], rotMat[1], rotMat[2], 0.0f,
			rotMat[3], rotMat[4], rotMat[5], 0.0f,
			rotMat[6], rotMat[7], rotMat[8], 0.0f,
			tv[0], tv[1], tv[2], 1.0f };

		glm::mat4 mtx = glm::make_mat4(RTMat);
		return { true, mtx, rvecs, tvecs };
	}
	else {
		return { false, glm::mat4(), cv::Mat(), cv::Mat() };
	}
}
