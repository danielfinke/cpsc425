/* 
 * File:   QuadrupleGenerator.h
 * Author: claire
 *
 * Created on November 19, 2013, 1:35 PM
 */

#ifndef QUADRUPLEGENERATOR_H
#define	QUADRUPLEGENERATOR_H
#include "ASTNode.h"

class QuadrupleGenerator {
public:
    QuadrupleGenerator();
    QuadrupleGenerator(const QuadrupleGenerator& orig);
    virtual ~QuadrupleGenerator();
    
    void GenerateQuadruples(ASTNode* top);
private:

    
};

#endif	/* QUADRUPLEGENERATOR_H */
