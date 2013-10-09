/* 
 * File:   ASTMarkerNode.h
 * Author: daniel
 *
 * Created on October 8, 2013, 2:07 PM
 */

#ifndef ASTMARKERNODE_H
#define	ASTMARKERNODE_H

#include "ASTStatementNode.h"

class ASTMarkerNode : public ASTStatementNode {
public:
	ASTMarkerNode();
	ASTMarkerNode(const ASTMarkerNode& orig);
        ASTMarkerNode& operator= (const ASTMarkerNode &rhs);
	virtual ~ASTMarkerNode();
	
	void printNode(int indent);
	
	int type;
private:

};

#endif	/* ASTMARKERNODE_H */

