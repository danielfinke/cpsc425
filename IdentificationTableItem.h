/* 
 * File:   IdentificationTableItem.h
 * Author: daniel
 *
 * Created on October 28, 2013, 12:53 AM
 */

#ifndef IDENTIFICATIONTABLEITEM_H
#define	IDENTIFICATIONTABLEITEM_H

#include "ASTDeclarationNode.h"

/* Used to represent a single item in the identification table
 */
class IdentificationTableItem {
public:
	IdentificationTableItem();
	IdentificationTableItem(int level, int next, int lexicalIndex, ASTDeclarationNode * decNode);
	IdentificationTableItem(const IdentificationTableItem& orig);
	virtual ~IdentificationTableItem();
	
	// Getters/setters
	int getBlockLevel();
	int getNext();
	int getLexicalIndex();
	ASTDeclarationNode * getDecPtr();
	void setNext(int next);
private:
	int blockLevel, next, lexI;
	ASTDeclarationNode * decPtr;
};

#endif	/* IDENTIFICATIONTABLEITEM_H */

