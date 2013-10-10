/* 
 * File:   ASTMarkerNode.h
 * Author: daniel
 *
 * Created on October 8, 2013, 2:07 PM
 */

#ifndef ASTMARKERNODE_H
#define	ASTMARKERNODE_H

#include "ASTStatementNode.h"

/*The ASTMarkerNode is used to represent the three markers: exit, continue, and null
 * which only contain their key word (in the case of null it contains nothing), 
 * and a semi colon.It is a subclass of ASTStatementNode and only contains the
 * type (exit, continue, nullstmt) is is as an ENUM.
 */
class ASTMarkerNode : public ASTStatementNode {
public:
	ASTMarkerNode();
	ASTMarkerNode(const ASTMarkerNode& orig);
        ASTMarkerNode& operator= (const ASTMarkerNode &rhs);
	virtual ~ASTMarkerNode();
	
	void printNode(int indent, ostream * output);
	
	int type;
private:

};

#endif	/* ASTMARKERNODE_H */

