#pragma once

#include "BddCppLib/Token.h"
#include "BddCppLib/SourceLocation.h"

#include <string>

namespace bddcpp {

class Scanner
{
public:
	typedef std::vector<std::string> Errors;

	Scanner(std::string fileName, char const * source) :
		location(fileName),
		line(source),
		start(source),
		current(source)
	{
	}

	void ScanTokens(TokenList & tokens);

	bool Successful() const
	{
		return errors.empty();
	}

	bool HasErrors() const
	{
		return !errors.empty();
	}

	Errors GetErrors() const
	{
		return errors;
	}

private:
	void Advance();
	void ScanToken(TokenList & tokens);
	void ScanComment();
	void ScanWord(TokenList & tokens);
	void ScanKeyword(TokenList & tokens);
	bool ScanKeyword(char const * keyword, TokenType type, TokenList & tokens);
	bool InvalidKeyword();
	void ScanNumber(TokenList & tokens);
	void ScanString(TokenList & tokens);
	bool MoreChars() const;
	char NextChar() const;
	size_t TokenLength() const;
	size_t LineLength() const;
	void Error(std::string message);

	SourceLocation location;
	char const * line;
	char const * start;
	char const * current;
	Errors errors;
};

} // namespace bddcpp
