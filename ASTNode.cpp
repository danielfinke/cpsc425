/* 
 * File:   ASTNode.cpp
 * Author: claire
 * 
 * Created on October 6, 2013, 11:47 AM
 */

#include "ASTNode.h"
#include "ScopeTable.h"

Scanner * ASTNode::lookup = 0;
SemanticAnalyzer * ASTNode::sa = 0;
int ASTNode:: tempCounter=1;
int ASTNode:: labelCounter=1;
bool ASTNode::init = true;
vector<Quadruple> ASTNode::vec;

int ASTNode::curLevel = 0;
int ASTNode::curDisplacement = 0;

ASTNode::ASTNode() : next(NULL), output(NULL), isGlobalDec(false), lineNumber(0){
}

ASTNode::ASTNode(const ASTNode& orig) : next(orig.next), output(orig.output),
        isGlobalDec(orig.isGlobalDec), lineNumber(orig.lineNumber)
{
}

ASTNode& ASTNode:: operator = (const ASTNode& rhs){
	next = rhs.next;
	output = rhs.output;
        isGlobalDec = rhs.isGlobalDec;
        lineNumber = rhs.lineNumber;
	
    return *this;
}

ASTNode::~ASTNode() {
    delete next;
}

string ASTNode::genQuadruples() {
	return "";
}

void ASTNode::semAnalyze(){
    
    if(init || !this->isGlobalDec){
        this->scopeAnalyze();
        if(init)
            return;
    }
    
    this->typeAnalyze();
    
     if(this->next != NULL)
        this->next->semAnalyze();
}

void ASTNode::scopeAnalyze(){
    
    
}

void ASTNode::typeAnalyze() {
	
}

void ASTNode::printIndented(string text, int indent) {
	for(int i = 0; i < indent; i++) {
		*output << " ";
	}
	*output << text << endl;
}

string ASTNode::getLabel(){
    
    std::stringstream sstm;
    sstm<<"L"<<labelCounter++;
    return sstm.str();
}

string ASTNode::getTemp(){
    std:stringstream sstm;
    //sstm<<"t"<<tempCounter++;
	
	sstm << "(" << curLevel << "," << curDisplacement << ")";
	curDisplacement++;
	
    return sstm.str();
}