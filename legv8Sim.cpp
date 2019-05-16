#include "legv8Sim.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>

//praseFileToVector takes the input file and line by line
//adds it to the PGM vector

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

//parseLines takes a string from PGM which represents one line
//of legv8 assembly, and parses it to a more workable legv8Line class object

legv8Line legv8Sim::parseLine(int lineToGrab) {
    //create the strong to be iterated through
    std::string pgmLine = PGM[lineToGrab];

    //create the lev8line that will eventually be returned
    legv8Line lineToReturn;

    //handling for tabs or spaces used in place of tabs in front of lines
    std::string::iterator iter = pgmLine.begin();
    if(*iter == '\t')
        iter++;
    else if(*iter == ' ')
        iter += 4;
    //check to catch mis-formatted blank lines in file
    else if(*iter == '\000')
    {
        std::cout << "Blank line in input file, exiting";
        exit(1);
    }

    //iterates through the first "token" of the line
    std::string instrOrLabel;
    while(*iter != ' ')
    {
        instrOrLabel += *iter;
        iter++;
    }
    //finding a ':' determines if if its a instruction or the label of a assembly function
    if(instrOrLabel.back() == ':')
    {
        //if it has a label at the front, push that to the label vector and find the next token
        //this next token will be the instruction name
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
        //otherwise its just the instruction name
        lineToReturn.setInstrName(instrOrLabel);

    iter++;
    //next token to be found will be the register your result is stored in
    std::string storeReg;
    while(*iter != ',')
    {
        storeReg += *iter;
        iter++;
    }
    //this erase removes the 'x' from the beginning of the register
    storeReg.erase(0,1);
    lineToReturn.setStoreReg(std::stoi(storeReg));

    iter += 2;
    //special handling for CBZ and CBNZ instructions
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
    //otherwise you simply find the first and second operands
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
        //second operand can be a immediate, this check determines such
        if(secondOperand.front() == '#')
            lineToReturn.setIsSecondImmediate(true);

        secondOperand.erase(0,1);
        lineToReturn.setSecondOperand(std::stoi(secondOperand));
    }

    return lineToReturn;
}

//parseLineToPGMLine takes the stores strings from PGM, and parses them one by one
//into the more usable legv8Line class objects

void legv8Sim::parseLineToPGMLine(){
  int index = 0;
  while(index < PGM.size())
    {
      PGMLines.push_back(parseLine(index));
      index++;
    }
}

//runLine takes the PMGLines vector that was created, and iterates through it to run the lines
    //needs to be altered to just run and use a helper function if you want to run all lines
    //This will assist getting the the program to allow you to run line by line

void legv8Sim::runLine() {
  int pgmline = 0;

  //All the following instructions following a similar format, with slight differences
  //depending on if its a immediate or non-immediate

  //The getRFILE() function is used if you need the value held within the register vector at that index
  //Otherwise, in the case of immedaites, the PGMLines[pgmline] valuse are used directly
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
        //the implentation of this needs to be double checked
        //pgmline = PGMLines[pgmline].getStoreReg();
    }

    if (PGMLines[pgmline].getInstrName() == "BR") {
        //the implentation of this needs to be double checked
        //if (PGMLines[pgmline].getStoreReg() == 30)
        //    pgmline = getRFILE(PGMLines[pgmline].getStoreReg());
    }

    if (PGMLines[pgmline].getInstrName() == "CBZ") {
        //the implentation of this needs to be double checked
        //These are causing some kind of seg fault when being run.
        //the first operand is never net in CBZ, just the second operand label and
        //the getStoreReg.

        //if (getRFILE(PGMLines[pgmline].getStoreReg()) == 0)
        //    pgmline = PGMLines[pgmline].getFirstOperand();
    }

    if (PGMLines[pgmline].getInstrName() == "CBNZ") {
        //the implentation of this needs to be double checked
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
