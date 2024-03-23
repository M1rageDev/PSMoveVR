#include "vr_controllers.h"

void vr::VRController::connect(Controller* controller) {

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

vr::VRControllerHandler::VRControllerHandler() {

}

void vr::VRControllerHandler::connect(Controller* controller) {
	// Info
	printf("Controller connected: %s\n", controller->serial);

	// See if the controller is already allocated
	for (VRController vrc : controllers) {
		// Compare the serial number
		if (strcmp(controller->serial, vrc.serial.c_str())) {
			vrc.connect(controller);
			return;
		}
	}

	// We haven't found any allocated controller, so do it now
	VRController vrc = VRController();
	vrc.serial = controller->serial;
	vrc.connect(controller);

	// Add controller to list
	controllers.push_back(vrc);
}

void vr::VRControllerHandler::update(Controller* controller) {
	// Iterate over every controller
	for (VRController vrc : controllers) {
		// Compare the serial number
		if (strcmp(controller->serial, vrc.serial.c_str())) {
			// Update the correct controller and break the loop
			vrc.update(controller);
			break;
		}
	}
}

void vr::VRControllerHandler::disconnect(Controller* controller) {
	// Remove from list
	for (int i = 0; i < controllers.size(); i++) {
		if (strcmp(controller->serial, controllers[i].serial.c_str())) {
			controllers.erase(controllers.begin() + i);
			break;
		}
	}

	// Info
	printf("Controller disconnected: %s\n", controller->serial);
}

void vr::VRControllerHandler::allocateControllerSpace(const char* serial) {
	// Allocate the space in the controller vector for a controller, allowing data to be passed before the controller is loaded by PSMoveAPI
	VRController controller = VRController();
	controller.serial = serial;

	controllers.push_back(controller);
}

glm::vec3 vr::VRControllerHandler::getGyro(uint8_t index) {
	return controllers[index].gyro;
}

glm::vec3 vr::VRControllerHandler::getAccel(uint8_t index) {
	return controllers[index].gyro;
}
