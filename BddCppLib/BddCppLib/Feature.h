#pragma once

#include <string>

namespace bddcpp {

class FeatureDescription
{
public:
	FeatureDescription(std::string const& name, std::string const& description) :
		name(name),
		description(description)
	{
	}

	std::string GetName() const { return name; }
	std::string GetDescription() const { return description; }

private:
	std::string name;
	std::string description;
};

class Feature
{
public:
	Feature(FeatureDescription const & description) :
		description(description)
	{
	}

private:
	FeatureDescription description;
};

} // namespace bddcpp
