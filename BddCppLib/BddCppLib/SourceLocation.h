#pragma once

#include <string>

namespace bddcpp {

class SourceLocation
{
public:
    SourceLocation(std::string fileName) :
        fileName(fileName)
    {
    }
    std::string GetFileName() const
    {
        return fileName;
    }
    int GetLine() const
    {
        return line;
    }
    int GetColumn() const
    {
        return column;
    }
    void NextLine()
    {
        ++line;
        column = 1;
    }
    void NextColumn()
    {
        ++column;
    }
private:
    std::string fileName;
    int line = 1;
    int column = 1;
};

} // namespace bddcpp
