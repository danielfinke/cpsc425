/* 
 * File:   ASTNode.cpp
 * Author: claire
 * 
 * Created on October 6, 2013, 11:47 AM
 */

#include "ASTNode.h"

Scanner * ASTNode::lookup = 0;

ASTNode::ASTNode() : next(NULL), output(NULL) {
}

ASTNode::ASTNode(const ASTNode& orig) : next(orig.next), output(orig.output) {
}

ASTNode& ASTNode:: operator = (const ASTNode& rhs){
	next = rhs.next;
	output = rhs.output;
	
    return *this;
}

ASTNode::~ASTNode() {
    delete next;
}

void ASTNode::printIndented(string text, int indent) {
	for(int i = 0; i < indent; i++) {
		*output << " ";
	}
	*output << text << endl;
}