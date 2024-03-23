#include "clock.h"

psmovevr::Clock::Clock() {

}

void psmovevr::Clock::tick() {
	// Check timestep
	curTime = clock();
	timestep = (curTime - lastTime) / 1000.f;
	lastTime = curTime;

	// Run events
	int i = 0;
	for (auto e : events) {
		// Run function
		if (curTime >= e.timestamp) {
			e.func(e.timestamp, curTime);

			// Remove event
			events.erase(events.begin() + i);
		}
		
		i++;
	}
}

void psmovevr::Clock::createEvent(float delay, void (*func)(float, float)) {
	psmovevr::ClockEvent e;
	e.func = func;
	e.timestamp = curTime + delay;

	events.push_back(e);
}
