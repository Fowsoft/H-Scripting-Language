//Standalone Math Interpreter of H
//by MegapolisPlayer/Fowsoft, 2022
//Licensed under CC-BY-4.0

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <algorithm>
#include <vector>
#include <map>

#define elif else if
#define loopover continue

//UTILS

uint64_t StringLength(const char* const aString) {
	uint64_t lId = 0;
	while(aString[lId] != '\0') { lId++; }
	return lId;
}
bool IsOneOf(const char aChar, const char* aChars, const uint64_t aCharsAmount) {
	for(uint64_t lId = 0; lId < aCharsAmount; lId++) {
		if(aChar == aChars[lId]) { return true; }
	}
	return false;
}
bool Contains(const char* aString, const char aChar) {
	for(uint64_t lId = 0; lId < StringLength(aString); lId++) {
		if(aString[lId] == aChar) { return true; }
	}
	return false;
}
bool ContainsMultiple(const char* aString, const char* aChars, const uint64_t aCharsAmount) {
	for(uint64_t lId = 0; lId < StringLength(aString); lId++) {
		for(uint64_t lId2 = 0; lId2 < aCharsAmount; lId2++) {
			if(aString[lId] == aChars[lId2]) { return true;	}
		}
	}
	return false;
}

bool IsNumber(char aChar) {
	if((aChar < 48) || (aChar > 57)) { return false; }
	return true;
}
bool IsNumber(const char* aString) {
	for(uint64_t lId = 0; lId < StringLength(aString); lId++) {
		if((aString[lId] < 48) || (aString[lId] > 57)) { return false; }
	}
	return true;
}

bool IsMathExpression(const char* aString) {
	for(uint64_t lId = 0; lId < StringLength(aString); lId++) {
		if(aString[lId] != '+' && aString[lId] != '-' && aString[lId] != '*' && aString[lId] != '/' && aString[lId] != '%') {
			if((aString[lId] < 48) || (aString[lId] > 57)) { return false; }
		}
	}
	return true;
}

void RemoveSpacesFl(std::string* aString) {
	uint64_t lId = 0;
	while(lId < aString->size()) {
		if((*aString)[lId] == ' ') {
			aString->erase(aString->begin() + lId); loopover;
		}
		lId++;
	}
}

void ReplaceString(std::string* aString, const std::string& aReplaceWhat, const std::string& aReplacement) {
	uint64_t lId = 0;
	while(lId < aString->size()) {
		if((*aString)[lId] == aReplaceWhat[0]) {
			for(uint64_t lId2 = 0; lId2 < aReplaceWhat.size(); lId2++) {
				if((*aString)[lId + lId2] != aReplaceWhat[lId2]) { break; }
			}
			aString->erase(lId, aReplaceWhat.size()); //-1 because lId already has first letter
			aString->insert(lId, aReplacement);
		}
		lId++;
	}
}

//MAIN

const char Operators[] = {
	'+',
	'-',
	'*',
	'/',
};
constexpr uint64_t OperatorsAmount = 4;

enum OperatorsEnum {
	OPERATOR_PLUS = 0,
	OPERATOR_MINUS = 1,
	OPERATOR_MULT = 2,
	OPERATOR_DIV = 3,
	OPERATOR_MOD = 4
};

int64_t OperatorBlockProc(const int64_t aNumber1, const int64_t aNumber2, const OperatorsEnum aEnum) {
	//std::cout << "OB: N1: " << aNumber1 << " N2: " << aNumber2 << " OP: " << aEnum << "\n"; //debug
	switch(aEnum) {
		case(OPERATOR_PLUS):
			std::cout << aNumber1 + aNumber2 << "\n";
			return aNumber1 + aNumber2;
		case(OPERATOR_MINUS):
			return aNumber1 - aNumber2;
		case(OPERATOR_MULT):
			return aNumber1 * aNumber2;
		case(OPERATOR_DIV):
			return aNumber1 / aNumber2;
		case(OPERATOR_MOD):
			return aNumber1 % aNumber2;
	}
	return 0;
}

int64_t ParenthesisBlockProc(const std::string aParenthesisBlock) {
	//std::cout << "PB: " << aParenthesisBlock << "\n"; //debug
	
	if(!IsMathExpression(aParenthesisBlock.c_str())) {
		std::cout << "ERROR: non-expression characters found\n";
		char lBuffer;
		std::cin >> lBuffer;
		exit(EXIT_FAILURE);
	}
	
	std::string lTempString = aParenthesisBlock;
	std::string lNumber1;
	std::string lNumber2;
	std::string lInsertTemp;
	
	uint64_t lId = 0;
	uint64_t lId2 = 0;
	
	//first pass -> *, / and mod
	while(ContainsMultiple(lTempString.c_str(), "*/%", 3) && lId < lTempString.size()) {
		if(lTempString[lId] == '+') {
			lNumber1.clear();
		}
		elif(lTempString[lId] == '-') {
			lNumber1.clear();
		}
		elif(lTempString[lId] == '*') {
			if(lTempString[lId + 1] == '-') {
				lId2 = lId + 2; //skip - operator
				while(!IsOneOf(lTempString[lId2], Operators, OperatorsAmount) && lId2 < lTempString.size()) {
					lNumber2 += lTempString[lId2];
					lId2++;
				}
				lNumber2 = std::to_string(-std::stoi(lNumber2)); //set to negative
			}
			else {
				lId2 = lId + 1;
				while(!IsOneOf(lTempString[lId2], Operators, OperatorsAmount) && lId2 < lTempString.size()) {
					lNumber2 += lTempString[lId2];
					lId2++;
				}
			}
			lTempString.erase(lId - lNumber1.size(), lNumber1.size() + lNumber2.size() + 1);
			//std::cout << "ERA " << lTempString << "\n"; //debug
			lInsertTemp = std::to_string(OperatorBlockProc(std::stoi(lNumber1), std::stoi(lNumber2), OPERATOR_MULT));
			lTempString.insert(lId - lNumber1.size(), lInsertTemp);
			//std::cout << "INS " << lTempString << "\n"; //debug
			lId = 0; //to prevent overflow
			lId2 = 0;
			lNumber1.clear();
			lNumber2.clear();
			lInsertTemp.clear();
			lNumber1 += lTempString[lId]; //when loops over adds 1
		}
		elif(lTempString[lId] == '/') {
			if(lTempString[lId + 1] == '-') {
				lId2 = lId + 2; //skip - operator
				while(!IsOneOf(lTempString[lId2], Operators, OperatorsAmount) && lId2 < lTempString.size()) {
					lNumber2 += lTempString[lId2];
					lId2++;
				}
				lNumber2 = std::to_string(-std::stoi(lNumber2)); //set to negative
			}
			else {
				lId2 = lId + 1;
				while(!IsOneOf(lTempString[lId2], Operators, OperatorsAmount) && lId2 < lTempString.size()) {
					lNumber2 += lTempString[lId2];
					lId2++;
				}
			}
			lTempString.erase(lId - lNumber1.size(), lNumber1.size() + lNumber2.size() + 1);
			//std::cout << "ERA " << lTempString << "\n"; //debug
			lInsertTemp = std::to_string(OperatorBlockProc(std::stoi(lNumber1), std::stoi(lNumber2), OPERATOR_DIV));
			lTempString.insert(lId - lNumber1.size(), lInsertTemp);
			//std::cout << "INS " << lTempString << "\n"; //debug
			lId = 0; //to prevent overflow
			lId2 = 0;
			lNumber1.clear();
			lNumber2.clear();
			lInsertTemp.clear();
			lNumber1 += lTempString[lId]; //when loops over adds 1
		}
		elif(lTempString[lId] == '%') {
			if(lTempString[lId + 1] == '-') {
				lId2 = lId + 2; //skip - operator
				while(!IsOneOf(lTempString[lId2], Operators, OperatorsAmount) && lId2 < lTempString.size()) {
					lNumber2 += lTempString[lId2];
					lId2++;
				}
				lNumber2 = std::to_string(-std::stoi(lNumber2)); //set to negative
			}
			else {
				lId2 = lId + 1;
				while(!IsOneOf(lTempString[lId2], Operators, OperatorsAmount) && lId2 < lTempString.size()) {
					lNumber2 += lTempString[lId2];
					lId2++;
				}
			}
			lTempString.erase(lId - lNumber1.size(), lNumber1.size() + lNumber2.size() + 1);
			//std::cout << "ERA " << lTempString << "\n"; //debug
			lInsertTemp = std::to_string(OperatorBlockProc(std::stoi(lNumber1), std::stoi(lNumber2), OPERATOR_MOD));
			lTempString.insert(lId - lNumber1.size(), lInsertTemp);
			//std::cout << "INS " << lTempString << "\n"; //debug
			lId = 0; //to prevent overflow
			lId2 = 0;
			lNumber1.clear();
			lNumber2.clear();
			lInsertTemp.clear();
			lNumber1 += lTempString[lId]; //when loops over adds 1
		}
		else { lNumber1 += lTempString[lId]; }
		std::cout << lTempString << "\n"; //debug
		lId++;
	}
	
	lNumber1.clear();
	lNumber2.clear();
	lInsertTemp.clear();
	
	lId = 0;
	lId2 = 0;
	
	std::cout << "SP, current PB: " << lTempString << "\n"; //debug
	
	//second pass -> + -
	while(ContainsMultiple(lTempString.c_str(), "+-", 2) && lId < lTempString.size()) {
		if(lTempString[lId] == '+') {
			if(lTempString[lId + 1] == '-') {
				lId2 = lId + 2; //skip - operator
				while(!IsOneOf(lTempString[lId2], Operators, OperatorsAmount) && lId2 < lTempString.size()) {
					lNumber2 += lTempString[lId2];
					lId2++;
				}
				lNumber2 = std::to_string(-std::stoi(lNumber2)); //set to negative
			}
			else {
				lId2 = lId + 1;
				while(!IsOneOf(lTempString[lId2], Operators, OperatorsAmount) && lId2 < lTempString.size()) {
					lNumber2 += lTempString[lId2];
					lId2++;
				}
			}
			lTempString.erase(lId - lNumber1.size(), lNumber1.size() + lNumber2.size() + 1);
			//std::cout << "ERA " << lTempString << "\n"; //debug
			lInsertTemp = std::to_string(OperatorBlockProc(std::stoi(lNumber1), std::stoi(lNumber2), OPERATOR_PLUS));
			lTempString.insert(lId - lNumber1.size(), lInsertTemp);
			//std::cout << "INS " << lTempString << "\n"; //debug
			lId = 0; //to prevent overflow
			lId2 = 0;
			lNumber1.clear();
			lNumber2.clear();
			lInsertTemp.clear();
			lNumber1 += lTempString[lId]; //when loops over adds 1
		}
		elif(lTempString[lId] == '-') {
			if(lNumber1.empty()) { //if number empty -> -X, add - and carry on
				lNumber1 += lTempString[lId]; lId++; loopover;
			}
			elif(lTempString[lId + 1] == '-') {
				lId2 = lId + 2; //skip - operator
				while(!IsOneOf(lTempString[lId2], Operators, OperatorsAmount) && lId2 < lTempString.size()) {
					lNumber2 += lTempString[lId2];
					lId2++;
				}
				lNumber2 = std::to_string(-std::stoi(lNumber2)); //set to positive
			}
			else {
				lId2 = lId + 1;
				while(!IsOneOf(lTempString[lId2], Operators, OperatorsAmount) && lId2 < lTempString.size()) {
					lNumber2 += lTempString[lId2];
					lId2++;
				}
			}
			lTempString.erase(lId - lNumber1.size(), lNumber1.size() + lNumber2.size() + 1);
			//std::cout << "ERA " << lTempString << "\n"; //debug
			lInsertTemp = std::to_string(OperatorBlockProc(std::stoi(lNumber1), std::stoi(lNumber2), OPERATOR_MINUS));
			lTempString.insert(lId - lNumber1.size(), lInsertTemp);
			//std::cout << "INS " << lTempString << "\n"; //debug
			lId = 0; //to prevent overflow
			lId2 = 0;
			lNumber1.clear();
			lNumber2.clear();
			lInsertTemp.clear();
			lNumber1 += lTempString[lId]; //when loops over adds 1
		}
		else {
			lNumber1 += lTempString[lId];
		}
		std::cout << lTempString << "\n"; //debug
		lId++;
	}
	//std::cout << "FNL: " << lTempString << "\n"; //debug
	return std::stoi(lTempString);
}

int64_t MathProc(std::string aExpression) {
	std::cout << "Original: " << aExpression << "\n";
	
	ReplaceString(&aExpression, "mod", "%");
	RemoveSpacesFl(&aExpression);
	
	std::cout << "Processed: " << aExpression << "\n";
	
	uint64_t lId = 0;
	uint64_t lLastPos = 0;
	while(ContainsMultiple(aExpression.c_str(), "()", 2)) {
		while(lId < aExpression.size()) {
			if(aExpression[lId] == '(') {
				if(lId >= aExpression.size()) {
					std::cout << "BAD!\n\n\n\n\n\n\n\n\n\n"; //throw, ( at last pos
				}
				lLastPos = lId;
			}
			elif(aExpression[lId] == ')') {
				int64_t lValue = ParenthesisBlockProc((aExpression.substr(lLastPos + 1, lId - lLastPos - 1))); //lId doesn't include )
				aExpression.erase(aExpression.begin() + lLastPos, aExpression.begin() + lId + 1);
				aExpression.insert(lLastPos, std::to_string(lValue));
				break;
			}
			
			lId++;
		}
		lId = 0;
		lLastPos = 0;
		//std::cout << aExpression << "\n";
	}
	uint64_t lFinal = ParenthesisBlockProc(aExpression);
	return lFinal;
}

//THIS APPLICATION ONLY
void Wrapper() {
	std::cout << "Mathematical processor for the H scripting language.\nHMINT - H Mathematical INTerpreter\nby MegapolisPlayer, 2022. Licensed under CC-BY-4.0\nPlease enter a mathematical expression: \n";
	std::string lExpression;
	std::getline(std::cin, lExpression);
	
	int64_t lValue = MathProc(lExpression);
	
	RemoveSpacesFl(&lExpression);
	
	std::cout << lExpression << " = " << lValue << "\n";
	std::cout << "Press any key and ENTER to continue...\n";
	
	char lBuffer;
	std::cin >> lBuffer;
	exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
	Wrapper();
	//Sample expressions tested
	//32+(7*(5+4)+6)*40
	//4+7*8+5
	//115-114
	//(2+3)*10-8
	//45*(148-146)
	//101 mod 45
	//2 * (10 mod 4)
	//-5 * 10
	//10 * -5
	//-5+6
	//114+115+-78
	//114 - 78 + 45 * (3 - 1) - 4
}
