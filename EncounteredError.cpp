/* 
 * File:   EncounteredError.cpp
 * Author: daniel
 * 
 * Created on October 29, 2013, 7:07 PM
 */

#include "EncounteredError.h"

EncounteredError::EncounteredError() : blockLevel(0), id(0) {
}

EncounteredError::EncounteredError(int blockLevel, int id) : blockLevel(blockLevel),
		id(id)
{
}

EncounteredError::EncounteredError(const EncounteredError& orig) :
	blockLevel(orig.blockLevel), id(orig.id)
{
}

EncounteredError::~EncounteredError() {
}

int EncounteredError::getBlockLevel() { return blockLevel; }
int EncounteredError::getId() { return id; }