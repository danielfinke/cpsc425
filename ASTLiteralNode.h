/* 
 * File:   ASTLiteralNode.h
 * Author: daniel
 *
 * Created on October 8, 2013, 9:08 AM
 */

#ifndef ASTLITERALNODE_H
#define	ASTLITERALNODE_H

#include "ASTExpressionNode.h"

/*ASTLiteral Node represents literals in the language, such as 4 or true.
 * It is a subclass of ASTExpressionNode and overrides the print method.
 * the node only keeps track of the value
 * of the literal, such as 4 or true. if the type (inherited from expressionnode)
 * is boolean, the value will be 0 or 1, if its num, it will be the integer value 
 */
class ASTLiteralNode : public ASTExpressionNode {
public:
	ASTLiteralNode();
	ASTLiteralNode(const ASTLiteralNode& orig);
        ASTLiteralNode& operator= (const ASTLiteralNode &rhs);
	virtual ~ASTLiteralNode();
	
	void semAnalyze();
	void semAnalyze(bool restrictIdents);
	void scopeAnalyze();
        string genQuadruples();
	
	ASTLiteralNode * calc();
	void printNode(int indent, ostream * output);
private:

};

#endif	/* ASTLITERALNODE_H */

