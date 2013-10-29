/* 
 * File:   ASTAssignmentNode.cpp
 * Author: daniel
 * 
 * Created on October 8, 2013, 1:19 AM
 */

#include "ASTAssignmentNode.h"
#include "ScopeTable.h"

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
    
    if(this->next != NULL)
        this->next->semAnalyze();
    
    
    this->typeAnalyze();
    
}

void ASTAssignmentNode::scopeAnalyze(){
    
   if(! ST->isInScope(this->id))
   {
       //throw scope error
   }
       
    
}

void ASTAssignmentNode::typeAnalyze() {
	if(left == NULL || exp == NULL) {
		// Throw exception
	}
	
	if(left->declarationType != exp->type &&
			exp->type != -1) {
		// Semantic error - assigning wrongly typed value
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