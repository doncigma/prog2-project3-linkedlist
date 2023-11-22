#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>

#include "linkedlist.hpp"
#include "linkedlisttest.hpp"

using namespace std;

bool TestAppend(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine);
bool TestPrepend(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine);
bool TestInsert(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine);
bool TestRemove(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine);
bool TestGet(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine);
bool TestSize(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine);
bool TestClear(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine);
bool TestEmpty(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine);
bool TestFind(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine);
bool TestFindIndex(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine);
bool TestForeach(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine);
bool TestPrint(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine);

vector<TestFunctionEntry> linkedListTestCommands = {
    {"append", "append <value>", TestAppend},
    {"prepend", "prepend <value>", TestPrepend},
    {"insertat", "insertat <value> <position>", TestInsert},
    {"removeat", "removeat <value> <position>", TestRemove},
    {"size", "size", TestSize},
    {"empty", "empty", TestEmpty},
    {"clear", "clear", TestClear},
    {"get", "get <position>", TestGet},
    {"[]", "[] <position>", TestGet},
    {"size", "size", TestSize},
    {"find", "find <value>", TestFind},
    {"findindex", "findindex <value>", TestFindIndex},
    {"foreach", "foreach", TestForeach},
    {"print", "print", TestPrint},
};

LinkedList<int> myNameList;

bool TestAppend(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine)
{
    if (params.size() != 1)
    {
        throw invalid_argument("append requires 1 parameter");
    }

    myNameList.Append(stoi(params[0]));
    output = "";
    return true;
}

bool TestPrepend(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine)
{
    if (params.size() != 1)
    {
        throw invalid_argument("prepend requires 1 parameter");
    }

    myNameList.Prepend(stoi(params[0]));
    output = "";
    return true;
}

bool TestInsert(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine)
{
    if (params.size() != 2)
    {
        throw invalid_argument("insertat requires 2 parameters");
    }

    myNameList.InsertAt(stoi(params[0]), stoi(params[1]));
    output = "";
    return true;
}

bool TestRemove(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine)
{
    if (params.size() != 1)
    {
        throw invalid_argument("removeat requires 1 parameter");
    }

    myNameList.RemoveAt(stoi(params[0]));
    output = "";
    return true;
}

bool TestGet(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine)
{
    if (params.size() != 1)
    {
        throw invalid_argument("get requires 1 parameter");
    }

    output = to_string(myNameList.Get(stoi(params[0])));

    return true;
}

bool TestSize(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine)
{
    if (params.size() != 0)
    {
        throw invalid_argument("size does not take any parameters");
    }

    output = to_string(myNameList.Size());

    return true;
}

bool TestClear(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine)
{
    if (params.size() != 0)
    {
        throw std::invalid_argument("clear does not take any parameters");
    }

    myNameList.Clear();
    output = "";

    return true;
}

bool TestEmpty(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine)
{
    if (params.size() != 0)
    {
        throw std::invalid_argument("empty does not take any parameters");
    }

    output = to_string(myNameList.Empty());

    return true;
}

bool TestFind(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine)
{
    if (params.size() != 1)
    {
        throw std::invalid_argument("find requires 1 parameter");
    }

    int valueToFind = stoi(params[0]);
    auto predicate = [valueToFind](const int &value)
    { return value == valueToFind; };

    output = to_string(myNameList.Find(predicate));

    return true;
}

bool TestFindIndex(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine)
{
    if (params.size() != 1)
    {
        throw std::invalid_argument("findindex requires 1 parameter");
    }

    int valueToFind = stoi(params[0]);
    auto predicate = [valueToFind](int value)
    { return value == valueToFind; };

    output = to_string(myNameList.FindIndex(predicate));

    return true;
}

bool TestForeach(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine)
{
    if (params.size() != 0)
    {
        throw std::invalid_argument("foreach does not take any parameters");
    }

    output = "";
    myNameList.ForEach([&output](int value)
                       { output.append(to_string(value) + ","); });

    return true;
}

bool TestPrint(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine)
{
    if (params.size() != 0)
    {
        throw std::invalid_argument("print does not take any parameters");
    }

    output = "";
    myNameList.ForEach([&output](int value)
                       { output.append(to_string(value) + ","); });

    return true;
}
