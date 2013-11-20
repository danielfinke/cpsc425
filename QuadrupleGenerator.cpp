/* 
 * File:   QuadrupleGenerator.cpp
 * Author: claire
 * 
 * Created on November 19, 2013, 1:35 PM
 */

#include "QuadrupleGenerator.h"
#include "ASTVariableDeclarationNode.h"
#include "Admin.h"

QuadrupleGenerator::QuadrupleGenerator() : admin(NULL) {
}

QuadrupleGenerator::QuadrupleGenerator(Admin * admin) :
		admin(admin)
{
}

QuadrupleGenerator::QuadrupleGenerator(const QuadrupleGenerator& orig) :
		admin(orig.admin)
{
}

QuadrupleGenerator::~QuadrupleGenerator() {
}

void QuadrupleGenerator:: GenerateQuadruples(ASTNode* top){
	int numGlobals = 0;
	ASTNode * cur = top;
	
	while(cur != NULL) {
		if(dynamic_cast<ASTVariableDeclarationNode *>(cur) != NULL) {
			numGlobals++;
		}
		cur = cur->next;
	}
	
	stringstream ss;
	ss << numGlobals;
	
    top->vec.push_back(Quadruple("start", ss.str(), "", ""));
	top->vec.push_back(Quadruple("rval", "", "", top->getTemp()));
	top->vec.push_back(Quadruple("call", "main", "", ""));
	top->vec.push_back(Quadruple("hlt", "", "", ""));
	
	top->genQuadruples();
	
	if(admin != NULL) {
		admin->outputExecutable(top->vec);
	}
}
