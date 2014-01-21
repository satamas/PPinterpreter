/*
 * Visitor.h
 *
 *  Created on: 16.01.2014
 *      Author: atamas
 */

#ifndef VISITOR_H_
#define VISITOR_H_

class ASTUnMinus;
class ASTBinOp;
class ASTVar;
class ASTNum;
class ASTFunc;
class ASTVarDef;
class ASTIf;
class ASTPrint;
class ASTRead;
class ASTWhile;
class ASTReturn;

class Visitor{
public:
	Visitor(){};
	virtual void visit(ASTUnMinus const *) const = 0;
	virtual void visit(ASTBinOp const *) const = 0;
	virtual void visit(ASTVar const *) const = 0;
	virtual void visit(ASTNum const *) const = 0;
	virtual void visit(ASTFunc const *) const = 0;
	virtual void visit(ASTVarDef const *) const = 0;
	virtual void visit(ASTIf const *) const = 0;
	virtual void visit(ASTPrint const *) const = 0;
	virtual void visit(ASTRead const *) const = 0;
	virtual void visit(ASTWhile const *) const = 0;
	virtual void visit(ASTReturn const *) const = 0;
	virtual ~Visitor(){};
};

#endif /* VISITOR_H_ */
