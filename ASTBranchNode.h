/* 
 * File:   ASTBranchNode.h
 * Author: claire
 *
 * Created on October 9, 2013, 12:09 AM
 */

#ifndef ASTBRANCHNODE_H
#define	ASTBRANCHNODE_H

#include "ASTStatementNode.h"
#include "ASTExpressionNode.h"
#include "ASTCaseNode.h"

/*ASTBranchNode represents the branch statements within the code. It is a subclass
 * of ASTStatementNode. It contains an override of the
 * print statement. It keeps track of:
 *              -pointer to the expression which it branches on
 *              -pointer to the first case statement
 */

class ASTBranchNode : public ASTStatementNode{
public:
    ASTBranchNode();
    ASTBranchNode(const ASTBranchNode& orig);
    ASTBranchNode& operator = (const ASTBranchNode &rhs);
    virtual ~ASTBranchNode();
	
	void semAnalyze();
	void scopeAnalyze();
	void typeAnalyze();
	void printNode(int indent, ostream * output);
	void genQuadruples();
        
        void caseAnalyze();
	bool contains(int x);
    
	bool returnAnalyze();
        
    vector<int> caseList;
    bool defaultReached;
    ASTExpressionNode * expression;
    ASTCaseNode * firstCase;
    
private:

};

#endif	/* ASTBRANCHNODE_H */

