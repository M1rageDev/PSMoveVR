#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>

namespace vr
{
	float clamp(float x, float l, float h);

	float clamp01(float x);

	float lerp(float a, float b, float t);

	glm::vec3 lerp3(glm::vec3 a, glm::vec3 b, float t);

	glm::vec4 lowpass(glm::vec4 l, glm::vec4 c, float xy_delta, float z_delta);
}
