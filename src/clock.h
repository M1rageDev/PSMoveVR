#pragma once
#include <iostream>
#include <vector>

namespace psmovevr
{
	struct ClockEvent {
		float timestamp = 0.f;
		void (*func)(float, float);
	};

	class Clock {
	public:
		Clock();
		void tick();
		void createEvent(float delay, void (*func)(float, float));

		std::vector<psmovevr::ClockEvent> events;
		float curTime = 0.f;
		float lastTime = 0.f;
		float timestep = 0.f;
	};
}
