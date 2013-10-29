/* 
 * File:   SemanticAnalyzer.cpp
 * Author: daniel
 * 
 * Created on October 27, 2013, 11:22 PM
 */

#include "SemanticAnalyzer.h"

SemanticAnalyzer::SemanticAnalyzer() {
}

SemanticAnalyzer::SemanticAnalyzer(const SemanticAnalyzer& orig) {
}

SemanticAnalyzer::~SemanticAnalyzer() {
}

void SemanticAnalyzer::semAnalyze(ASTNode* topNode){
    
    topNode->init = true;
    topNode->semAnalyze();
    topNode->init = false;
    topNode->semAnalyze();
}