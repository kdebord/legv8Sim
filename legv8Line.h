#ifndef LEGV8SIM_LEGV8LINE_H
#define LEGV8SIM_LEGV8LINE_H

#include <string>

//Written By Kyle de Bord, Aidan Shirey, Jasmine De La Torre, Kiyoko Mangrobang

class legv8Line {

	public:
		legv8Line() {instrName = "", storeReg = -1, firstOperand = -1, secondOperand = -1,
			   				isSecondImmediate = false, isSecondOperandLabel = false;
							secondOperand_Label = "",
	       					v = false, c = false, n = false, z = false; };
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
		std::string secondOperand_Label;
        bool v, c, n ,z;
		bool isSecondImmediate, isSecondOperandLabel;
public:

	bool isIsSecondImmediate() const;

	void setIsSecondImmediate(bool isSecondImmediate);
	
	bool isIsSecondOperandLabel() const;

	void setSecondOperandLabel(bool isSecondOperandLabel);
	
	const std::string &getSecondOperand_Label() const;

	void setSecondOperand_Label(const std::string &secondOperand_Label);

    bool checkV() const;

    void setV(bool result);

    bool checkC() const;

    void setC(bool result);

    bool checkN() const;

    void setN(bool result);

    bool checkZ() const;

    void setZ(bool result);
};


#endif //LEGV8SIM_LEGV8LINE_H
