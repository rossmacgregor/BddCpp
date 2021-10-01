#pragma once

#include "BddCppLib/Token.h"
#include "BddCppLib/Feature.h"

#include <string>

namespace bddcpp {

class Parser
{
public:
	typedef std::vector<std::string> Errors;

	Parser(TokenList const & tokens);

	Feature Parse();

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
	bool MoreTokens() const;
	char NextToken() const;
	size_t StatementLength() const;
	size_t LineLength() const;
	void Error(std::string message);

	TokenList::const_iterator start;
	TokenList::const_iterator current;
	TokenList const & tokens;
	Errors errors;
};

} // namespace bddcpp
