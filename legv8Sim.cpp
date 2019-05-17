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
    while(std::getline(file,tempLine))
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

    if (lineToReturn.getInstrName() == "BR")
      {
	return lineToReturn;
      }
    iter += 2;
    //special handling for CBZ and CBNZ instructions
    if(lineToReturn.getInstrName() == "CBZ" || lineToReturn.getInstrName() == "CBNZ"
       || lineToReturn.getInstrName() == "BL" || lineToReturn.getInstrName() == "B")
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
        while(*iter != '\000' && *iter != ' ')
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
	if (PGMLines[pgmline].getInstrName() == "ADD") {
	  long long storeRegValue = getRFILE(PGMLines[pgmline].getStoreReg());
	  long long firstOpValue = getRFILE(PGMLines[pgmline].getFirstOperand());
	  long long secondOpValue = getRFILE(PGMLines[pgmline].getSecondOperand());
	  // The following if statements check for correct instruction syntax
	  
	  if (!PGMLines[pgmline].isIsSecondImmediate())
	    storeRegValue = std::abs(firstOpValue) + std::abs(secondOpValue);
	  else {
	    std::cout << "ADD Instruction Syntax Incorrect: Immediate Detected";
	    exit(1);
	  }
	  setRFILE(PGMLines[pgmline].getStoreReg(), storeRegValue);
	 
	}
	
	if (PGMLines[pgmline].getInstrName() == "ADDS") {
          long long storeRegValue = getRFILE(PGMLines[pgmline].getStoreReg());
          long long firstOpValue = getRFILE(PGMLines[pgmline].getFirstOperand());
          long long secondOpValue = getRFILE(PGMLines[pgmline].getSecondOperand());
          unsigned long long int x1 = storeRegValue;
          unsigned long long int y1 = firstOpValue;
          unsigned long long int z1 = secondOpValue;
          // The following if statements check for correct instruction syntax
          if (!PGMLines[pgmline].isIsSecondImmediate())
	    storeRegValue = std::abs(firstOpValue) + std::abs(secondOpValue);
          else {
	    std::cout << "ADD Instruction Syntax Incorrect: Immediate Detected";
	    exit(1);
          }
          setRFILE(PGMLines[pgmline].getStoreReg(), storeRegValue);
          // setting V flag
          long long tempZ = std::abs(storeRegValue) + std::abs(firstOpValue);
          setRFILE(PGMLines[pgmline].getSecondOperand(), tempZ);
          if ((storeRegValue < 0 && firstOpValue >= 0)
              || (storeRegValue >= 0 && firstOpValue < 0)
              || (storeRegValue < 0 && secondOpValue >= 0)
              || (storeRegValue >= 0 && secondOpValue < 0))
              PGMLines[pgmline].setV(false);
          else if (!((tempZ >= 0 && storeRegValue < 0)
                     || (tempZ < 0 && storeRegValue >= 0)
                     || (tempZ >= 0 && firstOpValue < 0)
                     || (tempZ < 0 && firstOpValue >= 0)))
              PGMLines[pgmline].setV(true);
          else
              PGMLines[pgmline].setV(false);
          // check C
          x1 = y1 + z1;
          if (x1 < y1 || x1 < z1)
              PGMLines[pgmline].setC(true);
          else
              PGMLines[pgmline].setC(false);
          //check Z
          if (getRFILE(PGMLines[pgmline].getStoreReg()) == 0)
              PGMLines[pgmline].setZ(true);
          else
              PGMLines[pgmline].setZ(false);
          //check N
          if (getRFILE(PGMLines[pgmline].getStoreReg()) < 0)
              PGMLines[pgmline].setN(true);
          else
              PGMLines[pgmline].setN(false);
      }



      if (PGMLines[pgmline].getInstrName() == "ADDI") {
          long long storeRegValue = getRFILE(PGMLines[pgmline].getStoreReg());
          long long firstOpValue = getRFILE(PGMLines[pgmline].getFirstOperand());
          long long secondOpValue = PGMLines[pgmline].getSecondOperand();
          // The following if statements check for correct instruction syntax
          if (PGMLines[pgmline].isIsSecondImmediate())
              storeRegValue = std::abs(firstOpValue) + std::abs(secondOpValue);
          else {
              std::cout << "ADDI Instruction Syntax Incorrect: Immediate Expected In Second Operand";
              exit(1);
          }
          setRFILE(PGMLines[pgmline].getStoreReg(), storeRegValue);
      }

      if (PGMLines[pgmline].getInstrName() == "ADDIS") {
          long long storeRegValue = getRFILE(PGMLines[pgmline].getStoreReg());
          long long firstOpValue = getRFILE(PGMLines[pgmline].getFirstOperand());
          long long secondOpValue = PGMLines[pgmline].getSecondOperand();
          unsigned long long int x1 = storeRegValue;
          unsigned long long int y1 = firstOpValue;
          unsigned long long int z1 = secondOpValue;
          // The following if statements check for correct instruction syntax
          if (PGMLines[pgmline].isIsSecondImmediate())
              storeRegValue = std::abs(firstOpValue) + std::abs(secondOpValue);
          else {
              std::cout << "ADDI Instruction Syntax Incorrect: Immediate Expected In Second Operand";
              exit(1);
          }
          setRFILE(PGMLines[pgmline].getStoreReg(), storeRegValue);
          // setting V flag
          long long tempZ = std::abs(storeRegValue) + std::abs(firstOpValue);
          setRFILE(PGMLines[pgmline].getSecondOperand(), tempZ);
          if ((storeRegValue < 0 && firstOpValue >= 0)
              || (storeRegValue >= 0 && firstOpValue < 0)
              || (storeRegValue < 0 && secondOpValue >= 0)
              || (storeRegValue >= 0 && secondOpValue < 0))
              PGMLines[pgmline].setV(false);
          else if (!((tempZ >= 0 && storeRegValue < 0)
                     || (tempZ < 0 && storeRegValue >= 0)
                     || (tempZ >= 0 && firstOpValue < 0)
                     || (tempZ < 0 && firstOpValue >= 0)))
              PGMLines[pgmline].setV(true);
          else
              PGMLines[pgmline].setV(false);
          // check C
          x1 = y1 + z1;
          if (x1 < y1 || x1 < z1)
              PGMLines[pgmline].setC(true);
          else
              PGMLines[pgmline].setC(false);
          //check Z
          if (getRFILE(PGMLines[pgmline].getStoreReg()) == 0)
              PGMLines[pgmline].setZ(true);
          else
              PGMLines[pgmline].setZ(false);
          //check N
          if (getRFILE(PGMLines[pgmline].getStoreReg()) < 0)
              PGMLines[pgmline].setN(true);
          else
              PGMLines[pgmline].setN(false);
      }

      if (PGMLines[pgmline].getInstrName() == "SUB") {
          long long storeRegValue = getRFILE(PGMLines[pgmline].getStoreReg());
          long long firstOpValue = getRFILE(PGMLines[pgmline].getFirstOperand());
          long long secondOpValue = getRFILE(PGMLines[pgmline].getSecondOperand());
          // The following if statements check for correct instruction syntax
          if (!PGMLines[pgmline].isIsSecondImmediate())
              storeRegValue = std::abs(firstOpValue) - std::abs(secondOpValue);
          else {
              std::cout << "SUB Instruction Syntax Incorrect: Immediate Detected";
              exit(1);
          }
          setRFILE(PGMLines[pgmline].getStoreReg(), storeRegValue);
      }

      if (PGMLines[pgmline].getInstrName() == "SUBS") {
          long long storeRegValue = getRFILE(PGMLines[pgmline].getStoreReg());
          long long firstOpValue = getRFILE(PGMLines[pgmline].getFirstOperand());
          long long secondOpValue = getRFILE(PGMLines[pgmline].getSecondOperand());
          unsigned long long int x1 = storeRegValue;
          unsigned long long int y1 = firstOpValue;
          // The following if statements check for correct instruction syntax
          if (!PGMLines[pgmline].isIsSecondImmediate())
              storeRegValue = std::abs(firstOpValue) - std::abs(secondOpValue);
          else {
              std::cout << "SUB Instruction Syntax Incorrect: Immediate Detected";
              exit(1);
          }
          setRFILE(PGMLines[pgmline].getStoreReg(), storeRegValue);
          long long tempZ = secondOpValue * -1;
          unsigned long long int z1 = secondOpValue * -1;
          setRFILE(PGMLines[pgmline].getSecondOperand(), tempZ);
          if ((storeRegValue < 0 && firstOpValue >= 0)
              || (storeRegValue >= 0 && firstOpValue < 0)
              || (storeRegValue < 0 && secondOpValue >= 0)
              || (storeRegValue >= 0 && secondOpValue < 0))
              PGMLines[pgmline].setV(false);
          else if (!((tempZ >= 0 && storeRegValue < 0)
                     || (tempZ < 0 && storeRegValue >= 0)
                     || (tempZ >= 0 && firstOpValue < 0)
                     || (tempZ < 0 && firstOpValue >= 0)))
              PGMLines[pgmline].setV(true);
          else
              PGMLines[pgmline].setV(false);
          // check C
          x1 = y1 + z1;
          if (x1 < y1 || x1 < z1)
              PGMLines[pgmline].setC(true);
          else
              PGMLines[pgmline].setC(false);
          //check Z
          if (getRFILE(PGMLines[pgmline].getStoreReg()) == 0)
              PGMLines[pgmline].setZ(true);
          else
              PGMLines[pgmline].setZ(false);
          //check N
          if (getRFILE(PGMLines[pgmline].getStoreReg()) < 0)
              PGMLines[pgmline].setN(true);
          else
              PGMLines[pgmline].setN(false);
	  
      }
      
      if (PGMLines[pgmline].getInstrName() == "SUBI") {
          long long storeRegValue = getRFILE(PGMLines[pgmline].getStoreReg());
          long long firstOpValue = getRFILE(PGMLines[pgmline].getFirstOperand());
          long long secondOpValue = PGMLines[pgmline].getSecondOperand();
          // The following if statements check for correct instruction syntax
          if (PGMLines[pgmline].isIsSecondImmediate())
              storeRegValue = std::abs(firstOpValue) - std::abs(secondOpValue);
          else {
              std::cout << "SUBI Instruction Syntax Incorrect: Immediate Expected In Second Operand";
              exit(1);
          }
          setRFILE(PGMLines[pgmline].getStoreReg(), storeRegValue);
	  
      }

      if (PGMLines[pgmline].getInstrName() == "SUBIS") {
          long long storeRegValue = getRFILE(PGMLines[pgmline].getStoreReg());
          long long firstOpValue = getRFILE(PGMLines[pgmline].getFirstOperand());
          long long secondOpValue = PGMLines[pgmline].getSecondOperand();
          unsigned long long int x1 = storeRegValue;
          unsigned long long int y1 = firstOpValue;
          // The following if statements check for correct instruction syntax
          if (PGMLines[pgmline].isIsSecondImmediate())
              storeRegValue = std::abs(firstOpValue) - std::abs(secondOpValue);
          else {
              std::cout << "SUB Instruction Syntax Incorrect: Immediate Detected";
              exit(1);
          }
          setRFILE(PGMLines[pgmline].getStoreReg(), storeRegValue);
          long long tempZ = secondOpValue * -1;
          unsigned long long int z1 = secondOpValue * -1;
          setRFILE(PGMLines[pgmline].getSecondOperand(), tempZ);
          if ((storeRegValue < 0 && firstOpValue >= 0)
              || (storeRegValue >= 0 && firstOpValue < 0)
              || (storeRegValue < 0 && secondOpValue >= 0)
              || (storeRegValue >= 0 && secondOpValue < 0))
              PGMLines[pgmline].setV(false);
          else if (!((tempZ >= 0 && storeRegValue < 0)
                     || (tempZ < 0 && storeRegValue >= 0)
                     || (tempZ >= 0 && firstOpValue < 0)
                     || (tempZ < 0 && firstOpValue >= 0)))
              PGMLines[pgmline].setV(true);
          else
              PGMLines[pgmline].setV(false);
          // check C
          x1 = y1 + z1;
          if (x1 < y1 || x1 < z1)
              PGMLines[pgmline].setC(true);
          else
              PGMLines[pgmline].setC(false);
          //check Z
          if (getRFILE(PGMLines[pgmline].getStoreReg()) == 0)
              PGMLines[pgmline].setZ(true);
          else
              PGMLines[pgmline].setZ(false);
          //check N
          if (getRFILE(PGMLines[pgmline].getStoreReg()) < 0)
              PGMLines[pgmline].setN(true);
          else
              PGMLines[pgmline].setN(false);
	  
      }

      if (PGMLines[pgmline].getInstrName() == "AND") {
          long long storeRegValue = getRFILE(PGMLines[pgmline].getStoreReg());
          long long firstOpValue = getRFILE(PGMLines[pgmline].getFirstOperand());
          long long secondOpValue = getRFILE(PGMLines[pgmline].getSecondOperand());

          if (!PGMLines[pgmline].isIsSecondImmediate())
              storeRegValue = firstOpValue & secondOpValue;
          else {
              std::cout << "AND Instruction Syntax Incorrect: Immediate Found In Syntax";
              exit(1);
          }
          setRFILE(PGMLines[pgmline].getStoreReg(), storeRegValue);
	  
      }

      if (PGMLines[pgmline].getInstrName() == "ANDS") {
          long long storeRegValue = getRFILE(PGMLines[pgmline].getStoreReg());
          long long firstOpValue = getRFILE(PGMLines[pgmline].getFirstOperand());
          long long secondOpValue = getRFILE(PGMLines[pgmline].getSecondOperand());

          if (!PGMLines[pgmline].isIsSecondImmediate())
              storeRegValue = firstOpValue & secondOpValue;
          else {
              std::cout << "AND Instruction Syntax Incorrect: Immediate Found In Syntax";
              exit(1);
          }
          setRFILE(PGMLines[pgmline].getStoreReg(), storeRegValue);
          //check Z
          if (getRFILE(PGMLines[pgmline].getStoreReg()) == 0)
              PGMLines[pgmline].setZ(true);
          else
              PGMLines[pgmline].setZ(false);
          //check N
          if (getRFILE(PGMLines[pgmline].getStoreReg()) < 0)
              PGMLines[pgmline].setN(true);
          else
              PGMLines[pgmline].setN(false);
	  
      }

      if (PGMLines[pgmline].getInstrName() == "ORR") {
          long long storeRegValue = getRFILE(PGMLines[pgmline].getStoreReg());
          long long firstOpValue = getRFILE(PGMLines[pgmline].getFirstOperand());
          long long secondOpValue = getRFILE(PGMLines[pgmline].getSecondOperand());

          if (!PGMLines[pgmline].isIsSecondImmediate())
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

          if (!PGMLines[pgmline].isIsSecondImmediate())
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

      if (PGMLines[pgmline].getInstrName() == "ANDIS" && pgmline < PGMLines.size()) {
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
          if (getRFILE(PGMLines[pgmline].getStoreReg()) == 0)
              PGMLines[pgmline].setZ(true);
          else
              PGMLines[pgmline].setZ(false);
          //check N
          if (getRFILE(PGMLines[pgmline].getStoreReg()) < 0)
              PGMLines[pgmline].setN(true);
          else
              PGMLines[pgmline].setN(false);
	  
      }

      if (PGMLines[pgmline].getInstrName() == "ORRI" && pgmline < PGMLines.size()) {
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

      if (PGMLines[pgmline].getInstrName() == "EORI" && pgmline < PGMLines.size()) {
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
	  pgmline++;
      }


      if (PGMLines[pgmline].getInstrName() == "LSL" && pgmline < PGMLines.size()) {
          long long storeRegValue = getRFILE(PGMLines[pgmline].getStoreReg());
          long long firstOpValue = getRFILE(PGMLines[pgmline].getFirstOperand());
          long long secondOpValue = PGMLines[pgmline].getSecondOperand();

          if ( PGMLines[pgmline].isIsSecondImmediate())
              storeRegValue = firstOpValue << secondOpValue;
          else {
              std::cout << "EORI Instruction Syntax Incorrect: Immediate Expected In Syntax";
              exit(1);
          }
          setRFILE(PGMLines[pgmline].getStoreReg(), storeRegValue);
	  
      }

      if (PGMLines[pgmline].getInstrName() == "LSR" && pgmline < PGMLines.size()) {
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
          // B SYNTAX: B label
          // set the instruction name and set the second operand to be the label name using setSecondOperand_Label()
          if (PGMLines[pgmline].isIsSecondOperandLabel()) {
              // if the second operand is a label, we search the label vector
              // and make the pgmline equal the labels corresponding line_num
              std::string labelname = PGMLines[pgmline].getSecondOperand_Label();
              for (int i = 0; i < LABELS.size(); i++) {
                  if (LABELS[i].label == labelname)
                      pgmline = LABELS[i].line_num;
              }
          }
	  pgmline--;
      }

      if (PGMLines[pgmline].getInstrName() == "BL" ) {
          // BL SYNTAX: BL label
          // store instruction name and only set second operand as a label
          setRFILE(30, pgmline + 1);
          if (pgmline < PGMLines.size() && PGMLines[pgmline].isIsSecondOperandLabel()) {
              // if the second operand is a label, we search the label vector
              // and make the pgmline equal the labels corresponding line_num
              std::string labelname = PGMLines[pgmline].getSecondOperand_Label();
              for (int i = 0; i < LABELS.size(); i++) {
                  if (LABELS[i].label == labelname)
                      pgmline = LABELS[i].line_num;
              }
          }
	  pgmline--;
      }

      if (PGMLines[pgmline].getInstrName() == "BR" ) {
          // BR SYNTAX: BR x30
          // store instruction name as BR and set store register with value 30
          if (PGMLines[pgmline].getStoreReg() == 30)
              pgmline = getRFILE(PGMLines[pgmline].getStoreReg());
	  pgmline--;
      }

      if (PGMLines[pgmline].getInstrName() == "CBZ") {
          // CBZ SYNTAX : CBZ x0, DONE
          // store x0 in store register, and set the second operand to be the label with the correct labelname
          if (getRFILE(PGMLines[pgmline].getStoreReg()) == 0)
              if (PGMLines[pgmline].isIsSecondOperandLabel()) {
                  // if the second operand is a label, we search the label vector
                  // and make the pgmline equal the labels corresponding line_num
                  std::string labelname = PGMLines[pgmline].getSecondOperand_Label();
                  for (int i = 0; i < LABELS.size(); i++) {
                      if (LABELS[i].label == labelname)
                          pgmline = LABELS[i].line_num;
                  }
              }
	  pgmline--;
      }

      if (PGMLines[pgmline].getInstrName() == "CBNZ") {
          // see CBNZ for line construction information
          if (getRFILE(PGMLines[pgmline].getStoreReg()) != 0)
              if (PGMLines[pgmline].isIsSecondOperandLabel()) {
                  std::string labelname = PGMLines[pgmline].getSecondOperand_Label();
                  for (int i = 0; i < LABELS.size(); i++) {
                      if (LABELS[i].label == labelname)
                          pgmline = LABELS[i].line_num;
                  }
              }
	  pgmline--;
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
