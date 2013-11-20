/* 
 * File:   Quadruple.cpp
 * Author: claire
 * 
 * Created on November 19, 2013, 8:50 PM
 */

#include "Quadruple.h"

Quadruple::Quadruple() {
}

Quadruple::Quadruple(const Quadruple& orig) {
}

Quadruple::~Quadruple() {
}

Quadruple::Quadruple(string oper, string arg1, string arg2, string result){
    
    this->operation = oper;
    this->arg1 = arg1;
    this->arg2=arg2;
    this->result = result;
    
}
