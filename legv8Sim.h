#ifndef LEGV8SIM_LEGV8SIM_H
#define LEGV8SIM_LEGV8SIM_H

#include "legv8Line.h"
#include <vector>
#include <string>
#include <fstream>

//Written By Kyle de Bord, Aidan Shirey, Jasmine De La Torre, Kiyoko Mangrobang

class legv8Sim {
	public:
		legv8Sim () {
			for(int i = 0; i < 32; i++)
			{
				RFILE.push_back(0);
				MEM.push_back(0);
			}
		};
		void initilzieMemory();
		legv8Line parseLine(int lineToGrab);
		void parseFileToVector(std::string inputFile);
		void parseLineToPGMLine();
		std::string promtForUserInput();
		void executeUserInput(std::string userInput);
		void runLine();
		void setRFILE(int index, long long value);
		long long getRFILE(int index);

	private:
	struct Label {
            std::string label;
            int line_num;
	};
		std::vector<legv8Line> PGMLines;
		std::vector<std::string> PGM;
		std::vector<long long> RFILE;
		std::vector<Label> LABELS;
		std::vector<int> MEM;
		bool stepByStep;
public:
	bool isStepByStep() const;
	void setStepByStep(bool stepByStep);
};

#endif
