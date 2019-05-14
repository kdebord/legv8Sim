#include "legv8Sim.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>

void legv8Sim::parseFileToVector(std::string inputFile) {
    std::ifstream file;
    file.open(inputFile);
    std::string tempLine;
    while( ! file.eof())
    {
        std::getline(file,tempLine);
        PGM.push_back(tempLine);
    }

    return;
}

legv8Line legv8Sim::parseLine(int lineToGrab) {
    std::string pgmLine = PGM[lineToGrab];
    legv8Line lineToReturn;
    std::string temp = "";
    char iter = 'X';
    int i = 0;
    while(iter != ' ')
    {
        iter = pgmLine[i];
        temp.insert(temp.end(), iter);
        i++;
    }
    temp.resize(temp.size() - 1);
    lineToReturn.setInstrName(temp);

    temp = "";
    iter = 'X';
    i++;
    while (iter != ',') {
        iter = pgmLine[i];
        temp.insert(temp.end(), iter);
        i++;
    }
    temp.resize(temp.size() - 1);
    lineToReturn.setStoreReg(std::stoi(temp));

    //needs a case that if using a immediate will store as a int instead of a register
    temp = "";
    iter = 'X';
    i++;
    if(pgmLine[i] == '#')
        lineToReturn.setIsfirstImmediate(true);
    i++;
    while (iter != ',') {
        iter = pgmLine[i];
        temp.insert(temp.end(), iter);
        i++;
    }
    temp.resize(temp.size() - 1);
    lineToReturn.setFirstOperand(std::stoi(temp));

    temp = "";
    iter = 'X';
    i++;
    if(pgmLine[i] == '#')
        lineToReturn.setIsSecondImmediate(true);
    i++;
    while (isalnum(iter)) {
        iter = pgmLine[i];
        temp.insert(temp.end(), iter);
        i++;
    }
    temp.resize(temp.size() - 1);
    lineToReturn.setSecondOperand(std::stoi(temp));

    return lineToReturn;
}

void legv8Sim::parseLineToPGMLine(){
  int index = 0;
  while(index < PGM.size())
    {
      PGMLines.push_back(parseLine(index));
      index++;
    }
}

void legv8Sim::runLine() {
  int pgmline = 0;
  parseLineToPGMLine();
  while (pgmline < PGMLines.size()) {
      if (PGMLines[pgmline].getInstrName() == "ADD") {
          long long storeRegValue = getRFILE(PGMLines[pgmline].getStoreReg());
          long long firstOpValue = getRFILE(PGMLines[pgmline].getFirstOperand());
          long long secondOpValue = getRFILE(PGMLines[pgmline].getSecondOperand());
          // The following if statements check for correct instruction syntax

          if (!PGMLines[pgmline].isIsfirstImmediate() && !PGMLines[pgmline].isIsSecondImmediate())
              storeRegValue = std::abs(firstOpValue) + std::abs(secondOpValue);
          else {
              std::cout << "ADD Instruction Syntax Incorrect: Immediate Detected";
              exit(1);
          }
          setRFILE(PGMLines[pgmline].getStoreReg(), storeRegValue);
      }

      if (PGMLines[pgmline].getInstrName() == "ADDI") {
          long long storeRegValue = getRFILE(PGMLines[pgmline].getStoreReg());
          long long firstOpValue = getRFILE(PGMLines[pgmline].getFirstOperand());
          long long secondOpValue = PGMLines[pgmline].getSecondOperand();
          // The following if statements check for correct instruction syntax

          if (!PGMLines[pgmline].isIsfirstImmediate() && PGMLines[pgmline].isIsSecondImmediate())
              storeRegValue = std::abs(firstOpValue) + std::abs(secondOpValue);
          else {
              std::cout << "ADDI Instruction Syntax Incorrect: Immediate Expected In Second Operand";
              exit(1);
          }
          setRFILE(PGMLines[pgmline].getStoreReg(), storeRegValue);
      }

      if (PGMLines[pgmline].getInstrName() == "ADDS"){
	long long storeRegValue = getRFILE(PGMLines[pgmline].getStoreReg());
	long long firstOpValue = getRFILE(PGMLines[pgmline].getFirstOperand());
	long long secondOpValue = getRFILE(PGMLines[pgmline].getSecondOperand());

	if (!PGMLines[pgmline].isIsfirstImmediate() && PGMLines[pgmline].isIsSecondImmediate())
              storeRegValue = std::abs(firstOpValue) + std::abs(secondOpValue);
          else {
              std::cout << "ADDI Instruction Syntax Incorrect: Immediate Expected In Second Operand";
              exit(1);
          }
	// store value of Z here
	long long tempZ = std::abs(storeRegValue) + std::abs(firstOpValue);
	if ((storeRegValue < 0 && firstOpValue >= 0)
	    || (storeRegValue >= 0 && secondOpValue < 0 ))
	  PGMLines[pgmline].setV(false);
	else if (!((tempZ >= 0 && storeRegValue < 0) || (tempZ < 0 && storeRegValue >= 0)))
	
          setRFILE(PGMLines[pgmline].getStoreReg(), storeRegValue);
	
      }

      if (PGMLines[pgmline].getInstrName() == "SUB") {
          long long storeRegValue = getRFILE(PGMLines[pgmline].getStoreReg());
          long long firstOpValue = getRFILE(PGMLines[pgmline].getFirstOperand());
          long long secondOpValue = getRFILE(PGMLines[pgmline].getSecondOperand());
          // The following if statements check for correct instruction syntax

          if (!PGMLines[pgmline].isIsfirstImmediate() && !PGMLines[pgmline].isIsSecondImmediate())
              storeRegValue = std::abs(firstOpValue) - std::abs(secondOpValue);
          else {
              std::cout << "SUB Instruction Syntax Incorrect: Immediate Detected";
              exit(1);
          }
          setRFILE(PGMLines[pgmline].getStoreReg(), storeRegValue);
      }

      if (PGMLines[pgmline].getInstrName() == "SUBI") {
          long long storeRegValue = getRFILE(PGMLines[pgmline].getStoreReg());
          long long firstOpValue = getRFILE(PGMLines[pgmline].getFirstOperand());
          long long secondOpValue = PGMLines[pgmline].getSecondOperand();
          // The following if statements check for correct instruction syntax

          if (!PGMLines[pgmline].isIsfirstImmediate() && PGMLines[pgmline].isIsSecondImmediate())
              storeRegValue = std::abs(firstOpValue) - std::abs(secondOpValue);
          else {
              std::cout << "SUBI Instruction Syntax Incorrect: Immediate Expected In Second Operand";
              exit(1);
          }
          setRFILE(PGMLines[pgmline].getStoreReg(), storeRegValue);
      }

      if (PGMLines[pgmline].getInstrName() == "AND") {
          long long storeRegValue = getRFILE(PGMLines[pgmline].getStoreReg());
          long long firstOpValue = getRFILE(PGMLines[pgmline].getFirstOperand());
          long long secondOpValue = getRFILE(PGMLines[pgmline].getSecondOperand());

          if ( ! PGMLines[pgmline].isIsfirstImmediate() && ! PGMLines[pgmline].isIsSecondImmediate())
              storeRegValue = firstOpValue & secondOpValue;
          else {
              std::cout << "AND Instruction Syntax Incorrect: Immediate Found In Syntax";
              exit(1);
          }
          setRFILE(PGMLines[pgmline].getStoreReg(), storeRegValue);
      }

      if (PGMLines[pgmline].getInstrName() == "ORR") {
          long long storeRegValue = getRFILE(PGMLines[pgmline].getStoreReg());
          long long firstOpValue = getRFILE(PGMLines[pgmline].getFirstOperand());
          long long secondOpValue = getRFILE(PGMLines[pgmline].getSecondOperand());

          if ( ! PGMLines[pgmline].isIsfirstImmediate() && ! PGMLines[pgmline].isIsSecondImmediate())
              storeRegValue = firstOpValue | secondOpValue;
          else {
              std::cout << "ORR Instruction Syntax Incorrect: Immediate Found In Syntax";
              exit(1);
          }
          setRFILE(PGMLines[pgmline].getStoreReg(), storeRegValue);
      }

      if (PGMLines[pgmline].getInstrName() == "EOR") {
          long long storeRegValue = getRFILE(PGMLines[pgmline].getStoreReg());
          long long firstOpValue = getRFILE(PGMLines[pgmline].getFirstOperand());
          long long secondOpValue = getRFILE(PGMLines[pgmline].getSecondOperand());

          if ( ! PGMLines[pgmline].isIsfirstImmediate() && ! PGMLines[pgmline].isIsSecondImmediate())
              storeRegValue = firstOpValue ^ secondOpValue;
          else {
              std::cout << "EOR Instruction Syntax Incorrect: Immediate Found In Syntax";
              exit(1);
          }
          setRFILE(PGMLines[pgmline].getStoreReg(), storeRegValue);
      }

      if (PGMLines[pgmline].getInstrName() == "ANDI") {
          long long storeRegValue = getRFILE(PGMLines[pgmline].getStoreReg());
          long long firstOpValue = getRFILE(PGMLines[pgmline].getFirstOperand());
          long long secondOpValue = PGMLines[pgmline].getSecondOperand();

          if ( ! PGMLines[pgmline].isIsfirstImmediate() && PGMLines[pgmline].isIsSecondImmediate())
              storeRegValue = firstOpValue & secondOpValue;
          else {
              std::cout << "ANDI Instruction Syntax Incorrect: Immediate Expected In Syntax";
              exit(1);
          }
          setRFILE(PGMLines[pgmline].getStoreReg(), storeRegValue);
      }

      if (PGMLines[pgmline].getInstrName() == "ORRI") {
          long long storeRegValue = getRFILE(PGMLines[pgmline].getStoreReg());
          long long firstOpValue = getRFILE(PGMLines[pgmline].getFirstOperand());
          long long secondOpValue = PGMLines[pgmline].getSecondOperand();

          if ( ! PGMLines[pgmline].isIsfirstImmediate() && PGMLines[pgmline].isIsSecondImmediate())
              storeRegValue = firstOpValue | secondOpValue;
          else {
              std::cout << "EOR Instruction Syntax Incorrect: Immediate Found In Syntax";
              exit(1);
          }
          setRFILE(PGMLines[pgmline].getStoreReg(), storeRegValue);
      }

      if (PGMLines[pgmline].getInstrName() == "EORI") {
          long long storeRegValue = getRFILE(PGMLines[pgmline].getStoreReg());
          long long firstOpValue = getRFILE(PGMLines[pgmline].getFirstOperand());
          long long secondOpValue = PGMLines[pgmline].getSecondOperand();

          if ( ! PGMLines[pgmline].isIsfirstImmediate() && PGMLines[pgmline].isIsSecondImmediate())
              storeRegValue = firstOpValue ^ secondOpValue;
          else {
              std::cout << "EORI Instruction Syntax Incorrect: Immediate Expected In Syntax";
              exit(1);
          }
          setRFILE(PGMLines[pgmline].getStoreReg(), storeRegValue);
      }

      if (PGMLines[pgmline].getInstrName() == "B")
	{
	  pgmline = PGMLines[pgmline].getStoreReg();
	}

      if (PGMLines[pgmline].getInstrName() == "BR")
	{
	  if (PGMLines[pgmline].getStoreReg() == 30)
	    pgmline = getRFILE(PGMLines[pgmline].getStoreReg()); 
	}
      
      if (PGMLines[pgmline].getInstrName() == "CBZ")
	{
	  if (getRFILE(PGMLines[pgmline].getStoreReg()) == 0)
	    pgmline = PGMLines[pgmline].getFirstOperand();
	}

      if (PGMLines[pgmline].getInstrName() == "CBNZ")
	{
	  // CBNZ 
	  if (getRFILE(PGMLines[pgmline].getStoreReg()) != 0)
	    pgmline = PGMLines[pgmline].getFirstOperand();
	}

      pgmline++;
  }
 
}

void legv8Sim::setRFILE(int index, long long value)
{
  RFILE[index] = value;
}

long long legv8Sim::getRFILE(int index)
{
  return RFILE[index];
}
