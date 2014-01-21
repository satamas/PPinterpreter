/*
 * Tokenizer.cpp
 *
 *  Created on: 29.12.2013
 *      Author: atamas
 */

#include "Tokenizer.h"
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <limits>
#include "UnEqPredicat.h"

using std::vector;
using std::any_of;

Tokenizer::Tokenizer(std::ifstream & inf):
_inf(inf){
	_lineNo = 1;
	if(!this->eof()){
		_nextToken = readTokenFromFile();
	}
}

bool Tokenizer::contents(vector<char> const & arrOfChars, char const charWeAreLookingFor) const{
	return any_of(arrOfChars.begin(), arrOfChars.end(), UnEqPredicat<char>(charWeAreLookingFor));
}

bool Tokenizer::eof(){
	char nextChar;
    if(_inf.get(nextChar)){
        _inf.seekg(-1, std::ios_base::cur);
        return false;
    }else{
    	return true;
    }
}

void Tokenizer::operator >>(Token & token){
	token = _nextToken;

	if(!this->eof()){
		_nextToken = readTokenFromFile();
	}
}

Token Tokenizer::readTokenFromFile(){
	std::vector<char> const static spaceSeparators = {' ', '\t', '\v'};
	std::vector<char> const static binOpFirstChar = {'>', '<', '=', '!'};
	std::vector<char> const static allSeparators = {'+', '-', '*', '/', '=', '(', ')', ':', ',', '!', '<', '>', '#', ' ', '\t', '\v', '\n'};
	char nextChar = 0;
	_inf >> std::noskipws >> nextChar;

	while( contents(spaceSeparators,nextChar) && _inf.get(nextChar) ){
	}

	if(nextChar == '#'){
		_inf.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		++_lineNo;
		return Token(std::string(1,'\n'), _lineNo-1);
	}

	if ( contents(binOpFirstChar,nextChar) ){
		std::string op;
		op.push_back(nextChar);

		_inf >> nextChar;
		if( nextChar == '=' ){
			op.push_back(nextChar);
		} else{
			_inf.seekg(-1, std::ios_base::cur	);
		}
		return Token(op, _lineNo);
	}

	if ( contents(allSeparators,nextChar) ){
		if ( nextChar == '\n' ){
			++_lineNo;
			return Token(std::string(1,nextChar), _lineNo-1);
		}
		return Token(std::string(1,nextChar), _lineNo);
	}

	std::string token;
	token.push_back(nextChar);
	while(  _inf.get(nextChar) && !contents(allSeparators,nextChar) ){
		token.push_back(nextChar);
	}
	_inf.seekg(-1, std::ios_base::cur);
	return Token(token, _lineNo);
}

void Tokenizer::assertNextToken(Token::Type typeToCompare){
	Token token;
	*this >> token;
	token.assertTokenType(typeToCompare);
}

Token Tokenizer::getNextTokenVal(){
	return _nextToken;
}

void Tokenizer::skipToken(){
	Token token;
	*this>>token;
}


void Tokenizer::assertNextToken(std::string strToCompare){
	Token token;
	*this >> token;
	token.assertTokenType(strToCompare);
}

Tokenizer::~Tokenizer() {
	_inf.close();
}

