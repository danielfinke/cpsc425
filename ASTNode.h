/* 
 * File:   ASTNode.h
 * Author: claire
 *
 * Created on October 6, 2013, 11:47 AM
 */

#ifndef ASTNODE_H
#define	ASTNODE_H

#include <iostream>
#include <string>
#include <sstream>
#include "Scanner.h"

using namespace std;

class ASTNode {
public:
    ASTNode();
    ASTNode(const ASTNode& orig);
    ASTNode& operator= (const ASTNode& rhs);
    virtual ~ASTNode();
	
	virtual void printNode(int indent, ostream * output) = 0;
	
	ASTNode * next;
	ostream * output;
	
	static Scanner * lookup;
protected:
    void printIndented(string text, int indent);
private:

};

#endif	/* ASTNODE_H */

