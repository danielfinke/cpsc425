/* 
 * File:   SemanticAnalyzer.h
 * Author: daniel
 *
 * Created on October 27, 2013, 11:22 PM
 */

#ifndef SEMANTICANALYZER_H
#define	SEMANTICANALYZER_H

#include "ASTNode.h"
#include "ASTDeclarationNode.h"
class Admin;
class ASTFunctionNode;

class SemanticAnalyzer {
public:
	SemanticAnalyzer();
	SemanticAnalyzer(Admin * adminRef);
	SemanticAnalyzer(const SemanticAnalyzer& orig);
	virtual ~SemanticAnalyzer();
        
	ScopeTable * getST();
    void semAnalyze(ASTNode* topNode);
	// Reflects admin's semanticError
	void semanticError(string desc, int lineNumber);
	void incError();
	int getErrorCount();
private:
	int errorCount;
	Admin * admin;
	ScopeTable * ST;
	
	bool isIntMainVoidLast(ASTDeclarationNode * lastDec);
};

#endif	/* SEMANTICANALYZER_H */

