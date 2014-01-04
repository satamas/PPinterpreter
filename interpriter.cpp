//============================================================================
// Name        : interpriter.cpp
// Author      : atamas
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <iostream>
#include "Parser.h"
#include "Function.h"


int main (){
	std::ifstream inf("program");
	std::map<std::string, int> varNamespace;
	std::map<std::string, Function> funcNamespace;
	Parser parser(inf, &funcNamespace);

	int pos = 0;
	while(pos < parser.getSize()){
		bool returnFlag;
		command * c = parser.getCommand(pos);
		c->execute(varNamespace, funcNamespace, pos, returnFlag);
		++pos;
	}

}
