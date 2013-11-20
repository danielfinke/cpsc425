/* 
 * File:   ASTFunctionCallNode.cpp
 * Author: claire
 * 
 * Created on October 6, 2013, 2:40 PM
 */

#include "ASTFunctionCallNode.h"
#include "ScopeTable.h"
#include "ASTVariableNode.h"

ASTFunctionCallNode::ASTFunctionCallNode() : ASTStatementNode(), ASTExpressionNode(), id(0),
		funcDec(NULL), argument(NULL), isStatement(false) {
}

ASTFunctionCallNode::ASTFunctionCallNode(const ASTFunctionCallNode& orig): ASTStatementNode(orig), 
        ASTExpressionNode(orig), id(orig.id), funcDec(orig.funcDec),
		argument(orig.argument), isStatement(orig.isStatement)
{
}

ASTFunctionCallNode& ASTFunctionCallNode::operator= (const ASTFunctionCallNode &rhs)
{
	ASTStatementNode::operator=(rhs);
        ASTExpressionNode::operator = (rhs);
	
    // do the copy
	id = rhs.id;
	funcDec = rhs.funcDec;
	argument = rhs.argument;
        isStatement = rhs.isStatement;
 
    // return the existing object
    return *this;
}

ASTFunctionCallNode::~ASTFunctionCallNode() {
    delete argument;
}

string ASTFunctionCallNode::genQuadruples() {
    //statements dont return things
    //expressions do
    
	string rVal = getTemp();
	
	if(funcDec->declarationType != Scanner::VOID) {
		vec.push_back(Quadruple("rval","","",rVal));
	}
	
	vector<Quadruple> revArgVec;
	
    ASTExpressionNode * curArg = argument;
    while(curArg!=NULL)
    {
		ASTVariableNode * arrayArg = dynamic_cast<ASTVariableNode *>(curArg);
        if(arrayArg != NULL && arrayArg->isArray) {
			revArgVec.push_back(Quadruple("arg",curArg->genQuadruples(),"",""));
		}
		else if(arrayArg != NULL && !arrayArg->isArray && arrayArg->varDec->isArray) {
			revArgVec.push_back(Quadruple("arga",curArg->genQuadruples(),"",""));
		}
		else {
			revArgVec.push_back(Quadruple("arg",curArg->genQuadruples(),"",""));
		}
		
		curArg = dynamic_cast<ASTExpressionNode *>(curArg->next);
    }
	
	for(int i = revArgVec.size()-1; i >=0; i--) {
		vec.push_back(revArgVec[i]);
	}
	
	string funcName = lookup->getIdentifierName(funcDec->id);
	vec.push_back(Quadruple("call",funcName,"",""));
	
	if(this->next != NULL) {
		this->next->genQuadruples();
	}
	
	if(!isStatement) {
		return rVal;
	}
	else {
		return "";
	}
    
    /*
     * retval of temp
     * arguments
     * cal
     */
}

void ASTFunctionCallNode::semAnalyze(){
    
    if(init || !this->isGlobalDec){
        this->scopeAnalyze();
        if(init)
            return;
    }
    
    if(this->argument != NULL)
        this->argument->semAnalyze();
    
    this->typeAnalyze();
    
     if(this->next != NULL)
        this->next->semAnalyze();
    
}

void ASTFunctionCallNode::semAnalyze(bool restrictIdents){
	
	// Should not be making a call here
	if(restrictIdents) {
		sa->semanticError("Array size must be static", lineNumber);
		restrictIdents = false;
	}
    
    if(init || !this->isGlobalDec){
        this->scopeAnalyze();
        if(init)
            return;
    }
    
    if(this->argument != NULL)
        this->argument->semAnalyze(restrictIdents);
	
    this->typeAnalyze();
    
     if(this->next != NULL)
        this->next->semAnalyze();
    
}

void ASTFunctionCallNode::scopeAnalyze(){
    
	funcDec = (ASTFunctionNode *)(sa->getST()->getDeclaration(id, lineNumber));
    
}

void ASTFunctionCallNode::typeAnalyze() {
	if(funcDec == NULL) {
		type = -1;
		return;
	}
	
	type = funcDec->declarationType;
	
	if(funcDec->getParamCount() != getArgCount()) {
		// Semantic error - incorrect number of parameters
		sa->semanticError("Incorrect number of arguments", lineNumber);
		return;
	}
	
	// Check all of the params for correct types
	ASTExpressionNode * arg = argument;
	ASTParamNode * param = funcDec->param;
	while(arg != NULL && param != NULL) {
		ASTVariableNode * argAsVar = dynamic_cast<ASTVariableNode *>(arg);
		
		if((arg->type == Scanner::NUM || arg->type == Scanner::INT) &&
				(param->declarationType == Scanner::NUM || param->declarationType == Scanner::INT)) {
			
		}
		else if((arg->type == Scanner::BLIT || arg->type == Scanner::BOOL) &&
				(param->declarationType == Scanner::BLIT || param->declarationType == Scanner::BOOL)) {
			
		}
		else if(argAsVar != NULL) {
			if(argAsVar->varDec->isArray) {
				if(argAsVar->isArray == param->isArray) {
					// Semantic error - mismatched argument types
					sa->semanticError("Mismatched argument types", lineNumber);
				}
			}
			else {
				if(argAsVar->isArray != param->isArray) {
					// Semantic error - mismatched argument types
					sa->semanticError("Mismatched argument types", lineNumber);
				}
			}
		}
		else {
			// Semantic error - mismatched argument types
			sa->semanticError("Mismatched argument types", lineNumber);
		}
		arg = dynamic_cast<ASTExpressionNode *>(arg->next);
		param = (ASTParamNode *)(param->next);
	}
        
        //checks that stmt function calls are void and expression stmt calls are not
         if (isStatement && type != Scanner::VOID)
             sa->semanticError("Function call not void",lineNumber);
        else if(!isStatement && (type != Scanner::INT && type!= Scanner::BOOL))
                sa->semanticError("Function call must have return type",lineNumber);
        
}

void ASTFunctionCallNode::printNode(int indent, ostream * output) {
	this->ASTStatementNode::output = output;
	
	ASTStatementNode::printIndented("function call", indent);
	if(ASTNode::lookup != NULL) {
		ASTStatementNode::printIndented("id: " + ASTNode::lookup->getIdentifierName(id), indent + 2);
	}
	
	if(argument != NULL) {
		ASTStatementNode::printIndented("args:", indent + 2);
		argument->printNode(indent + 4, output);
	}
	
	if(ASTStatementNode::next != NULL) {
		ASTStatementNode::next->printNode(indent, output);
	}
}

int ASTFunctionCallNode::getArgCount() {
	int count = 0;
	ASTExpressionNode * arg = argument;
	
	while(arg != NULL) {
		count++;
		arg = dynamic_cast<ASTExpressionNode *>(arg->next);
	}
	
	return count;
}