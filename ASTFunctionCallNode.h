/* 
 * File:   ASTFunctionCallNode.h
 * Author: claire
 *
 * Created on October 6, 2013, 2:40 PM
 */

#ifndef ASTFUNCTIONCALLNODE_H
#define	ASTFUNCTIONCALLNODE_H

class ASTFunctionCallNode : public ASTNode {
public:
    ASTFunctionCallNode();
    ASTFunctionCallNode(const ASTFunctionCallNode& orig);
    virtual ~ASTFunctionCallNode();
private:

};

#endif	/* ASTFUNCTIONCALLNODE_H */

