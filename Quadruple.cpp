/* 
 * File:   Quadruple.cpp
 * Author: claire
 * 
 * Created on November 19, 2013, 8:50 PM
 */

#include "Quadruple.h"

Quadruple::Quadruple() : operation(""), arg1(""), arg2(""), result("") {
}

Quadruple::Quadruple(const Quadruple& orig) : operation(orig.operation),
		arg1(orig.arg1), arg2(orig.arg2), result(orig.result)
{
}

Quadruple::~Quadruple() {
}

string Quadruple::getQuadrupleTuple() {
	string tuple = "(" + operation + ",";
	if(arg1 != "") {
		tuple += arg1;
	}
	else {
		tuple += "-";
	}
	tuple += ",";
	if(arg2 != "") {
		tuple += arg2;
	}
	else {
		tuple += "-";
	}
	tuple += ",";
	if(result != "") {
		tuple += result;
	}
	else {
		tuple += "-";
	}
	tuple += ")";
	
	return tuple;
}