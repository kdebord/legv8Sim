#ifndef LEGV8SIM_LEGV8SIM_H
#define LEGV8SIM_LEGV8SIM_H

#include "legv8Line.h"
#include <vector>
#include <string>
#include <fstream>

class legv8Sim {
	public:
		legv8Sim () {
			for(int i = 0; i < 32; i++)
			{
				RFILE.push_back(0);
			}
		};
		legv8Line parseLine(int lineToGrab);
		void parseFileToVector(std::string inputFile);
		void parseLineToPGMLine();
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
};

#endif
