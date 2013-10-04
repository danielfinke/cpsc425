#include "Token.h"


Token::Token(void) : tokenType(-2), attributeValue(-2)
{
}

Token::Token(int tokenType, int attributeValue) : tokenType(tokenType),
	attributeValue(attributeValue) { }
Token::Token(const Token &other) : tokenType(other.tokenType), attributeValue(other.attributeValue) { }
Token& Token::operator= (const Token &rhs)
{
    // do the copy
    tokenType = rhs.tokenType;
	attributeValue = rhs.attributeValue;
 
    // return the existing object
    return *this;
}

Token::~Token(void)
{
}

int Token::getTokenType() { return tokenType; }
int Token::getAttributeValue() { return attributeValue; }