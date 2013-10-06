/* 
 * File:   ASTNode.h
 * Author: claire
 *
 * Created on October 6, 2013, 11:47 AM
 */

#ifndef ASTNODE_H
#define	ASTNODE_H

class ASTNode {
public:
    ASTNode();
    ASTNode(const ASTNode& orig);
    ASTNode& operator= (const ASTNode& rhs);
    virtual ~ASTNode();
private:
    

};

#endif	/* ASTNODE_H */

