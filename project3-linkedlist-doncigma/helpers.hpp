#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <map>

/// @brief Prints error messages to stderr
/// @param line Line number which is 1 based.
/// @param position Position in the string which is 0 based.
/// @param message The error message to print.
extern void PrintError(int line, int position, const std::string &message);

/// @brief Trims whitespace from the beginning and end of a string
/// @param str The string to trim
/// @return The trimmed string
extern std::string trim(const std::string &str);

/// @brief Indicates if a string is whitespace
/// @param str The string to check
/// @return Whether the string is whitespace
extern bool isWhitespace(const std::string &str);

/// @brief Splits a string into a vector of strings based on delimited and supporting quotes.
/// @param str The string to parse
/// @return A vector of strings
extern std::vector<std::string> SplitString(const std::string &str, char delimiter = ' ');

/// @brief Compares two input streams and writes the differences to an output stream.
/// @param stream1 The first input stream to compare.
/// @param stream2 The second input stream to compare.
/// @param output The output stream to write the differences to.
/// @param stream1NamePrefix The string of the first stream to use in the output.
/// @param stream2NamePrefix The string of the second stream to use in the output.
void CompareStreams(std::istream &stream1, std::istream &stream2, std::ostream &output, const std::string stream1NamePrefix, const std::string stream2NamePrefix);

/// @brief The function to call when name is entered in the repl.
/// @param params A vector of strings containing the parameters passed to the function.
/// @param output The output string.
/// @param interactive Whether the function is being called in an interactive session.
/// @param currentLine The current line number in the input stream.
/// @return Whether the function was successful.
typedef bool (*TestFunction)(const std::vector<std::string> &params, std::string &output, bool interactive, int currentLine);

/// @brief A mapping of function names to their corresponding TestFunctionEntry.
class TestFunctionEntry
{
public:
    TestFunctionEntry(const std::string &name, const std::string &help, TestFunction function) : _name(name), _help(help), _function(function) {}
    TestFunctionEntry() : _name(""), _help(""), _function(NULL) {}
    std::string _name; ///< The name of the function.
    std::string _help; ///< A brief description of the function.
    TestFunction _function; ///< The function to call.
};

/// @brief Adds a list of commands to a command map.
/// @param commands A vector of TestFunctionEntry objects representing the commands to add.
/// @param commandMap The map to add the commands to.
extern void AddCommands(const std::vector<TestFunctionEntry> &commands, std::map<std::string, TestFunctionEntry> &commandMap);

/// @brief Returns all strings in a map that start with a given prefix.
/// @param commandMap The map to search.
/// @param prefix The prefix to search for.
/// @return A vector of strings containing all keys in the map that start with the given prefix.
extern std::vector<std::string> FindPrefixMatch(const std::map<std::string, TestFunctionEntry> &commandMap, const std::string &prefix);


/// @brief Parses a line of input into its command, result, and comment components separated by commandDelimiter and commentDelimiter.
/// @param line The line to parse.
/// @param command The output parameter to store the command component of the line.
/// @param result The output parameter to store the result component of the line.
/// @param comment The output parameter to store the comment component of the line.
extern void ParseLine(const std::string &line, std::string &command, std::string &result, std::string &comment, char commandDelimiter = ';', char commentDelimiter = '#');
