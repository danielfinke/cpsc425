/* 
 * File:   ASTLoopNode.cpp
 * Author: daniel
 * 
 * Created on October 8, 2013, 2:28 AM
 */

#include "ASTLoopNode.h"

ASTLoopNode::ASTLoopNode() : ASTStatementNode(), statement(NULL), contLabel(""),
		endLabel("")
{
}

ASTLoopNode::ASTLoopNode(const ASTLoopNode& orig) : ASTStatementNode(orig), statement(orig.statement),
		contLabel(""), endLabel("")
{
}

ASTLoopNode& ASTLoopNode::operator= (const ASTLoopNode &rhs)
{
	ASTStatementNode::operator=(rhs);
	
    // do the copy
    statement = rhs.statement;
	contLabel = rhs.contLabel;
    endLabel = rhs.endLabel;
 
    // return the existing object
    return *this;
}

ASTLoopNode::~ASTLoopNode() {
    delete statement;
}

void ASTLoopNode::semAnalyze(){
    
    if(init || !this->isGlobalDec){
        this->scopeAnalyze();
        if(init)
            return;
    }
    
    this->statement->semAnalyze();
    
     if(this->next != NULL)
        this->next->semAnalyze();
    
    //this->typeAnalyze();
    
}

string ASTLoopNode::genQuadruples(){
	contLabel = getLabel();
	endLabel = getLabel();
	// Start of loop
    vec.push_back(Quadruple("lab","","",contLabel));
	
	// Do stuff
	statement->genQuadruples();
	
	// Repeat from start of loop
	vec.push_back(Quadruple("goto","","",contLabel));
	// Outside end of loop
	vec.push_back(Quadruple("lab","","",endLabel));
	
	if(this->next != NULL) {
		this->next->genQuadruples();
	}
	
	return "";
}

void ASTLoopNode::scopeAnalyze(){
    //nothing
    
}

bool ASTLoopNode::returnAnalyze() {
	// If first statement returns, then so does this
	bool firstRets = this->statement->returnAnalyze();
	
	if(next != NULL) {
		return firstRets || dynamic_cast<ASTStatementNode *>(next)->returnAnalyze();
	}
	
	return firstRets;
}

void ASTLoopNode::printNode(int indent, ostream * output) {
	this->output = output;
	
	printIndented("loop", indent);
	printIndented("statement:", indent + 2);
	statement->printNode(indent + 4, output);
	
	if(next != NULL) {
		next->printNode(indent, output);
	}
}