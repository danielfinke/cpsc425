/* 
 * File:   ASTNode.cpp
 * Author: claire
 * 
 * Created on October 6, 2013, 11:47 AM
 */

#include "ASTNode.h"

ASTNode::ASTNode() : next(NULL) {
}

ASTNode::ASTNode(const ASTNode& orig) : next(orig.next) {
}

ASTNode& ASTNode:: operator = (const ASTNode& rhs){
    return *this;
}

ASTNode::~ASTNode() {
    delete next;
}

void ASTNode::printIndented(string text, int indent) {
	for(int i = 0; i < indent; i++) {
		cout << " ";
	}
	cout << text << endl;
}