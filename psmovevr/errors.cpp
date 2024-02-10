#include "errors.h"

void psmovevr::throwNoCameras() {
	printf("Error: No cameras connected.");
	exit(1);
}

void psmovevr::throwNoControllers() {
	printf("Error: No controllers connected.");
	exit(2);
}

void psmovevr::throwTimeout() {
	printf("Error: Driver timeout.");
	exit(3);
}
