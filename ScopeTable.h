/* 
 * File:   ScopeTable.h
 * Author: daniel
 *
 * Created on October 27, 2013, 11:41 PM
 */

#ifndef SCOPETABLE_H
#define	SCOPETABLE_H

#include <vector>
#include "ASTDeclarationNode.h"
#include "IdentificationTableItem.h"

/* This class handles the access and identification tables for the scope
 * analyzer. At any level of scope, this class can produce the set of
 * available identifiers and their respective AST nodes
 */
class ScopeTable {
public:
	ScopeTable();
	ScopeTable(const ScopeTable& orig);
	virtual ~ScopeTable();
	
	void initAccessTable(int length);
	
	// Scope/declaration-related functions
	bool isInScope(int id);
	void insertDeclaration(int id, ASTDeclarationNode * decNode);
	ASTDeclarationNode * getDeclaration(int id);
	void enterBlock();
	void exitBlock();
private:
	int blockLevel;
	vector<int> accessTable;
	vector<IdentificationTableItem> identificationTable;
};

#endif	/* SCOPETABLE_H */

