#include "vr_controllers.h"

void vr::VRController::update(Controller* controller, float mBeta) {
	float curTime = clock();
	timestep = (curTime - lastTime) / 1000.f;
	lastTime = curTime;

	accel = glm::vec3(controller->accelerometer.x, controller->accelerometer.y, controller->accelerometer.z);
	gyro = glm::vec3(controller->gyroscope.x, controller->gyroscope.y, controller->gyroscope.z);

	controller->color = color;
	controller->rumble = rumble;

	buttons.square = (controller->buttons & Btn_SQUARE) != 0;
	buttons.triangle = (controller->buttons & Btn_TRIANGLE) != 0;
	buttons.cross = (controller->buttons & Btn_CROSS) != 0;
	buttons.circle = (controller->buttons & Btn_CIRCLE) != 0;
	buttons.move = (controller->buttons & Btn_MOVE) != 0;
	buttons.select = (controller->buttons & Btn_SELECT) != 0;
	buttons.start = (controller->buttons & Btn_START) != 0;

	buttons.trigger = controller->trigger;
}

vr::VRControllerHandler::VRControllerHandler(const char* leftSerialNumber, const char* rightSerialNumber) {
	left = VRController();
	leftSerial = leftSerialNumber;

	right = VRController();
	rightSerial = rightSerialNumber;
}

vr::VRControllerHandler::VRControllerHandler(glm::quat initialRot, const char* leftSerialNumber, const char* rightSerialNumber, float madgwickBeta_ = 0.035f) {
	madgwickBeta = madgwickBeta_;

	left = VRController();
	leftSerial = leftSerialNumber;

	right = VRController();
	rightSerial = rightSerialNumber;
}

void vr::VRControllerHandler::connect(Controller* controller) {
	const char* handeness;
	if (strcmp(controller->serial, rightSerial)) {
		rightConnected = true;
		handeness = "Right";
	}
	else {
		leftConnected = true;
		handeness = "Left";
	}
	printf("%s controller connected: %s\n", handeness, controller->serial);
}

void vr::VRControllerHandler::update(Controller* controller) {
	if (strcmp(controller->serial, leftSerial)) {
		left.update(controller, madgwickBeta);
	}
	else {
		right.update(controller, madgwickBeta);
	}
}

void vr::VRControllerHandler::disconnect(Controller* controller) {
	const char* handeness;
	if (strcmp(controller->serial, rightSerial)) {
		rightConnected = false;
		handeness = "Right";
	}
	else {
		leftConnected = false;
		handeness = "Left";
	}
	printf("%s controller disconnected: %s\n", handeness, controller->serial);
}

glm::vec3 vr::VRControllerHandler::getGyro(bool leftFunction) {
	if (leftFunction) {
		return left.gyro;
	}
	else {
		return right.gyro;
	}
}

glm::vec3 vr::VRControllerHandler::getAccel(bool leftFunction) {
	if (leftFunction) {
		return left.accel;
	}
	else {
		return right.accel;
	}
}
