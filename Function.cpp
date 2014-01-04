/*
 * Function.cpp
 *
 *  Created on: 03.01.2014
 *      Author: atamas
 */

#include "Function.h"
#include "Commands.h"


Function::Function(std::vector <std::string> parNames, std::vector<command *> body, std::map<std::string, Function> * funcNamespace):
_parNames(parNames),
_body(body){
		_funcNamespace = funcNamespace;
}

int Function::eval(std::vector<int> & parameters){
	std::map<std::string, int> varNamespace;
	int pos = 0;
	for(unsigned int i = 0; i < parameters.size(); ++i){
		varNamespace.insert( std::pair<std::string, int>(_parNames[i], parameters[i]) );
	}
	while(pos < (int)_body.size()){
		command * c = _body[pos];
		bool returnFlag;
		int val;
		val = c->execute(varNamespace, *_funcNamespace, pos, returnFlag);
		if(returnFlag){
			return val;
		}
		++pos;
	}
	return 0;
}

Function::~Function() {

}

