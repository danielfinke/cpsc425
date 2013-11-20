/* 
 * File:   ASTDeclarationNode.h
 * Author: claire
 *
 * Created on October 6, 2013, 2:43 PM
 */

#ifndef ASTDECLARATIONNODE_H
#define	ASTDECLARATIONNODE_H

#include <string>
#include "ASTNode.h"

using namespace std;

/*ASTDeclarationNode represents general declarations within the language. It
 * is a subclass of ASTNode and is a superclass to ASTVariableDelcarationNode
 * and to ASTfunctionNode. It contains an override of the print statement.
 * It keeps track of the:
 *      -declaration type as an ENUM (int, void, bool)
 *      -id of what is being declared
 */

class ASTDeclarationNode : public ASTNode {
public:
    ASTDeclarationNode();
    ASTDeclarationNode(const ASTDeclarationNode& orig);
    ASTDeclarationNode& operator= (const ASTDeclarationNode &rhs);
    virtual ~ASTDeclarationNode();
	
	virtual void semAnalyze();
	virtual void scopeAnalyze();
	// todo: make pure virtual
	virtual void printNode(int indent, ostream * output);
	
    int declarationType;
    int id;
private:

};

#endif	/* ASTDECLARATIONNODE_H */

