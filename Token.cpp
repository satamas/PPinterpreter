/*
 * Token.cpp
 *
 *  Created on: 04.01.2014
 *      Author: atamas
 */

#include "Token.h"
#include <iostream>
#include <algorithm>
#include "UnEqPredicat.h"
using std::any_of;

bool Token::contents(vector<string> const &  arrOfStrings, string const stringWeAreLookingFor) const{
	return any_of(arrOfStrings.begin(), arrOfStrings.end(), UnEqPredicat<string>(stringWeAreLookingFor));
}

Token::Type Token::defType(std::string const & token) const{
		vector<string> const static comparisionOp = {">", "<", "==", "!=", ">=", "<="};
		vector<string> const static uncategorizedTokens = {"+", "-", "*", "/", "=", "(", ")", ":", ",", "<", ">", std::string(1,'\n')};
		if( contents(comparisionOp,token) ){
			return COMPARISION_OP;
		}else if( contents(uncategorizedTokens,token) ){
			return UNCATHEGORIZED_ONE_CHAR_TOKEN;
		}else if( token == "def"){
			return DEF;
		} else if(token == "if"){
			return IF;
		} else if(token == "while"){
			return WHILE;
		} else if(token == "print"){
			return PRINT;
		} else if(token == "read"){
			return READ;
		} else if(token == "end"){
			return END;
		} else if(token == "return"){
			return RETURN;
		} else{
			bool isName = false;
			bool isNum = false;

			if ( '0' <= token[0] && token[0] <= '9' ){
				isNum = true;
				for(unsigned int i = 0; i < token.length(); ++i){
					if( !('0' <= token[i] && token[i] <= '9') ){
						isNum = false;
						break;
					}
				}
			}

			if(('a' <= token[0] && token[0] <= 'z') ||
			   ('A' <= token[0] && token[0] <= 'Z') ){
				isName = true;
				for(unsigned int i = 0; i < token.length(); ++i){
						if( !(( 'a' <= token[i] && token[i] <= 'z' ) ||
							  ( 'A' <= token[i] && token[i] <= 'Z' ) ||
							  ( '0' <= token[i] && token[i] <= '9' ) ||
							  token[i] == '_') ){
							isName = false;
							break;
						}
				}

			}

			if(isNum){
				return NUM;
			} else if(isName){
				return NAME;
			} else{
				std::cerr << "line " << _lineNo <<  ": syntax error" << std::endl;
				std::exit(0);
			}

		}
	}



