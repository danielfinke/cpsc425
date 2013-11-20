/* 
 * File:   ASTCompoundNode.cpp
 * Author: claire
 * 
 * Created on October 6, 2013, 2:42 PM
 */

#include "ASTCompoundNode.h"
#include "ScopeTable.h"

ASTCompoundNode::ASTCompoundNode() : ASTStatementNode(), dec(NULL), statement(NULL) {
}

ASTCompoundNode::ASTCompoundNode(const ASTCompoundNode& orig):ASTStatementNode(orig),
		dec(orig.dec), statement(orig.statement)
{
}

ASTCompoundNode& ASTCompoundNode::operator= (const ASTCompoundNode &rhs)
{
	ASTStatementNode::operator=(rhs);
	
	dec = rhs.dec;
	statement = rhs.statement;
 
    // return the existing object
    return *this;
}

ASTCompoundNode::~ASTCompoundNode() {
	delete dec;
	delete statement;
}

string ASTCompoundNode::genQuadruples(){
	int numLocals = 0;
	ASTDeclarationNode * decNode = dec;
	
	while(decNode != NULL) {
		numLocals++;
		decNode = dynamic_cast<ASTDeclarationNode *>(decNode->next);
	}
	
	stringstream ss;
	ss << numLocals;
	
    vec.push_back(Quadruple("ecs",ss.str(),"",""));
    statement->genQuadruples();
    vec.push_back(Quadruple("lcs","","",""));
	
	if(this->next != NULL) {
		this->next->genQuadruples();
	}
	
	return "";
}

void ASTCompoundNode::semAnalyze(){
    
    if(init || !this->isGlobalDec){
        this->scopeAnalyze();
        if(init)
            return;
    }
    sa->getST()->enterBlock();
    
    if(this->dec != NULL)
        this->dec->semAnalyze();
    
    this->statement->semAnalyze();
    sa->getST()->exitBlock();
    
     if(this->next != NULL)
        this->next->semAnalyze();
    
    //this->typeAnalyze();
    
}

void ASTCompoundNode::scopeAnalyze(){
    
    //nothing
}

bool ASTCompoundNode::returnAnalyze() {
	bool children = this->statement->returnAnalyze();
	
	if(next != NULL) {
		return children || dynamic_cast<ASTStatementNode *>(next)->returnAnalyze();
	}
	
	return children;
}

void ASTCompoundNode::printNode(int indent, ostream * output) {
	this->output = output;
	
	if(dec != NULL) {
		printIndented("declarations", indent);
		dec->printNode(indent + 2, output);
	}
	
	if(statement != NULL) {
		printIndented("statements", indent);
		statement->printNode(indent + 2, output);
	}
	
	if(next != NULL) {
		next->printNode(indent, output);
	}	
}