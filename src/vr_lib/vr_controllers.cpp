#include "vr_controllers.h"

void vr::VRController::connect(Controller* controller) {
	connected = true;
}

void vr::VRController::update(Controller* controller) {
	float curTime = clock();
	timestep = (curTime - lastTime) / 1000.f;
	lastTime = curTime;

	accel = glm::vec3(controller->accelerometer.x, controller->accelerometer.y, controller->accelerometer.z);
	gyro = glm::vec3(controller->gyroscope.x, controller->gyroscope.y, controller->gyroscope.z);
	orientation.update(gyro - gyroOffsets, accel, timestep);

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

vr::VRControllerHandler::VRControllerHandler(std::string leftSerial, std::string rightSerial) {
	leftController = vr::VRController();
	rightController = vr::VRController();

	leftController.serial = leftSerial;
	rightController.serial = rightSerial;
}

void vr::VRControllerHandler::connect(Controller* controller) {
	// Info
	printf("Controller connected: %s\n", controller->serial);

	// Compare the serial number
	if (strcmp(controller->serial, leftController.serial.c_str())) {
		leftController.connect(controller);
		return;
	}
	else {
		rightController.connect(controller);
	}
}

void vr::VRControllerHandler::update(Controller* controller) {
	// Compare the serial number
	if (strcmp(controller->serial, leftController.serial.c_str())) {
		leftController.update(controller);
	}
	else {
		rightController.update(controller);
	}
}

void vr::VRControllerHandler::disconnect(Controller* controller) {
	// Remove
	if (strcmp(controller->serial, leftController.serial.c_str())) {
		// Left
		leftController.connected = false;
	}
	else {
		// Right
		rightController.connected = false;
	}

	// Info
	printf("Controller disconnected: %s\n", controller->serial);
}

vr::VRController* vr::VRControllerHandler::getController(uint8_t index) {
	if (index == 0) return &leftController;
	else return &rightController;
}

bool vr::VRControllerHandler::isConnected(uint8_t index) {
	if (index == 0) return leftController.connected;
	else return rightController.connected;
}

glm::vec3 vr::VRControllerHandler::getGyro(uint8_t index) {
	if (index == 0) return leftController.gyro;
	else return rightController.gyro;
}

glm::vec3 vr::VRControllerHandler::getAccel(uint8_t index) {
	if (index == 0) return leftController.accel;
	else return rightController.accel;
}
