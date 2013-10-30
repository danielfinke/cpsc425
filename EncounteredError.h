/* 
 * File:   EncounteredError.h
 * Author: daniel
 *
 * Created on October 29, 2013, 7:07 PM
 */

#ifndef ENCOUNTEREDERROR_H
#define	ENCOUNTEREDERROR_H

class EncounteredError {
public:
	EncounteredError();
	EncounteredError(int blockLevel, int id);
	EncounteredError(const EncounteredError& orig);
	virtual ~EncounteredError();
	
	int getBlockLevel();
	int getId();
private:
	int blockLevel, id;
};

#endif	/* ENCOUNTEREDERROR_H */

