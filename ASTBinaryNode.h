/* 
 * File:   ASTBinaryNode.h
 * Author: claire
 *
 * Created on October 6, 2013, 2:47 PM
 */

#ifndef ASTBINARYNODE_H
#define	ASTBINARYNODE_H

#include "ASTExpressionNode.h"

/*The ASTBinaryNode represents binary operations within the code, such as 
 * multiplication, addition, subtraction, division. It is a subclass of 
 * ASTExpressionNode. It contains an override of the
 * print statement. It has pointers to the left and right side of the operation,
 * which are expressions. It also has the type of operation as an int representing
 * and ENUM.
*/

class ASTBinaryNode : public ASTExpressionNode {
public:
    ASTBinaryNode();
    ASTBinaryNode(const ASTBinaryNode& orig);
    ASTBinaryNode& operator= (const ASTBinaryNode &rhs);
    virtual ~ASTBinaryNode();
    
	void printNode(int indent, ostream * output);
	
	int oper;
	ASTExpressionNode * left, *right;
private:

};

#endif	/* ASTBINARYNODE_H */

