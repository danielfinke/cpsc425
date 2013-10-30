/* 
 * File:   ScopeTable.cpp
 * Author: daniel
 * 
 * Created on October 27, 2013, 11:41 PM
 */

#include "ScopeTable.h"
#include "ASTDeclarationNode.h"

ScopeTable::ScopeTable() : blockLevel(0), accessTable(vector<int>()),
		identificationTable(vector<IdentificationTableItem>()), admin(NULL)
{
	// Push the null item onto the identification stack
	identificationTable.push_back(IdentificationTableItem());
}

ScopeTable::ScopeTable(Admin * adminRef) : blockLevel(0), accessTable(vector<int>()),
		identificationTable(vector<IdentificationTableItem>()), admin(adminRef)
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
	// Out of bounds
	// Using the assumption that the ids will only ever be inserted in order of
	// appearance in program, so only just push back the new one
	while(accessTable.size() <= id) {
		accessTable.push_back(0);
	}
	/*else if(accessTable.size() < id) {
		for(int i = 0; i < accessTable.size(); i++) {
			cout << admin->lookupasdf(identificationTable[accessTable[i]].getDecPtr()->id) << endl;
		}
		cout << admin->lookupasdf(id) << " is " << id << endl;
		throw "Non-sequential insertion of declaration into access table";
	}*/
	
	// New declaration
	if(accessTable[id] == 0) {
		identificationTable.push_back(IdentificationTableItem(blockLevel, 0, id, decNode));
		//cout << "new dec " << id << " is " << admin->lookupasdf(id) << endl;
		accessTable[id] = identificationTable.size()-1;
		//cout << "ident at lex#" << id << " produces " << identificationTable[accessTable[id]].getDecPtr()->id << endl;
	}
	// Update declaration scope (sub-block)
	else if(identificationTable[accessTable[id]].getBlockLevel() != blockLevel) {
		identificationTable.push_back(IdentificationTableItem(blockLevel, accessTable[id], id, decNode));
		accessTable[id] = identificationTable.size()-1;
	}
	// Semantic error - double definition
	else {
		ASTDeclarationNode * decPtr = identificationTable[accessTable[id]].getDecPtr();
		admin->semanticError("Identifier redefinition: "
				+ decPtr->lookup->getIdentifierName(id),
				decPtr->lineNumber);
	}
}

/* Returns the ASTDeclarationNode specified by the id parameter.
 * May throw an undeclared identifier if the identifier is not available
 * in the current scope
 */
ASTDeclarationNode * ScopeTable::getDeclaration(int id, int lineNumber) {
	// Semantic error - undeclared identifier
	if(id >= accessTable.size()) {
		admin->semanticError("Undeclared identifier: " + admin->getIdentifierName(id),
				lineNumber);
	}
	// Pre-declared identifier - return it
	else if(accessTable[id] != 0) {
		return identificationTable[accessTable[id]].getDecPtr();
	}
	// Semantic error - undeclared identifier
	else {
		admin->semanticError("Undeclared identifier: " + admin->getIdentifierName(id),
				lineNumber);
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
		throw "Trying to exit the global block (block 0)";
	}
	
	blockLevel--;
	// From top of stack, remove until out of blockLevel
	for(int i = identificationTable.size()-1; i >= 0 &&
			identificationTable[i].getBlockLevel() > blockLevel; i--) {
		accessTable[identificationTable[i].getLexicalIndex()] =
				identificationTable[i].getNext();
	}
}