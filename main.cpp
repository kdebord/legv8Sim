#include <iostream>
#include "legv8Sim.h"

int main (int argc, char *argv[]) {
	if ( argc != 2) {
		//command line target is not entered
		std::cout << "usage: " << argv[0] << " nameOfAnInputFile" << std::endl;
		exit(1);
	}
	legv8Sim Simulator;
	Simulator.initilzieMemory();
	Simulator.parseFileToVector(argv[1]);
	Simulator.parseLineToPGMLine();
	std::cout << "Step By Step (1) or Run All (0):  ";
	int i;
	std::cin >> i;
	if(i == 1) {
		Simulator.setStepByStep(true);
		Simulator.runLine();
	}
	else {
		Simulator.setStepByStep(false);
		Simulator.runLine();
		std::string input = Simulator.promtForUserInput();
		Simulator.executeUserInput(input);
	}
	// checking the RFILE to see if the register is updated correctly
	//should return 16 if using the given test file
	return 0;
}
