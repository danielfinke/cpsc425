/* 
 * File:   SyncSetBuilder.h
 * Author: daniel
 *
 * Created on October 15, 2013, 2:02 PM
 */

#ifndef SYNCSETBUILDER_H
#define	SYNCSETBUILDER_H

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class SyncSetBuilder {
public:
	SyncSetBuilder();
	SyncSetBuilder(const SyncSetBuilder& orig);
	virtual ~SyncSetBuilder();
	
	static vector<int> getSyncSet(string functionName);
	static void buildSyncSetMap();
private:
	static unordered_map<string, vector<int> > syncSetMap;
};

#endif	/* SYNCSETBUILDER_H */

