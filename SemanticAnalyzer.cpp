/* 
 * File:   SemanticAnalyzer.cpp
 * Author: daniel
 * 
 * Created on October 27, 2013, 11:22 PM
 */

#include "SemanticAnalyzer.h"
#include "ASTFunctionNode.h"
#include "ScopeTable.h"
#include "Admin.h"

SemanticAnalyzer::SemanticAnalyzer() : errorCount(0), ST(NULL), admin(NULL) {
	
}

SemanticAnalyzer::SemanticAnalyzer(Admin * adminRef) : errorCount(0),
		ST(NULL), admin(adminRef) {
	ST = new ScopeTable(adminRef);
	
	ASTFunctionNode * fn = new ASTFunctionNode;
	fn->declarationType = Scanner::INT;
	fn->id = 0;
	ASTParamNode * p = new ASTParamNode;
	fn->param = p;
	
	ST->insertDeclaration(0, fn);
	
	fn = new ASTFunctionNode;
	fn->declarationType = Scanner::VOID;
	fn->id = 1;
	p = new ASTParamNode;
	p->declarationType = Scanner::INT;
	p->id = 2;
	fn->param = p;
	
	ST->insertDeclaration(1, fn);
	ST->insertDeclaration(2, p);
	
	fn = new ASTFunctionNode;
	fn->declarationType = Scanner::BOOL;
	fn->id = 3;
	p = new ASTParamNode;
	fn->param = p;
	
	ST->insertDeclaration(3, fn);
	
	fn = new ASTFunctionNode;
	fn->declarationType = Scanner::VOID;
	fn->id = 4;
	p = new ASTParamNode;
	p->declarationType = Scanner::BOOL;
	p->id = 5;
	fn->param = p;
	
	ST->insertDeclaration(4, fn);
	ST->insertDeclaration(5, p);
}

SemanticAnalyzer::SemanticAnalyzer(const SemanticAnalyzer& orig) {
}

SemanticAnalyzer::~SemanticAnalyzer() {
}

ScopeTable * SemanticAnalyzer::getST() { return ST; }

void SemanticAnalyzer::semAnalyze(ASTNode* topNode){
	ASTNode * current = topNode;
	
	// Loop over all global declarations for forward-declarations
	try {
		ASTNode::init = true;
		while(current != NULL) {
			current->semAnalyze();

			// Meet semantic condition that int main(void) is last
			if(current->next == NULL && !isIntMainVoidLast((ASTDeclarationNode *)current)) {
				// Semantic error - int main (void) not found as last declaration
				admin->semanticError("Last declaration must be int main(void)", current->lineNumber);
			}
			current = current->next;
		}

		// Analyze the rest of the source code
		ASTNode::init = false;
		topNode->semAnalyze();
	}
	catch(char const * ex) {
		cout << "Compiler exception thrown: " << ex;
	}
}

void SemanticAnalyzer::semanticError(string desc, int lineNumber) {
	admin->semanticError(desc, lineNumber);
}

void SemanticAnalyzer::incError() { errorCount++; }

bool SemanticAnalyzer::isIntMainVoidLast(ASTDeclarationNode * lastDec) {
	return lastDec->declarationType == Scanner::INT &&
			lastDec->lookup->getIdentifierName(lastDec->id) == "main" &&
			((ASTFunctionNode *)lastDec)->getParamCount() == 0;
}