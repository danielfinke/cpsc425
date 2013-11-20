/* 
 * File:   ASTLoopNode.h
 * Author: daniel
 *
 * Created on October 8, 2013, 2:28 AM
 */

#ifndef ASTLOOPNODE_H
#define	ASTLOOPNODE_H

#include "ASTStatementNode.h"

/*ASTLoopNode represents the loop statement. It is a subclass of ASTStatementNode
 * and overrides the print method. It has a pointer to the first statement of
 * the statement list within the loop construct. 
 */
class ASTLoopNode : public ASTStatementNode {
public:
	ASTLoopNode();
	ASTLoopNode(const ASTLoopNode& orig);
        ASTLoopNode& operator= (const ASTLoopNode &rhs);
	virtual ~ASTLoopNode();
	
	void semAnalyze();
	void scopeAnalyze();
	void printNode(int indent, ostream * output);
    string genQuadruples();
	
	bool returnAnalyze();
	
	string contLabel, endLabel;
	ASTStatementNode * statement;
private:

};

#endif	/* ASTLOOPNODE_H */

