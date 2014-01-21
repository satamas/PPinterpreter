/*
 * Function.h
 *
 *  Created on: 03.01.2014
 *      Author: atamas
 */

#ifndef FUNCTION_H_
#define FUNCTION_H_

#include <vector>
#include <string>
using std::vector;
using std::string;

class ASTNode;

class Function {
public:
	Function(vector<string> const & parNames, vector <ASTNode const *> const & body):
	_parNames(parNames),
	_body(body){
	}

	vector <ASTNode const *> const & getBody() const{
		return _body;
	}

	vector<string> const & getParNames() const{
		return _parNames;
	}

	~Function();
private:
	vector<string> const _parNames;
	vector <ASTNode const *> const _body;
};

#endif /* FUNCTION_H_ */
