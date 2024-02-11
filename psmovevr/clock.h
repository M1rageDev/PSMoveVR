#pragma once
#include <iostream>

namespace psmovevr
{
	class Clock {
	public:
		Clock();
		void tick();

		float lastTime = 0.f;
		float timestep = 0.f;
	};
}
