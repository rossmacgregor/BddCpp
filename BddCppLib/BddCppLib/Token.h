#pragma once

#include "BddCppLib/TokenType.h"
#include "BddCppLib/SourceLocation.h"

#include <string_view>
#include <vector>
#include <variant>
#include <cstdint>

namespace bddcpp {

using TokenValue = std::variant<int32_t, double, std::string>;

class Token
{
public:
	Token(TokenType type, SourceLocation location) :
		type(type),
		location(location)
	{
	}
	Token(TokenType type, SourceLocation location, char const * s, size_t length) :
		type(type),
		location(location),
		text(s, length)
	{
	}
    Token(TokenType type, SourceLocation location, char const * s, size_t length, TokenValue value) :
        type(type),
		location(location),
        text(s, length),
        value(value)
    {
    }

    TokenType GetType() const
    {
        return type;
    }

	SourceLocation GetLocation() const
	{
		return location;
	}

	std::string_view GetText() const
	{
		return text;
	}

	TokenValue GetValue() const
	{
		return value;
	}

private:
    TokenType type = TokenType::None;
	SourceLocation location;
    std::string_view text;
    TokenValue value;
};

using TokenList = std::vector<Token>;

} // namespace bddcpp
