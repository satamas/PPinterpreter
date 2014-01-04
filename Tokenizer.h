/*
 * Tokenizer.h
 *
 *  Created on: 29.12.2013
 *      Author: atamas
 */

#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#include <fstream>
#include <string>
#include <stack>

struct Token{
public:

	enum Type{
		NAME,
		NUM,
		COMMAND,
		COMPARISION_OP,
		ALGEBRAIC_OP,
		UNCATHEGORIZED_ONE_CHAR_TOKEN
	};

	Token(){
	}

	Token(std::string token, Type type){
		_token = token;
		_type = type;
	}

	std::string getToken(){
		return _token;
	}

	Type getType(){
		return _type;
	}

private:
	Type _type;
	std::string _token;
};


class Tokenizer {
public:
	Tokenizer(std::ifstream & inf);
	bool eof();
	void operator << (Token const token);
	void operator >> (Token & token);
	virtual ~Tokenizer();
private:
	std::stack<Token> returnedTokens;
	Token readTokenFromFile();
	std::ifstream & _inf;
};

#endif /* TOKENIZER_H_ */
