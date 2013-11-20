/* 
 * File:   QuadrupleGenerator.cpp
 * Author: claire
 * 
 * Created on November 19, 2013, 1:35 PM
 */

#include "QuadrupleGenerator.h"

QuadrupleGenerator::QuadrupleGenerator() {
}

QuadrupleGenerator::QuadrupleGenerator(const QuadrupleGenerator& orig) {
}

QuadrupleGenerator::~QuadrupleGenerator() {
}

void QuadrupleGenerator:: GenerateQuadruples(ASTNode* top){
    top->vec.push_back(Quadruple("start", "1", "", ""));
	top->vec.push_back(Quadruple("rval", "", "", "t1"));
	top->vec.push_back(Quadruple("call", "main", "", ""));
	top->vec.push_back(Quadruple("hlt", "", "", ""));
	
	top->genQuadruples();
	
	for(int i = 0; i < top->vec.size(); i++) {
		cout << top->vec[i].getQuadrupleTuple() << endl;
	}
}
