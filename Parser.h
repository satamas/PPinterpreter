/*
 * Parser.h
 *
 *  Created on: 29.12.2013
 *      Author: atamas
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include "Function.h"
#include "Tokenizer.h"
#include "ASTNode.h"
#include "Commands.h"

class Parser {
public:
	Parser(std::ifstream & inf, std::map<std::string, Function> * funcNameSpace);

	command * getCommand(unsigned int pos){
		return _mainScope[pos];
	}

	int getSize(){
		return _mainScope.size();
	}

	virtual ~Parser();
private:
	void parseScope(bool isMainScope, std::vector<command *> & scopeBody, std::map<std::string, Function> * funcNameSpace = 0);
	void parseFuncDefinition(std::map<std::string, Function> * funcNameSpace);
	void parseLogicScope(std::vector<command *> & scopeBodybool, bool isWhile);
	VarDef * parseVarDef();
	ASTNode * parseSummand(char eofSymbol = 0, Token::Type * eofTokenType = 0);
	ASTNode * parseFormula(char eofSymbol = 0, Token::Type * eofTokenType = 0);//eof- end of formula
	Tokenizer _tokenizer;
	std::vector<command *> _mainScope;
};

#endif /* PARSER_H_ */
