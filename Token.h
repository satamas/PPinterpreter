/*
 * Token.h
 *
 *  Created on: 04.01.2014
 *      Author: atamas
 */

#ifndef TOKEN_H_
#define TOKEN_H_

#include <string>
#include <iostream>
#include <vector>

using std::vector;
using std::cerr;
using std::endl;
using std::string;

struct Token{
public:

	enum Type{
		NAME,
		NUM,
		DEF,
		IF,
		WHILE,
		PRINT,
		READ,
		END,
		RETURN,
		COMPARISION_OP,
		UNCATHEGORIZED_ONE_CHAR_TOKEN
	};

	Token(){
	}

	void assertTokenType(string const stringToCompare) const{
		if (_token != stringToCompare){
			cerr << "line " << _lineNo <<  ": syntax error" << endl;
			exit(0);
		}
	}

	void assertTokenType(Type const typeToCompare) const{
		if (_type != typeToCompare){
			cerr << "line " << _lineNo <<  ": syntax error" << endl;
			exit(0);
		}
	}

	Token(string const & token, int const lineNo){
		_token = token;
		_lineNo = lineNo;
		_type = defType(token);
	}

	bool operator ==(std::string const b) const{
		return _token == b;
	}

	bool operator !=(std::string const b) const{
		return _token != b;
	}

	operator std::string const () const{
		return _token;
	}

	int getLineNo() const{
		return _lineNo;
	}

	Type getType() const{
		return _type;
	}

private:
	int _lineNo;
	Type _type;
	std::string _token;
	bool contents(vector<string> const &  arrOfStrings, string const stringWeAreLookingFor) const;
	Type defType(std::string const & token) const;
};


#endif /* TOKEN_H_ */
