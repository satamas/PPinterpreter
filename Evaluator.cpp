/*
 * Evaluator.cpp
 *
 *  Created on: 16.01.2014
 *      Author: atamas
 */

#include "Evaluator.h"
#include "ASTNode.h"

#include <iostream>
using std::cin;

Evaluator::Evaluator(map<string, int> varNamespace, map<string, Function const *> funcNamespace):
_varNamespace(varNamespace),
_funcNamespace(funcNamespace){
	returnFlag = false;
	returnVal = 0;
}

void Evaluator::visit(ASTUnMinus const * unMinus) const{
	unMinus->getChild()->visit(this);
	int res = -_results.top();
	_results.pop();
	_results.push(res);
}

void Evaluator::visit(ASTBinOp const * binOp) const{
	binOp->getLeftChild()->visit(this);
	int d1 = _results.top();
	_results.pop();

	binOp->getRightChild()->visit(this);
	int d2 = _results.top();
	_results.pop();

	_results.push( binOp->eval(d1, d2) );
}

void Evaluator::visit(ASTNum const * num) const{
	_results.push(num->getNum());
}

void Evaluator::visit(ASTVar const * var) const{
	if( _varNamespace.count(var->getVarName()) == 0 ){
		std::cerr << "Runtime error (line " << var->getLineNo() << "): undefined variable '" << var->getVarName() << "'"<< std::endl;
		exit(0);
	}
	_results.push(_varNamespace[var->getVarName()]);
}

void Evaluator::visit(ASTVarDef const * varDef) const{
	varDef->getChild()->visit(this);
	_varNamespace[varDef->getVarName()] = _results.top();
	_results.pop();
}

void Evaluator::visit(ASTRead const * read) const{
	string varName = read->getVarName();
	std::cin >> _varNamespace[varName];
}

void Evaluator::visit(ASTPrint const * print) const{
	print->getChild()->visit(this);
	cout << _results.top() << std::endl;
	_results.pop();
}

void Evaluator::visit(ASTIf const * ifStatment) const{
	ifStatment->getChild()->visit(this);
	if(_results.top()){
		_results.pop();
		vector<ASTNode const *> body = ifStatment->getBody();
		for(unsigned int pos = 0; pos < body.size() && !this->getReturnFlag(); ++pos){
			body[pos]->visit(this);
		}
	}
}

void Evaluator::visit(ASTWhile const * whileStatment) const{
	whileStatment->getChild()->visit(this);
	while(_results.top()){
		_results.pop();
		vector<ASTNode const *> body = whileStatment->getBody();
		for(unsigned int pos = 0; pos < body.size() && !this->getReturnFlag(); ++pos){
			body[pos]->visit(this);
		}
		whileStatment->getChild()->visit(this);
	}
	_results.pop();
}

void Evaluator::visit(ASTFunc const * function) const{
	string name = function->getName();
	vector<ASTNode *> parameters = function->getParameters();


	if( !_funcNamespace.count(name) ){
		std::cerr << "Runtime error (line " << function->getLineNo() << "): undefined function '" << name << "'"<< std::endl;
		std::exit(0);
	}

	vector<string> const parNames= _funcNamespace.at(name)->getParNames();

	if (parameters.size() != parNames.size() ){
		std::cerr << "line " << function->getLineNo() << ": arguments number mismatch for " << function->getLineNo() << std::endl;
		std::exit(0);
	}

	map<string, int> varNamespace;

	for(unsigned int i = 0; i < parameters.size(); ++i){
		parameters[i]->visit(this);
		varNamespace[parNames[i] ] = _results.top();
		_results.pop();
	}

	Evaluator * evaluator = new Evaluator(varNamespace, _funcNamespace);

	vector<ASTNode const *> const body = _funcNamespace.at(name)->getBody();

	for(unsigned int pos = 0; pos < body.size() && ! evaluator->getReturnFlag(); ++pos){
		body[pos]->visit(evaluator);
	}

	_results.push(evaluator->getReturnVal());
}

void Evaluator::visit(ASTReturn const * returnCommand) const{
	returnCommand->getChild()->visit(this);
	returnFlag = true;
	returnVal = _results.top();
}

Evaluator::~Evaluator() {

}

