/*
 * Commands.h
 *
 *  Created on: 31.12.2013
 *      Author: atamas
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include "ASTNode.h"
#include <iostream>
#include <string>
#include <map>

struct command{
	command(){};
	virtual int execute(std::map<std::string,int> & varNamespace, std::map<std::string, Function> & funcNamespace, int & pos, bool & returnFlag) = 0;
	virtual ~command(){};
};

struct VarDef: public command{
	VarDef(std::string var, ASTNode * formula){
		_var = var;
		_formula = formula;
	}

	int execute(std::map<std::string,int> & varNamespace, std::map<std::string, Function> & funcNamespace, int & pos, bool & returnFlag){
		returnFlag = false;
		varNamespace[_var] = _formula->eval(varNamespace, funcNamespace);
		return 0;
	}
	std::string _var;
	ASTNode * _formula;
};

struct commandPrint: public command{
	commandPrint(ASTNode * formula){
		_formula = formula;
	}
	int execute(std::map<std::string,int> & varNamespace, std::map<std::string, Function> & funcNamespace, int & pos, bool & returnFlag){
		returnFlag = false;
		std::cout << _formula->eval(varNamespace, funcNamespace) << std::endl;
		return 0;
	}
	ASTNode * _formula;
};

struct commandGoTo: public command{
	commandGoTo(int pos){
		_pos = pos;
	}

	int execute(std::map<std::string,int> & varNamespace, std::map<std::string, Function> & funcNamespace, int & pos, bool & returnFlag){
		returnFlag = false;
		pos = _pos;
		return 0;
	}

	~commandGoTo(){

	}
private:
	int _pos;
};

struct commandIf: public command{
public:
	commandIf(ASTNode * leftPart, std::string comparator, ASTNode * rightPart, int endIfPos){
		_leftPart = leftPart;
		_comparator = comparator;
		_rightPart = rightPart;
		_endIfPos = endIfPos;
	}

	int execute(std::map<std::string,int> & varNamespace, std::map<std::string, Function> & funcNamespace, int & pos, bool & returnFlag){
			returnFlag = false;
			if(_comparator == "=="){
				if (!(_leftPart->eval(varNamespace, funcNamespace) == _rightPart->eval(varNamespace, funcNamespace))){
					pos = _endIfPos;
				}
			} else if(_comparator == ">="){
				if (!(_leftPart->eval(varNamespace, funcNamespace) >= _rightPart->eval(varNamespace, funcNamespace))){
					pos = _endIfPos;
				}
			} else if(_comparator == "<="){
				if (!(_leftPart->eval(varNamespace, funcNamespace) <= _rightPart->eval(varNamespace, funcNamespace))){
					pos = _endIfPos;
				}
			} else if(_comparator == "!="){
				if (!(_leftPart->eval(varNamespace, funcNamespace) != _rightPart->eval(varNamespace, funcNamespace))){
					pos = _endIfPos;
				}
			} else if(_comparator == ">"){
				if (!(_leftPart->eval(varNamespace, funcNamespace) > _rightPart->eval(varNamespace, funcNamespace))){
					pos = _endIfPos;
				}
			} else if(_comparator == "<"){
				if (!(_leftPart->eval(varNamespace, funcNamespace) < _rightPart->eval(varNamespace, funcNamespace))){
					pos = _endIfPos;
				}
			}
			return 0;
	}

private:
	ASTNode * _leftPart;
	std::string _comparator;
	ASTNode * _rightPart;
	int _endIfPos;

};

struct commandRead: public command{
	commandRead(std::string var){
		_var = var;
	}

	int execute(std::map<std::string,int> & varNamespace, std::map<std::string, Function> & funcNamespace, int & pos, bool & returnFlag){
		returnFlag = false;
		int val;
		std::cin >> val;
		varNamespace[_var] = val;
		return 0;
	}
	std::string _var;
};

struct commandReturn: public command{
	commandReturn(ASTNode * formula){
		_formula = formula;
	}

	int execute(std::map<std::string,int> & varNamespace, std::map<std::string, Function> & funcNamespace, int & pos, bool & returnFlag){
		returnFlag = true;
		return _formula->eval(varNamespace, funcNamespace);
	}

private:
	ASTNode * _formula;
};

#endif /* COMMANDS_H_ */
