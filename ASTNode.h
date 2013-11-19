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
class ScopeTable;
class SemanticAnalyzer;

using namespace std;

class ASTNode {
public:
    ASTNode();
    ASTNode(const ASTNode& orig);
    ASTNode& operator= (const ASTNode& rhs);
    virtual ~ASTNode();
	
	virtual void semAnalyze();
	virtual void scopeAnalyze();
	virtual void typeAnalyze();
	virtual void printNode(int indent, ostream * output) = 0;
    virtual string genQuadruples();
	
	bool isGlobalDec;
	ASTNode * next;
	int lineNumber;
	ostream * output;
	
	static Scanner * lookup;
    static SemanticAnalyzer * sa;
    static bool init;
protected:
    void printIndented(string text, int indent);
private:

};

#endif	/* ASTNODE_H */

