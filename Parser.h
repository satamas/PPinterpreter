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
#include <vector>
#include <map>
#include "ASTNode.h"
#include "Function.h"
#include "Tokenizer.h"
using std::ifstream;
using std::map;
using std::string;
using std::vector;

class Parser {
public:
	Parser(ifstream & inf);

	vector<ASTNode const *> parse(map<string, Function const *> & funcNameSpace){
		vector<ASTNode const *> mainScope;
		parseScope(true, mainScope, funcNameSpace);
		return mainScope;
	}

	virtual ~Parser();
private:
	void parseScope(bool const isMainScope, vector<ASTNode const *> & scopeBody, map<string, Function const *> & funcNameSpace);
	ASTNode * parseVarDef(string const varName);
	ASTNode * parseIf(map<string, Function const *> & funcNamespace);
	ASTNode * parseWhile(map<string, Function const *> & funcNamespace);
	ASTNode * parseCondition(int & lineNo);
	void parseFuncDef(map<string, Function const *> & funcNamespace);
	ASTNode * parseFuncCall(string name);
	ASTNode * parseSummand( Token & eofToken, ASTNode * leftPart = 0 );
	ASTNode * parseFactor( Token & eofToken );
	ASTNode * parseFormula( Token & eofToken, ASTNode * leftPart = 0  );//eof- end of formula
	Tokenizer _tokenizer;
};

#endif /* PARSER_H_ */
