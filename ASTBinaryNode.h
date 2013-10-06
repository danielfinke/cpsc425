/* 
 * File:   ASTBinaryToken.h
 * Author: claire
 *
 * Created on October 6, 2013, 2:47 PM
 */

#ifndef ASTBINARYTOKEN_H
#define	ASTBINARYTOKEN_H

class ASTBinaryToken : public ASTExpressionNode {
public:
    ASTBinaryToken();
    ASTBinaryToken(const ASTBinaryToken& orig);
    virtual ~ASTBinaryToken();
private:

};

#endif	/* ASTBINARYTOKEN_H */

