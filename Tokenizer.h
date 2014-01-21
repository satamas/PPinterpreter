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
#include <string>
#include <vector>
#include "Token.h"

using std::string;
using std::vector;

class Tokenizer {
public:
	Tokenizer(std::ifstream & inf);
	bool eof();
	void operator >> (Token & token);
	void assertNextToken(Token::Type const typeToCompare);
	void assertNextToken(std::string const strToCompare);
	Token getNextTokenVal();
	void skipToken();
	virtual ~Tokenizer();
private:
	Token _nextToken;
	int _lineNo;
	bool contents(vector<char> const & arrOfChars, char const charWeAreLookingFor) const;
	Token readTokenFromFile();
	std::ifstream & _inf;
};

#endif /* TOKENIZER_H_ */
