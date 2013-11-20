/* 
 * File:   ASTIfNode.cpp
 * Author: daniel
 * 
 * Created on October 8, 2013, 1:35 AM
 */

#include "ASTIfNode.h"
#include "SemanticAnalyzer.h"
#include "Quadruple.h"

ASTIfNode::ASTIfNode() : ASTStatementNode(), exp(NULL), statement(NULL), elseStatement(NULL) {
}

ASTIfNode::ASTIfNode(const ASTIfNode& orig) : ASTStatementNode(orig), exp(orig.exp),
        statement(orig.statement),elseStatement(orig.statement)
{
}

ASTIfNode& ASTIfNode::operator= (const ASTIfNode &rhs)
{
	ASTStatementNode::operator=(rhs);
	
    // do the copy
        exp=rhs.exp;
        statement = rhs.statement;
        elseStatement = rhs.elseStatement;
    
 
    // return the existing object
    return *this;
}

ASTIfNode::~ASTIfNode() {
    delete exp;
    delete statement;
    delete elseStatement;
}

string ASTIfNode::genQuadruples(){
    
    Quadruple ifquad;
    ifquad.operation ="iff";
    ifquad.arg1 = exp->genQuadruples();
 
    if(elseStatement!=NULL)
    {
        string elseLabel = getLabel();
        string endLabel = getLabel();
        ifquad.result=elseLabel;
        
        vec.push_back(ifquad);
        statement->genQuadruples();
        vec.push_back(Quadruple("goto","","",endLabel));
        vec.push_back(Quadruple("lab","","",elseLabel));
        elseStatement->genQuadruples();
        vec.push_back(Quadruple("lab","","",endLabel));
    }
    else
    {
        string endLabel=getLabel();
        ifquad.result=endLabel;
        
        vec.push_back(ifquad);
        statement->genQuadruples();
        vec.push_back(Quadruple("lab","","",endLabel));
    }
    
}

void ASTIfNode::semAnalyze(){
    
    if(init || !this->isGlobalDec){
        this->scopeAnalyze();
        if(init)
            return;
    }
    
    this->exp->semAnalyze();
	
    this->typeAnalyze();
	
    this->statement->semAnalyze();
    
    if(elseStatement != NULL)
        this->elseStatement->semAnalyze();
    
    if(this->next != NULL)
        this->next->semAnalyze();
    
}

void ASTIfNode::scopeAnalyze(){
    
    
}

void ASTIfNode::typeAnalyze() {
	if(exp == NULL) {
		return;
	}
	
	if(exp->type != Scanner::BOOL) {
		// Semantic error - expression must be of boolean type
		sa->semanticError("Expression must be of boolean type", lineNumber);
	}
}

bool ASTIfNode::returnAnalyze() {
	bool retInBranches = true;
	ASTStatementNode * curState = statement;
	
	// Must have an else statement in order to return true
	if(elseStatement != NULL) {
		while(curState != NULL) {
			retInBranches = retInBranches && curState->returnAnalyze();
			curState = dynamic_cast<ASTStatementNode *>(curState->next);
		}
		curState = elseStatement;
		while(curState != NULL) {
			retInBranches = retInBranches && curState->returnAnalyze();
			curState = dynamic_cast<ASTStatementNode *>(curState->next);
		}
	}
	else {
		retInBranches = false;
	}
	
	if(next != NULL) {
		return retInBranches || dynamic_cast<ASTStatementNode *>(next)->returnAnalyze();
	}
	
	return retInBranches;
}

void ASTIfNode::printNode(int indent, ostream * output) {
	this->output = output;
	
	printIndented("if", indent);
	printIndented("condition:", indent + 2);
	exp->printNode(indent + 4, output);
	printIndented("true branch:", indent + 2);
	statement->printNode(indent + 4, output);
	
	if(elseStatement != NULL) {
		printIndented("false branch:", indent + 2);
		elseStatement->printNode(indent + 4, output);
	}
	
	if(next != NULL) {
		next->printNode(indent, output);
	}
}