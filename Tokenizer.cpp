/*
 * Tokenizer.cpp
 *
 *  Created on: 29.12.2013
 *      Author: atamas
 */

#include "Tokenizer.h"
#include <array>
#include <algorithm>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <limits>

Tokenizer::Tokenizer(std::ifstream & inf):
_inf(inf){

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

template <typename T>
struct unEqPredicat{
	unEqPredicat(T charToCompare){
		a = charToCompare;
	}
	bool operator ()(T const b){return (a == b);}
private:
	T a;
};

void Tokenizer::operator <<(Token const token){
	returnedTokens.push(token);
}

void Tokenizer::operator >>(Token & token){
	if(returnedTokens.empty()){
		token = readTokenFromFile();
	} else{
		token = returnedTokens.top();
		 returnedTokens.pop();
	}
}

Token Tokenizer::readTokenFromFile(){
	std::array<char,3> const static spaceSeparators = {' ', '\t', '\v'};
	std::array<char,4> const static algebraicOperators = {'+', '-', '*', '/'};
	std::array<char,4> const static binOpFirstChar = {'>', '<', '=', '!'};
	std::array<char,17> const static allSeparators = {'+', '-', '*', '/', '=', '(', ')', ':', ',', '!', '<', '>', '#', ' ', '\t', '\v', '\n'};
	std::array<std::string, 7> const static commands = { "def", "if", "while", "print", "read", "end", "return" };
	char nextChar = 0;
	_inf >> std::noskipws >> nextChar;



	while( std::any_of(spaceSeparators.begin(), spaceSeparators.end(), unEqPredicat<char>(nextChar)) && _inf.get(nextChar) ){
	}

	while(nextChar == '#'){
		_inf.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		_inf.get(nextChar);
	}

	if ( std::any_of(binOpFirstChar.begin(), binOpFirstChar.end(), unEqPredicat<char>(nextChar) ) ){
		std::string op;
		op.push_back(nextChar);

		_inf >> nextChar;
		if( nextChar == '=' ){
			op.push_back(nextChar);
		} else{
			_inf.seekg(-1, std::ios_base::cur	);
			if( op == "!" ){
				std::cout << "Syntax error";
				std::exit(0);
			}
		}

		if( op != "="){
			return Token(op,Token::COMPARISION_OP);
		} else{
			return Token(op,Token::UNCATHEGORIZED_ONE_CHAR_TOKEN);
		}
	}

	if ( std::any_of(algebraicOperators.begin(), algebraicOperators.end(), unEqPredicat<char>(nextChar) ) ){
		return Token(std::string(1,nextChar), Token::ALGEBRAIC_OP);
	}

	if ( std::any_of(allSeparators.begin(), allSeparators.end(), unEqPredicat<char>(nextChar)) ){
		return Token(std::string(1,nextChar), Token::UNCATHEGORIZED_ONE_CHAR_TOKEN);
	}

	if ( '0' <= nextChar && nextChar <= '9' ){
		std::string num;
		num.push_back(nextChar);
		while(  _inf.get(nextChar) && !std::any_of(allSeparators.begin(), allSeparators.end(), unEqPredicat<char>(nextChar)) ){
			if ( '0' <= nextChar && nextChar <= '9' ){
				num.push_back(nextChar);
			}else{
				std::cout << "Syntax error";
				std::exit(0);
			}
		}
		_inf.seekg(-1, std::ios_base::cur);
		return Token(num, Token::NUM);
	}

	if(('a' <= nextChar && nextChar <= 'z') ||
	   ('A' <= nextChar && nextChar <= 'Z') ){
		std::string name;
		name.push_back(nextChar);
		while(  _inf.get(nextChar) && !std::any_of(allSeparators.begin(), allSeparators.end(), unEqPredicat<char>(nextChar)) ){
			if( ( 'a' <= nextChar && nextChar <= 'z' ) ||
				( 'A' <= nextChar && nextChar <= 'Z' ) ||
				( '0' <= nextChar && nextChar <= '9' ) ||
				nextChar == '_'){
					name.push_back(nextChar);

			}else{
				std::cout << "Syntax error";
				std::exit(0);
			}
		}
		_inf.seekg(-1, std::ios_base::cur);

		if( std::any_of(commands.begin(), commands.end(), unEqPredicat<std::string>(name)) ){
			return Token(name, Token::COMMAND);
		}

		return Token(name, Token::NAME);
	}

	std::cout << "Syntax error";
	std::exit(0);
}

Tokenizer::~Tokenizer() {
	_inf.close();
}

