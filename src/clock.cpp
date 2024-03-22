#include "clock.h"

psmovevr::Clock::Clock() {

}

void psmovevr::Clock::tick() {
	// Check timestep
	float curTime = clock();
	timestep = (curTime - lastTime) / 1000.f;
	lastTime = curTime;
}
