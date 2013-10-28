/* 
 * File:   ScopeTable.cpp
 * Author: daniel
 * 
 * Created on October 27, 2013, 11:41 PM
 */

#include "ScopeTable.h"

ScopeTable::ScopeTable() : blockLevel(0), accessTable(vector<int>()),
		identificationTable(vector<IdentificationTableItem>())
{
	// Push the null item onto the identification stack
	identificationTable.push_back(IdentificationTableItem());
}

ScopeTable::ScopeTable(const ScopeTable& orig) : blockLevel(0),
accessTable(orig.accessTable),
		identificationTable(orig.identificationTable)
{
}

ScopeTable::~ScopeTable() {
	accessTable.clear();
	identificationTable.clear();
}

/* Prepares the access table to match the size of the Scanner's symbol table
 */
void ScopeTable::initAccessTable(int length) {
	for(int i = 0; i < length; i++) {
		accessTable.push_back(0);
	}
}

/* Returns true if the selected identifier is within the current scope
 */
bool ScopeTable::isInScope(int id) { return accessTable[id] != 0; }

/* Adds a new declaration to the identifier table and updates the access table.
 * Also throws semantic errors if the declaration is being redefined a second
 * time
 */
void ScopeTable::insertDeclaration(int id, ASTDeclarationNode * decNode) {
	// New declaration
	if(accessTable[id] == 0) {
		identificationTable.push_back(IdentificationTableItem(blockLevel, 0, id, decNode));
		accessTable[id] = identificationTable.size()-1;
	}
	// Update declaration scope (sub-block)
	else if(identificationTable[accessTable[id]].getBlockLevel() != blockLevel) {
		identificationTable.push_back(IdentificationTableItem(blockLevel, accessTable[id], id, decNode));
		accessTable[id] = identificationTable.size()-1;
	}
	// Semantic error - double definition
	else {
		
	}
}

/* Returns the ASTDeclarationNode specified by the id parameter.
 * May throw an undeclared identifier if the identifier is not available
 * in the current scope
 */
ASTDeclarationNode * ScopeTable::getDeclaration(int id) {
	// Pre-declared identifier - return it
	if(accessTable[id] != 0) {
		return identificationTable[accessTable[id]].getDecPtr();
	}
	// Semantic error - undeclared identifier
	else {
		
	}
}

/* Increase the block level
 */
void ScopeTable::enterBlock() { blockLevel++; }
/* Decrease the block level and remove all identifiers from the scope that is
 * being left. Also, update the access table so that identifiers which hid
 * external definitions can now access those external definitions
 */
void ScopeTable::exitBlock() {
	// Program exception - trying to leave global block
	if(blockLevel == 0) {
		
	}
	
	blockLevel--;
	// From top of stack, remove until out of blockLevel
	for(int i = identificationTable.size()-1; i >= 0 &&
			identificationTable[i].getBlockLevel() > blockLevel; i--) {
		accessTable[identificationTable[i].getLexicalIndex()] =
				identificationTable[i].getNext();
	}
}