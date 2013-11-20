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
#include "Quadruple.h"
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

	static string getTemp();
	static string getLabel();
	
	bool isGlobalDec;
	ASTNode * next;
	int lineNumber;
	ostream * output;
	
	static Scanner * lookup;
    static SemanticAnalyzer * sa;
    static bool init;
    static vector <Quadruple> vec; // How to initialize??
    static int tempCounter;
    static int labelCounter;
	
	static int curLevel;
	static int curDisplacement;
protected:
    void printIndented(string text, int indent);
private:

};

#endif	/* ASTNODE_H */

