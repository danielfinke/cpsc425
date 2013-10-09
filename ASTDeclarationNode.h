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

class ASTDeclarationNode : public ASTNode {
public:
    ASTDeclarationNode();
    ASTDeclarationNode(const ASTDeclarationNode& orig);
    ASTDeclarationNode& operator= (const ASTDeclarationNode &rhs);
    virtual ~ASTDeclarationNode();
	
	// todo: make pure virtual
	virtual void printNode(int indent);
	
    int declarationType;
    string idName;
private:

};

#endif	/* ASTDECLARATIONNODE_H */

