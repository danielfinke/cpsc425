/* 
 * File:   SyncSetBuilder.cpp
 * Author: daniel
 * 
 * Created on October 15, 2013, 2:02 PM
 */

#include "SyncSetBuilder.h"
#include "Scanner.h"

SyncSetBuilder::SyncSetBuilder() {
}

SyncSetBuilder::SyncSetBuilder(const SyncSetBuilder& orig) {
}

SyncSetBuilder::~SyncSetBuilder() {
}

/* Looks up the sync set for a given function name or nickname
 * Returns an empty set if the function name not in the map
 */
vector<int> SyncSetBuilder::getSyncSet(string functionName) {
	if(syncSetMap.find(functionName) != syncSetMap.end()) {
		return syncSetMap.at(functionName);
	}
	else {
		return vector<int>();
	}
}

unordered_map<string, vector<int> > SyncSetBuilder::syncSetMap;

/* A massive list of all the sync/follow sets of various productions and options
 */
void SyncSetBuilder::buildSyncSetMap() {
	vector<int> syncSet;
	
	syncSet.push_back(Scanner::ENDFILE);
	syncSetMap.insert(make_pair("program", syncSet));
	syncSet.clear();
	
	syncSet.push_back(Scanner::INT);
	syncSet.push_back(Scanner::BOOL);
	syncSet.push_back(Scanner::VOID);
	syncSet.push_back(Scanner::ENDFILE);
	syncSetMap.insert(make_pair("declaration", syncSet));
	syncSet.clear();
	
	syncSet.push_back(Scanner::ID);
	syncSetMap.insert(make_pair("nonVoidSpecifier", syncSet));
	syncSet.clear();
	
	syncSet.push_back(Scanner::INT);
	syncSet.push_back(Scanner::BOOL);
	syncSet.push_back(Scanner::VOID);
	syncSet.push_back(Scanner::ENDFILE);
	syncSetMap.insert(make_pair("decTail", syncSet));
	syncSet.clear();
	
	syncSet.push_back(Scanner::INT);
	syncSet.push_back(Scanner::BOOL);
	syncSet.push_back(Scanner::VOID);
	syncSet.push_back(Scanner::ENDFILE);
	syncSet.push_back(Scanner::LCRLY);
	syncSet.push_back(Scanner::IF);
	syncSet.push_back(Scanner::LOOP);
	syncSet.push_back(Scanner::EXIT);
	syncSet.push_back(Scanner::CONTINUE);
	syncSet.push_back(Scanner::RETURN);
	syncSet.push_back(Scanner::SEMI);
	syncSet.push_back(Scanner::ID);
	syncSet.push_back(Scanner::BRANCH);
	syncSetMap.insert(make_pair("varDecTail", syncSet));
	syncSet.clear();
	
	syncSet.push_back(Scanner::COMMA);
	syncSet.push_back(Scanner::SEMI);
	syncSetMap.insert(make_pair("varName", syncSet));
	syncSet.clear();
	
	syncSet.push_back(Scanner::INT);
	syncSet.push_back(Scanner::BOOL);
	syncSet.push_back(Scanner::VOID);
	syncSet.push_back(Scanner::ENDFILE);
	syncSetMap.insert(make_pair("funDecTail", syncSet));
	syncSet.clear();
	
	syncSet.push_back(Scanner::RPAREN);
	syncSetMap.insert(make_pair("params", syncSet));
	syncSetMap.insert(make_pair("arguments", syncSet));
	syncSet.clear();
	
	syncSet.push_back(Scanner::COMMA);
	syncSet.push_back(Scanner::RPAREN);
	syncSetMap.insert(make_pair("param", syncSet));
	syncSet.clear();
	
	syncSet.push_back(Scanner::LCRLY);
	syncSet.push_back(Scanner::IF);
	syncSet.push_back(Scanner::LOOP);
	syncSet.push_back(Scanner::EXIT);
	syncSet.push_back(Scanner::CONTINUE);
	syncSet.push_back(Scanner::RETURN);
	syncSet.push_back(Scanner::SEMI);
	syncSet.push_back(Scanner::ID);
	syncSet.push_back(Scanner::RCRLY);
	syncSet.push_back(Scanner::ELSE);
	syncSet.push_back(Scanner::END);
	syncSet.push_back(Scanner::BRANCH);
	syncSet.push_back(Scanner::CASE);
	syncSet.push_back(Scanner::DEFAULT);
	syncSetMap.insert(make_pair("statement", syncSet));
	syncSetMap.insert(make_pair("idStmt", syncSet));
	syncSetMap.insert(make_pair("idStmtTail", syncSet));
	syncSetMap.insert(make_pair("assignStmtTail", syncSet));
	syncSetMap.insert(make_pair("callStmtTail", syncSet));
	syncSetMap.insert(make_pair("ifStmt", syncSet));
	syncSetMap.insert(make_pair("loopStmt", syncSet));
	syncSetMap.insert(make_pair("exitStmt", syncSet));
	syncSetMap.insert(make_pair("continueStmt", syncSet));
	syncSetMap.insert(make_pair("returnStmt", syncSet));
	syncSetMap.insert(make_pair("nullStmt", syncSet));
	syncSetMap.insert(make_pair("branchStmt", syncSet));
	syncSetMap.insert(make_pair("caseStmt", syncSet));
	syncSet.push_back(Scanner::INT);
	syncSet.push_back(Scanner::VOID);
	syncSet.push_back(Scanner::BOOL);
	syncSet.push_back(Scanner::ENDFILE);
	syncSetMap.insert(make_pair("compoundStmt", syncSet));
	syncSet.clear();
	
	syncSet.push_back(Scanner::RPAREN);
	syncSet.push_back(Scanner::SEMI);
	syncSet.push_back(Scanner::COMMA);
	syncSetMap.insert(make_pair("expression", syncSet));
	syncSet.push_back(Scanner::RSQR);
	syncSet.push_back(Scanner::LTEQ);
	syncSet.push_back(Scanner::LT);
	syncSet.push_back(Scanner::GT);
	syncSet.push_back(Scanner::GTEQ);
	syncSet.push_back(Scanner::EQ);
	syncSet.push_back(Scanner::NEQ);
	syncSetMap.insert(make_pair("addExp", syncSet));
	syncSet.push_back(Scanner::PLUS);
	syncSet.push_back(Scanner::MINUS);
	syncSet.push_back(Scanner::OR);
	syncSet.push_back(Scanner::ORELSE);
	syncSetMap.insert(make_pair("term", syncSet));
	syncSet.push_back(Scanner::MULT);
	syncSet.push_back(Scanner::DIV);
	syncSet.push_back(Scanner::MOD);
	syncSet.push_back(Scanner::AND);
	syncSet.push_back(Scanner::ANDTHEN);
	syncSetMap.insert(make_pair("callTail", syncSet));
	syncSetMap.insert(make_pair("factor", syncSet));
	syncSetMap.insert(make_pair("nidFactor", syncSet));
	syncSetMap.insert(make_pair("idFactor", syncSet));
	syncSetMap.insert(make_pair("idTail", syncSet));
	syncSetMap.insert(make_pair("varTail", syncSet));
	syncSet.clear();
	
	// Now starting on follow sets for option and repetition phrases
	syncSet.push_back(Scanner::RPAREN);
	syncSetMap.insert(make_pair("{, param}", syncSet));
	syncSetMap.insert(make_pair("[arguments]", syncSet));
	syncSetMap.insert(make_pair("{, expression}", syncSet));
	syncSet.push_back(Scanner::COMMA);
	syncSetMap.insert(make_pair("[ref]", syncSet));
	syncSetMap.insert(make_pair("[]", syncSet));
	syncSet.push_back(Scanner::RSQR);
	syncSet.push_back(Scanner::SEMI);
	syncSetMap.insert(make_pair("[relop add-exp]", syncSet));
	syncSet.push_back(Scanner::RSQR);
	syncSet.push_back(Scanner::LTEQ);
	syncSet.push_back(Scanner::LT);
	syncSet.push_back(Scanner::GT);
	syncSet.push_back(Scanner::GTEQ);
	syncSet.push_back(Scanner::EQ);
	syncSet.push_back(Scanner::NEQ);
	syncSetMap.insert(make_pair("{addop term}", syncSet));
	syncSet.push_back(Scanner::PLUS);
	syncSet.push_back(Scanner::MINUS);
	syncSet.push_back(Scanner::OR);
	syncSet.push_back(Scanner::ORELSE);
	syncSetMap.insert(make_pair("{multop factor}", syncSet));
	syncSet.push_back(Scanner::MULT);
	syncSet.push_back(Scanner::DIV);
	syncSet.push_back(Scanner::MOD);
	syncSet.push_back(Scanner::AND);
	syncSet.push_back(Scanner::ANDTHEN);
	syncSet.push_back(Scanner::ASSIGN);
	syncSetMap.insert(make_pair("[add-exp]", syncSet));
	syncSet.clear();
	
	syncSet.push_back(Scanner::SEMI);
	syncSetMap.insert(make_pair("{, var-name}", syncSet));
	syncSetMap.insert(make_pair("[expression]", syncSet));
	syncSet.push_back(Scanner::INT);
	syncSet.push_back(Scanner::BOOL);
	syncSet.push_back(Scanner::LCRLY);
	syncSet.push_back(Scanner::IF);
	syncSet.push_back(Scanner::LOOP);
	syncSet.push_back(Scanner::EXIT);
	syncSet.push_back(Scanner::CONTINUE);
	syncSet.push_back(Scanner::RETURN);
	syncSet.push_back(Scanner::ID);
	syncSet.push_back(Scanner::BRANCH);
	syncSetMap.insert(make_pair("{nonvoid-specifier ID var-dec-tail}", syncSet));
	syncSet.clear();
	
	syncSet.push_back(Scanner::LCRLY);
	syncSet.push_back(Scanner::IF);
	syncSet.push_back(Scanner::LOOP);
	syncSet.push_back(Scanner::EXIT);
	syncSet.push_back(Scanner::CONTINUE);
	syncSet.push_back(Scanner::RETURN);
	syncSet.push_back(Scanner::SEMI);
	syncSet.push_back(Scanner::ID);
	syncSet.push_back(Scanner::RCRLY);
	syncSet.push_back(Scanner::ELSE);
	syncSet.push_back(Scanner::END);
	syncSet.push_back(Scanner::BRANCH);
	syncSet.push_back(Scanner::CASE);
	syncSet.push_back(Scanner::DEFAULT);
	syncSetMap.insert(make_pair("[else statement]", syncSet));
	syncSet.clear();
	
	syncSet.push_back(Scanner::NOT);
	syncSet.push_back(Scanner::LPAREN);
	syncSet.push_back(Scanner::NUM);
	syncSet.push_back(Scanner::BLIT);
	syncSet.push_back(Scanner::ID);
	syncSetMap.insert(make_pair("[uminus]", syncSet));
}