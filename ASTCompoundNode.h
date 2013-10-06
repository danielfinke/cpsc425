/* 
 * File:   ASTCompoundNode.h
 * Author: claire
 *
 * Created on October 6, 2013, 2:42 PM
 */

#ifndef ASTCOMPOUNDNODE_H
#define	ASTCOMPOUNDNODE_H

class ASTCompoundNode :public ASTNode{
public:
    ASTCompoundNode();
    ASTCompoundNode(const ASTCompoundNode& orig);
    virtual ~ASTCompoundNode();
private:

};

#endif	/* ASTCOMPOUNDNODE_H */

