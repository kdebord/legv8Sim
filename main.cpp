#include <iostream>
#include "legv8Sim.h"

int main (int argc, char *argv[]) {
	if ( argc != 2) {
		//command line target is not entered
		std::cout << "usage: " << argv[0] << " nameOfAnInputFile" << std::endl;
		exit(1);
	}
	legv8Sim Simulator;
	Simulator.parseFileToVector(argv[1]);
	Simulator.parseLineToPGMLine();
	Simulator.runLine();
	// checking the RFILE to see if the register is updated correctly
	//should return 16 if using the given test file
	std::cout << Simulator.getRFILE(10);
	std::cout << "\ntest ended\n";
	return 0;
}
