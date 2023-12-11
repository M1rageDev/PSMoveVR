#pragma once
#include <vector>
#include <iostream>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "vr_controllers.h"

namespace vr {
	glm::vec3 calibrateGyroscope(unsigned int sampleCount, vr::VRController* controller, psmoveapi::PSMoveAPI* api);
}
