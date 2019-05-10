#ifndef LEGV8SIM_LEGV8LINE_H
#define LEGV8SIM_LEGV8LINE_H

#include <string>

class legv8Line {

	public:
		legv8Line() {instrName = "", storeReg = 0, firstOperand = 0, secondOperand = 0,
						isfirstImmediate = false, isSecondImmediate = false; };
		const std::string &getInstrName() const;
		void setInstrName(const std::string &instrName);
		int getStoreReg() const;
		void setStoreReg(int storeReg);
		int getFirstOperand() const;
		void setFirstOperand(int firstOperand);
		int getSecondOperand() const;
		void setSecondOperand(int secondOperand);

	private:
		std::string instrName;
		int storeReg, firstOperand, secondOperand;
		bool isfirstImmediate, isSecondImmediate;
public:
	bool isIsfirstImmediate() const;

	void setIsfirstImmediate(bool isfirstImmediate);

	bool isIsSecondImmediate() const;

	void setIsSecondImmediate(bool isSecondImmediate);


};


#endif //LEGV8SIM_LEGV8LINE_H