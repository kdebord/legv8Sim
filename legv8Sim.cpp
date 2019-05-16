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
    while( std::getline(file,tempLine) )
    {
        PGM.push_back(tempLine);
    }
    return;
}

legv8Line legv8Sim::parseLine(int lineToGrab) {
    std::string pgmLine = PGM[lineToGrab];

    legv8Line lineToReturn;
    std::string::iterator iter = pgmLine.begin();
    if(*iter == '\t')
        iter++;
    else if(*iter == ' ')
        iter += 4;
    else if(*iter == '\000')
    {
        std::cout << "Blank line in input file, exiting";
        exit(1);
    }

    std::string instrOrLabel;
    while(*iter != ' ')
    {
        instrOrLabel += *iter;
        iter++;
    }
    if(instrOrLabel.back() == ':')
    {
        Label newLabel;
        newLabel.label = instrOrLabel.substr(0, instrOrLabel.size() - 1);
        newLabel.line_num = lineToGrab;
        LABELS.push_back(newLabel);
        iter++;
        std::string instrName;
        while(*iter != ' ')
        {
            instrName += *iter;
            iter++;
        }
        lineToReturn.setInstrName(instrName);
    }
    else
        lineToReturn.setInstrName(instrOrLabel);

    iter++;
    std::string storeReg;
    while(*iter != ',')
    {
        storeReg += *iter;
        iter++;
    }
    storeReg.erase(0,1);
    lineToReturn.setStoreReg(std::stoi(storeReg));
    iter += 2;
    if(lineToReturn.getInstrName() == "CBZ" || lineToReturn.getInstrName() == "CBNZ")
    {
        std::string secondOperandLabel;
        while(isalnum(*iter))
        {
            secondOperandLabel += *iter;
            iter++;
        }
        lineToReturn.setSecondOperandLabel(true);
        lineToReturn.setSecondOperand_Label(secondOperandLabel);
        return lineToReturn;
    }
    else
    {
        std::string firstOperand;
        while(*iter != ',')
        {
            firstOperand += *iter;
            iter++;
        }
        firstOperand.erase(0,1);
        lineToReturn.setFirstOperand(std::stoi(firstOperand));

        std::string secondOperand;
        iter += 2;
        while(*iter != '\000')
        {
            secondOperand += *iter;
            iter++;
        }

        if(secondOperand.front() == '#')
            lineToReturn.setIsSecondImmediate(true);

        secondOperand.erase(0,1);
        lineToReturn.setSecondOperand(std::stoi(secondOperand));
    }

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
  while (pgmline < PGMLines.size())
    {
      if(PGMLines[pgmline].getInstrName() == "ADD")
	{
	  long long storeRegValue = getRFILE(PGMLines[pgmline].getStoreReg());
	  long long firstOpValue = getRFILE(PGMLines[pgmline].getFirstOperand());
	  long long secondOpValue = getRFILE(PGMLines[pgmline].getSecondOperand());
	  // The following if statements check for correct instruction syntax
	  if (!PGMLines[pgmline].isIsSecondImmediate())
	    storeRegValue = std::abs(firstOpValue) + std::abs(secondOpValue);
	  else{
	    std::cout << "ADD Instruction Syntax Incorrect: Immediate Detected";
	    exit(1);
	  }
	  setRFILE(PGMLines[pgmline].getStoreReg(), storeRegValue);
	}
      
      if (PGMLines[pgmline].getInstrName() == "ADDI")
	{
	  long long storeRegValue = getRFILE(PGMLines[pgmline].getStoreReg());
	  long long firstOpValue = getRFILE(PGMLines[pgmline].getFirstOperand());
	  long long secondOpValue = PGMLines[pgmline].getSecondOperand();
	  // The following if statements check for correct instruction syntax
	  if (PGMLines[pgmline].isIsSecondImmediate())
	    storeRegValue = std::abs(firstOpValue) + std::abs(secondOpValue);
	  else{
	    std::cout << "ADDI Instruction Syntax Incorrect: Immediate Expected In Second Operand";
	    exit(1);
	  }
	  setRFILE(PGMLines[pgmline].getStoreReg(), storeRegValue);	  
	}
      
      if (PGMLines[pgmline].getInstrName() == "SUB")
	{
	  long long storeRegValue = getRFILE(PGMLines[pgmline].getStoreReg());
	  long long firstOpValue = getRFILE(PGMLines[pgmline].getFirstOperand());
	  long long secondOpValue = getRFILE(PGMLines[pgmline].getSecondOperand());
	  // The following if statements check for correct instruction syntax
	  if (!PGMLines[pgmline].isIsSecondImmediate())
	    storeRegValue = std::abs(firstOpValue) - std::abs(secondOpValue);
	  else{
	    std::cout << "SUB Instruction Syntax Incorrect: Immediate Detected";
	    exit(1);
	  }
	  setRFILE(PGMLines[pgmline].getStoreReg(), storeRegValue);
	}
      
      if (PGMLines[pgmline].getInstrName() == "SUBI")
	{
	  long long storeRegValue = getRFILE(PGMLines[pgmline].getStoreReg());
	  long long firstOpValue = getRFILE(PGMLines[pgmline].getFirstOperand());
	  long long secondOpValue = PGMLines[pgmline].getSecondOperand();
	  // The following if statements check for correct instruction syntax
	  if (PGMLines[pgmline].isIsSecondImmediate())
	    storeRegValue = std::abs(firstOpValue) - std::abs(secondOpValue);
	  else{
	    std::cout << "SUBI Instruction Syntax Incorrect: Immediate Expected In Second Operand";
	    exit(1);
	  }
	  setRFILE(PGMLines[pgmline].getStoreReg(), storeRegValue);
	}
	
	if (PGMLines[pgmline].getInstrName() == "AND") {
          long long storeRegValue = getRFILE(PGMLines[pgmline].getStoreReg());
          long long firstOpValue = getRFILE(PGMLines[pgmline].getFirstOperand());
          long long secondOpValue = getRFILE(PGMLines[pgmline].getSecondOperand());

          if (! PGMLines[pgmline].isIsSecondImmediate())
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

          if (! PGMLines[pgmline].isIsSecondImmediate())
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

          if (! PGMLines[pgmline].isIsSecondImmediate())
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

          if (PGMLines[pgmline].isIsSecondImmediate())
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

          if (PGMLines[pgmline].isIsSecondImmediate())
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

          if (PGMLines[pgmline].isIsSecondImmediate())
              storeRegValue = firstOpValue ^ secondOpValue;
          else {
              std::cout << "EORI Instruction Syntax Incorrect: Immediate Expected In Syntax";
              exit(1);
          }
          setRFILE(PGMLines[pgmline].getStoreReg(), storeRegValue);
		}
      
	
	if (PGMLines[pgmline].getInstrName() == "LSL") {
          long long storeRegValue = getRFILE(PGMLines[pgmline].getStoreReg());
          long long firstOpValue = getRFILE(PGMLines[pgmline].getFirstOperand());
          long long secondOpValue = PGMLines[pgmline].getSecondOperand();

          if (PGMLines[pgmline].isIsSecondImmediate())
              storeRegValue = firstOpValue << secondOpValue;
          else {
              std::cout << "EORI Instruction Syntax Incorrect: Immediate Expected In Syntax";
              exit(1);
          }
          setRFILE(PGMLines[pgmline].getStoreReg(), storeRegValue);
      }

    if (PGMLines[pgmline].getInstrName() == "LSR") {
          long long storeRegValue = getRFILE(PGMLines[pgmline].getStoreReg());
          long long firstOpValue = getRFILE(PGMLines[pgmline].getFirstOperand());
          long long secondOpValue = PGMLines[pgmline].getSecondOperand();

          if (PGMLines[pgmline].isIsSecondImmediate())
              storeRegValue = firstOpValue >> secondOpValue;
          else {
              std::cout << "EORI Instruction Syntax Incorrect: Immediate Expected In Syntax";
              exit(1);
          }
          setRFILE(PGMLines[pgmline].getStoreReg(), storeRegValue);
		}

    if (PGMLines[pgmline].getInstrName() == "B") {

        pgmline = PGMLines[pgmline].getStoreReg();
    }

    if (PGMLines[pgmline].getInstrName() == "BR") {

        if (PGMLines[pgmline].getStoreReg() == 30)
            pgmline = getRFILE(PGMLines[pgmline].getStoreReg());
    }

    if (PGMLines[pgmline].getInstrName() == "CBZ") {
        //These are causing some kind of seg fault when being run.
        //the first operand is never net in CBZ, just the second operand label and
        //the getStoreReg.

        //if (getRFILE(PGMLines[pgmline].getStoreReg()) == 0)
        //    pgmline = PGMLines[pgmline].getFirstOperand();
    }

    if (PGMLines[pgmline].getInstrName() == "CBNZ") {
        //These are causing some kind of seg fault when being run.
        //the first operand is never net in CBZ, just the second operand label and
        //the getStoreReg.

        //if (getRFILE(PGMLines[pgmline].getStoreReg()) != 0)
        //    pgmline = PGMLines[pgmline].getFirstOperand();
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
