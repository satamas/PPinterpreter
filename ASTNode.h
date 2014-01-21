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
#include <iostream>
#include "Visitor.h"
#include "Function.h"
using std::cout;
using std::map;

class ASTNode {
public:
	ASTNode(int const lineNo):
	_lineNo(lineNo){
	};
	virtual void visit(Visitor const * visitor) const = 0;
	int const getLineNo()const{
		return _lineNo;
	}
	virtual ~ASTNode(){};

protected:
	int const _lineNo;
};

class ASTOneChildNode: public ASTNode{
public:
	ASTOneChildNode( ASTNode * child, int lineNo ):
	ASTNode(lineNo){
		_child = child;
	}

	ASTNode * getChild() const{
		return _child;
	}

	virtual ~ASTOneChildNode(){
		delete _child;
	}
protected:
	ASTNode * _child;
};

class ASTBinOp: public ASTNode{
public:
	ASTBinOp(ASTNode * leftChild, ASTNode * rightChild, int lineNo):
	ASTNode(lineNo){
		_leftChild = leftChild;
		_rightChild = rightChild;
	}

	virtual int eval(int a, int b) const = 0;

	ASTNode * getLeftChild() const{
		return _leftChild;
	}

	ASTNode * getRightChild() const{
		return _rightChild;
	}

	virtual ~ASTBinOp(){
		delete _leftChild;
		delete _rightChild;
	}
protected:
	ASTNode * _leftChild;
	ASTNode * _rightChild;
};

class ASTUnMinus: public ASTOneChildNode{
public:
	ASTUnMinus(ASTNode * child, int const lineNo):
	ASTOneChildNode(child,lineNo){
		_child = child;
	}

	void visit(Visitor const * visitor) const{
		visitor->visit(this);
	}

	~ASTUnMinus(){
		delete _child;
	}
};

class ASTMult: public ASTBinOp{
public:
	ASTMult(ASTNode * leftChild, ASTNode * rightChild, int const lineNo):
	ASTBinOp(leftChild, rightChild, lineNo){
	}

	void visit(Visitor const * visitor) const{
		visitor->visit(this);
	}

	int eval(int x, int y) const{
		return x*y;
	}

	~ASTMult(){};
};

class ASTPlus: public ASTBinOp{
public:
	ASTPlus(ASTNode * leftChild, ASTNode * rightChild, int const lineNo):
	ASTBinOp(leftChild, rightChild, lineNo){
	}

	void visit(Visitor const * visitor) const{
		visitor->visit(this);
	}

	int eval(int x, int y) const{
		return x+y;
	}

	~ASTPlus(){};
};

class ASTMinus: public ASTBinOp{
public:
	ASTMinus(ASTNode * leftChild, ASTNode * rightChild, int const lineNo):
	ASTBinOp(leftChild, rightChild, lineNo){
	}

	void visit(Visitor const * visitor) const{
		visitor->visit(this);
	}

	int eval(int x, int y) const{
		return x-y;
	}

	~ASTMinus(){};
};

class ASTDev: public ASTBinOp{
public:
	ASTDev(ASTNode * leftChild, ASTNode * rightChild, int const lineNo):
	ASTBinOp(leftChild, rightChild, lineNo){
	}

	void visit(Visitor const * visitor) const{
		visitor->visit(this);
	}

	int eval(int x, int y) const{
		if( y == 0){
			std::cerr << "line " << _lineNo << ": devision by zero" << std::endl;
			std::exit(0);
		}
		return x/y;
	}

	~ASTDev(){};
};
class ASTlt: public ASTBinOp{
public:
	ASTlt(ASTNode * leftChild, ASTNode * rightChild, int const lineNo):
	ASTBinOp(leftChild, rightChild, lineNo){
	}

	void visit(Visitor const * visitor) const{
		visitor->visit(this);
	}

	int eval(int x, int y) const{
		return x<y;
	}

	~ASTlt(){};
};

class ASTgt: public ASTBinOp{
public:
	ASTgt(ASTNode * leftChild, ASTNode * rightChild, int const lineNo):
	ASTBinOp(leftChild, rightChild, lineNo){
	}

	void visit(Visitor const * visitor) const{
		visitor->visit(this);
	}

	int eval(int x, int y) const{
		return x>y;
	}

	~ASTgt(){};
};

class ASTleq: public ASTBinOp{
public:
	ASTleq(ASTNode * leftChild, ASTNode * rightChild, int const lineNo):
	ASTBinOp(leftChild, rightChild, lineNo){
	}

	void visit(Visitor const * visitor) const{
		visitor->visit(this);
	}

	int eval(int x, int y) const{
		return x<=y;
	}

	~ASTleq(){};
};

class ASTgeq: public ASTBinOp{
public:
	ASTgeq(ASTNode * leftChild, ASTNode * rightChild, int const lineNo):
	ASTBinOp(leftChild, rightChild, lineNo){
	}

	void visit(Visitor const * visitor) const{
		visitor->visit(this);
	}

	int eval(int x, int y) const{
		return x>=y;
	}

	~ASTgeq(){};
};

class ASTeq: public ASTBinOp{
public:
	ASTeq(ASTNode * leftChild, ASTNode * rightChild, int const lineNo):
	ASTBinOp(leftChild, rightChild, lineNo){
	}

	void visit(Visitor const * visitor) const{
		visitor->visit(this);
	}

	int eval(int x, int y) const{
		return x==y;
	}

	~ASTeq(){};
};

class ASTneq: public ASTBinOp{
public:
	ASTneq(ASTNode * leftChild, ASTNode * rightChild, int const lineNo):
	ASTBinOp(leftChild, rightChild, lineNo){
	}

	void visit(Visitor const * visitor) const{
		visitor->visit(this);
	}

	int eval(int x, int y) const{
		return x!=y;
	}

	~ASTneq(){};
};




class ASTVar: public ASTNode{
public:
	ASTVar(std::string var, int const lineNo):
	ASTNode(lineNo){
		_varName = var;
	}

	void visit(Visitor const * visitor) const{
		visitor->visit(this);
	}

	string getVarName() const{
		return _varName;
	}

	~ASTVar(){}
private:
	string _varName;
};

class ASTNum: public ASTNode{
public:

	ASTNum(int num, int lineNo):
	ASTNode(lineNo){
		_num = num;
	}

	void visit(Visitor const * visitor) const{
		visitor->visit(this);
	}

	int getNum() const{
		return _num;
	}

protected:
	int _num;
};

class ASTFunc: public ASTNode{
public:
	ASTFunc(std::string name, std::vector<ASTNode *> & parameters, int const lineNo):
	ASTNode(lineNo){
		_name = name;
		_parameters = parameters;
	}

	void visit(Visitor const * visitor) const{
		visitor->visit(this);
	}

	string getName() const{
		return _name;
	}

	std::vector<ASTNode *> getParameters() const{
		return _parameters;
	}

private:
	std::vector<ASTNode *> _parameters;
	string _name;
};




class ASTVarDef: public ASTOneChildNode{
public:
	ASTVarDef(string varName, ASTNode * formula, int const lineNo):
	ASTOneChildNode(formula,lineNo){
		_varName = varName;
	}

	void visit(Visitor const * visitor) const{
		visitor->visit(this);
	}

	string getVarName() const{
		return _varName;
	}

private:
	string _varName;
};

class ASTPrint: public ASTOneChildNode{
public:
	ASTPrint(ASTNode * formula, int const lineNo):
		ASTOneChildNode(formula, lineNo){
	}

	void visit(Visitor const * visitor) const{
		visitor->visit(this);
	}
};

class ASTRead: public ASTNode{
public:
	ASTRead(string varName, int const lineNo):
	ASTNode(lineNo){
		_varName = varName;
	}

	void visit(Visitor const * visitor) const{
		visitor->visit(this);
	}

	string getVarName() const{
		return _varName;
	}

private:
	string _varName;
};

class ASTIf: public ASTOneChildNode{
public:
	ASTIf(ASTNode * condition, vector<ASTNode const *> body, int const lineNo):
	ASTOneChildNode(condition,lineNo){
		_body = body;
	}

	void visit(Visitor const * visitor) const{
		visitor->visit(this);
	}

	vector<ASTNode const *> getBody() const{
		return _body;
	}
private:
	vector<ASTNode const *> _body;
};

class ASTWhile: public ASTOneChildNode{
public:
	ASTWhile(ASTNode * condition, vector<ASTNode const *> body, int const lineNo):
	ASTOneChildNode(condition,lineNo){
		_body = body;
	}

	void visit(Visitor const * visitor) const{
		visitor->visit(this);
	}

	vector<ASTNode const *> getBody() const{
		return _body;
	}
private:
	vector<ASTNode const *> _body;
};

class ASTReturn: public ASTOneChildNode{
public:
	ASTReturn(ASTNode * formula, int const lineNo):
	ASTOneChildNode(formula, lineNo){
	}

	void visit(Visitor const * visitor) const{
		visitor->visit(this);
	}
};

#endif /* ASTNODE_H_ */
