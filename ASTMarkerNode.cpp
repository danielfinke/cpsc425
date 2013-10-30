/* 
 * File:   ASTMarkerNode.cpp
 * Author: daniel
 * 
 * Created on October 8, 2013, 2:07 PM
 */

#include "ASTMarkerNode.h"
#include "SemanticAnalyzer.h"

ASTMarkerNode::ASTMarkerNode() : ASTStatementNode(), type(0), enabled(false) {
}

ASTMarkerNode::ASTMarkerNode(const ASTMarkerNode& orig) : ASTStatementNode(orig),type(orig.type),
		enabled(orig.enabled)
{
}

ASTMarkerNode& ASTMarkerNode::operator= (const ASTMarkerNode &rhs)
{
	ASTStatementNode::operator=(rhs);
	
    // do the copy
    type = rhs.type;
	enabled = rhs.enabled;
 
    // return the existing object
    return *this;
}

ASTMarkerNode::~ASTMarkerNode() {
}

void ASTMarkerNode::semAnalyze(){
    
    if(init || !this->isGlobalDec){
        this->scopeAnalyze();
        if(init)
            return;
    }
	
	loopAnalyze();
    
     if(this->next != NULL)
        this->next->semAnalyze();
    //this->typeAnalyze();
    
}

void ASTMarkerNode::scopeAnalyze(){
    
    
}

void ASTMarkerNode::loopAnalyze() {
	if(!enabled && (type == Scanner::EXIT ||
			type == Scanner::CONTINUE)) {
		sa->semanticError("Use of exit or continue outside of loop body", lineNumber);
	}
}

void ASTMarkerNode::printNode(int indent, ostream * output) {
	this->output = output;
	
	printIndented("marker", indent);
	printIndented("type: " + Scanner::namesRev[type], indent + 2);
	
	if(next != NULL) {
		next->printNode(indent, output);
	}
}