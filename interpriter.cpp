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
#include "Evaluator.h"
using std::string;
using std::map;
using std::ifstream;

int main (int argc, char **argv){
	ifstream inf(argv[1]);
	map<string, int> varNamespace;
	map<string, Function const *> funcNamespace;
	Parser parser(inf);

	vector<ASTNode const *> mainScope = parser.parse(funcNamespace);

	Evaluator * evaluator = new Evaluator(varNamespace, funcNamespace);

	for(unsigned int pos = 0; pos < mainScope.size(); ++pos){
		mainScope[pos]->visit(evaluator);
	}

}
