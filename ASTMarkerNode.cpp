/* 
 * File:   ASTMarkerNode.cpp
 * Author: daniel
 * 
 * Created on October 8, 2013, 2:07 PM
 */

#include "ASTMarkerNode.h"

ASTMarkerNode::ASTMarkerNode() : ASTStatementNode(), type(0) {
}

ASTMarkerNode::ASTMarkerNode(const ASTMarkerNode& orig) {
}

ASTMarkerNode::~ASTMarkerNode() {
}

void ASTMarkerNode::printNode(int indent) {
	printIndented("marker", indent);
	printIndented("type: " + Scanner::namesRev[type], indent + 2);
	
	if(next != NULL) {
		next->printNode(indent);
	}
}