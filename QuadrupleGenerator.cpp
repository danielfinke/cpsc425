/* 
 * File:   QuadrupleGenerator.cpp
 * Author: claire
 * 
 * Created on November 19, 2013, 1:35 PM
 */

#include "QuadrupleGenerator.h"
#include "ASTVariableDeclarationNode.h"
#include "ASTLiteralNode.h"
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
	
        //calculates number of global variables
	while(cur != NULL) {
		ASTVariableDeclarationNode * decAsVar =
				dynamic_cast<ASTVariableDeclarationNode *>(cur);
		if(decAsVar != NULL) {
			if(!decAsVar->isArray) {
				numGlobals++;
				decAsVar->level = cur->curLevel;
				decAsVar->displacement = cur->curDisplacement;
				cur->curDisplacement++;
			}
			else {
				ASTLiteralNode * sizeVal = decAsVar->arrayExp->calc();
				numGlobals += sizeVal->value;
				decAsVar->level = cur->curLevel;
				decAsVar->displacement = cur->curDisplacement;
				cur->curDisplacement += sizeVal->value;
			}
		}
		cur = cur->next;
	}
	
	stringstream ss;
	ss << numGlobals;
	
        //creates the 4 standard quadruples to start the file
    top->vec.push_back(Quadruple("start", ss.str(), "", ""));
	top->vec.push_back(Quadruple("rval", "", "", top->getTemp()));
	top->vec.push_back(Quadruple("call", "main", "", ""));
	top->vec.push_back(Quadruple("hlt", "", "", ""));
	
	// Update temp size
	// (Only 1 extra displacement because of main return value)
	ss.str("");
	ss << top->curDisplacement;
	top->vec[0].arg1 = ss.str();
	
        //starts the process of generating the rest of the quadruples
	top->genQuadruples();
	
        //outputs the executable quadruple code
	if(admin != NULL) {
		admin->outputExecutable(top->vec);
	}
}
