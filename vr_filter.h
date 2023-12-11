#pragma once
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

namespace vr {
	class VRMadgwick {
	public:
		glm::quat q;
		float b;
		VRMadgwick();
		VRMadgwick(glm::quat initialPose, float beta);

		void update(glm::vec3 g, glm::vec3 a, float timestep);
	};
}
