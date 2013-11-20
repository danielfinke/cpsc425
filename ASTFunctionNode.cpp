/* 
 * File:   ASTFunctionNode.cpp
 * Author: claire
 * 
 * Created on October 6, 2013, 2:47 PM
 */

#include "ASTFunctionNode.h"
#include "ASTLiteralNode.h"
#include "ScopeTable.h"

ASTFunctionNode::ASTFunctionNode() : ASTDeclarationNode(),
		param(NULL), compound(NULL) {
}

ASTFunctionNode::ASTFunctionNode(const ASTFunctionNode& orig):ASTDeclarationNode(orig),
		param(orig.param),
        compound(orig.compound)
{
}

ASTFunctionNode& ASTFunctionNode::operator= (const ASTFunctionNode &rhs)
{
	ASTDeclarationNode::operator=(rhs);
	
    // do the copy
	param = rhs.param;
	compound = rhs.compound;
 
    // return the existing object
    return *this;
}

ASTFunctionNode::~ASTFunctionNode() {
    delete param;
    delete compound;
}

string ASTFunctionNode::genQuadruples() {
	int numLocals = 0;
	ASTParamNode * curParam = param;
	ASTDeclarationNode * decNode = compound->dec;
	
	curLevel++;
	curDisplacement = 2;
	
	int i = -1;
	// Calculate parameters level/displacement
	while(curParam != NULL) {
		curParam->level = curLevel;
		curParam->displacement = i;
		i--;
		curParam = dynamic_cast<ASTParamNode *>(curParam->next);
	}
	
	// Calculate locals size
	while(decNode != NULL) {
		ASTVariableDeclarationNode * decAsVar =
				dynamic_cast<ASTVariableDeclarationNode *>(decNode);
		if(decAsVar != NULL) {
			if(!decAsVar->isArray) {
				numLocals++;
				decAsVar->level = curLevel;
				decAsVar->displacement = curDisplacement;
				curDisplacement++;
			}
			else {
				ASTLiteralNode * sizeVal = decAsVar->arrayExp->calc();
				numLocals += sizeVal->value;
				decAsVar->level = curLevel;
				decAsVar->displacement = curDisplacement;
				curDisplacement += sizeVal->value;
			}
		}
		decNode = dynamic_cast<ASTDeclarationNode *>(decNode->next);
	}
	
	stringstream ss;
	ss << numLocals;
	
	// Will return to update size
	int ecsIndex = vec.size();
	
	Quadruple funQuad;
	funQuad.operation = "fun";
	funQuad.arg1 = lookup->getIdentifierName(id);
	funQuad.arg2 = ss.str();
	
	vec.push_back(funQuad);
	
	compound->statement->genQuadruples();
	
	// Update temp size
	// -2 because of link and return address on stack
	ss.str("");
	ss << (curDisplacement - 2);
	vec[ecsIndex].arg2 = ss.str();
	
	curLevel--;
	
	if(this->next != NULL) {
		this->next->genQuadruples();
	}
	
	return "";
}

void ASTFunctionNode::semAnalyze(){
    
    if(init || !this->isGlobalDec){
        this->scopeAnalyze();
        if(init)
            return;
    }
	
	returnAnalyze();
	
	if(getParamCount() > 0) {
		this->param->semAnalyze();
	}
    this->compound->semAnalyze();
    
     if(this->next != NULL)
        this->next->semAnalyze();
    //this->typeAnalyze();
    
}

void ASTFunctionNode::scopeAnalyze(){
    
    sa->getST()->insertDeclaration(id, this);
}

void ASTFunctionNode::returnAnalyze() {
	if(!this->compound->statement->returnAnalyze()) {
		sa->semanticError("Missing return statement for function: " +
			ASTNode::lookup->getIdentifierName(id), lineNumber);
	}
}

void ASTFunctionNode::printNode(int indent, ostream * output) {
	this->output = output;
	
	printIndented("function", indent);
	if(ASTNode::lookup != NULL) {
		printIndented("id: " + ASTNode::lookup->getIdentifierName(id), indent + 2);
	}
	printIndented("type: " + Scanner::namesRev[declarationType], indent + 2);
	if(param != NULL) {
		printIndented("params:", indent + 2);
		param->printNode(indent + 4, output);
	}
	printIndented("body:", indent + 2);
	compound->printNode(indent + 4, output);
	
	if(next != NULL) {
		next->printNode(indent, output);
	}
}

int ASTFunctionNode::getParamCount() {
	int count = 0;
	ASTParamNode * p = param;
	
	// VOID param
	if(p->declarationType == 0) {
		return 0;
	}
	
	while(p != NULL) {
		count++;
		p = dynamic_cast<ASTParamNode *>(p->next);
	}
	
	return count;
}