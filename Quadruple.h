/* 
 * File:   Quadruple.h
 * Author: claire
 *
 * Created on November 19, 2013, 8:50 PM
 */

#ifndef QUADRUPLE_H
#define	QUADRUPLE_H
#include <string>
using namespace std;

/*
 *Quadruple class represents the quadruples which are created in the quadruple
 * generator. Has an operation, arg1, arg2, and result.
 */
class Quadruple {
public:
    Quadruple();
    Quadruple(const Quadruple& orig);
    virtual ~Quadruple();
    Quadruple(string oper, string arg1, string arg2, string result);
    
    string operation;
    string arg1;
    string arg2;
    string result;
	
	string getQuadrupleTuple();
private:

    
    
};

#endif	/* QUADRUPLE_H */

