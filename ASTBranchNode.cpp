/* 
 * File:   ASTBranchNode.cpp
 * Author: claire
 * 
 * Created on October 9, 2013, 12:09 AM
 */

#include "ASTBranchNode.h"
#include "SemanticAnalyzer.h"
#include "Quadruple.h"

ASTBranchNode::ASTBranchNode(): ASTStatementNode(), expression(NULL), firstCase(NULL), 
        defaultReached(false), caseList(0)
{
}

ASTBranchNode::ASTBranchNode(const ASTBranchNode& orig):ASTStatementNode(orig),
        expression(orig.expression), firstCase(orig.firstCase), defaultReached(orig.defaultReached),
        caseList(orig.caseList)
{
}

ASTBranchNode& ASTBranchNode::operator= (const ASTBranchNode &rhs)
{
	ASTStatementNode::operator=(rhs);
	
    // do the copy
        expression = rhs.expression;
        firstCase = rhs.firstCase;
        defaultReached = rhs.defaultReached;
        caseList = rhs.caseList;
    
 
    // return the existing object
        return *this;
}
  

ASTBranchNode::~ASTBranchNode() {
    delete expression;
    delete firstCase;
}

string ASTBranchNode::genQuadruples() {
	
	/* temp vars
	 * iff on case's val, skip to next case iff
	 * recurse into case
	 * goto end
	 * next case iff
	 */
	string expTemp = expression->genQuadruples();
	ASTCaseNode * cNode = firstCase;
	ASTCaseNode * defNode = NULL;
	string endLabel = getLabel();
	
	while(cNode != NULL) {
		
		if(cNode->type == Scanner::DEFAULT) {
			defNode = cNode;
			cNode = dynamic_cast<ASTCaseNode *>(cNode->next);
			continue;
		}
		
		stringstream ss;
		ss << cNode->num;
		
		Quadruple eqQuad;
		eqQuad.operation = "eq";
		eqQuad.arg1 = expTemp;
		eqQuad.arg2 = ss.str();
		eqQuad.result = getTemp();
		
		vec.push_back(eqQuad);
		
		Quadruple ifQuad;
		ifQuad.operation = "iff";
		ifQuad.arg1 = eqQuad.result;
		ifQuad.result = getLabel();
		
		vec.push_back(ifQuad);
		
		cNode->genQuadruples();
		
		vec.push_back(Quadruple("goto", "", "", endLabel));
		vec.push_back(Quadruple("lab", "", "", ifQuad.result));
		
		cNode = dynamic_cast<ASTCaseNode *>(cNode->next);
	}
	
	if(defNode != NULL) {
		defNode->genQuadruples();
	}
	
	vec.push_back(Quadruple("lab", "", "", endLabel));
	
	if(this->next != NULL) {
		this->next->genQuadruples();
	}
	
	return "";
}

void ASTBranchNode ::semAnalyze(){
    
    if(init || !this->isGlobalDec){
        this->scopeAnalyze();
        if(init)
            return;
    }
    
    this->expression->semAnalyze();
    this-> firstCase->semAnalyze();
    this->caseAnalyze();
    this->typeAnalyze();
    
     if(this->next != NULL)
        this->next->semAnalyze();
    
    
}

void ASTBranchNode :: caseAnalyze(){
    ASTCaseNode * current = firstCase;
    
    do{
        if(current->type == Scanner::DEFAULT){
            if(defaultReached)
                sa->semanticError("Multiple default statements found",lineNumber);
            defaultReached = true;
        }
        else{
            if(contains(current->num))
                sa->semanticError("Case value repeated",lineNumber);
            caseList.push_back(current->num);
        }
        current = dynamic_cast<ASTCaseNode *>(current->next);
    }while(current!= NULL);
    
}

void ASTBranchNode:: scopeAnalyze(){
    
    //nothing?
}

void ASTBranchNode::typeAnalyze() {
	if(expression == NULL) {
		return;
	}
	
	if(expression->type != Scanner::INT) {
		// Semantic error - integral expression expected
		sa->semanticError("Integral expression expected", lineNumber);
	}
}

bool ASTBranchNode::returnAnalyze() {
	bool retInCases = true;
	ASTCaseNode * curCase = firstCase;
	
	while(curCase != NULL) {
		retInCases = retInCases && curCase->returnAnalyze();
		curCase = dynamic_cast<ASTCaseNode *>(curCase->next);
	}
	
	if(next != NULL) {
		return retInCases || dynamic_cast<ASTStatementNode *>(next)->returnAnalyze();
	}
	
	return retInCases;
}

void ASTBranchNode::printNode(int indent, ostream * output) {
	this->output = output;
	
	printIndented("branch", indent);
	printIndented("exp:", indent + 2);
	expression->printNode(indent + 4, output);
	printIndented("cases:", indent + 2);
	firstCase->printNode(indent + 4, output);
	
	if(next != NULL) {
		next->printNode(indent, output);
	}
}

//helper functions, checks caseList to see if contains x
bool ASTBranchNode :: contains(int x){
    
    for(int i = 0; i<caseList.size(); i++){
        if(caseList.at(i)==x)
            return true;
    }
    
    return false;
}
