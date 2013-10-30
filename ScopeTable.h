/* 
 * File:   ScopeTable.h
 * Author: daniel
 *
 * Created on October 27, 2013, 11:41 PM
 */

#ifndef SCOPETABLE_H
#define	SCOPETABLE_H

#include <vector>
#include "IdentificationTableItem.h"
#include "Admin.h"
class ASTDeclarationNode;

/* This class handles the access and identification tables for the scope
 * analyzer. At any level of scope, this class can produce the set of
 * available identifiers and their respective AST nodes
 */
class ScopeTable {
public:
	ScopeTable();
	ScopeTable(Admin * adminRef);
	ScopeTable(const ScopeTable& orig);
	virtual ~ScopeTable();
	
	// Not using this function in the current implementation
	void initAccessTable(int length);
	
	// Scope/declaration-related functions
	bool isInScope(int id);
	void insertDeclaration(int id, ASTDeclarationNode * decNode);
	ASTDeclarationNode * getDeclaration(int id, int lineNumber);
	void enterBlock();
	void exitBlock();
private:
	int blockLevel;
	vector<int> accessTable;
	vector<IdentificationTableItem> identificationTable;
	Admin * admin;
};

#endif	/* SCOPETABLE_H */

