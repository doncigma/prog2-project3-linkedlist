#include <iostream>
#include <string>
#include <sstream>

#include "helpers.hpp"

using namespace std;

void PrintError(int line, int position, const string &message)
{
    cerr << line << ":" << position + 1 << ": Error - " << message << endl;
}

std::string trim(const std::string &str)
{
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos)
    {
        return ""; // No non-whitespace characters
    }
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

bool isWhitespace(const std::string &str)
{
    return str.find_first_not_of(" \t\n\r") == std::string::npos;
}

std::vector<std::string> SplitString(const std::string &str, char delimiter)
{
    std::vector<std::string> result;
    std::string current;

    bool inQuote = false;
    for (char c : str)
    {
        if (c == '\"')
        {
            inQuote = !inQuote;
        }
        else if (c == delimiter && !inQuote)
        {
            if (!current.empty())
            {
                result.push_back(current);
                current.clear();
            }
        }
        else
        {
            current += c;
        }
    }

    if (!current.empty())
    {
        result.push_back(current);
    }

    return result;
}

void AddCommands(const std::vector<TestFunctionEntry> &commands, std::map<std::string, TestFunctionEntry> &commandMap)
{
    for (auto &command : commands)
    {
        commandMap[command._name] = command;
    }
}

std::vector<std::string> FindPrefixMatch(const std::map<std::string, TestFunctionEntry> &commandMap, const std::string &prefix)
{
    std::vector<std::string> result;

    for (auto it = commandMap.lower_bound(prefix); it != commandMap.end() && it->first.substr(0, prefix.length()) == prefix; it++)
    {
        result.push_back(it->first);
    }

    return result;
}

void CompareStreams(std::istream& stream1, std::istream& stream2, std::ostream& output, const string stream1NamePrefix, const string stream2NamePrefix)
{
    std::string line1, line2;
    int lineNum = 1;

    // Go to the beginning of each stream
    stream1.clear();
    stream1.seekg(0, std::ios::beg);
    stream2.clear();
    stream2.seekg(0, std::ios::beg);
    
    while (std::getline(stream1, line1) && std::getline(stream2, line2))
    {
        if (line1 != line2)
        {
            output << "Line " << lineNum << " differs:" << std::endl;
            output << stream1NamePrefix << line1 << std::endl;
            output << stream2NamePrefix << line2 << std::endl;
        }
        lineNum++;
    }

    // Check if one stream has more lines than the other
    while (std::getline(stream1, line1))
    {
        output << "Stream 1 has extra line: " << line1 << std::endl;
        lineNum++;
    }

    while (std::getline(stream2, line2))
    {
        output << "Stream 2 has extra line: " << line2 << std::endl;
        lineNum++;
    }
}

void ParseLine(const std::string &line, std::string &command, std::string &result, std::string &comment, char commandDelimiter, char commentDelimiter)
{
    size_t commentStart = line.find(commentDelimiter);
    if (commentStart != std::string::npos)
    {
        comment = line.substr(commentStart);
    }
    else
    {
        comment = "";
    }

    string nonComment = line.substr(0, commentStart);

    size_t resultStart = nonComment.find(commandDelimiter);
    if (resultStart == std::string::npos)
    {
        command = nonComment;
        result = "";
        return;
    }
    else
    {
        command = nonComment.substr(0, resultStart);
        result = nonComment.substr(resultStart + 1);
    }
}
