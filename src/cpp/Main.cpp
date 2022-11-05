//H C++ Interpreter
//by MegapolisPlayer, as part of Fowsoft
//Licensed under CC-BY 4.0

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <algorithm>
#include <vector>
#include <map>

//Utilties by MegapolisPlayer
//https://github.com/MegapolisPlayer/CppUtils
//Licensed under CC-BY-4.0
//Math Interpreter by MegapolisPlayer
//https://github.com/Fowsoft/H-Scripting-Language/blob/fa4b4d2b39bc3ac78e592e76193bb9658823c8b1/src/cpp/MathInterpreter.cpp
//Part of H Interpreter, licensed under CC-BY-4.0

#ifndef elif
#define elif else if
#endif
#ifndef loopover
#define loopover continue
#endif
#ifndef incr
#define incr ++
#endif

namespace Utils {
#ifdef _WIN32
	enum Color {
		COLOR_BLACK = 0,
		COLOR_BLUE,
		COLOR_GREEN,
		COLOR_CYAN,
		COLOR_RED,
		COLOR_PURPLE,
		COLOR_BROWN,
		COLOR_LIGHT_GREY,
		COLOR_GREY,
		COLOR_LIGHT_BLUE,
		COLOR_LIGHT_GREEN,
		COLOR_LIGHT_CYAN,
		COLOR_LIGHT_RED,
		COLOR_PINK,
		COLOR_YELLOW,
		COLOR_WHITE,
		COLOR_DEFAULT
	};
#endif
#ifdef __linux
	enum Color {
		COLOR_BLACK = 0,
		COLOR_RED,
		COLOR_GREEN,
		COLOR_BROWN,
		COLOR_BLUE,
		COLOR_PURPLE,
		COLOR_CYAN,
		COLOR_LIGHT_GREY,
		COLOR_DARK_GREY = 8,
		COLOR_LIGHT_RED,
		COLOR_LIGHT_GREEN,
		COLOR_YELLOW,
		COLOR_LIGHT_BLUE,
		COLOR_PINK,
		COLOR_LIGHT_CYAN,
		COLOR_WHITE,
		COLOR_DEFAULT = 39
	};
#endif
	void SetColor(const Color aForeground, const Color aBackground = COLOR_DEFAULT) {
#ifdef WIN32
		HANDLE lHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		if(aForeground == COLOR_DEFAULT) { SetConsoleTextAttribute(lHandle, 7); }
		else { SetConsoleTextAttribute(lHandle, (aBackground * 16) + aForeground); }
#endif
#ifdef __linux
		std::string lTemp;
		lTemp.append("\x1b[0;");
		if(aForeground == COLOR_DEFAULT) { lTemp.append("39"); }
		else if(aForeground <= 8) { lTemp.append(std::to_string(aForeground + 30)); }
		else { lTemp.append(std::to_string(aForeground + 82)); }
		lTemp += ';';
		if(aBackground == COLOR_DEFAULT) { lTemp.append("49"); }
		else if(aBackground <= 8) { lTemp.append(std::to_string(aBackground + 40)); }
		else { lTemp.append(std::to_string(aBackground + 92)); }
		lTemp += 'm';
		std::cout << lTemp;
#endif
	}
	void ResetColor() {
#ifdef WIN32
		HANDLE lHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(lHandle, 7);
#endif
#ifdef __linux
		std::cout << "\x1b[0;39;49m";
#endif
	}
	class ScopedColor {
	public:
		ScopedColor(const Color aForeground, const Color aBackground = COLOR_DEFAULT) { SetColor(aForeground, aBackground); }
		~ScopedColor() { ResetColor(); }
	};
	
	void Toggle(bool* aBool) {
		if(*aBool == true) { *aBool = false; }
		else { *aBool = true; }
	}
	
	uint64_t StringLength(const char* const aString) {
		uint64_t lId = 0;
		while(aString[lId] != '\0') { lId++; }
		return lId;
	}
	
	bool StringCompare(const char* const aString1, const char* const aString2) {
		uint64_t lString1Len = StringLength(aString1);
		uint64_t lString2Len = StringLength(aString2);
		if(lString1Len != lString2Len) { return false; } //string size not equal
		for(uint64_t lId = 0; lId < lString1Len; lId++) {
			if(aString1[lId] != aString2[lId]) { return false; }
		}
		return true;
	}
	bool IsStringInArray(const char** aArray, const uint64_t aArrayAmount, const char* aString) {
		for(uint64_t lId = 0; lId < aArrayAmount; lId++) {
			if(StringCompare(aArray[lId], aString)) { return true; }
		}
		return false;
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
				if(aString[lId] == aChars[lId2]) { return true; }
			}
		}
		return false;
	}
	
	bool IsMathExpression(const char* aString) {
		for(uint64_t lId = 0; lId < StringLength(aString); lId++) {
			if(aString[lId] != '+' && aString[lId] != '-' && aString[lId] != '*' && aString[lId] != '/' && aString[lId] != '%') {
				if((aString[lId] < 48) || (aString[lId] > 57)) { return false; }
			}
		}
		return true;
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
	
	uint64_t AmountOccurences(const char* aString, const char aChar) {
		uint64_t lCount = 0;
		for(uint64_t lId = 0; lId < StringLength(aString); lId++) {
			if(aString[lId] == aChar) { lCount++; }
		}
		return lCount;
	}
	
	//returns -1 if fails, otherwise 0 or 1
	int8_t TextToBool(const char* aString) {
		uint64_t lStringLength = StringLength(aString);
		if(lStringLength < 4 || lStringLength > 5) { return -1; }
		//checks for true
		if(aString[0] == 't' || aString[0] == 'T') {
			if(aString[1] != 'r' && aString[1] != 'R') { return -1; }
			if(aString[2] != 'u' && aString[2] != 'U') { return -1; }
			if(aString[3] != 'e' && aString[3] != 'E') { return -1; }
			return true;
		}
		//checks for false
		if(aString[0] == 'f' || aString[0] == 'F') {
			if(aString[1] != 'a' && aString[1] != 'A') { return -1; }
			if(aString[2] != 'l' && aString[2] != 'L') { return -1; }
			if(aString[3] != 's' && aString[3] != 'S') { return -1; }
			if(aString[4] != 'e' && aString[4] != 'E') { return -1; }
			return false;
		}
		return -1;
	}
	
	bool IsLower(const char aChar) {
		if(aChar < 97 || aChar > 122) { return false; } return true;
	}
	bool IsUpper(const char aChar) {
		if(aChar < 65 || aChar > 90) { return false; } return true;
	}
	
	char ToLower(const char aChar) {
		if(IsUpper(aChar)) { return aChar + 32; } //relies on ascii, lower 128 letter always implemented
		else { return aChar; }
	}
	
	void ToLower(char* aString) {
		for(uint64_t lId = 0; lId < StringLength(aString); lId++) {
			if(IsUpper(aString[lId])) {
				aString[lId] += 32;
			}
			else { continue; }
		}
	}
	
	char ToUpper(const char aChar) {
		if(IsLower(aChar)) { return aChar - 32; }
		else { return aChar; }
	}
	
	void ToUpper(char* aString) {
		for(uint64_t lId = 0; lId < StringLength(aString); lId++) {
			if(IsLower(aString[lId])) {
				aString[lId] -= 32;
			}
			else { continue; }
		}
	}
	
	[[noreturn]] void PauseExit(int aCode = EXIT_SUCCESS) {
		std::cout << "Press any key and enter to exit.\n";
		char lBuffer;
		std::cin >> lBuffer;
		exit(aCode);
	}
	[[noreturn]] void ErrorExit(const std::string& aMessage) {
		ScopedColor lSC(COLOR_RED);
		std::cout << "[ERROR]: " << aMessage << '\n';
		PauseExit(EXIT_FAILURE);
	}
}
namespace H {
	namespace Info {
		const std::string VendorString = "H Language (Revision 1) C++ Interpreter by Fowsoft (MegapolisPlayer)";
	}
	namespace StdLib {
		//If function designated as void, return value is ignored
		//If void function returns something, this is treated as error and passed to the error handler
		
		std::string Print(std::string aText) {
			for(uint64_t lId = 0; lId < aText.size(); lId++) {
				if(aText[lId] == '\\' && lId < aText.size() - 1) {
					switch(aText[lId + 1]) {
						case('n'): std::putchar('\n'); loopover;
						case('H'): std::cout << H::Info::VendorString; loopover;
						case('\"'): std::putchar('\"'); loopover;
						default: loopover;
					}
				}
				else {std::putchar(aText[lId]); }
			}
			std::putchar('\n');
			return "";
		}
		std::string Input(std::string aPrompt) {
			std::string lTemp;
			if(aPrompt.empty()) {
				Print("Please enter input: ");
			}
			else {
				Print(aPrompt + ": ");
			}
			std::getline(std::cin, lTemp);
			return lTemp;
		}
		
		//C++ Specific
		
		std::string SetColor(std::string aColor) {
			if(aColor == "black")         { Utils::SetColor(Utils::COLOR_BLACK);  }
			elif(aColor == "red")         { Utils::SetColor(Utils::COLOR_RED);    }
			elif(aColor == "green")       { Utils::SetColor(Utils::COLOR_GREEN);  }
			elif(aColor == "brown")       { Utils::SetColor(Utils::COLOR_BROWN);  }
			elif(aColor == "blue")        { Utils::SetColor(Utils::COLOR_BLUE);   }
			elif(aColor == "purple")      { Utils::SetColor(Utils::COLOR_PURPLE); }
			elif(aColor == "cyan")        { Utils::SetColor(Utils::COLOR_CYAN);  }
			elif(aColor == "light_grey")  { Utils::SetColor(Utils::COLOR_LIGHT_GREY);  }
			elif(aColor == "dark_grey")   { Utils::SetColor(Utils::COLOR_DARK_GREY);  }
			elif(aColor == "light_red")   { Utils::SetColor(Utils::COLOR_LIGHT_RED);  }
			elif(aColor == "light_green") { Utils::SetColor(Utils::COLOR_LIGHT_GREEN);  }
			elif(aColor == "yellow")      { Utils::SetColor(Utils::COLOR_YELLOW);  }
			elif(aColor == "light_blue")  { Utils::SetColor(Utils::COLOR_LIGHT_BLUE);  }
			elif(aColor == "pink")        { Utils::SetColor(Utils::COLOR_PINK);  }
			elif(aColor == "light_cyan")  { Utils::SetColor(Utils::COLOR_LIGHT_CYAN);  }
			elif(aColor == "white")       { Utils::SetColor(Utils::COLOR_WHITE);  }
			else { return "Unknown color \"" + aColor + "\""; }
			return "";
		}
		
		std::string ResetColor(std::string aIgnore) {
			Utils::ResetColor(); return "";
		}
		
		std::string HInformation(std::string aIgnore) {
			std::cout << "H Language (Revision 1) by Fowsoft, 2022\n";
			return "";
		}
		
		
		//Internal Standard Library Functions
		
		std::string HExit(std::string aCode) {
			std::string lTemp; //will return Code
			if(Utils::IsNumber(aCode.c_str())) {
				lTemp.append(aCode);
			}
			else {
				lTemp.append(std::to_string(1));
			}
			return aCode;
		}
	}
}
namespace H {
	//Mathematical Operators
	const char Operators[] = {
		'+',
		'-',
		'*',
		'/',
		'%'
	};
	constexpr uint64_t OperatorsAmount = 5;
	
	enum OperatorsEnum {
		OPERATOR_PLUS = 0,
		OPERATOR_MINUS = 1,
		OPERATOR_MULT = 2,
		OPERATOR_DIV = 3,
		OPERATOR_MOD = 4
	};
	
	//Return types
	const char* ReturnTypes[] = {
		"void", "bool", "num", "text" //types
	};
	//Size of ReturnRypes[]
	constexpr uint64_t ReturnTypesAmount = 4;
	
	//Variable AND return types
	const char* VariableTypes[] = {
		"bool", "num", "text" //types
	};
	//Size of VariableTypes[]
	constexpr uint64_t VariableTypesAmount = 3;
	
	//H Keywords
	const char* Keywords[] = {
		"if",
		"else",
		"loop",
		"while",
		"return",
		"exit"
	};
	//Size of Keywords[]
	constexpr uint64_t KeywordsAmount = 6;
	
	//VariableType enum
	enum struct VariableType { HBOOL = 0, HNUM, HTEXT };
	//Converts VariableType enum into string.
	const char* VTypeToString(const VariableType aVType) {
		switch(aVType) {
			case(VariableType::HBOOL): return "HBOOL";
			case(VariableType::HNUM): return "HNUM";
			case(VariableType::HTEXT): return "HTEXT";
			default: return "";
		}
	}
	//Converts string to VType
	//returns 1 if failed
	bool StringToVType(VariableType* aType, const std::string& aString) {
		if(aString == "bool" || aString == "Bool" || aString == "BOOL" || aString == "hbool" || aString == "HBool" || aString == "Hbool" || aString == "HBOOL") {
			*aType = VariableType::HBOOL;
			return false;
		}
		elif(aString == "num"  || aString == "Num"  || aString == "NUM"  || aString == "hnum"  || aString == "HNum"  || aString == "Hnum"  || aString == "HNUM") {
			*aType = VariableType::HNUM;
			return false;
		}
		elif(aString == "text" || aString == "Text" || aString == "TEXT" || aString == "htext" || aString == "HText" || aString == "Htext" || aString == "HTEXT") {
			*aType = VariableType::HTEXT;
			return false;
		}
		else {
			return true;
		} //if argument wrong
	}
	//Variable type
	struct Variable {
		VariableType ValueType;
		std::string Value;
		uint64_t DeclLine; //declaration line, for errors
	};
	
	//FunctionType enum
	enum struct FunctionType { HVOID = 0, HBOOL, HNUM, HTEXT };
	//Converts FunctionType enum into string.
	const char* FTypeToString(const FunctionType aVType) {
		switch(aVType) {
			case(FunctionType::HVOID): return "HVOID";
			case(FunctionType::HBOOL): return "HBOOL";
			case(FunctionType::HNUM): return "HNUM";
			case(FunctionType::HTEXT): return "HTEXT";
			default: return "";
		}
	}
	//Converts string to FType
	//returns 1 if failed
	bool StringToFType(FunctionType* aType, const std::string& aString) {
		if(aString == "void" || aString == "Void" || aString == "VOID" || aString == "hvoid" || aString == "HVoid" || aString == "Hvoid" || aString == "HVOID") {
			*aType = FunctionType::HVOID; return true;
		}
		elif(aString == "bool" || aString == "Bool" || aString == "BOOL" || aString == "hbool" || aString == "HBool" || aString == "Hbool" || aString == "HBOOL") {
			*aType = FunctionType::HBOOL; return true;
		}
		elif(aString == "num"  || aString == "Num"  || aString == "NUM"  || aString == "hnum"  || aString == "HNum"  || aString == "Hnum"  || aString == "HNUM") {
			*aType = FunctionType::HNUM; return true;
		}
		elif(aString == "text" || aString == "Text" || aString == "TEXT" || aString == "htext" || aString == "HText" || aString == "Htext" || aString == "HTEXT") {
			*aType = FunctionType::HTEXT; return true;
		}
		else { return false; } //if argument wrong
	}
	//Function type
	struct Function {
		FunctionType ReturnType;
		std::vector<std::string> Script;
		std::vector<VariableType> Arguments;
		uint64_t ScopeId;
		uint64_t DeclLine; //declaration line, for errors
	};
	
	//Interpreter function type
	struct InterpreterFunction {
		FunctionType ReturnType;
		std::vector<VariableType> Arguments;
		std::string(*FunctionPtr)(const std::string);
		//Interpreter Function Type
		
		InterpreterFunction(const FunctionType aReturnType, const std::vector<VariableType>& aArgs, std::string(*aFunctionPtr)(const std::string)) {
			this->ReturnType = aReturnType;
			this->Arguments = aArgs;
			this->FunctionPtr = aFunctionPtr;
		}
	};
	
	//The main interpreter
	class Interpreter {
	public:
		Interpreter() {
			this->mInterpreterFunctions.insert({"print", InterpreterFunction(FunctionType::HVOID, {VariableType::HTEXT}, H::StdLib::Print)});
			this->mInterpreterFunctions.insert({"input", InterpreterFunction(FunctionType::HTEXT, {VariableType::HTEXT}, H::StdLib::Input)});
			this->mInterpreterFunctions.insert({"exit", InterpreterFunction(FunctionType::HNUM, {VariableType::HNUM}, H::StdLib::HExit)});
		}
		//only disable if compatibility is needed with the java interpreter by @tomas-macha
		void EnableCppExtensions() {
			this->mInterpreterFunctions.insert({"setcolor", InterpreterFunction(FunctionType::HVOID, {VariableType::HNUM}, H::StdLib::SetColor)});
			this->mInterpreterFunctions.insert({"resetcolor", InterpreterFunction(FunctionType::HVOID, {}, H::StdLib::ResetColor)});
			this->mInterpreterFunctions.insert({"hinf", InterpreterFunction(FunctionType::HVOID, {}, H::StdLib::HInformation)});
			
		}
		void AddInterpreterFunction(const std::string& aName,  const InterpreterFunction& aFunction) {
			this->mInterpreterFunctions.insert({aName, aFunction});
		}
		void ListInterpreterFunctions() const {
			if(this->mInterpreterFunctions.empty()) { std::cout << "IFT empty\n"; }
			else {
				for(auto& [lKey, lVal] : this->mInterpreterFunctions) {
					std::cout << "Name: " << lKey << ", Return Type: " << FTypeToString(lVal.ReturnType) << " (int. function)\n";
				}
			}
		}
		void Load(const std::string& aFilename) {
			this->mScript.clear();
			this->mFile.open(aFilename, std::ios::in | std::ios::out | std::ios::binary);
			if(!this->mFile.is_open()) { Utils::ErrorExit("No file " + aFilename + " exists.\n"); }
			char lTemp;
			while(this->mFile.get(lTemp)) { this->mScript += lTemp; }
			this->mFile.close();
		}
		uint64_t Run() {
			uint64_t lId = 0;
			uint64_t lLineId = 1;
			std::string lBuffer;
			struct {
				bool InString = false; 		//in string
				bool InFunction = false; 	//in function definition
				bool InKeyword = false;		//in something like an if statement
			} lRunState;
			
			while(lId < this->mScript.size()) {
				//exit set
				if(this->mExitNow.mSet) {
					std::cout << "Exiting with code " << this->mExitNow.mExitCode << "\n";
					if(!this->mExitNow.mMessage.empty()) {
						std::cout << "Included message: " << this->mExitNow.mMessage << ".\n";
					}
					return this->mExitNow.mExitCode;
				}
				//comments - skip line
				if(this->mScript[lId] == '#') {
					while(this->mScript[lId] != '\n') { lId++; }
					lLineId++;
				}
				else {
					//end of command
					if(this->mScript[lId] == ';') {
						//if last letter also semicolon - extra
						if(this->mScript.size() > 1) {
							if(this->mScript[lId - 1] == ';') {
								this->Warning("Extra semicolon", lLineId);
								lId++;
								loopover;
							}
						}
						//empty or full of spaces and still semicolon - extra
						if(lBuffer.empty() || lBuffer.find_first_not_of(' ') == lBuffer.npos) {
							this->Warning("Extra semicolon", lLineId);
							lId++;
							loopover;
						}
						//function will deal with commands differently
						if(lRunState.InFunction) { this->FuncProc(&lBuffer, ScopePlacement::MID, lLineId); }
						else {
							//check for types, if matching - variable declaration (functions already detected)
							for(uint64_t lId2 = 0; lId2 < VariableTypesAmount; lId2++) {
								if(strcmp(VariableTypes[lId2], lBuffer.substr(0, lBuffer.find(" ")).c_str()) == 0) {
									this->VarAssignProc(&lBuffer, lLineId); lId++; lLineId++; loopover; //assign, loopover
								}
							}
							if(!lBuffer.empty()) {
								this->ComProc(&lBuffer, lLineId);
							}
						}
					}
					//string beginning
					elif(this->mScript[lId] == '\"') {
						Utils::Toggle(&lRunState.InString); lBuffer += '\"';
					}
					//found left parenthesis, entering function or keyword scope
					elif(this->mScript[lId] == '{') {
						if(!lRunState.InString) {
							if(Utils::IsStringInArray(ReturnTypes, ReturnTypesAmount, lBuffer.substr(0, lBuffer.find(" ")).c_str())) { //this line
								//has return types at beginning, function
								lRunState.InFunction = true; this->FuncProc(&lBuffer, ScopePlacement::BEG, lLineId);
							}
							else {
								//has something else, keyword
								lRunState.InKeyword = true; this->KeywordProc(&lBuffer, ScopePlacement::BEG, lLineId);
							}
						}
						else { lBuffer += '{'; }
					}
					//found right parenthesis, exiting function or keyword scope
					elif(this->mScript[lId] == '}') {
						if(!lRunState.InString) {
							if(lRunState.InFunction) { //this line
								this->FuncProc(&lBuffer, ScopePlacement::END, lLineId);
								lRunState.InFunction = false;
							}
							elif(lRunState.InKeyword) {
								this->KeywordProc(&lBuffer, ScopePlacement::END, lLineId);
								lRunState.InKeyword = false;
							}
							else {
								this->Error("Extra right parenthesis", lLineId);
							}
						}
						else { lBuffer += '}'; }
					}
					//end of line, clear gibberish (newlines) and increment
					elif(this->mScript[lId] == '\n') {
						lLineId++; lBuffer.clear();
					}
					//no matches, add to buffer
					else { lBuffer += this->mScript[lId]; }
				}
				lId++;
			}
			
			Utils::ResetColor();
			
			if(!this->mExitNow.mSet) {
				this->Warning("It is recommended to exit using an exit() call", lLineId);
			}
			
			return 0;
		}
		void Print() const {
			std::cout << mScript << "\n";
		}
		void PrintVT() const {
			if(this->mGlobalVariables.empty()) { std::cout << "VT empty\n"; }
			else {for(auto& [lKey, lVal] : this->mGlobalVariables) { std::cout << "Name: " << lKey << ", Value: " << lVal.Value << ", Line: " << lVal.DeclLine << "\n"; } }
		}
		void PrintFT() const {
			if(this->mFunctions.empty()) { std::cout << "FT empty\n"; }
			else {
				for(auto& [lKey, lVal] : this->mFunctions) {
					std::cout << "Name: " << lKey << ", Return Type: " << FTypeToString(lVal.ReturnType) << ", Line: " << lVal.DeclLine << ", Script:\n";
					for(uint64_t lId = 0; lId < lVal.Script.size(); lId++) {
						std::cout << 'G' << lVal.DeclLine + 1 + lId << "/L" << lId + 1 << ": " << lVal.Script[lId];
					}
				}
			}
		}
		
		~Interpreter() {}
	protected:
	private:
		//Variables
		
		std::fstream mFile;
		std::string mScript;
		std::map<std::string, Variable> mGlobalVariables; //stores vars in string to avoid template in map stuff
		std::map<std::string, Function> mFunctions;
		std::map<std::string, InterpreterFunction> mInterpreterFunctions;
		
		struct {
			bool mSet = false;
			uint64_t mExitCode = 0;
			std::string mMessage = "";
		} mExitNow;
		
		enum ScopePlacement { BEG = 0, MID, END };
		
		//MATHS!
		
		int64_t OperatorBlockProc(const int64_t aNumber1, const int64_t aNumber2, const OperatorsEnum aEnum, const uint64_t aLineNo) {
			switch(aEnum) {
				case(OPERATOR_PLUS):
					return aNumber1 + aNumber2;
				case(OPERATOR_MINUS):
					return aNumber1 - aNumber2;
				case(OPERATOR_MULT):
					return aNumber1 * aNumber2;
				case(OPERATOR_DIV):
					if(aNumber2 == 0) {
						this->Error("Dividing by zero", aLineNo);
						return 0;
					}
					return aNumber1 / aNumber2;
				case(OPERATOR_MOD):
					return aNumber1 % aNumber2;
			}
			return 0;
		}
		int64_t ParenthesisBlockProc(const std::string aParenthesisBlock, const uint64_t aLineNo) {
			std::string lTempString = aParenthesisBlock;
			std::string lNumber1;
			std::string lNumber2;
			std::string lInsertTemp;
			
			uint64_t lId = 0;
			uint64_t lId2 = 0;
			
			//first pass -> *, / and mod
			while(Utils::ContainsMultiple(lTempString.c_str(), "*/%", 3) && lId < lTempString.size()) {
				if(lTempString[lId] == '+') {
					lNumber1.clear();
				}
				elif(lTempString[lId] == '-') {
					lNumber1.clear();
				}
				elif(lTempString[lId] == '*') {
					if(lTempString[lId + 1] == '-') {
						lId2 = lId + 2; //skip - operator
						while(!Utils::IsOneOf(lTempString[lId2], Operators, OperatorsAmount) && lId2 < lTempString.size()) {
							lNumber2 += lTempString[lId2];
							lId2++;
						}
						lNumber2 = std::to_string(-std::stoi(lNumber2)); //set to negative
					}
					else {
						lId2 = lId + 1;
						while(!Utils::IsOneOf(lTempString[lId2], Operators, OperatorsAmount) && lId2 < lTempString.size()) {
							lNumber2 += lTempString[lId2];
							lId2++;
						}
					}
					lTempString.erase(lId - lNumber1.size(), lNumber1.size() + lNumber2.size() + 1);
					lInsertTemp = std::to_string(OperatorBlockProc(std::stoi(lNumber1), std::stoi(lNumber2), OPERATOR_MULT, aLineNo));
					lTempString.insert(lId - lNumber1.size(), lInsertTemp);
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
						while(!Utils::IsOneOf(lTempString[lId2], Operators, OperatorsAmount) && lId2 < lTempString.size()) {
							lNumber2 += lTempString[lId2];
							lId2++;
						}
						lNumber2 = std::to_string(-std::stoi(lNumber2)); //set to negative
					}
					else {
						lId2 = lId + 1;
						while(!Utils::IsOneOf(lTempString[lId2], Operators, OperatorsAmount) && lId2 < lTempString.size()) {
							lNumber2 += lTempString[lId2];
							lId2++;
						}
					}
					lTempString.erase(lId - lNumber1.size(), lNumber1.size() + lNumber2.size() + 1);
					lInsertTemp = std::to_string(OperatorBlockProc(std::stoi(lNumber1), std::stoi(lNumber2), OPERATOR_DIV, aLineNo));
					lTempString.insert(lId - lNumber1.size(), lInsertTemp);
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
						while(!Utils::IsOneOf(lTempString[lId2], Operators, OperatorsAmount) && lId2 < lTempString.size()) {
							lNumber2 += lTempString[lId2];
							lId2++;
						}
						lNumber2 = std::to_string(-std::stoi(lNumber2)); //set to negative
					}
					else {
						lId2 = lId + 1;
						while(!Utils::IsOneOf(lTempString[lId2], Operators, OperatorsAmount) && lId2 < lTempString.size()) {
							lNumber2 += lTempString[lId2];
							lId2++;
						}
					}
					lTempString.erase(lId - lNumber1.size(), lNumber1.size() + lNumber2.size() + 1);
					lInsertTemp = std::to_string(OperatorBlockProc(std::stoi(lNumber1), std::stoi(lNumber2), OPERATOR_MOD, aLineNo));
					lTempString.insert(lId - lNumber1.size(), lInsertTemp);
					lId = 0; //to prevent overflow
					lId2 = 0;
					lNumber1.clear();
					lNumber2.clear();
					lInsertTemp.clear();
					lNumber1 += lTempString[lId]; //when loops over adds 1
				}
				else { lNumber1 += lTempString[lId]; }
				lId++;
			}
			
			lNumber1.clear();
			lNumber2.clear();
			lInsertTemp.clear();
			lId = 0;
			lId2 = 0;
			
			//second pass -> + -
			while(Utils::ContainsMultiple(lTempString.c_str(), "+-", 2) && lId < lTempString.size()) {
				if(lTempString[lId] == '+') {
					if(lTempString[lId + 1] == '-') {
						lId2 = lId + 2; //skip - operator
						while(!Utils::IsOneOf(lTempString[lId2], Operators, OperatorsAmount) && lId2 < lTempString.size()) {
							lNumber2 += lTempString[lId2];
							lId2++;
						}
						lNumber2 = std::to_string(-std::stoi(lNumber2)); //set to negative
					}
					else {
						lId2 = lId + 1;
						while(!Utils::IsOneOf(lTempString[lId2], Operators, OperatorsAmount) && lId2 < lTempString.size()) {
							lNumber2 += lTempString[lId2];
							lId2++;
						}
					}
					lTempString.erase(lId - lNumber1.size(), lNumber1.size() + lNumber2.size() + 1);
					lInsertTemp = std::to_string(OperatorBlockProc(std::stoi(lNumber1), std::stoi(lNumber2), OPERATOR_PLUS, aLineNo));
					lTempString.insert(lId - lNumber1.size(), lInsertTemp);
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
						while(!Utils::IsOneOf(lTempString[lId2], Operators, OperatorsAmount) && lId2 < lTempString.size()) {
							lNumber2 += lTempString[lId2];
							lId2++;
						}
						lNumber2 = std::to_string(-std::stoi(lNumber2)); //set to positive
					}
					else {
						lId2 = lId + 1;
						while(!Utils::IsOneOf(lTempString[lId2], Operators, OperatorsAmount) && lId2 < lTempString.size()) {
							lNumber2 += lTempString[lId2];
							lId2++;
						}
					}
					lTempString.erase(lId - lNumber1.size(), lNumber1.size() + lNumber2.size() + 1);
					lInsertTemp = std::to_string(OperatorBlockProc(std::stoi(lNumber1), std::stoi(lNumber2), OPERATOR_MINUS, aLineNo));
					lTempString.insert(lId - lNumber1.size(), lInsertTemp);
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
				lId++;
			}
			return std::stoi(lTempString);
		}
		
		int64_t MathProc(std::string aExpression, const uint64_t aLineNo) {
			for(auto& [lKey, lVal] : this->mGlobalVariables) {
				if(lVal.ValueType == VariableType::HNUM) {
					if(Utils::IsMathExpression(lVal.Value.c_str())) {
						ReplaceString(&aExpression, lKey, lVal.Value);
					}
					else {
						this->Error("Variable " + lKey + " has non-numeric characters.", aLineNo);
					}
				}
				else {
					this->Error("Variable " + lKey + " is not of type num.", aLineNo);
				}
			}
			
			if(!Utils::IsMathExpression(aExpression.c_str())) {
				this->Error("Non-mathematical characters detected", aLineNo); return 0;
			}
			
			ReplaceString(&aExpression, "mod", "%");
			RemoveSpacesFl(&aExpression);
			
			uint64_t lId = 0;
			uint64_t lLastPos = 0;
			while(Utils::ContainsMultiple(aExpression.c_str(), "()", 2)) {
				while(lId < aExpression.size()) {
					if(aExpression[lId] == '(') {
						if(lId >= aExpression.size()) {
							this->Error("Opening parenthesis at end of expression", aLineNo); return 0;
						}
						lLastPos = lId;
					}
					elif(aExpression[lId] == ')') {
						if(lId == 0) {
							this->Error("Closing parenthesis at beginning of expression", aLineNo); return 0;
						}
						int64_t lValue = ParenthesisBlockProc((aExpression.substr(lLastPos + 1, lId - lLastPos - 1)), aLineNo); //lId doesn't include )
						aExpression.erase(aExpression.begin() + lLastPos, aExpression.begin() + lId + 1);
						aExpression.insert(lLastPos, std::to_string(lValue));
						break;
					}
					
					lId++;
				}
				lId = 0;
				lLastPos = 0;
			}
			uint64_t lFinal = ParenthesisBlockProc(aExpression, aLineNo);
			return lFinal;
		}
		
		//Variable Processing
		
		void VarAssignProc(std::string* aVarAssign, const uint64_t aLineNo) {
			Variable lTemp;
			lTemp.DeclLine = aLineNo; //set declaration line
			std::string lTempName;
			std::string lTempType = aVarAssign->substr(0, aVarAssign->find(" "));
			
			StringToVType(&lTemp.ValueType, lTempType); //convert type string to enum
			aVarAssign->erase(aVarAssign->begin(), aVarAssign->begin() + lTempType.size()); //delete type (first word) of string
			this->RemoveSpacesIgnr(aVarAssign); //remove spaces, ignore strings
			//find name
			uint64_t lId = 0;
			while((*aVarAssign)[lId] != '=') {
				lTempName += (*aVarAssign)[lId];
				lId++;
			}
			this->ValidateName(lTempName, aLineNo); //throws if already exists
			aVarAssign->erase(aVarAssign->begin(), aVarAssign->begin() + lTempName.size() + 1); //delete name (second word, first deleted) of string (+1 for =)
			
			//process value after =
			switch(lTemp.ValueType) {
				case(VariableType::HBOOL):
					if(Utils::TextToBool(aVarAssign->c_str()) == -1) {
						this->Error("Invalid value " + *aVarAssign + " for variable of type bool", aLineNo); //add to docs: IMPORTANT - check that you havent added any quotes
					}
					else {
						for(uint64_t lId2 = 0; lId2 < aVarAssign->size(); lId2++) {
							(*aVarAssign)[lId2] = Utils::ToLower((*aVarAssign)[lId2]);
						}
						lTemp.Value = *aVarAssign;
					}
					break;
				case(VariableType::HNUM):
					lTemp.Value = std::to_string(MathProc(*aVarAssign, aLineNo));
					break;
				case(VariableType::HTEXT):
					ConcaternateString(aVarAssign, aLineNo, this->mGlobalVariables);
					if(Utils::AmountOccurences(aVarAssign->c_str(), '\"') >= 2) {
						lTemp.Value = aVarAssign->substr(1, (aVarAssign->size() - 1) - 1); //1 at beginning to skip first ", at end first -1 to get last el., second to remove "
					}
					else { this->Error("String doesnt contain double-quotes", aLineNo); }
					break;
			}
			
			aVarAssign->clear();
			
			this->mGlobalVariables.insert({lTempName, lTemp}); //push full, if exists doesnt insert, detected earlier
		}
		void VarChangeProc(std::string* aVarChange, const uint64_t aLineNo) {
			std::string lTempName = aVarChange->substr(0, aVarChange->find("="));
			RemoveSpacesFl(&lTempName);
			Variable lTemp = this->mGlobalVariables.at(lTempName);
			lTemp.DeclLine = aLineNo;
			
			aVarChange->erase(aVarChange->begin(), aVarChange->begin() + lTempName.size() + 1); //+1 for equals
			
			switch(lTemp.ValueType) {
				case(VariableType::HBOOL):
					if(Utils::TextToBool(aVarChange->c_str()) == -1) {
						this->Error("Invalid value " + *aVarChange + " for variable of type bool", aLineNo); //add to docs: IMPORTANT - check that you havent added any quotes
					}
					else {
						for(uint64_t lId2 = 0; lId2 < aVarChange->size(); lId2++) {
							(*aVarChange)[lId2] = Utils::ToLower((*aVarChange)[lId2]);
						}
						lTemp.Value = *aVarChange;
					}
					break;
				case(VariableType::HNUM):
					lTemp.Value = std::to_string(MathProc(*aVarChange, aLineNo));
					break;
				case(VariableType::HTEXT):
					if(Utils::AmountOccurences(aVarChange->c_str(), '\"') >= 2) {
						lTemp.Value = aVarChange->substr(1, (aVarChange->size() - 1) - 1); //1 at beginning to skip first ", at end first -1 to get last el., second to remove "
					}
					else { this->Error("String doesnt contain double-quotes", aLineNo); }
					break;
			}
			
			this->mGlobalVariables.insert_or_assign(lTempName, lTemp); //update
		}
		
		//Function-related
		
		Variable LocalVarAssignProc(std::string* aVarAssign, const uint64_t aLine) {
			Variable lTemp;
			std::string lTempName;
			std::string lTempType = aVarAssign->substr(0, aVarAssign->find(" "));
			
			lTemp.DeclLine = aLine; //set declaration line
			StringToVType(&lTemp.ValueType, lTempType); //convert type string to enum
			aVarAssign->erase(aVarAssign->begin(), aVarAssign->begin() + lTempType.size()); //delete type (first word) of string
			this->RemoveSpacesIgnr(aVarAssign); //remove spaces
			//find name
			uint64_t lId = 0;
			while((*aVarAssign)[lId] != '=') {
				lTempName += (*aVarAssign)[lId];
				lId++;
			}
			this->RemoveSpacesFl(&lTempName); //remove spaces - ALL of them
			this->ValidateName(lTempName, aLine); //throws if already exists
			aVarAssign->erase(aVarAssign->begin(), aVarAssign->begin() + lTempName.size() + 1); //delete name (second word, first deleted) of string (+1 for =)
			
			//process value after =
			switch(lTemp.ValueType) {
				case(VariableType::HBOOL):
					if(Utils::TextToBool(aVarAssign->c_str()) != -1) { lTemp.Value = *aVarAssign; }
					else { this->Error("Invalid value " + *aVarAssign + " for variable of type bool", aLine); }
					break;
				case(VariableType::HNUM):
					lTemp.Value = std::to_string(MathProc(*aVarAssign, aLine));
					break;
				case(VariableType::HTEXT):
					if(Utils::AmountOccurences(aVarAssign->c_str(), '\"') >= 2) {
						lTemp.Value = aVarAssign->substr(1, (aVarAssign->back() - 1));
					}
					else { this->Error("String doesnt contain double-quotes", aLine); }
					break;
			}
			
			aVarAssign->clear();
			
			return lTemp;
		}
		void LocalVarChangeProc(std::string* aVarChange) {
		
		}
		
		void FuncCallProc(std::string* aFuncCall, const uint64_t aLineNo) {
			std::string lTempName = aFuncCall->substr(0, std::min(aFuncCall->find(" "), aFuncCall->find("(")));
			Function lFunc = this->mFunctions.at(lTempName);
			static std::vector<Variable> lVariables;
			
			for(uint64_t lId = 0; lId < lFunc.Script.size(); lId++) {
				std::string lTempFuncCallName = lFunc.Script[lId].substr(0, std::min(lFunc.Script[lId].find(" "), lFunc.Script[lId].find("(")));
				std::cout << "FCL: " << lFunc.Script[lId] << "\n";
				if(lTempFuncCallName == lTempName) {
					this->Error("Recursive calls are not allowed, as they might result in a hang", aLineNo);
					return;
				}
				this->ComProc(&lFunc.Script[lId], aLineNo);
			}
			
			lVariables.clear();
		}
		
		void FuncProc(std::string* aFuncDecl, const ScopePlacement aPlace, const uint64_t lLine) {
			static Function lTemp;
			static std::string lTempName; //name
			//add later inprocess
			
			std::string lTempType = aFuncDecl->substr(0, aFuncDecl->find(" "));
			
			if(aPlace == ScopePlacement::BEG) {
				lTemp.DeclLine = lLine; //cpy into declline, for errhandling
				
				StringToFType(&lTemp.ReturnType, lTempType); //no need to check return, already passed types[] check
				aFuncDecl->erase(aFuncDecl->begin(), aFuncDecl->begin() + lTempType.size()); //delete type (first word) of string
				uint64_t lId = 0;
				while((*aFuncDecl)[lId] != '(') {
					lTempName += (*aFuncDecl)[lId];
					lId++;
				}
				this->RemoveSpacesFl(&lTempName);
				
				this->ValidateName(lTempName, lLine);
				
				return;
			}
			elif(aPlace == ScopePlacement::END) {
				this->mFunctions.insert({lTempName, lTemp}); //push full, if exists doesnt insert, fine for now, use lInProcess variable later
				lTempType.clear(); 				//clear return type
				lTempName.clear(); 				//clear function name
				lTemp.Script.clear(); 			//clear script buffer
				lTemp.Arguments.clear();		//clear list of arguments
				return;
			}
			
			RemoveSpacesIgnr(aFuncDecl);
			lTemp.Script.push_back(*aFuncDecl);
			lTemp.Script.back() += '\n';
			
			aFuncDecl->clear();
		}
		
		void InterpreterFuncCallProc(std::string* aFuncCall, const uint64_t lLine) {
			std::string lArgs;
			std::string lName;
			
			uint64_t lId = 0;
			while(!((*aFuncCall)[lId] == '(')) {
				lName += (*aFuncCall)[lId];
				lId++;
			}
			lId++;
			while(!((*aFuncCall)[lId] == ')')) {
				lArgs += (*aFuncCall)[lId];
				lId++;
			}
			
			RemoveSpacesIgnr(&lArgs);
			
			lId = 0;
			while(lArgs[lId] != ',' && lId < lArgs.size()) {
				lId++;
			}
			
			//std::vector
			
			InterpreterFunction lFunction = this->mInterpreterFunctions.at(lName);
			std::string lReturnValue = lFunction.FunctionPtr(lArgs);
			
			switch(lFunction.ReturnType) {
				case(FunctionType::HVOID):
					if(!lReturnValue.empty()) {
						this->Error(lReturnValue, lLine);
					}
					break;
				case(FunctionType::HBOOL):
				
					break;
				case(FunctionType::HNUM):
				
					break;
				case(FunctionType::HTEXT):
				
					break;
			}
		}
		
		void InterpreterFuncCallProcVarAssign() {
		
		}
		
		//Command processing (function calls, etc.)
		
		void ComProc(std::string* aCom, const uint64_t aLine) {
			std::string lTemp;
			
			this->RemoveSpacesIgnr(aCom);
			
			uint64_t lId = 0;
			while(!Utils::IsOneOf((*aCom)[lId], " (=", 3) && lId < aCom->size()) {
				lTemp += (*aCom)[lId];
				lId++;
			}
			
			if(Utils::IsStringInArray(Keywords, KeywordsAmount, lTemp.c_str())) {
				std::cout << "KEYWORD INST: " << *aCom << "\n";
				this->KeywordProc(aCom, ScopePlacement::BEG, aLine);
			}
			elif(this->mGlobalVariables.count(lTemp)) {
				this->VarChangeProc(aCom, aLine);
			}
			elif(this->mFunctions.count(lTemp)) {
				this->FuncCallProc(aCom, aLine);
			}
			elif(this->mInterpreterFunctions.count(lTemp)) {
				this->InterpreterFuncCallProc(aCom, aLine);
			}
			else {
				this->Error("Invalid command " + lTemp, aLine);
			}
		}
		
		void KeywordProc(std::string* aCom, const ScopePlacement aPlace, const uint64_t lLine) {
			//static bool lCondition = false; //if false ignore when ScopePlacement::MID
			static std::vector<Variable> lVariables;
			//process condition if beg
			//if false, find end of condition, ignore else
			//at end delete
			lVariables.clear();
		}
		
		//Error Handling
		
		void SetExit(const uint64_t& aCode, const std::string& aMessage) {
			this->mExitNow.mSet = true;
			this->mExitNow.mExitCode = aCode;
			this->mExitNow.mMessage = aMessage;
		}
		
		void Warning(const std::string& aMessage, const uint64_t& aLine) {
			Utils::ScopedColor lSC(Utils::COLOR_YELLOW);
			std::cout << "H/Warning: " << aMessage << " on line " << aLine << ".\n";
		}
		void Error(const std::string& aMessage, const uint64_t& aLine) {
			Utils::ScopedColor lSC(Utils::COLOR_RED);
			std::cout << "H/Error: " << aMessage << " on line " << aLine << ".\n";
			this->SetExit(1, aMessage);
		}
		
		//Internal Utilities
		
		//Internal because ignores spaces in "strings"
		void RemoveSpacesIgnr(std::string* aString) {
			uint64_t lId = 0;
			bool lInString = false;
			while(lId < aString->size()) {
				if((*aString)[lId] == '\"') {
					Utils::Toggle(&lInString);
				}
				elif((*aString)[lId] == ' ' && !lInString) {
					aString->erase(aString->begin() + lId); loopover;
				}
				lId++;
			}
		}
		//Internal because uses std::string, cant add to CppUtils
		void RemoveSpacesFl(std::string* aString) {
			uint64_t lId = 0;
			while(lId < aString->size()) {
				if((*aString)[lId] == ' ') {
					aString->erase(aString->begin() + lId); loopover;
				}
				lId++;
			}
		}
		//Internal because uses std::string, cant add to CppUtils
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
		
		//Just internal
		void ConcaternateString(std::string* aString, const uint64_t aLineNo, const std::vector<Variable> aVariables) {
			RemoveSpacesIgnr(aString);
			bool lTextPart = false;
			std::string lTemp;
			uint64_t lId = 0;
			while(lId < aString->size()) {
				if((*aString)[lId] == '\"') {
					Utils::Toggle(&lTextPart);
					if(lTextPart == false) {
						uint64_t lId2 = 0;
						while((*aString)[lId2] != '"') {
							lTemp += (*aString)[lId2];
							lId2++;
						}
						this->ValidateName(lTemp, aLineNo);
						
					}
				}
				lId++;
			}
		}
		
		//Just internal
		void ConcaternateString(std::string* aString, const uint64_t aLineNo, const std::map<std::string, Variable> aVariables) {
			RemoveSpacesIgnr(aString);
			bool lTextPart = false;
			std::string lTemp;
			uint64_t lId = 0;
			while(lId < aString->size()) {
				if((*aString)[lId] == '\"') {
					Utils::Toggle(&lTextPart);
					uint64_t lId2 = 0;
					if(lTemp.empty()) { //empty only if new
						while((*aString)[lId2] != '"') {
							lTemp += (*aString)[lId2];
							lId2++;
						}
					}
					if(aVariables.count(lTemp)) {
						aString->erase(lId, lId2 + 1);
						aString->insert(lId, aVariables.at(lTemp).Value);
					}
					else {
						this->Error("No variable " + lTemp, aLineNo);
					}
					lTemp.clear();
				}
				elif(!lTextPart) {
					lTemp += (*aString)[lId];
					lId++;
				}
				lId++;
			}
		}
		
		//Validates name and throws
		void ValidateName(const std::string& aString, const uint64_t aLine) {
			//check if function name is keyword - error
			if(Utils::IsStringInArray(Keywords, KeywordsAmount, aString.c_str())) {
				this->Error("Redefinition: " + aString + " is a keyword", aLine);
			}
			//check if variable with same name exists - error
			elif(this->mGlobalVariables.count(aString)) {
				this->Error("Redefinition: " + aString + " is a variable", aLine);
			}
			//check if a standard library function with the same name exists - error
			elif(this->mInterpreterFunctions.count(aString)) {
				this->Error("Redefinition: " + aString + " is a standard library function", aLine);
			}
			//check if function already exists - error
			elif(this->mFunctions.count(aString)) {
				this->Error("Redefinition: " + aString + " is a function", aLine);
			}
		}
	};
}
namespace HShell {
	struct Arguments {
		bool Debug = false; //dumps at end
		bool NoCppExtensions = false; //cpp extensions
		bool File = false; //skips file loading
		std::string Filename; //filearg trail
		
		void FindArguments(int argc, char** argv) {
			uint64_t lId = 1; //skipping first
			while(lId < argc) {
				if(strcmp(argv[lId], "-debug") == 0) {
					this->Debug = true;
				}
				if(strcmp(argv[lId], "-nocppext") == 0) {
					this->NoCppExtensions = true;
				}
				if(strcmp(argv[lId], "-file") == 0 && lId < argc - 1) {
					this->File = true;
					this->Filename = argv[lId + 1];
					lId++;
				}
				lId++;
			}
		}
	};
	//Shell Handler
	class Shell {
	public:
		Shell() { Utils::ErrorExit("DEV: Shell uninitialized properly"); }
		Shell(H::Interpreter* aInterpreterPtr) {
			std::cout << H::Info::VendorString << "\n";
			this->mInterpreterPtr = aInterpreterPtr;
		}
		void PrintArguments(const Arguments* aArgs) const {
			Utils::ScopedColor lSC(Utils::COLOR_YELLOW);
			std::cout << "ARGUMENT INFO:\n\n";
			
			if(aArgs->Debug) { std::cout << "DEBUG MODE (-debug) ENABLED\n"; }
			else { std::cout << "DEBUG MODE (-debug) DISABLED\n"; }
			
			if(aArgs->File) { std::cout << "FILE SKIP (-file <name>) ENABLED\n"; }
			else { std::cout << "FILE SKIP (-file <name>) DISABLED\n"; }
			
			if(aArgs->NoCppExtensions) { std::cout << "CPP INTERPRETER EXTENSIONS (-nocppext) DISABLED\n"; }
			else { std::cout << "CPP INTERPRETER EXTENSIONS (-nocppext) ENABLED"; }
			
			std::putchar('\n');
		}
		void Run(Arguments* aArgs) const {
			PrintArguments(aArgs);
			//shell art
			{
				Utils::ScopedColor lSC(Utils::COLOR_RED);
				std::cout <<
				          "\n"
				          "# #   #   ### #   # ### # # ### ### ###          #\n"
				          "# #   #   # # ##  # #   # # # # #   #           ##\n"
				          "###   #   ### # # # # # # # ### # # ###    # # # #\n"
				          "# #   #   # # #  ## # # # # # # # # #      # #   #\n"
				          "# #   ### # # #   # ### ### # # ### ###     #    #\n"
				          "\n";
				          
			}
			if(!aArgs->File) {
				std::cout << H::Info::VendorString <<
				          "\nH C++ Shell v1 by Fowsoft (MegapolisPlayer)\n"
				          "All parts of the H C++ Interpreter are licensed under CC-BY-4.0.\n\n"
				          "Please enter script name (without extension): ";
				std::cin >> aArgs->Filename;
			}
			if(aArgs->Debug) {
				Utils::SetColor(Utils::COLOR_BLUE);
				std::cout << "List of interpreter functions:\n";
				Utils::SetColor(Utils::COLOR_LIGHT_BLUE);
				mInterpreterPtr->ListInterpreterFunctions();
				Utils::ResetColor();
			}
			mInterpreterPtr->Load(aArgs->Filename + ".hsc");
			mInterpreterPtr->Run();
			if(aArgs->Debug) {
				Utils::SetColor(Utils::COLOR_BLUE);
				std::cout << "Debug info for script " << aArgs->Filename << ".hsc:\n"
				          "Variable Table (VT):\n";
				Utils::SetColor(Utils::COLOR_LIGHT_BLUE);
				mInterpreterPtr->PrintVT();
				Utils::SetColor(Utils::COLOR_BLUE);
				std::cout << "Function Table (FT):\n";
				Utils::SetColor(Utils::COLOR_LIGHT_BLUE);
				mInterpreterPtr->PrintFT();
				Utils::ResetColor();
			}
		}
	protected:
	private:
		H::Interpreter* mInterpreterPtr;
	};
}

int main(int argc, char** argv) {
	HShell::Arguments lArgs;
	H::Interpreter lInt;
	HShell::Shell lShell(&lInt);
	
	lArgs.FindArguments(argc, argv);
	if(!lArgs.NoCppExtensions) { lInt.EnableCppExtensions(); }
	lShell.Run(&lArgs);
	
	Utils::PauseExit();
}
