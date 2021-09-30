#pragma once

namespace bddcpp {

inline bool IsAlpha(char c)
{
	return
		(c >= 'a' && c <= 'z') ||
		(c >= 'A' && c <= 'Z') ||
		c == '_';
}

inline bool IsLowerCase(char c)
{
	return c >= 'a' && c <= 'z';
}

inline bool IsDigit(char c)
{
	return c >= '0' && c <= '9';
}

inline bool IsAlphaNumeric(char c)
{
	return IsAlpha(c) || IsDigit(c);
}

inline bool IsEnum(char const *s, size_t length)
{
	if (length < 2)
		return false;

	for (size_t i = 0; i < length; ++i, ++s)
	{
		if (IsLowerCase(*s))
			return false;
	}

	return true;
}

} // namespace bddcpp
