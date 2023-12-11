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
