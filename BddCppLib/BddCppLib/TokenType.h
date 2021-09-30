#pragma once

namespace bddcpp {

enum class TokenType
{
	None,
	Word,
	Int,
	Real,
	String,
	Enum,
	Background,
	Example,
	Feature,
	Rule,
	Scenario,
	ScenarioOutline,
	Eol,
	Eof,
};

} // namespace bddcpp
