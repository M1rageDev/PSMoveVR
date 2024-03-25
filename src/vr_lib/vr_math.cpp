#include "vr_math.h"

float vr::clamp(float x, float l, float h) {
	return std::min(std::max(x, l), h);
}

float vr::clamp01(float x) {
	return clamp(x, 0.f, 1.f);
}

float vr::lerp(float a, float b, float t) {
	return a * (1.f - t) + b * t;
}

glm::vec3 vr::lerp3(glm::vec3 a, glm::vec3 b, float t) {
	return a * (1.f - t) + (b * t);
}

glm::vec4 vr::lowpass(glm::vec4 l, glm::vec4 c, float xy_delta, float z_delta) {
	// Calculate weights
	float mag = glm::length(l - c);
	float XYweight = clamp01(lerp(0.4f, 0.75f, mag / 5.f + xy_delta));
	float Zweight = clamp01(lerp(0.05f, 0.1f, mag / 5.f + z_delta));

	// Interpolate
	return glm::vec4(lerp(l.x, c.x, XYweight), lerp(l.y, c.y, XYweight), lerp(l.z, c.z, Zweight), c.w);
}
