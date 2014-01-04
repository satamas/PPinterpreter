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
#include <map>
class command;
class Function {
public:
	Function(std::vector<std::string> parNames, std::vector <command *> body, std::map<std::string, Function> * funcNamespace);
	int eval(std::vector<int> & parameters);
	virtual ~Function();
private:
	std::map<std::string, Function> * _funcNamespace;
	std::vector<std::string> _parNames;
	std::vector <command *> _body;
};

#endif /* FUNCTION_H_ */
