/* 
 * File:   ASTDeclarationNode.h
 * Author: claire
 *
 * Created on October 6, 2013, 2:43 PM
 */

#ifndef ASTDECLARATIONNODE_H
#define	ASTDECLARATIONNODE_H

using namespace std;

class ASTDeclarationNode : public ASTNode {
public:
    ASTDeclarationNode();
    ASTDeclarationNode(const ASTDeclarationNode& orig);
    virtual ~ASTDeclarationNode();
private:
    int declarationType;
    string idName;

};

#endif	/* ASTDECLARATIONNODE_H */

