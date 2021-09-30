#pragma once

#include "pch.h"

#include "BddCppLib/Scanner.h"
#include "BddCppLib/ScanningUtil.h"

#include <sstream>
#include <iterator>
#include <string>
#include <string_view>

namespace {

template<typename T>
T FromString(char const * s)
{
	static std::istringstream iss;
	T value;
	iss.str(s);
	iss.clear();
	iss >> value;
	return value;
}

}

namespace bddcpp {

void Scanner::ScanTokens(TokenList & tokens)
{
	while (MoreChars()) 
	{
		// We are at the beginning of the next token
		start = current;
		ScanToken(tokens);
		Advance();
	}

	tokens.emplace_back(TokenType::Eof, location);
}

void Scanner::ScanToken(TokenList & tokens)
{
	using namespace std;

	char c = *current;
	switch (c)
	{
	case ' ':
	case '\r':
	case '\t':
		// Ignore whitespace.
		break;

	case '\n':
		tokens.emplace_back(TokenType::Eol, location);
		break;

	case '#':
		ScanComment();
		break;

	case '"':
		ScanString(tokens);
		break;

	default:
		if (IsAlpha(c)) 
		{
			ScanWord(tokens);
		}
		else if (IsDigit(c))
		{
			ScanNumber(tokens);
		}
		else 
		{
			string message = "Unexpected character '";
			message += c;
			message += "'.";
			Error(message);
		}
	}
}

void Scanner::Advance()
{
	if (*current == '\n')
	{
		location.NextLine();
		++current;
		line = current;
	}
	else
	{
		location.NextColumn();
		++current;
	}
}

void Scanner::ScanComment()
{
	while (MoreChars() && NextChar() != '\n')
	{
		Advance();
	}
}

void Scanner::ScanWord(TokenList & tokens)
{
	while (IsAlphaNumeric(NextChar()))
	{
		Advance();
	}

	if (NextChar() == ':')
	{
		ScanKeyword(tokens);
		Advance();
		return;
	}

	if (IsEnum(start, TokenLength()))
	{
		tokens.emplace_back(TokenType::Enum, location, start, TokenLength());
		return;
	}

	tokens.emplace_back(TokenType::Word, location, start, TokenLength());
}

void Scanner::ScanKeyword(TokenList & t)
{
	ScanKeyword("Background", TokenType::Background, t) ||
	ScanKeyword("Example", TokenType::Example, t) ||
	ScanKeyword("Feature", TokenType::Feature, t) ||
	ScanKeyword("Scenario", TokenType::Scenario, t) ||
	ScanKeyword("Scenario Outline", TokenType::ScenarioOutline, t) ||
	ScanKeyword("Rule", TokenType::Rule, t) ||
	InvalidKeyword();
}

bool Scanner::InvalidKeyword()
{
	using namespace std;
	string_view keyword(start, TokenLength());
	string message = "Invalid keyword \"";
	message += keyword;
	message += "\".";
	Error(message);
	return false;
}

bool Scanner::ScanKeyword(char const *keyword, TokenType type,  TokenList & tokens)
{
	if (strncmp(start, keyword, TokenLength()) == 0)
	{
		tokens.emplace_back(type, location, start, TokenLength());
		return true;
	}
	return false;
}

void Scanner::ScanNumber(TokenList & tokens)
{
	while (IsDigit(NextChar()))
	{
		Advance();
	}

	if (NextChar() != '.')
	{
		std::string_view text(start, TokenLength());
		TokenValue value = FromString<int32_t>(text.data());
		tokens.emplace_back(TokenType::Int, location, start, TokenLength(), value);
		return;
	}

	// Must be a real number.
	Advance();
	if (!IsDigit(NextChar()))
	{
		Error("Real number is missing its fractional part.");
		return;
	}

	while (IsDigit(NextChar()))
	{
		Advance();
	}

	std::string_view text(start, TokenLength());
	TokenValue value = FromString<double>(text.data());
	tokens.emplace_back(TokenType::Real, location, start, TokenLength(), value);
}

void Scanner::ScanString(TokenList & tokens)
{
	using namespace std;

	string text;

	for (;;)
	{
		if (!MoreChars())
		{
			Error("Unterminated string.");
			return;
		}

		char c = NextChar();
		switch (c)
		{
			case '"':
			{
				Advance();
				TokenValue value = text;
				tokens.emplace_back(TokenType::String, location, start, TokenLength(), text);
				return;
			}
			case '\n':
			{
				Error("Unterminated string.");
				return;
			}
			default:
			{
				Advance();
				text += c;
			}
		}
	}
}

bool Scanner::MoreChars() const
{
	return *(current + 1) != '\0';
}

char Scanner::NextChar() const
{
	return *(current + 1);
}

size_t Scanner::TokenLength() const
{
	return std::distance(start, current) + 1;
}

size_t Scanner::LineLength() const
{
	return std::distance(line, start);
}

void Scanner::Error(std::string message)
{
	using namespace std;
	ostringstream fullMessage;
	fullMessage
		<< location.GetFileName() << " ("
		<< location.GetLine() << ":" << location.GetColumn() << ") Error: "
		<< message;
	errors.push_back(fullMessage.str());

	string_view lineText(line, LineLength());
	string_view tokenText(start, TokenLength());
	ostringstream context;
	context
		<< location.GetFileName() << " ("
		<< location.GetLine() << ":" << location.GetColumn() << ")  Line: "
		<< lineText << "|" << tokenText << "|";
	errors.push_back(context.str());
}

} // namespace bddcpp
