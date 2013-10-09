/* 
 * File:   ASTParamNode.cpp
 * Author: daniel
 * 
 * Created on October 7, 2013, 11:15 PM
 */

#include "ASTParamNode.h"

ASTParamNode::ASTParamNode() : ASTVariableDeclarationNode(), isRef(false) {
}

ASTParamNode::ASTParamNode(const ASTParamNode& orig) {
}

ASTParamNode::~ASTParamNode() {
}

void ASTParamNode::printNode(int indent) {
	ostringstream oss;
	
	printIndented("param", indent);
	printIndented("id: " + idName, indent + 2);
	printIndented("type: " + Scanner::namesRev[declarationType], indent + 2);
	oss << "ref? " << (isRef ? "YES" : "NO");
	printIndented(oss.str(), indent + 2);
	oss.str("");
	oss << "arrayParam? " << (isArray ? "YES" : "NO");
	printIndented(oss.str(), indent + 2);

	if(next != NULL) {
		next->printNode(indent);
	}
}