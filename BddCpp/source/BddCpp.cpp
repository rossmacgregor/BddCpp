// BddCpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

#include "cxxopts.hpp"

#include "BddCppLib/Scanner.h"

struct ProgramOptions
{
	std::vector<std::string> featureFiles;
};

void parseProgramOptions(int argc, char ** argv, ProgramOptions &programOptions)
{
	using namespace std;

	cxxopts::Options options("BddCpp", "Behavior-driven development for Cpp Projects");
	options.add_options()
		("f,feature-file", "Run a feature file", cxxopts::value<vector<string>>())
		//		("d,debug", "Enable debugging", cxxopts::value<bool>()->default_value("false"))
		//		("f,foo", "Param foo", cxxopts::value<int>()->default_value("10"))
		("h,help", "Print usage")
		;

	cxxopts::ParseResult result = options.parse(argc, argv);
	vector<string> unmatchedOptions = result.unmatched();
	if (result.count("help") || !unmatchedOptions.empty())
	{
		cerr << options.help() << "\n";
		exit(1);
	}

	programOptions.featureFiles = result["feature-file"].as<vector<string>>();
}

int main(int argc, char ** argv)
{
	using namespace std;
	using namespace bddcpp;

	ProgramOptions options;

	try
	{
		parseProgramOptions(argc, argv, options);
	}
	catch (exception & e)
	{
		cerr << e.what() << "\n";
		exit(2);
	}

	for (string & fileName : options.featureFiles)
	{
		filesystem::path filePath(fileName);

		ifstream fileStream(fileName);
		if (!fileStream)
			return 1;

		auto fileSize = filesystem::file_size(filePath);
		vector<char> file(fileSize + 1);
		fileStream.read(file.data(), fileSize);
		fileStream.close();
		file[fileSize] = '\0';

		Scanner scanner(fileName, file.data());
		TokenList tokens;
		scanner.ScanTokens(tokens);

		if (scanner.HasErrors())
		{
			auto errors = scanner.GetErrors();
			for (string const & e : errors)
			{
				cerr << e << "\n";
			}
			return 2;
		}

		for (Token const & t : tokens)
		{
			cout
				<< "Token;"
				<< " type=" << static_cast<int>(t.GetType())
				<< " line=" << t.GetLocation().GetLine()
				<< " column=" << t.GetLocation().GetColumn()
				<< " text=" << t.GetText()
				<< " value=";

			TokenValue value = t.GetValue();

			visit([value](const auto & elem) { cout << elem; }, value);

			cout << "\n";
		}
	}

	return 0;
}
