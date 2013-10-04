#ifndef Token_H
#define Token_H

#include <string>

using namespace std;

/* The Token class is a general token class for IDs, NUMs, BLITs, and keywords.
 * Each token has a type as defined by the Scanner enum, and may contain a value.
 * For example, NUM (numeral) tokens may have a value of 284, meaning there is an integer literal of 284 in the input.
 *
 * There is a sentinel value of -2 for tokens who do not need an attribute value.
 */
class Token
{
private:
	int tokenType;
	int attributeValue;
public:
	// Constructors/deconstructors/related
	Token(void);
	Token(int type, int attributeValue);
	Token(const Token &other);
        Token& operator= (const Token &rhs);
	~Token(void);

	// Getters/setters
	int getTokenType();
	int getAttributeValue();
};

#endif
