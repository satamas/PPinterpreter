/*
 * Parser.cpp
 *
 *  Created on: 29.12.2013
 *      Author: atamas
 */

#include "Parser.h"

Parser::Parser(std::ifstream & inf, std::map<std::string, Function> * funcNameSpace):
_tokenizer(inf),
_mainScope(){
	parseScope(true, _mainScope, funcNameSpace);
}

void Parser::parseFuncDefinition(std::map<std::string, Function> * funcNamespace){
	std::string funcName;
	std::vector<std::string> funcParameters;
	Token token;
	_tokenizer >> token;
	if(token.getType() == Token::NAME){
		funcName = token.getToken();
		_tokenizer >> token;
		if(token.getToken() == "("){
			_tokenizer >> token;
			if(token.getToken() != ")"){
				while(true){
					if(token.getType() == Token::NAME){
						funcParameters.push_back(token.getToken());
						_tokenizer >> token;
						if(token.getToken() == ")"){
							break;
						}
						if(token.getToken() != ","){
							std::cout << "Syntax error";
							std::exit(0);
						}
						_tokenizer >> token;
					}else{
						std::cout << "Syntax error";
						std::exit(0);
					}
				}
			}
			_tokenizer >> token;
			if(token.getToken() != ":"){
				std::cout << "Syntax error";
				std::exit(0);
			}
		}else{
			std::cout << "Syntax error";
			std::exit(0);
		}
	}else{
		std::cout << "Syntax error";
		std::exit(0);
	}
	std::vector<command*> body;
	parseScope(false, body);
	if(funcNamespace->count(funcName)){
		std::cout << "Multiple function definition" << std::endl;
	}else{
		funcNamespace->insert( std::pair<std::string, Function>( funcName,Function(funcParameters, body, funcNamespace) ) );
	}

}

void Parser::parseScope(bool isMainScope, std::vector<command *> & scopeBody, std::map<std::string, Function> * funcNameSpace){
	while(!_tokenizer.eof()){
		Token token;
		_tokenizer >> token;
		switch (token.getType()){

		case Token::NAME:{
			std::string var = token.getToken();
			_tokenizer >> token;
			if(token.getToken() == "="){
				VarDef * varDef = new VarDef(var, parseFormula('\n'));
				scopeBody.push_back(varDef);
			} else{
				std::cout << "Syntax error";
				std::exit(0);
			}
			break;
		}

		case Token::COMMAND:{
			if(token.getToken() == "print"){
				commandPrint * print = new commandPrint(parseFormula('\n'));
				scopeBody.push_back(print);
			}

			if(token.getToken() == "read"){
				Token token;
				_tokenizer >> token;
				commandRead * read;

				if(token.getType() == Token::NAME){
					read = new commandRead(token.getToken());
				} else{
					std::cout << "Syntax error";
					std::exit(0);
				}

				_tokenizer >> token;
				if(token.getToken() != "\n"){
					std::cout << "Syntax error";
					std::exit(0);
				}

				scopeBody.push_back(read);

			}

			if(token.getToken() == "if"){
				parseLogicScope(scopeBody, false);
			}

			if(token.getToken() == "while"){
				parseLogicScope(scopeBody, true);
			}

			if(token.getToken() == "end"){
				if(isMainScope){
					std::cout << "Syntax error";
					std::exit(0);
				} else{
					return;
				}
			}

			if(token.getToken() == "def"){
				if(isMainScope){
					parseFuncDefinition(funcNameSpace);
				} else{
					std::cout << "Syntax error";
					std::exit(0);
				}


			}

			if(token.getToken() == "return"){
				scopeBody.push_back(new commandReturn(parseFormula('\n')));
			}

	//			if(token.getToken() == "read"){
	//				Token token;
	//				_tokenizer >> token;
	//				if(token.getType() == Token::NAME){
	//					Read print(parseFormula);
	//				}
	//			}
			break;
		}

		case Token::UNCATHEGORIZED_ONE_CHAR_TOKEN:{
			if (!(token.getToken() == "\n")){
				std::cout << "Syntax error";
				std::exit(0);
			}
			break;
		}

		default:
			std::cout << "Syntax error";
			std::exit(0);
		break;
		}
	}

	if(!isMainScope){
		std::cout << "Syntax error";
		std::exit(0);
	}

}

void Parser::parseLogicScope(std::vector<command *> & scopeBody, bool isWhile){
	int ifPos = scopeBody.size();
	Token::Type type = Token::COMPARISION_OP;
	ASTNode * leftPart = parseFormula(0,&type);
	Token token;
	_tokenizer >> token;
	if(token.getType() == Token::COMPARISION_OP){
		std::string comparator = token.getToken();
	} else{
		std::cout << "Syntax error";
		std::exit(0);
	}
	ASTNode * rightPart = parseFormula(':');
	parseScope(false, scopeBody);
	if(isWhile){
		scopeBody.push_back(new commandGoTo(ifPos-1));
		command * c = new commandIf(leftPart, token.getToken(), rightPart, scopeBody.size());
		scopeBody.insert(scopeBody.begin() + ifPos + 1, c);
	} else{
		command * c = new commandIf(leftPart, token.getToken(), rightPart, scopeBody.size());
		scopeBody.insert(scopeBody.begin() + ifPos, c);
	}

}

//VarDef * Parser::parseVarDef(){
//	Token token;
//	_tokenizer >> token;
//	if(token.getType() == Token::NAME){
//		std::string var = token.getToken();
//		_tokenizer >> token;
//		if(token.getToken() == "="){
//			VarDef * varDef = new VarDef(var, parseFormula('\n'));
//			return varDef;
//		} else{
//			std::cout << "Syntax error";
//			std::exit(0);
//		}
//	} else{
//		std::cout << "Syntax error";
//		std::exit(0);
//	}
//}

ASTNode * Parser::parseSummand(char eofSymbol, Token::Type * eofTokenType){
	Token token;
	_tokenizer >> token;

	ASTNode * num = 0;
	if(token.getType() == Token::NUM){
		num = new ASTNum(std::stoi(token.getToken(),0,10));
	}
	else if(token.getType() == Token::NAME){
		Token nextToken;
		_tokenizer >> nextToken;
		if(nextToken.getToken() != "("){
			_tokenizer << nextToken;
			num = new ASTVar(token.getToken());
		} else{
			std::vector<ASTNode *> parameters;
			_tokenizer >> nextToken;
			while(nextToken.getToken() != ")"){
				_tokenizer << nextToken;
				parameters.push_back(parseFormula(','));
				_tokenizer >> nextToken;
			}
			num = new ASTFunc(token.getToken(), parameters);
		}
	}
	else if(token.getToken() == "("){
		num = parseFormula(')');
	}
	else{
			std::cout << "Syntax error";
			std::exit(0);
	}

	_tokenizer >> token;
	if(eofTokenType != 0){
		if(token.getType() == *eofTokenType || token.getToken() == "+" || token.getToken() == "-"){
			_tokenizer << token;
			return num;
		}
	} else{
		if(token.getToken() == std::string(1,eofSymbol) || token.getToken() == "+" || token.getToken() == "-"){
			_tokenizer << token;
			return num;
		}
	}
	if(token.getToken() == "*"){
		ASTMult * mult = new ASTMult(num, parseSummand(eofSymbol, eofTokenType));
		return mult;
	}
	else if(token.getToken() == "/"){
		ASTDev * dev = new ASTDev(num, parseSummand(eofSymbol, eofTokenType) );
		return dev;
	}
	else {
		std::cout << "Syntax error";
		std::exit(0);
	}

}

ASTNode * Parser::parseFormula(char eofSymbol, Token::Type * eofTokenType){
	ASTNode * summand = parseSummand(eofSymbol, eofTokenType);
	Token token;
	_tokenizer >> token;
	if(eofTokenType == 0){
		if(token.getToken() == std::string(1,eofSymbol)){
			return summand;
		}
	} else{
		if(token.getType() == *eofTokenType){
			_tokenizer << token;
			return summand;
		}
	}

	if(token.getToken() == "+"){
		ASTPlus * plus = new ASTPlus(summand, parseFormula(eofSymbol, eofTokenType));
		return plus;
	}
	else if(token.getToken() == "-"){
		ASTMinus * minus = new ASTMinus(summand, parseFormula(eofSymbol, eofTokenType));
		return minus;
	}
	else {
		std::cout << "Syntax error";
		std::exit(0);
	}
}

Parser::~Parser() {

}

