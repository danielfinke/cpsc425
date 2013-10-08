/* 
 * File:   ASTLiteralNode.h
 * Author: daniel
 *
 * Created on October 8, 2013, 9:08 AM
 */

#ifndef ASTLITERALNODE_H
#define	ASTLITERALNODE_H

#include "ASTExpressionNode.h"

class ASTLiteralNode : public ASTExpressionNode {
public:
	ASTLiteralNode();
	ASTLiteralNode(const ASTLiteralNode& orig);
	virtual ~ASTLiteralNode();
	
	void printNode(int indent);
	
	int value;
private:

};

#endif	/* ASTLITERALNODE_H */

