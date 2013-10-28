/* 
 * File:   IdentificationTableItem.cpp
 * Author: daniel
 * 
 * Created on October 28, 2013, 12:53 AM
 */

#include "IdentificationTableItem.h"

IdentificationTableItem::IdentificationTableItem() : blockLevel(0),
		next(0), lexI(-1), decPtr(NULL)
{
}

IdentificationTableItem::IdentificationTableItem(int level, int next, int lexicalIndex, ASTDeclarationNode * decNode) :
blockLevel(level), next(next), lexI(lexicalIndex), decPtr(decNode)
{
}

IdentificationTableItem::IdentificationTableItem(const IdentificationTableItem& orig) :
blockLevel(orig.blockLevel), next(orig.next), lexI(orig.lexI), decPtr(orig.decPtr)
{
}

IdentificationTableItem::~IdentificationTableItem() {
}

int IdentificationTableItem::getBlockLevel() { return blockLevel; }
int IdentificationTableItem::getNext() { return next; }
int IdentificationTableItem::getLexicalIndex() { return lexI; }
ASTDeclarationNode * IdentificationTableItem::getDecPtr() { return decPtr; }

void IdentificationTableItem::setNext(int next) { this->next = next; }