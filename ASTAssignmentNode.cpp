/* 
 * File:   ASTAssignmentNode.cpp
 * Author: daniel
 * 
 * Created on October 8, 2013, 1:19 AM
 */

#include "ASTAssignmentNode.h"
#include "ASTVariableDeclarationNode.h"
#include "ASTLiteralNode.h"
#include "ScopeTable.h"
#include "Quadruple.h"

ASTAssignmentNode::ASTAssignmentNode() : ASTStatementNode(), isArray(false),
		id(0), left(NULL), exp(NULL), arrayExp(NULL)
{
}

ASTAssignmentNode::ASTAssignmentNode(const ASTAssignmentNode& orig) : ASTStatementNode(orig),
		isArray(orig.isArray), id(orig.id), left(NULL),
		exp(orig.exp), arrayExp(orig.arrayExp)
{
}

ASTAssignmentNode& ASTAssignmentNode::operator= (const ASTAssignmentNode &rhs)
{
	ASTStatementNode::operator=(rhs);
	
    // do the copy
    isArray = rhs.isArray;
	id = rhs.id;
	left = rhs.left;
	exp = rhs.exp;
	arrayExp = rhs.arrayExp;
 
    // return the existing object
    return *this;
}

ASTAssignmentNode::~ASTAssignmentNode() {
	delete exp;
	delete arrayExp;
}
void ASTAssignmentNode::semAnalyze(){
    
    if(init || !this->isGlobalDec){
        this->scopeAnalyze();
        if(init)
            return;
    }
    
    if(this->isArray)
        this->arrayExp->semAnalyze();
    
    this->exp->semAnalyze();
	
    this->typeAnalyze();
    
    if(this->next != NULL)
        this->next->semAnalyze();
    
}

void ASTAssignmentNode::scopeAnalyze(){
    
	left = sa->getST()->getDeclaration(id, lineNumber);
    
}

string ASTAssignmentNode::genQuadruples(){
    Quadruple quad;
	
	stringstream ss;
	ASTVariableDeclarationNode * leftDec = (ASTVariableDeclarationNode *)left;
	ss << "(" << leftDec->level << "," << leftDec->displacement << ")";
	//if its an array tae quadruple
	if(isArray) {
		quad.operation = "tae";
		quad.arg1 = exp->genQuadruples();
		quad.arg2 = arrayExp->genQuadruples();
		//quad.result = left->lookup->getIdentifierName(id);
		quad.result = ss.str();
		
		vec.push_back(quad);
	}
    //other wise asignement quadruple
    else {
		quad.operation="asg";
		//quad.result = left->lookup->getIdentifierName(id);
		quad.result = ss.str();
		// Special case, short-circuit assignment, don't bother storing temp var
		string asgShort = exp->genQuadruples();
		if(vec.back().operation == "rdi" ||
				vec.back().operation == "rdb") {
			//vec.back().result = left->lookup->getIdentifierName(id);
			vec.back().result = ss.str();
		}
		// Some sort of expression, a temp var is necessary
		else {
			quad.arg1 = asgShort;
			vec.push_back(quad);
		}
	}
	
	if(this->next != NULL) {
		this->next->genQuadruples();
	}
    
    return "";
   
}
void ASTAssignmentNode::typeAnalyze() {
	if(left == NULL || exp == NULL) {
		return;
	}
	
	// Already complained about an error
	if(exp->type == -1) {
		return;
	}
	if(exp->type == Scanner::VOID) {
		return;
	}
	
	if(left->declarationType == Scanner::INT &&
			(exp->type != Scanner::INT && exp->type != Scanner::NUM)) {
		// Semantic error - assigning wrongly typed value
		sa->semanticError("Assigning wrongly typed value", lineNumber);
	}
	else if(left->declarationType == Scanner::BOOL &&
			(exp->type != Scanner::BOOL && exp->type != Scanner::BLIT)) {
		// Semantic error - assigning wrongly typed value
		sa->semanticError("Assigning wrongly typed value", lineNumber);
	}
}

void ASTAssignmentNode::printNode(int indent, ostream * output) {
	ostringstream oss;
	
	this->output = output;
	
	printIndented("assignment", indent);
	if(ASTNode::lookup != NULL) {
		printIndented("id: " + ASTNode::lookup->getIdentifierName(id), indent + 2);
	}
	oss << "arrayIndex? " << (isArray ? "YES" : "NO");
	printIndented(oss.str(), indent + 2);
	
	if(isArray) {
		printIndented("index:", indent + 2);
		arrayExp->printNode(indent + 4, output);
	}
	
	printIndented("value:", indent + 2);
	exp->printNode(indent + 4, output);
	
	if(next != NULL) {
		next->printNode(indent, output);
	}
}