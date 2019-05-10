//
// Created by kdebo on 4/30/2019.
//

#include "legv8Line.h"

const std::string &legv8Line::getInstrName() const {
    return instrName;
}

void legv8Line::setInstrName(const std::string & instrName) {
    legv8Line::instrName = instrName;
}

int legv8Line::getStoreReg() const {
    return storeReg;
}

void legv8Line::setStoreReg(int storeReg) {
    legv8Line::storeReg = storeReg;
}

int legv8Line::getFirstOperand() const {
    return firstOperand;
}

void legv8Line::setFirstOperand(int firstOperand) {
    legv8Line::firstOperand = firstOperand;
}

int legv8Line::getSecondOperand() const {
    return secondOperand;
}

void legv8Line::setSecondOperand(int secondOperand) {
    legv8Line::secondOperand = secondOperand;
}

bool legv8Line::isIsfirstImmediate() const {
    return isfirstImmediate;
}

void legv8Line::setIsfirstImmediate(bool isfirstImmediate) {
    legv8Line::isfirstImmediate = isfirstImmediate;
}

bool legv8Line::isIsSecondImmediate() const {
    return isSecondImmediate;
}

void legv8Line::setIsSecondImmediate(bool isSecondImmediate) {
    legv8Line::isSecondImmediate = isSecondImmediate;
}
