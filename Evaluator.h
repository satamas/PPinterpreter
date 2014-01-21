/*
 * Evaluator.h
 *
 *  Created on: 16.01.2014
 *      Author: atamas
 */

#ifndef EVALUATOR_H_
#define EVALUATOR_H_


#include "Visitor.h"
#include "Function.h"
#include <stack>
#include <map>
#include <string>
using std::stack;
using std::string;
using std::map;

class Evaluator: public Visitor {
public:
	Evaluator(map<string, int> varNamespace, map<string, Function const *> funcNamespace);
	void visit(ASTUnMinus const *) const;
	void visit(ASTBinOp const *) const;
	void visit(ASTVar const *) const;
	void visit(ASTNum const *) const;
	void visit(ASTFunc const *) const;
	void visit(ASTVarDef const *) const;
	void visit(ASTIf const *) const;
	void visit(ASTPrint const *) const;
	void visit(ASTRead const *) const;
	void visit(ASTWhile const *) const;
	void visit(ASTReturn const *) const;
	int getReturnVal() const{
		return returnVal;
	}
	bool getReturnFlag() const{
		return returnFlag;
	}
	~Evaluator();
private:
	mutable bool returnFlag;
	mutable int returnVal;
	mutable map<string, int> _varNamespace;
	map<string, Function const *> _funcNamespace;
	mutable stack<int> _results;
};

#endif /* EVALUATOR_H_ */
