/* 
 * File:   ASTCaseNode.h
 * Author: claire
 *
 * Created on October 9, 2013, 12:09 AM
 */

#ifndef ASTCASENODE_H
#define	ASTCASENODE_H

#include "ASTStatementNode.h"
/*ASTCaseNode represents the case statements (including the default case), in
 * a branch statement. These cases are represented as a list of cases, with the
 * ASTBranchNode class pointing to the first of the cases. It is a subclass of
 * the ASTStatementNode.It contains an override of the
 * print statement.It records the:
 *      -Type of case (case or default)
 *      -the NUM which the case checks (if case, 0 if default)
 *      -pointer to the statement called after a case is found
 *      -pointer to the next case in the branch statement
 */
class ASTCaseNode : public ASTStatementNode {
public:
    ASTCaseNode();
    ASTCaseNode(const ASTCaseNode& orig);
    ASTCaseNode& operator = (const ASTCaseNode &rhs);
    virtual ~ASTCaseNode();
	void semAnalyze();
        void scopeAnalyze();
	void printNode(int indent, ostream * output);
    
    int type;
    int num;
    ASTStatementNode * statement;
    ASTCaseNode * nextCase;
private:

};

#endif	/* ASTCASENODE_H */

