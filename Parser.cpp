/*
 * Parser.cpp
 *
 *  Created on: 29.12.2013
 *      Author: atamas
 */

#include "Parser.h"
#include <iostream>
using std::pair;
using std::stoi;

Parser::Parser(ifstream & inf):
_tokenizer(inf){
}

void Parser::parseScope(bool const isMainScope, vector<ASTNode const *> & scopeBody, map<string, Function const *> & funcNamespace){
	while(!_tokenizer.eof()){
		Token token;
		_tokenizer >> token;
		switch (token.getType()){

		case Token::NAME:{
			string name = token;
			Token token = _tokenizer.getNextTokenVal();
			if(token == "="){
				_tokenizer.skipToken();
				scopeBody.push_back( parseVarDef(name) );
			} else{
				scopeBody.push_back( parseFuncCall(name) );
			}
			break;
		}

		case Token::PRINT:{
			Token eofToken;
			scopeBody.push_back(new ASTPrint(parseFormula(eofToken), eofToken.getLineNo()));
			eofToken.assertTokenType("\n");
			break;
		}

		case Token::READ:{
			_tokenizer >> token;
			token.assertTokenType(Token::NAME);
			scopeBody.push_back(new ASTRead( token, token.getLineNo() ));
			break;
		}

		case Token::IF:{
			scopeBody.push_back(parseIf(funcNamespace));
			break;
		}

		case Token::WHILE:{
			scopeBody.push_back(parseWhile(funcNamespace));
			break;
		}

		case Token::END:{
			if(isMainScope){
				cerr << "line " << token.getLineNo() <<  ": syntax error" << endl;
				exit(0);
			} else{
				return;
			}
			break;
		}

		case Token::DEF:{
			if(isMainScope){
				parseFuncDef(funcNamespace);
			} else{
				cerr << "line " << token.getLineNo() <<  ": syntax error" << endl;
				exit(0);
			}
			break;
		}

		case Token::RETURN:{
			Token eofToken;
			scopeBody.push_back(new ASTReturn(parseFormula(eofToken), eofToken.getLineNo()));
			eofToken.assertTokenType("\n");
			break;
		}

		case Token::UNCATHEGORIZED_ONE_CHAR_TOKEN:{
			token.assertTokenType("\n");
			break;
		}

		default:
			cerr << "line " << token.getLineNo() <<  ": syntax error" << endl;
			exit(0);
			break;
		}
	}

}

ASTNode * Parser::parseVarDef(string const varName){
	Token eofToken;
	ASTNode * varDef = new ASTVarDef(varName, parseFormula(eofToken), eofToken.getLineNo());
	eofToken.assertTokenType("\n");
	return varDef;
}

ASTNode * Parser::parseIf(map<string, Function const *> &  funcNamespace){
	vector<ASTNode const *> ifBody;
	int lineNo;
	ASTNode * condition = parseCondition(lineNo);
	parseScope(false, ifBody, funcNamespace);
	return new ASTIf(condition, ifBody, lineNo);
}

ASTNode * Parser::parseWhile(map<string, Function const *> & funcNamespace){
	vector<ASTNode const *> whileBody;
	int lineNo;
	ASTNode * condition = parseCondition(lineNo);
	parseScope(false, whileBody, funcNamespace);
	return new ASTWhile(condition, whileBody, lineNo);
}

void Parser::parseFuncDef(map<string, Function const *> & funcNamespace){
	string funcName;
	vector<string> funcParameters;
	Token token;
	_tokenizer >> token;
	token.assertTokenType(Token::NAME);
	funcName = token;
	_tokenizer.assertNextToken("(");
	_tokenizer >> token;
	if( token != ")" ){
		while(true){
			token.assertTokenType(Token::NAME);
			funcParameters.push_back(token);
			_tokenizer >> token;
			if( token == ")" ){
				break;
			}
			token.assertTokenType(",");
			_tokenizer >> token;
		}
	}
	_tokenizer.assertNextToken(":");


	vector<ASTNode const *> body;
	parseScope( false, body, funcNamespace);
	Function * function = new Function(funcParameters, body);
	if( funcNamespace.count(funcName) ){
		cerr << "Multiple function definition " << funcName << endl;
	}
	funcNamespace.insert( pair<string, Function const *>( funcName, function ) );

}

ASTNode * Parser::parseFuncCall(string name){
	Token token;
	Token eofToken;
	vector<ASTNode *> parameters;
	_tokenizer.assertNextToken("(");
	token = _tokenizer.getNextTokenVal();
	if (token != ")"){
		while(true){
			parameters.push_back(parseFormula(eofToken));
			if(eofToken == ")"){
				break;
			} else{
				eofToken.assertTokenType(",");
			}
		}
	} else{
		_tokenizer.skipToken();
	}
	return  new ASTFunc(name, parameters, token.getLineNo());
}


ASTNode * Parser::parseCondition(int & lineNo){
	Token eofToken;
	ASTNode * leftPart = parseFormula(eofToken);
	eofToken.assertTokenType(Token::COMPARISION_OP);
	Token comparator = eofToken;
	ASTNode * rightPart = parseFormula(eofToken);
	eofToken.assertTokenType(":");

	ASTNode * condition;
	if(comparator == "=="){
		condition = new ASTeq(leftPart, rightPart, comparator.getLineNo());
	} else if(comparator == ">="){
		condition = new ASTgeq(leftPart, rightPart, comparator.getLineNo());
	} else if(comparator == "<="){
		condition = new ASTleq(leftPart, rightPart, comparator.getLineNo());
	} else if(comparator == "!="){
		condition = new ASTneq(leftPart, rightPart, comparator.getLineNo());
	} else if(comparator == ">"){
		condition = new ASTgt(leftPart, rightPart, comparator.getLineNo());
	} else if(comparator == "<"){
		condition = new ASTlt(leftPart, rightPart, comparator.getLineNo());
	}
	return condition;
}
ASTNode * Parser::parseFormula( Token & eofToken, ASTNode * leftPart ){
	if(leftPart == 0){
		leftPart = parseSummand(eofToken);
	}

	Token token;
	_tokenizer >> token;
	if(token == "+"){
		return parseFormula(eofToken,new ASTPlus(leftPart, parseSummand(eofToken), token.getLineNo()));
	}
	else if(token == "-"){
		return parseFormula(eofToken,new ASTMinus(leftPart, parseSummand(eofToken), token.getLineNo()));
	}
	else {
		eofToken = token;
		return leftPart;
	}

}

ASTNode * Parser::parseSummand( Token & eofToken, ASTNode * leftPart){
	if(leftPart == 0){
		leftPart = parseFactor(eofToken);
	}

	Token  token = _tokenizer.getNextTokenVal();
	if(token == "*"){
		_tokenizer.skipToken();
		return parseSummand(eofToken, new ASTMult(leftPart, parseFactor(eofToken), token.getLineNo()));
	} else if(token == "/"){
		_tokenizer.skipToken();
		return parseSummand(eofToken, new ASTDev(leftPart, parseFactor(eofToken), token.getLineNo()));
	} else{
		return leftPart;
	}
}

ASTNode * Parser::parseFactor( Token & eofToken ){
	Token token;
	_tokenizer >> token;
	if(token.getType() == Token::NUM){
		return new ASTNum(stoi(token,0,10), token.getLineNo());
	} else if(token.getType() == Token::NAME){
		Token nextToken = _tokenizer.getNextTokenVal();
		if(nextToken != "("){
			return new ASTVar(token, token.getLineNo());
		} else{
			return parseFuncCall(token);
		}
	} else if(token == "("){
		Token eofToken;
		return parseFormula(eofToken);
		eofToken.assertTokenType(")");
	} else if(token == "-"){
		return new ASTUnMinus(parseFactor(eofToken), token.getLineNo());
	} else{
			cerr << "line " << token.getLineNo() <<  ": syntax error" << endl;
			exit(0);
	}
}

Parser::~Parser() {

}

