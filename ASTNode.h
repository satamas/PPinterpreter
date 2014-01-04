/*
 * ASTNode.h
 *
 *  Created on: 29.12.2013
 *      Author: atamas
 */

#ifndef ASTNODE_H_
#define ASTNODE_H_

#include <map>
#include <vector>
#include <string>
#include "Function.h"
using std::map;

class ASTNode {
public:
	ASTNode(){};
	virtual int eval(map<std::string, int> & variables, std::map<std::string, Function> & funcNamespace) = 0;
	virtual ~ASTNode(){};
};

class ASTMult: public ASTNode{
public:
	ASTMult(ASTNode * leftChild, ASTNode * rightChild){
		_leftChild = leftChild;
		_rightChild = rightChild;
	}

	int eval(std::map<std::string, int> & variables, std::map <std::string, Function> & funcNamespace){
		return _leftChild->eval(variables, funcNamespace) * _rightChild->eval(variables, funcNamespace);
	}

	~ASTMult(){
		delete _leftChild;
		delete _rightChild;
	}

private:
	ASTNode * _leftChild;
	ASTNode * _rightChild;
};

class ASTPlus: public ASTNode{
public:
	ASTPlus(ASTNode * leftChild, ASTNode * rightChild){
		_leftChild = leftChild;
		_rightChild = rightChild;
	}

	int eval(std::map<std::string, int> & variables, std::map <std::string, Function> & funcNamespace){
		return _leftChild->eval(variables, funcNamespace) + _rightChild->eval(variables, funcNamespace);
	}

	~ASTPlus(){

	}

private:
	ASTNode * _leftChild;
	ASTNode * _rightChild;
};

class ASTMinus: public ASTNode{
public:
	ASTMinus(ASTNode * leftChild, ASTNode * rightChild){
		_leftChild = leftChild;
		_rightChild = rightChild;
	}

	int eval(std::map<std::string, int> & variables, std::map <std::string, Function> & funcNamespace){
		return _leftChild->eval(variables, funcNamespace) - _rightChild->eval(variables, funcNamespace);
	}

	~ASTMinus(){

	}

private:
	ASTNode * _leftChild;
	ASTNode * _rightChild;
};

class ASTDev: public ASTNode{
public:
	ASTDev(ASTNode * leftChild, ASTNode * rightChild){
		_leftChild = leftChild;
		_rightChild = rightChild;
	}

	int eval(std::map<std::string, int> & variables, std::map <std::string, Function> & funcNamespace){
		return _leftChild->eval(variables, funcNamespace) / _rightChild->eval(variables, funcNamespace);
	}

	~ASTDev(){

	}

private:
	ASTNode * _leftChild;
	ASTNode * _rightChild;
};

class ASTVar: public ASTNode{
public:
	ASTVar(std::string var){
		_var = var;
	}

	int eval(std::map<std::string, int> & variables, std::map <std::string, Function> & funcNamespace){
		return variables[_var];
	}

	~ASTVar(){
	}
private:
	std::string _var;
};

class ASTNum: public ASTNode{
public:

	ASTNum(int num){
		_num = num;
	}

	int eval(std::map<std::string, int> & variables, std::map <std::string, Function>  & funcNamespace){
		return _num;
	}

	~ASTNum(){

	}
protected:
	int _num;
};

class ASTFunc: public ASTNode{
public:
	ASTFunc(std::string name, std::vector<ASTNode *> & parameters){
		_name = name;
		_parameters = parameters;
	}

	int eval(std::map<std::string, int> & variables, std::map <std::string, Function> & funcNamespace){
		std::vector <int> parameters;
		for(unsigned int i = 0; i < _parameters.size(); ++i){
			parameters.push_back( _parameters[i]->eval(variables, funcNamespace) );
		}
		return funcNamespace.at(_name).eval(parameters);
	}

private:
	std::vector<ASTNode *> _parameters;
	std::string _name;
};

#endif /* ASTNODE_H_ */
